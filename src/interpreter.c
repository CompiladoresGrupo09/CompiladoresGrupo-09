
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"

int erros_execucao = 0;

typedef struct Variavel
{
    char *nome;
    Valor valor;
} Variavel;

static Variavel *variaveis = NULL;
static int num_variaveis = 0;
static int capacidade_variaveis = 0;

static Valor *buscar_variavel(const char *nome)
{
    for (int i = 0; i < num_variaveis; i++)
    {
        if (strcmp(variaveis[i].nome, nome) == 0)
        {
            return &variaveis[i].valor;
        }
    }
    return NULL;
}

static void definir_variavel(const char *nome, Valor valor)
{
    Valor *existente = buscar_variavel(nome);
    if (existente != NULL)
    {
        *existente = valor;
        return;
    }

    if (num_variaveis == capacidade_variaveis)
    {
        capacidade_variaveis = capacidade_variaveis == 0 ? 8 : capacidade_variaveis * 2;
        variaveis = realloc(variaveis, capacidade_variaveis * sizeof(Variavel));
    }

    variaveis[num_variaveis].nome = strdup(nome);
    variaveis[num_variaveis].valor = valor;
    num_variaveis++;
}

static float valor_como_float(Valor v)
{
    if (v.tipo == TIPO_FLOAT)
        return v.como.f;
    if (v.tipo == TIPO_CHAR)
        return (float)v.como.c;
    return (float)v.como.i;
}

static int valor_como_int(Valor v)
{
    if (v.tipo == TIPO_FLOAT)
        return (int)v.como.f;
    if (v.tipo == TIPO_CHAR)
        return (int)v.como.c;
    return v.como.i;
}

Valor interpretarNo(ASTNode *no)
{
    Valor resultado = {0};

    switch (no->type)
    {
    case NODE_INT_LIT:
        resultado.tipo = TIPO_INT;
        resultado.como.i = no->intval;
        break;

    case NODE_FLOAT_LIT:
        resultado.tipo = TIPO_FLOAT;
        resultado.como.f = no->floatval;
        break;

    case NODE_CHAR_LIT:
        resultado.tipo = TIPO_CHAR;
        resultado.como.c = no->charval;
        break;

    case NODE_ID:
    {
        Valor *var = buscar_variavel(no->strval);
        if (var == NULL)
        {
            fprintf(stderr, "Erro de execucao [linha %d]: variavel '%s' sem valor\n",
                    no->line, no->strval);
            erros_execucao++;
            resultado.tipo = TIPO_INT;
            resultado.como.i = 0;
            break;
        }
        resultado = *var;
        break;
    }

    case NODE_BINOP:
    {
        Valor esquerda = interpretarNo(no->children[0]);
        Valor direita = interpretarNo(no->children[1]);
        const char *op = no->strval;
        int usa_float = esquerda.tipo == TIPO_FLOAT || direita.tipo == TIPO_FLOAT;

        if (strcmp(op, "&&") == 0)
        {
            resultado.tipo = TIPO_INT;
            resultado.como.i = (valor_como_int(esquerda) != 0) && (valor_como_int(direita) != 0);
            break;
        }
        if (strcmp(op, "||") == 0)
        {
            resultado.tipo = TIPO_INT;
            resultado.como.i = (valor_como_int(esquerda) != 0) || (valor_como_int(direita) != 0);
            break;
        }

        if (usa_float)
        {
            float a = valor_como_float(esquerda);
            float b = valor_como_float(direita);

            if (strcmp(op, "+") == 0)
            {
                resultado.tipo = TIPO_FLOAT;
                resultado.como.f = a + b;
            }
            else if (strcmp(op, "-") == 0)
            {
                resultado.tipo = TIPO_FLOAT;
                resultado.como.f = a - b;
            }
            else if (strcmp(op, "*") == 0)
            {
                resultado.tipo = TIPO_FLOAT;
                resultado.como.f = a * b;
            }
            else if (strcmp(op, "/") == 0)
            {
                resultado.tipo = TIPO_FLOAT;
                resultado.como.f = a / b;
            }
            else
            {
                resultado.tipo = TIPO_INT;
                if (strcmp(op, "==") == 0)
                    resultado.como.i = a == b;
                else if (strcmp(op, "!=") == 0)
                    resultado.como.i = a != b;
                else if (strcmp(op, "<") == 0)
                    resultado.como.i = a < b;
                else if (strcmp(op, ">") == 0)
                    resultado.como.i = a > b;
                else if (strcmp(op, "<=") == 0)
                    resultado.como.i = a <= b;
                else if (strcmp(op, ">=") == 0)
                    resultado.como.i = a >= b;
            }
            break;
        }

        int a = valor_como_int(esquerda);
        int b = valor_como_int(direita);
        resultado.tipo = TIPO_INT;

        if (strcmp(op, "+") == 0)
            resultado.como.i = a + b;
        else if (strcmp(op, "-") == 0)
            resultado.como.i = a - b;
        else if (strcmp(op, "*") == 0)
            resultado.como.i = a * b;
        else if (strcmp(op, "/") == 0)
        {
            if (b == 0)
            {
                fprintf(stderr, "Erro de execucao [linha %d]: divisao por zero\n", no->line);
                erros_execucao++;
                resultado.como.i = 0;
            }
            else
                resultado.como.i = a / b;
        }
        else if (strcmp(op, "%") == 0)
        {
            if (b == 0)
            {
                fprintf(stderr, "Erro de execucao [linha %d]: divisao por zero\n", no->line);
                erros_execucao++;
                resultado.como.i = 0;
            }
            else
                resultado.como.i = a % b;
        }
        else if (strcmp(op, "==") == 0)
            resultado.como.i = a == b;
        else if (strcmp(op, "!=") == 0)
            resultado.como.i = a != b;
        else if (strcmp(op, "<") == 0)
            resultado.como.i = a < b;
        else if (strcmp(op, ">") == 0)
            resultado.como.i = a > b;
        else if (strcmp(op, "<=") == 0)
            resultado.como.i = a <= b;
        else if (strcmp(op, ">=") == 0)
            resultado.como.i = a >= b;
        break;
    }

    case NODE_UNOP:
    {
        Valor operando = interpretarNo(no->children[0]);
        const char *op = no->strval;

        if (strcmp(op, "-") == 0)
        {
            if (operando.tipo == TIPO_FLOAT)
            {
                resultado.tipo = TIPO_FLOAT;
                resultado.como.f = -operando.como.f;
            }
            else
            {
                resultado.tipo = TIPO_INT;
                resultado.como.i = -valor_como_int(operando);
            }
        }
        else if (strcmp(op, "!") == 0)
        {
            resultado.tipo = TIPO_INT;
            resultado.como.i = valor_como_int(operando) == 0 ? 1 : 0;
        }
        break;
    }

    default:
        break;
    }

    return resultado;
}

