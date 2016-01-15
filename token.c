#include <stdlib.h>

#include "token.h"

static inline
int issymbolchr(char c)
{
    return !(c == '(' || c == ')' || isspace(c));
}

/* Gets length in tokens of program 'program' of size 'proglen' bytes */
size_t count_tokens(char *program, size_t proglen)
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
 * Recursive tokenize function
 */
size_t tokenize_rec(char *program, Token *tokens,
                    size_t prog_i, size_t *ent_i)
{
    /* Allocate space for this s-expression */
    Token *my_token = &tokens[(*ent_i)++];

    my_token->type = ET_SExpr;
    my_token->start = prog_i;
    my_token->children = 0;

    Token *symbol = NULL;

    while(1) {
        if(symbol == NULL && issymbolchr(program[prog_i])) {
            ++my_token->children;
            symbol = &tokens[(*ent_i)++];
            symbol->start = prog_i;
            symbol->type = ET_Symbol;
        }
        if(symbol != NULL && !issymbolchr(program[prog_i])) {
            symbol->end = prog_i;
            symbol = NULL;
        }
        switch(program[prog_i]) {
        case '(':
            ++my_token->children;
            prog_i = tokenize_rec( program, tokens, prog_i+1, ent_i );
            break;
        default:
            ++prog_i;
            break;
        case ')':
            my_token->end = prog_i;
            return prog_i + 1;
        }
    }
}

/*
 * User friendly wrapper for recursive tokenize function
 */
void tokenize(char *program, Token *tokens)
{
    size_t ent_i = 0;
    tokenize_rec(program, tokens, 1, &ent_i);
}
