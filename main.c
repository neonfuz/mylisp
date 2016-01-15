#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "mapfile.h"
#include "token.h"

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

int main(int argc, char *argv[])
{
    if(argc < 2) {
        fputs("Please specify a file as the first argument.\n", stderr);
        return EXIT_FAILURE;
    }

    MappedFile mf = map_file_rd(argv[1]);

    /* Tokenize file */
    size_t len = count_tokens(mf.file, mf.sb.st_size);
    Token *tokens = malloc(sizeof(Token) * len);
    tokenize(mf.file, tokens);

    /* Print tokens */
    printf("start end chldrn\n");
    size_t i;
    for(i=0; i<len; ++i) {
        print_token(tokens[i], mf.file);
    }

    unmap_file(mf);
    return 0;
}
