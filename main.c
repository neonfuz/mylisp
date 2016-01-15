#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "mapfile.h"

typedef enum {ET_SExpr, ET_Symbol} EntryType;

typedef struct {
    EntryType type;
    int start, end, children;
} Entry;

void print_entry(Entry e, char *program)
{
    printf("%3d, %3d, %3d:   ", e.start, e.end, e.children);
    putchar(e.type==ET_SExpr?'(':'"');
    long i;
    for(i=e.start; i<e.end; ++i)
        putchar(program[i]);
    putchar(e.type==ET_SExpr?')':'"');
    putchar('\n');
}

static inline
int issymbolchr(char c)
{
    return !(c == '(' || c == ')' || isspace(c));
}

/* Gets length in entries of program 'program' of size 'proglen' bytes */
size_t count(char *program, size_t proglen)
{
    size_t i, sexpr = 0, symbol = 0;
    int in_symbol = 0;
    for(i=0; i<proglen; ++i) {
        char c = program[i];
        if(c == '(')
            ++sexpr;
        if(in_symbol) {
            if(!issymbolchr(c))
                in_symbol = 0;
        } else {
            if(issymbolchr(c)) {
                in_symbol = 1;
                ++symbol;
            }
        }
    }

    return sexpr + symbol;
}

/*
 * Recursive parse function
 */
size_t parse_rec(char *program, Entry *entries,
                 size_t prog_i, size_t *ent_i)
{
    /* Allocate space for this s-expression */
    Entry *my_entry = &entries[(*ent_i)++];

    my_entry->type = ET_SExpr;
    my_entry->start = prog_i;
    my_entry->children = 0;

    Entry *symbol = NULL;

    while(1) {
        if(symbol == NULL && issymbolchr(program[prog_i])) {
            ++my_entry->children;
            symbol = &entries[(*ent_i)++];
            symbol->start = prog_i;
            symbol->type = ET_Symbol;
        }
        if(symbol != NULL && !issymbolchr(program[prog_i])) {
            symbol->end = prog_i;
            symbol = NULL;
        }
        switch(program[prog_i]) {
        case '(':
            ++my_entry->children;
            prog_i = parse_rec( program, entries, prog_i+1, ent_i );
            break;
        default:
            ++prog_i;
            break;
        case ')':
            my_entry->end = prog_i;
            return prog_i + 1;
        }
    }
}

/*
 * User friendly wrapper for recursive parse function
 */
void parse(char *program, Entry *entries)
{
    size_t ent_i = 0;
    parse_rec(program, entries, 1, &ent_i);
}

int main(int argc, char *argv[])
{
    if(argc < 2) {
        fputs("Please specify a file as the first argument.\n", stderr);
        return EXIT_FAILURE;
    }

    MappedFile mf = map_file_rd(argv[1]);

    /* Parse file */
    size_t len = count(mf.file, mf.sb.st_size);
    Entry *entries = malloc(sizeof(Entry) * len);
    parse(mf.file, entries);

    /* Print entries */
    printf("start end chldrn\n");
    size_t i;
    for(i=0; i<len; ++i) {
        print_entry(entries[i], mf.file);
    }

    unmap_file(mf);
    return 0;
}
