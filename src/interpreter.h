#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"

typedef struct Valor
{
    TipoDado tipo;
    union
    {
        int i;
        float f;
        char c;
    } como;
} Valor;

Valor interpretarNo(ASTNode *no);
int interpretar(ASTNode *raiz);
void configurar_limite_recursao(int limite);

#endif