static void executar_printf(ASTNode *chamada)
{
    if (chamada->num_children == 0)
        return;

    const char *formato = chamada->children[0]->strval;
    int indice_arg = 1;

    for (const char *p = formato; *p != '\0'; p++)
    {
        if (*p != '%')
        {
            putchar(*p);
            continue;
        }

        p++;
        if (*p == 'd')
        {
            Valor v = interpretarNo(chamada->children[indice_arg++]);
            printf("%d", valor_como_int(v));
        }
        else if (*p == 'f')
        {
            Valor v = interpretarNo(chamada->children[indice_arg++]);
            printf("%f", valor_como_float(v));
        }
        else if (*p == 'c')
        {
            Valor v = interpretarNo(chamada->children[indice_arg++]);
            putchar((char)valor_como_int(v));
        }
        else if (*p == '%')
        {
            putchar('%');
        }
        else
        {
            putchar('%');
            putchar(*p);
        }
    }
}

static void executar_declaracao_variavel(ASTNode *declaracao)
{
    for (int i = 0; i < declaracao->num_children; i++)
    {
        ASTNode *declarador = declaracao->children[i];

        if (declarador->type == NODE_ID)
        {
            Valor zero = {0};
            zero.tipo = (TipoDado)declaracao->intval;
            definir_variavel(declarador->strval, zero);
        }
        else if (declarador->type == NODE_BINOP)
        {
            ASTNode *id = declarador->children[0];
            Valor valor = interpretarNo(declarador->children[1]);
            definir_variavel(id->strval, valor);
        }
    }
}

static void executar_comando(ASTNode *comando)
{
    if (comando == NULL)
        return;

    switch (comando->type)
    {
    case NODE_VAR_DECL:
        executar_declaracao_variavel(comando);
        break;

    case NODE_EXPR_STMT:
    {
        ASTNode *expr = comando->children[0];
        if (expr == NULL)
            break;

        if (expr->type == NODE_CALL && strcmp(expr->strval, "printf") == 0)
            executar_printf(expr);
        else
            interpretarNo(expr);
        break;
    }

    case NODE_BLOCK:
        for (int i = 0; i < comando->num_children; i++)
            executar_comando(comando->children[i]);
        break;

    default:
        break;
    }
}

void interpretar(ASTNode *raiz)
{
    if (raiz == NULL)
        return;

    for (int i = 0; i < raiz->num_children; i++)
    {
        ASTNode *declaracao = raiz->children[i];
        if (declaracao != NULL && declaracao->type == NODE_FUNC_DECL &&
            strcmp(declaracao->strval, "main") == 0)
        {
            executar_comando(declaracao->children[1]);
            return;
        }
    }
}