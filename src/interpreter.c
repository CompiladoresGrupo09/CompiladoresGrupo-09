
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

typedef struct EscopoExec
{
    Variavel *variaveis;
    int quantidade;
    int capacidade;
} EscopoExec;

static EscopoExec *escopos = NULL;
static int numero_escopos = 0;
static int capacidade_escopos = 0;

static void garantir_capacidade_escopos_exec(void)
{
    if (numero_escopos < capacidade_escopos)
        return;

    int nova_capacidade = capacidade_escopos == 0 ? 4 : capacidade_escopos * 2;
    EscopoExec *novos = realloc(escopos, nova_capacidade * sizeof(EscopoExec));
    if (novos == NULL)
    {
        fprintf(stderr, "Erro: falha ao alocar escopos de execucao\n");
        exit(EXIT_FAILURE);
    }

    escopos = novos;
    capacidade_escopos = nova_capacidade;
}

static void garantir_capacidade_variaveis(EscopoExec *escopo)
{
    if (escopo->quantidade < escopo->capacidade)
        return;

    int nova_capacidade = escopo->capacidade == 0 ? 4 : escopo->capacidade * 2;
    Variavel *novas = realloc(escopo->variaveis, nova_capacidade * sizeof(Variavel));
    if (novas == NULL)
    {
        fprintf(stderr, "Erro: falha ao alocar variaveis do escopo\n");
        exit(EXIT_FAILURE);
    }

    escopo->variaveis = novas;
    escopo->capacidade = nova_capacidade;
}

static void entrar_escopo_exec(void)
{
    garantir_capacidade_escopos_exec();

    EscopoExec *escopo = &escopos[numero_escopos++];
    escopo->variaveis = NULL;
    escopo->quantidade = 0;
    escopo->capacidade = 0;
}

static void sair_escopo_exec(void)
{
    if (numero_escopos == 0)
        return;

    EscopoExec *escopo = &escopos[numero_escopos - 1];
    for (int i = 0; i < escopo->quantidade; i++)
        free(escopo->variaveis[i].nome);

    free(escopo->variaveis);
    numero_escopos--;
}
static Valor *ler_variavel(const char *nome)
{
    for (int e = numero_escopos - 1; e >= 0; e--)
    {
        EscopoExec *escopo = &escopos[e];
        for (int i = escopo->quantidade - 1; i >= 0; i--)
        {
            if (strcmp(escopo->variaveis[i].nome, nome) == 0)
                return &escopo->variaveis[i].valor;
        }
    }
    return NULL;
}

static void definir_variavel(const char *nome, Valor valor)
{
    if (numero_escopos == 0)
        entrar_escopo_exec();

    EscopoExec *escopo = &escopos[numero_escopos - 1];
    garantir_capacidade_variaveis(escopo);

    escopo->variaveis[escopo->quantidade].nome = strdup(nome);
    escopo->variaveis[escopo->quantidade].valor = valor;
    escopo->quantidade++;
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

static Valor converter_para_tipo(Valor v, TipoDado tipo)
{
    Valor convertido = {0};
    convertido.tipo = tipo;

    if (tipo == TIPO_FLOAT)
        convertido.como.f = valor_como_float(v);
    else if (tipo == TIPO_INT)
        convertido.como.i = valor_como_int(v);
    else if (tipo == TIPO_CHAR)
        convertido.como.c = (char)valor_como_int(v);
    else
        return v;

    return convertido;
}

static Valor *atribuir_variavel(const char *nome, Valor valor)
{
    Valor *existente = ler_variavel(nome);
    if (existente == NULL)
        return NULL;

    *existente = converter_para_tipo(valor, existente->tipo);
    return existente;
}

static Valor executar_atribuicao(ASTNode *no)
{
    ASTNode *alvo = no->children[0];
    Valor valor = interpretarNo(no->children[1]);

    Valor *guardado = atribuir_variavel(alvo->strval, valor);
    if (guardado == NULL)
    {
        fprintf(stderr, "Erro de execucao [linha %d]: variavel '%s' nao encontrada\n",
                no->line, alvo->strval);
        erros_execucao++;
        return valor;
    }

    return *guardado;
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
        Valor *var = ler_variavel(no->strval);
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
        if (strcmp(no->strval, "=") == 0)
        {
            resultado = executar_atribuicao(no);
            break;
        }

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
        if (*p == '\\')
        {
            p++;
            if (*p == '\0') break;
            
            if (*p == 'n') putchar('\n');
            else if (*p == 't') putchar('\t');
            else if (*p == '\\') putchar('\\');
            else if (*p == '"') putchar('"');
            else if (*p == '0') putchar('\0');
            else
            {
                putchar('\\');
                putchar(*p);
            }
            continue;
        }

        if (*p != '%')
        {
            putchar(*p);
            continue;
        }

        p++;
        if (*p == '\0')
        {
            putchar('%');
            break;
        }

        if (*p == 'd')
        {
            if (indice_arg >= chamada->num_children)
            {
                fprintf(stderr, "Erro de execucao [linha %d]: printf com menos argumentos do que especificadores\n", chamada->line);
                exit(5);
            }
            Valor v = interpretarNo(chamada->children[indice_arg++]);
            printf("%d", valor_como_int(v));
        }
        else if (*p == 'f')
        {
            if (indice_arg >= chamada->num_children)
            {
                fprintf(stderr, "Erro de execucao [linha %d]: printf com menos argumentos do que especificadores\n", chamada->line);
                exit(5);
            }
            Valor v = interpretarNo(chamada->children[indice_arg++]);
            printf("%f", valor_como_float(v));
        }
        else if (*p == 'c')
        {
            if (indice_arg >= chamada->num_children)
            {
                fprintf(stderr, "Erro de execucao [linha %d]: printf com menos argumentos do que especificadores\n", chamada->line);
                exit(5);
            }
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
            definir_variavel(id->strval, converter_para_tipo(valor, (TipoDado)declaracao->intval));
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
        entrar_escopo_exec();
        for (int i = 0; i < comando->num_children; i++)
            executar_comando(comando->children[i]);
        sair_escopo_exec();
        break;

    default:
        break;
    }
}
void interpretar(ASTNode *raiz)
{
    if (raiz == NULL)
        return;

    entrar_escopo_exec();

    for (int i = 0; i < raiz->num_children; i++)
    {
        ASTNode *declaracao = raiz->children[i];
        if (declaracao != NULL && declaracao->type == NODE_FUNC_DECL &&
            strcmp(declaracao->strval, "main") == 0)
        {
            executar_comando(declaracao->children[1]);
            break;
        }
    }

    while (numero_escopos > 0)
        sair_escopo_exec();

    free(escopos);
    escopos = NULL;
    capacidade_escopos = 0;
}