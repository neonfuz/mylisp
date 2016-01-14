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

void print_entry(Entry e)
{
    printf("%d, %d, %d\n", e.start, e.end, e.depth);
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
 * Parses program 'program' of size 'proglen' bytes, into 'entries', of
 * length 'len' entries. Is unsafe on misformatted programs.
 */
int parse(char *program, size_t proglen, Entry *entries, size_t len)
{
    int depth = 0;
    size_t entry_i = 0;

    size_t i;
    for(i=0; i<proglen; ++i) {
        if(program[i] == '(') {
            ++depth;
            entries[entry_i].start = i;
            entries[entry_i].depth = depth;
            ++entry_i;
        }
        if(program[i] == ')')
            entries[entry_i-depth].end = i;
            --depth;
    }

    return entry_i;
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
    parse(program, sb.st_size, entries, len);

    /* Print file */
    size_t i;
    for(i=0; i<len; ++i) {
        print_entry(entries[i]);
    }

    /* Clean up */
    munmap(program, sb.st_size);
    close(fd);

    return 0;
}
