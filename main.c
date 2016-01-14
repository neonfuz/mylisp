#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <assert.h>

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

    /* Print the program */
    long i;
    for(i=0; i<sb.st_size; ++i)
        putchar(program[i]);

    /* Clean up */
    munmap(program, sb.st_size);
    close(fd);

    return 0;
}
