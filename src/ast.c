#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode *criar_no(NodeType tipo, int linha) {
    ASTNode *no = calloc(1, sizeof(ASTNode));
    no->type = tipo;
    no->line = linha;
    return no;
}

void adicionar_filho(ASTNode *pai, ASTNode *filho) {
    if (pai->num_children == pai->capacidade_children) {
        pai->capacidade_children = pai->capacidade_children == 0 ? 4 : pai->capacidade_children * 2;
        pai->children = realloc(pai->children, pai->capacidade_children * sizeof(ASTNode *));
    }
    pai->children[pai->num_children++] = filho;
}

ASTNode *criar_no_id(const char *nome, int linha) {
    ASTNode *no = criar_no(NODE_ID, linha);
    no->strval = strdup(nome);
    return no;
}

ASTNode *criar_no_int(int valor, int linha) {
    ASTNode *no = criar_no(NODE_INT_LIT, linha);
    no->intval = valor;
    return no;
}

ASTNode *criar_no_float(float valor, int linha) {
    ASTNode *no = criar_no(NODE_FLOAT_LIT, linha);
    no->floatval = valor;
    return no;
}

ASTNode *criar_no_char(char valor, int linha) {
    ASTNode *no = criar_no(NODE_CHAR_LIT, linha);
    no->charval = valor;
    return no;
}

ASTNode *criar_no_string(const char *valor, int linha) {
    ASTNode *no = criar_no(NODE_STRING_LIT, linha);
    no->strval = strdup(valor);
    return no;
}

ASTNode *criar_no_binop(const char *op, ASTNode *esquerda, ASTNode *direita, int linha) {
    ASTNode *no = criar_no(NODE_BINOP, linha);
    no->strval = strdup(op);
    adicionar_filho(no, esquerda);
    adicionar_filho(no, direita);
    return no;
}

ASTNode *criar_no_unop(const char *op, ASTNode *operando, int linha) {
    ASTNode *no = criar_no(NODE_UNOP, linha);
    no->strval = strdup(op);
    adicionar_filho(no, operando);
    return no;
}

ASTNode *criar_no_addr(const char *nome, int linha) {
    ASTNode *no = criar_no(NODE_ADDR, linha);
    no->strval = strdup(nome);
    return no;
}

static const char *nomes_tipo[] = {
    "PROGRAM", "FUNC_DECL", "VAR_DECL", "PARAM", "LIST", "BLOCK",
    "IF", "WHILE", "FOR", "RETURN", "EXPR_STMT",
    "BINOP", "UNOP", "CALL", "ADDR",
    "ID", "INT_LIT", "FLOAT_LIT", "CHAR_LIT", "STRING_LIT"
};

const char *nome_tipo_no(NodeType tipo) {
    return nomes_tipo[tipo];
}

void imprimir_ast(const ASTNode *no, int nivel) {
    if (!no) {
        return;
    }

    for (int i = 0; i < nivel; i++) {
        printf("  ");
    }

    printf("%s", nome_tipo_no(no->type));
    if (no->strval) {
        printf(" '%s'", no->strval);
    }
    if (no->type == NODE_INT_LIT) {
        printf(" %d", no->intval);
    }
    if (no->type == NODE_FLOAT_LIT) {
        printf(" %f", no->floatval);
    }
    if (no->type == NODE_CHAR_LIT) {
        printf(" '%c'", no->charval);
    }
    if (no->type == NODE_VAR_DECL || no->type == NODE_PARAM) {
        printf(" (tipo=%d)", no->intval);
    }
    printf(" [linha %d]\n", no->line);

    for (int i = 0; i < no->num_children; i++) {
        imprimir_ast(no->children[i], nivel + 1);
    }
}

void liberar_ast(ASTNode *no) {
    if (!no) {
        return;
    }
    for (int i = 0; i < no->num_children; i++) {
        liberar_ast(no->children[i]);
    }
    free(no->children);
    free(no->strval);
    free(no);
}
