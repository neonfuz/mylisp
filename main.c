#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "mapfile.h"
#include "token.h"
#include "parse.h"

void print_token(Token e, char *program)
{
    printf("%3d, %3d, %3d:   ", e.start, e.end, e.children);
    putchar(e.type==TT_SExpr?'(':'"');
    long i;
    for(i=e.start; i<e.end; ++i)
        putchar(program[i]);
    putchar(e.type==TT_SExpr?')':'"');
    putchar('\n');
}

#define TABWIDTH 4

void print_node(Node n, int depth)
{
    static const char *type_names[] = {
        [NT_link] =   "link",
        [NT_number] = "number",
        [NT_string] = "string",
        [NT_symbol] = "symbol",
        [NT_bool] =   "bool",
    };

    char tabstr[depth*TABWIDTH + 1];
    int i;
    for(i=0; i<depth*TABWIDTH; ++i)
        tabstr[i] = ' ';
    tabstr[i] = '\0';

    printf("%sType: %s\n", tabstr, type_names[n.type]);

    switch(n.type) {
    case NT_link:
        printf("%s  Node: %p\n", tabstr, n.lin.node);
        printf("%s  Next: %p\n", tabstr, n.lin.next);
        LinkNode *next;
        for ( next=&n.lin; next != NULL; next=next->next ) {
            print_node( *next->node, depth+1 );
        }
        break;
    case NT_symbol:
        printf("%s  Name: %s\n", tabstr, n.sym.name);
        break;
    case NT_string:
        printf("%s  Value: \"%s\"\n", tabstr, n.str.value);
        break;
    case NT_number:
        printf("%s  Value: [%d]\n", tabstr, n.num.value);
        break;
    case NT_bool:
        printf("%s  Value: '%c'\n", tabstr, n.bol.value?'1':'0');
        break;
    }
}

void print_node_to_sexpr(Node n)
{
    switch(n.type) {
    case NT_link:
        putchar('(');
        LinkNode *next;
        for ( next=&n.lin; next != NULL; next=next->next ) {
            print_node_to_sexpr( *next->node );
            if(next->next != NULL)
                putchar(' ');
        }
        putchar(')');
        break;
    case NT_symbol:
        fputs(n.sym.name, stdout);
        break;
    case NT_string:
        fputs(n.str.value, stdout);
        break;
    case NT_number:
        printf("%d", n.num.value);
        break;
    case NT_bool:
        putchar(n.bol.value?'1':'0');
        break;
    }
}

int main(int argc, char *argv[])
{
    if(argc < 2) {
        fputs("Please specify a file as the first argument.\n", stderr);
        return EXIT_FAILURE;
    }

    MappedFile mf = map_file_rd(argv[1]);

    /* Original program */
    puts("Original program:");
    size_t i;
    for(i=0; i<mf.sb.st_size; ++i)
        putchar(mf.file[i]);
    puts("");

    /* Tokenize file */
    size_t len = count_tokens(mf.file, mf.sb.st_size);
    Token *tokens = malloc(sizeof(Token) * len);
    tokenize(mf.file, tokens);

    /* Print tokens */
    puts("Tokenized program:");
    printf("start end chldrn\n");
    for(i=0; i<len; ++i) {
        print_token(tokens[i], mf.file);
    }
    puts("");

    /* Parse tokens */
    Node n = parse_sexpr(tokens, mf.file);

    /* Print nodes */
    puts("Parsed tokens:");
    print_node(n, 0);
    puts("");

    /* Parsed to sexpression */
    puts("Parsed then re-printed as sexpr:");
    print_node_to_sexpr(n);
    puts("");

    puts("");
    unmap_file(mf);
    return 0;
}
