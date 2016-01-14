#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>

typedef struct {
    int start, end, depth;
} Entry;

void print_entry(Entry e, char *program)
{
    printf("%d, %d, %d ", e.start, e.end, e.depth);
    long i;
    for(i=e.start; i<e.end; ++i)
        putchar(program[i]);
    putchar('\n');
}

/* Gets length in entries of program 'program' of size 'proglen' bytes */
size_t get_len(char *program, size_t proglen)
{
    size_t i, len = 0;
    for(i=0; i<proglen; ++i) {
        if(program[i] == '(')
            ++len;
    }
    return len;
}

/*
 * Recursive parse function
 */
size_t parse_rec(char *program, Entry *entries,
                 size_t prog_i, size_t *ent_i, const int depth)
{
    Entry *my_entry = &entries[(*ent_i)++];

    my_entry->start = prog_i;
    my_entry->depth = depth;

    while(1) {
        switch(program[prog_i]) {
        case '(':
            prog_i = parse_rec( program, entries, prog_i+1, ent_i, depth+1 );
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
    parse_rec(program, entries, 1, &ent_i, 1);
}

int main(int argc, char *argv[])
{
    if(argc < 2) {
        fputs("No file specified\n", stderr);
        return EXIT_FAILURE;
    }

    /* Open the file */
    int fd = open(argv[1], O_RDONLY);
    assert(fd != 0);

    /* Get info about the file, including size */
    struct stat sb;
    int err = fstat(fd, &sb);
    assert(err == 0);

    /* Map the file */
    char *program = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    assert(program != MAP_FAILED);

    /* Parse file */
    size_t len = get_len(program, sb.st_size);
    Entry *entries = malloc(sizeof(Entry) * len);
    parse(program, entries);

    /* Print file */
    size_t i;
    for(i=0; i<len; ++i) {
        print_entry(entries[i], program);
    }

    /* Clean up */
    munmap(program, sb.st_size);
    close(fd);

    return 0;
}
