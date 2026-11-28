
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "erros.h"
#include "interpreter.h"

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
/* retornou: um return foi executado; abortou: houve erro que interrompe o programa */
static int retornou = 0;
static int abortou = 0;
static Valor valor_retornado;
static int profundidade_chamadas = 0;
static int limite_recursao = 1000;
static int limite_recursao_configurado = 0;
#define LIMITE_MAXIMO_RECURSAO 1000

/* Raiz da AST, para localizar as funcoes pelo nome na hora da chamada */
static ASTNode *programa = NULL;

/* Indice do primeiro escopo da funcao em execucao. O escopo 0 e o global. */
static int base_frame = 0;

static void executar_comando(ASTNode *comando);
static Valor chamar_funcao(ASTNode *chamada);

static int obter_limite_recursao(void)
{
    if (limite_recursao_configurado)
        return limite_recursao > LIMITE_MAXIMO_RECURSAO
                   ? LIMITE_MAXIMO_RECURSAO
                   : limite_recursao;

    const char *valor = getenv("INTERPRETADOR_LIMITE_RECURSAO");
    if (valor != NULL)
    {
        char *fim = NULL;
        long configurado = strtol(valor, &fim, 10);
        if (fim != valor && *fim == '\0' && configurado > 0)
        {
            if (configurado > LIMITE_MAXIMO_RECURSAO)
                return LIMITE_MAXIMO_RECURSAO;
            return (int)configurado;
        }
    }
    return limite_recursao;
}

void configurar_limite_recursao(int limite)
{
    if (limite > 0)
    {
        limite_recursao = limite > LIMITE_MAXIMO_RECURSAO
                              ? LIMITE_MAXIMO_RECURSAO
                              : limite;
        limite_recursao_configurado = 1;
    }
}

static int entrar_chamada(const char *nome, int linha)
{
    int limite = obter_limite_recursao();
    if (profundidade_chamadas >= limite)
    {
        reportar_erro(FASE_EXECUCAO, linha, "limite de profundidade de %d chamadas excedido na funcao '%s'",
                      limite, nome);
        abortou = 1;
        return 0;
    }

    profundidade_chamadas++;
    return 1;
}

static void sair_chamada(void)
{
    if (profundidade_chamadas > 0)
        profundidade_chamadas--;
}

static int execucao_interrompida(void)
{
    return retornou || abortou;
}

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
static Valor *buscar_no_escopo_exec(EscopoExec *escopo, const char *nome)
{
    for (int i = escopo->quantidade - 1; i >= 0; i--)
    {
        if (strcmp(escopo->variaveis[i].nome, nome) == 0)
            return &escopo->variaveis[i].valor;
    }
    return NULL;
}

/* Procura do escopo mais interno ate o inicio do frame atual e, depois, no global.
   Os escopos de quem chamou a funcao ficam de fora. */
static Valor *ler_variavel(const char *nome)
{
    for (int e = numero_escopos - 1; e >= base_frame && e >= 0; e--)
    {
        Valor *valor = buscar_no_escopo_exec(&escopos[e], nome);
        if (valor != NULL)
            return valor;
    }

    if (base_frame > 0 && numero_escopos > 0)
        return buscar_no_escopo_exec(&escopos[0], nome);

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

static int valor_verdadeiro(Valor v)
{
    if (v.tipo == TIPO_FLOAT)
        return v.como.f != 0.0f;
    return valor_como_int(v) != 0;
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
        reportar_erro(FASE_EXECUCAO, no->line, "variavel '%s' nao encontrada", alvo->strval);
        return valor;
    }

    return *guardado;
}

