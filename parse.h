typedef enum {NT_link, NT_number, NT_string, NT_symbol, NT_bool} NodeType;

typedef struct list_node_s {
    NodeType type;
    union node_u *node;
    struct list_node_s *next;
} LinkNode;

typedef struct {
    NodeType type;
    int value;
} NumberNode;

typedef struct {
    NodeType type;
    char *value;
} StringNode;

typedef struct {
    NodeType type;
    char *name;
} SymbolNode;

typedef struct {
    NodeType type;
    int val; /* Is zero or non zero */
} BoolNode;

typedef union node_u {
    NodeType type;
    LinkNode   lin;
    NumberNode num;
    StringNode str;
    SymbolNode sym;
} Node;

Node parse_symbol(Token t, char *program);
