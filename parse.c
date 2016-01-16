#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "token.h"
#include "parse.h"

static inline
int isnumchr(char c) {
    return isdigit(c) || c == '+' || c == '-';
}

static inline
int boolchr_to_int(char c) {
    switch(tolower(c)) {
    case 't': return 1;
    default: return 0;
    }
}

Node parse_symbol(Token *tp, char *program)
{
    Node n;
    Token t = *tp;

    if ( isnumchr( program[t.start] ) ) {
        n.type = NT_number;
        n.num.value = atoi( &program[t.start] ); /* potentially unsafe? */
    } else {
        switch(program[t.start]) {
        case '"':
            n.type = NT_string;
            n.str.value = strndup( &program[t.start+1], t.end-t.start-2 );
            break;
        case '#':
            n.type = NT_bool;
            n.bol.value = boolchr_to_int( program[t.start+1] );
            break;
        default:
            n.type = NT_symbol;
            n.sym.name = strndup( &program[t.start], t.end-t.start );
        }
    }

    return n;
}

Node parse_sexpr(Token *t, char *program)
{
    int n = t->children;
    ++t;

    LinkNode *links = calloc(n, sizeof(LinkNode));
    Node *nodes = calloc(n, sizeof(Node));

    int i, j;
    for(i=0, j=0; i<n; ++i, ++j) {
        links[i].type = NT_link;
        links[i].node = &nodes[i];
        if ( i+1 >= n )
            links[i].next = NULL;
        else
            links[i].next = &links[i+1];

        nodes[i] = parse( &t[j], program );
        if(t[j].type == TT_SExpr)
            j += t[j].children;
    }

    return (Node)links[0];
}

static Node (*parsers[])(Token*, char*) = {
    [TT_SExpr] = parse_sexpr,
    [TT_Symbol] = parse_symbol,
};

Node parse(Token *t, char *program)
{
    return parsers[t->type](t, program);
}

