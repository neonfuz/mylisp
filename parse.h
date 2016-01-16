typedef enum {NT_link, NT_symbol, NT_string, NT_number, NT_bool} NodeType;

typedef struct list_node_s {
    NodeType type;
    union node_u *node;
    struct list_node_s *next;
} LinkNode;

typedef struct {
    NodeType type;
    char *name;
} SymbolNode;

typedef struct {
    NodeType type;
    char *value;
} StringNode;

typedef struct {
    NodeType type;
    int value;
} NumberNode;

typedef struct {
    NodeType type;
    int value; /* Is zero or non zero */
} BoolNode;

typedef union node_u {
    NodeType type;
    LinkNode   lin;
    SymbolNode sym;
    StringNode str;
    NumberNode num;
    BoolNode   bol;
} Node;

Node parse_symbol(Token t, char *program);
