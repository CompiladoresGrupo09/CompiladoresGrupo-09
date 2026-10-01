#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"

typedef struct Valor {
    TipoDado tipo;
    union {
        int i;
        float f;
        char c;
    } como;
} Valor;

extern int erros_execucao;

Valor interpretarNo(ASTNode *no);
void interpretar(ASTNode *raiz);

#endif