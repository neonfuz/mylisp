#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if(argc < 2) {
        fputs("No file specified\n", stderr);
        return 1;
    }

    /* Open file */
    FILE *f = fopen(argv[1], "r");

    /* Get it's length */
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    /* Allocate space for the program */
    char *prog = malloc(len+1);
    prog[len] = '\0';

    /* Read the program into the buffer */
    long i;
    for(i=0; i<len; ++i) {
        prog[i] = fgetc(f);
    }

    /* Print the program */
    fputs(prog, stdout);

    return 0;
}
