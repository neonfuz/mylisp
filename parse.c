#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "token.h"
#include "parse.h"

Node parse_symbol(Token *tp, char *program)
{
    Node n;
    Token t = *tp;

    if ( isdigit( program[t.start] ) ) {
        n.type = NT_number;
        n.num.value = atoi( &program[t.start] ); /* potentially unsafe? */
    } else if ( program[t.start] == '"' ) {
        n.type = NT_string;
        n.str.value = strndup( &program[t.start+1], t.end-t.start-2 );
    } else {
        n.type = NT_symbol;
        n.sym.name = strndup( &program[t.start], t.end-t.start );
    }

    return n;
}

Node parse_sexpr(Token *t, char *program)
{
    int n = t->children;
    ++t;

    LinkNode *links = calloc(n, sizeof(LinkNode));
    Node *nodes = calloc(n, sizeof(Node));

    int i;
    for(i=0; i<n; ++i) {
        links[i].type = NT_link;
        links[i].node = &nodes[i];
        if ( i+1 >= n )
            links[i].next = NULL;
        else
            links[i].next = &links[i+1];

        nodes[i] = parse( &t[i], program );
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