static int executar_scanf(ASTNode *chamada, int usa_retorno)
{
    if (chamada->num_children == 0)
        return 0;

    const char *formato = chamada->children[0]->strval;
    int indice_arg = 1;
    int lidos = 0;

    for (const char *p = formato; *p != '\0'; p++)
    {
        if (*p == ' ' || *p == '\\')
        {
            if (*p == '\\' && *(p + 1) != '\0')
                p++;
            scanf(" ");
            continue;
        }

        if (*p != '%')
            continue;

        p++;
        if (*p != 'd' && *p != 'f' && *p != 'c')
        {
            if (*p == '\0')
                break;
            continue;
        }

        ASTNode *argumento = indice_arg < chamada->num_children ? chamada->children[indice_arg++] : NULL;
        if (argumento == NULL || argumento->type != NODE_ADDR)
        {
            reportar_erro(FASE_EXECUCAO, chamada->line, "scanf espera um argumento com '&' para cada especificador");
            abortou = 1;
            return lidos;
        }

        Valor *destino = ler_variavel(argumento->strval);
        if (destino == NULL)
        {
            reportar_erro(FASE_EXECUCAO, chamada->line, "variavel '%s' nao encontrada", argumento->strval);
            abortou = 1;
            return lidos;
        }

        Valor lido = {0};
        int resultado;
        if (*p == 'd')
        {
            lido.tipo = TIPO_INT;
            resultado = scanf("%d", &lido.como.i);
        }
        else if (*p == 'f')
        {
            lido.tipo = TIPO_FLOAT;
            resultado = scanf("%f", &lido.como.f);
        }
        else
        {
            lido.tipo = TIPO_CHAR;
            resultado = scanf("%c", &lido.como.c);
        }

        if (resultado != 1)
        {
            if (usa_retorno)
                return (resultado == EOF && lidos == 0) ? EOF : lidos;

            if (resultado == EOF)
                reportar_erro(FASE_EXECUCAO, chamada->line, "fim da entrada no scanf");
            else
                reportar_erro(FASE_EXECUCAO, chamada->line, "entrada invalida para %%%c no scanf", *p);
            abortou = 1;
            return lidos;
        }

        *destino = converter_para_tipo(lido, destino->tipo);
        lidos++;
    }

    return lidos;
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

    case NODE_CALL:
        if (strcmp(no->strval, "scanf") == 0)
        {
            resultado.tipo = TIPO_INT;
            resultado.como.i = executar_scanf(no, 1);
        }
        else if (strcmp(no->strval, "printf") == 0)
        {
            fprintf(stderr, "Erro interno [linha %d]: printf dentro de expressao nao suportado na execucao\n",
                    no->line);
            contar_erro(FASE_EXECUCAO);
            abortou = 1;
        }
        else
            resultado = chamar_funcao(no);
        break;

    case NODE_ID:
    {
        Valor *var = ler_variavel(no->strval);
        if (var == NULL)
        {
            reportar_erro(FASE_EXECUCAO, no->line, "variavel '%s' sem valor", no->strval);
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

        if (strcmp(no->strval, "&&") == 0)
        {
            resultado.tipo = TIPO_INT;
            resultado.como.i = valor_verdadeiro(interpretarNo(no->children[0])) &&
                               valor_verdadeiro(interpretarNo(no->children[1]));
            break;
        }

        if (strcmp(no->strval, "||") == 0)
        {
            resultado.tipo = TIPO_INT;
            resultado.como.i = valor_verdadeiro(interpretarNo(no->children[0])) ||
                               valor_verdadeiro(interpretarNo(no->children[1]));
            break;
        }
        Valor esquerda = interpretarNo(no->children[0]);
        Valor direita = interpretarNo(no->children[1]);
        const char *op = no->strval;
        int usa_float = esquerda.tipo == TIPO_FLOAT || direita.tipo == TIPO_FLOAT;

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
                reportar_erro(FASE_EXECUCAO, no->line, "divisao por zero");
                abortou = 1;
                resultado.como.i = 0;
            }
            else
                resultado.como.i = a / b;
        }
        else if (strcmp(op, "%") == 0)
        {
            if (b == 0)
            {
                reportar_erro(FASE_EXECUCAO, no->line, "divisao por zero");
                abortou = 1;
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
        fprintf(stderr, "Erro interno [linha %d]: no %s nao suportado na execucao\n",
                no->line, nome_tipo_no(no->type));
        contar_erro(FASE_EXECUCAO);
        abortou = 1;
        break;
    }

    return resultado;
}

static ASTNode *proximo_argumento_printf(ASTNode *chamada, int *indice_arg)
{
    if (*indice_arg >= chamada->num_children)
    {
        reportar_erro(FASE_EXECUCAO, chamada->line, "printf com menos argumentos do que especificadores");
        abortou = 1;
        return NULL;
    }
    return chamada->children[(*indice_arg)++];
}

static void imprimir_escape(char c)
{
    if (c == 'n')
        putchar('\n');
    else if (c == 't')
        putchar('\t');
    else if (c == '\\')
        putchar('\\');
    else if (c == '"')
        putchar('"');
    else if (c == '0')
        putchar('\0');
    else
    {
        putchar('\\');
        putchar(c);
    }
}

static void imprimir_texto(const char *texto)
{
    for (const char *p = texto; *p != '\0'; p++)
    {
        if (*p == '\\' && *(p + 1) != '\0')
        {
            p++;
            imprimir_escape(*p);
        }
        else
            putchar(*p);
    }
}

static void erro_tipo_printf(ASTNode *chamada, char especificador, const char *esperado)
{
    reportar_erro(FASE_EXECUCAO, chamada->line, "%%%c do printf espera %s", especificador, esperado);
    abortou = 1;
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
            if (*p == '\0')
                break;
            imprimir_escape(*p);
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

        if (*p == '%')
        {
            putchar('%');
            continue;
        }

        if (*p != 'd' && *p != 'f' && *p != 'c' && *p != 's')
        {
            putchar('%');
            putchar(*p);
            continue;
        }

        ASTNode *argumento = proximo_argumento_printf(chamada, &indice_arg);
        if (argumento == NULL)
            return;

        int e_texto = argumento->type == NODE_STRING_LIT;
        if (*p == 's' && !e_texto)
        {
            erro_tipo_printf(chamada, *p, "um texto entre aspas");
            return;
        }
        if (*p != 's' && e_texto)
        {
            erro_tipo_printf(chamada, *p, "um numero ou caractere, nao um texto");
            return;
        }

        if (*p == 's')
        {
            imprimir_texto(argumento->strval);
            continue;
        }

        Valor valor = interpretarNo(argumento);
        if (abortou)
            return;

        if (*p == 'd')
            printf("%d", valor_como_int(valor));
        else if (*p == 'f')
            printf("%f", valor_como_float(valor));
        else
            putchar((char)valor_como_int(valor));
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

static long limite_iteracoes(void)
{
    const char *valor = getenv("INTERPRETADOR_LIMITE_ITERACOES");
    if (valor != NULL && atol(valor) > 0)
        return atol(valor);
    return 1000000;
}

static int passou_do_limite(long *voltas, int linha)
{
    (*voltas)++;
    if (*voltas <= limite_iteracoes())
        return 0;
    reportar_erro(FASE_EXECUCAO, linha, "limite de %ld iteracoes excedido", limite_iteracoes());
    abortou = 1;
    return 1;
}

static void executar_comando(ASTNode *comando)
{
    if (comando == NULL || execucao_interrompida())
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
        else if (expr->type == NODE_CALL && strcmp(expr->strval, "scanf") == 0)
            executar_scanf(expr, 0);
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

    case NODE_IF:
        if (valor_verdadeiro(interpretarNo(comando->children[0])))
            executar_comando(comando->children[1]);
        else
            executar_comando(comando->children[2]);
        break;

    case NODE_WHILE:
    {
        long voltas = 0;
        while (!execucao_interrompida() && valor_verdadeiro(interpretarNo(comando->children[0])))
        {
            if (passou_do_limite(&voltas, comando->line))
                break;
            executar_comando(comando->children[1]);
        }
        break;
    }

    case NODE_FOR:
    {
        long voltas = 0;
        if (comando->children[0] != NULL)
            interpretarNo(comando->children[0]);
        while (!execucao_interrompida() && (comando->children[1] == NULL ||
                                            valor_verdadeiro(interpretarNo(comando->children[1]))))
        {
            if (passou_do_limite(&voltas, comando->line))
                break;
            executar_comando(comando->children[3]);
            if (!execucao_interrompida() && comando->children[2] != NULL)
                interpretarNo(comando->children[2]);
        }
        break;
    }

    case NODE_RETURN:
    {
        Valor valor = {0};
        if (comando->children[0] != NULL)
            valor = interpretarNo(comando->children[0]);
        if (abortou)
            break;
        valor_retornado = valor;
        retornou = 1;
        break;
    }

    default:
        fprintf(stderr, "Erro interno [linha %d]: no %s nao suportado na execucao\n",
                comando->line, nome_tipo_no(comando->type));
        contar_erro(FASE_EXECUCAO);
        abortou = 1;
        break;
    }
}

static ASTNode *buscar_funcao(const char *nome)
{
    for (int i = 0; programa != NULL && i < programa->num_children; i++)
    {
        ASTNode *declaracao = programa->children[i];
        if (declaracao != NULL && declaracao->type == NODE_FUNC_DECL &&
            strcmp(declaracao->strval, nome) == 0)
            return declaracao;
    }
    return NULL;
}

/* Executa o corpo da funcao num frame novo e devolve o valor do return,
   convertido para o tipo de retorno declarado. */
static Valor executar_funcao(ASTNode *funcao, const Valor *argumentos, int num_argumentos)
{
    ASTNode *parametros = funcao->children[0];
    int base_anterior = base_frame;

    entrar_escopo_exec();
    base_frame = numero_escopos - 1;

    for (int i = 0; parametros != NULL && i < parametros->num_children && i < num_argumentos; i++)
    {
        ASTNode *parametro = parametros->children[i];
        definir_variavel(parametro->strval,
                         converter_para_tipo(argumentos[i], (TipoDado)parametro->intval));
    }

    Valor zero = {0};
    valor_retornado = zero;
    executar_comando(funcao->children[1]);

    Valor resultado = retornou ? valor_retornado : zero;
    resultado = converter_para_tipo(resultado, (TipoDado)funcao->intval);
    if ((TipoDado)funcao->intval == TIPO_VOID)
        resultado.tipo = TIPO_VOID;

    /* O return termina so esta funcao; um erro (abortou) continua interrompendo tudo */
    retornou = 0;

    while (numero_escopos > base_frame)
        sair_escopo_exec();
    base_frame = base_anterior;

    return resultado;
}

static Valor chamar_funcao(ASTNode *chamada)
{
    Valor resultado = {0};
    ASTNode *funcao = buscar_funcao(chamada->strval);
    if (funcao == NULL)
    {
        reportar_erro(FASE_EXECUCAO, chamada->line, "funcao '%s' nao encontrada", chamada->strval);
        abortou = 1;
        return resultado;
    }

    if (!entrar_chamada(chamada->strval, chamada->line))
        return resultado;

    /* Os argumentos sao avaliados no escopo de quem chama, antes de abrir o frame novo */
    Valor *argumentos = NULL;
    if (chamada->num_children > 0)
    {
        argumentos = malloc(chamada->num_children * sizeof(Valor));
        if (argumentos == NULL)
        {
            fprintf(stderr, "Erro: falha ao alocar argumentos da chamada\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < chamada->num_children; i++)
    {
        argumentos[i] = interpretarNo(chamada->children[i]);
        if (abortou)
            break;
    }

    if (!abortou)
        resultado = executar_funcao(funcao, argumentos, chamada->num_children);

    free(argumentos);
    sair_chamada();
    return resultado;
}

int interpretar(ASTNode *raiz)
{
    if (raiz == NULL)
        return 0;

    programa = raiz;
    retornou = 0;
    abortou = 0;
    base_frame = 0;
    profundidade_chamadas = 0;

    /* Escopo 0: variaveis globais, definidas antes de chamar o main */
    entrar_escopo_exec();
    for (int i = 0; i < raiz->num_children && !abortou; i++)
    {
        ASTNode *declaracao = raiz->children[i];
        if (declaracao != NULL && declaracao->type == NODE_VAR_DECL)
            executar_declaracao_variavel(declaracao);
    }

    int codigo = 0;
    ASTNode *funcao_main = buscar_funcao("main");
    if (funcao_main != NULL && !abortou)
        codigo = valor_como_int(executar_funcao(funcao_main, NULL, 0));

    while (numero_escopos > 0)
        sair_escopo_exec();

    free(escopos);
    escopos = NULL;
    capacidade_escopos = 0;
    programa = NULL;
    return codigo;
}