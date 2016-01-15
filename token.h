typedef enum {TT_SExpr, TT_Symbol} TokenType;

typedef struct {
    TokenType type;
    int start, end, children;
} Token;

size_t count(char *program, size_t proglen);
size_t tokenize_rec(char *program, Token *tokens,
                    size_t prog_i, size_t *ent_i);
void tokenize(char *program, Token *tokens);
