#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROGRAM,
    NODE_FUNC_DECL,
    NODE_VAR_DECL,
    NODE_PARAM,
    NODE_LIST,
    NODE_BLOCK,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_RETURN,
    NODE_EXPR_STMT,
    NODE_BINOP,
    NODE_UNOP,
    NODE_CALL,
    NODE_ADDR,
    NODE_ID,
    NODE_INT_LIT,
    NODE_FLOAT_LIT,
    NODE_CHAR_LIT,
    NODE_STRING_LIT
} NodeType;

typedef enum {
    TIPO_INT,
    TIPO_FLOAT,
    TIPO_CHAR,
    TIPO_VOID
} TipoDado;

typedef struct ASTNode {
    NodeType type;
    int line;

    char *strval;
    int intval;
    float floatval;
    char charval;

    struct ASTNode **children;
    int num_children;
    int capacidade_children;
} ASTNode;

ASTNode *criar_no(NodeType tipo, int linha);
void adicionar_filho(ASTNode *pai, ASTNode *filho);

ASTNode *criar_no_id(const char *nome, int linha);
ASTNode *criar_no_int(int valor, int linha);
ASTNode *criar_no_float(float valor, int linha);
ASTNode *criar_no_char(char valor, int linha);
ASTNode *criar_no_string(const char *valor, int linha);
ASTNode *criar_no_binop(const char *op, ASTNode *esquerda, ASTNode *direita, int linha);
ASTNode *criar_no_unop(const char *op, ASTNode *operando, int linha);
ASTNode *criar_no_addr(const char *nome, int linha);

const char *nome_tipo_no(NodeType tipo);
void imprimir_ast(const ASTNode *no, int nivel);
void liberar_ast(ASTNode *no);

#endif
