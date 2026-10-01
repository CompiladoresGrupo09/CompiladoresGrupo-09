#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "semantic.h"
#include "tabela.h"
#include "interpreter.h"
#include "parser.tab.h"

extern FILE *yyin;
extern char *yytext;
extern int yylineno;
extern ASTNode *raiz_ast;
extern int erros_lexicos;
extern int erros_sintaticos;
int yyparse(void);
int yylex(void);

/* Codigos de saida: 0 = sucesso; os demais indicam a fase em que o pipeline parou */
#define SAIDA_USO 1
#define SAIDA_SINTATICO 2
#define SAIDA_LEXICO 3
#define SAIDA_SEMANTICO 4
#define SAIDA_EXECUCAO 5

typedef enum
{
    MODO_COMPLETO,
    MODO_TOKENS,
    MODO_AST,
    MODO_TABELA
} Modo;

/* Erros por fase; NAO_EXECUTADA marca as fases que o pipeline nao chegou a rodar */
#define NAO_EXECUTADA -1
enum { FASE_LEXICA, FASE_SINTATICA, FASE_SEMANTICA, FASE_EXECUCAO, NUM_FASES };

static const char *nomes_fases[NUM_FASES] = {"Lexica", "Sintatica", "Semantica", "Execucao"};
static int erros_fase[NUM_FASES] = {NAO_EXECUTADA, NAO_EXECUTADA, NAO_EXECUTADA, NAO_EXECUTADA};

static void imprimir_relatorio(void)
{
    fprintf(stderr, "\n==== Relatorio de erros ====\n");
    for (int i = 0; i < NUM_FASES; i++)
    {
        if (erros_fase[i] == NAO_EXECUTADA)
            fprintf(stderr, "%-10s nao executada\n", nomes_fases[i]);
        else
            fprintf(stderr, "%-10s %d erro(s)\n", nomes_fases[i], erros_fase[i]);
    }
}

static void imprimir_uso(const char *programa)
{
    fprintf(stderr, "Uso: %s <arquivo.c> [--tokens | --ast | --tabela]\n", programa);
    fprintf(stderr, "  (sem flag)  pipeline completo: analises lexica, sintatica e semantica + execucao\n");
    fprintf(stderr, "  --tokens    apenas a analise lexica, listando os tokens\n");
    fprintf(stderr, "  --ast       analises completas e impressao da AST, sem executar\n");
    fprintf(stderr, "  --tabela    analises completas e impressao da tabela de simbolos, sem executar\n");
}

static const char *nome_token(int token)
{
    switch (token)
    {
    case INT_LIT: return "INT_LIT";
    case FLOAT_LIT: return "FLOAT_LIT";
    case CHAR_LIT: return "CHAR_LIT";
    case STRING_LIT: return "STRING_LIT";
    case ID: return "ID";
    case INT: return "INT";
    case FLOAT: return "FLOAT";
    case CHAR: return "CHAR";
    case VOID: return "VOID";
    case IF: return "IF";
    case ELSE: return "ELSE";
    case WHILE: return "WHILE";
    case FOR: return "FOR";
    case RETURN: return "RETURN";
    case EQ: return "EQ";
    case NE: return "NE";
    case LE: return "LE";
    case GE: return "GE";
    case AND: return "AND";
    case OR: return "OR";
    default: return "SIMBOLO";
    }
}

static void listar_tokens(void)
{
    int token;

    printf("%-6s %-11s %s\n", "LINHA", "TOKEN", "LEXEMA");
    while ((token = yylex()) != 0)
    {
        printf("%-6d %-11s %s\n", yylineno, nome_token(token), yytext);
        if (token == ID || token == STRING_LIT)
            free(yylval.strval);
    }
}

static int encerrar(int codigo)
{
    imprimir_relatorio();
    liberar_ast(raiz_ast);
    liberar_simbolos_declarados();
    return codigo;
}

int main(int argc, char **argv)
{
    const char *arquivo = NULL;
    Modo modo = MODO_COMPLETO;

    for (int i = 1; i < argc; i++)
    {
        Modo flag = MODO_COMPLETO;

        if (strcmp(argv[i], "--tokens") == 0)
            flag = MODO_TOKENS;
        else if (strcmp(argv[i], "--ast") == 0)
            flag = MODO_AST;
        else if (strcmp(argv[i], "--tabela") == 0)
            flag = MODO_TABELA;
        else if (strncmp(argv[i], "--", 2) == 0 || arquivo != NULL)
        {
            imprimir_uso(argv[0]);
            return SAIDA_USO;
        }
        else
        {
            arquivo = argv[i];
            continue;
        }

        if (modo != MODO_COMPLETO)
        {
            imprimir_uso(argv[0]);
            return SAIDA_USO;
        }
        modo = flag;
    }

    if (arquivo == NULL)
    {
        imprimir_uso(argv[0]);
        return SAIDA_USO;
    }

    FILE *f = fopen(arquivo, "r");
    if (!f)
    {
        fprintf(stderr, "Nao foi possivel abrir o arquivo: %s\n", arquivo);
        return SAIDA_USO;
    }
    yyin = f;

    /* Fase 1 isolada: so o scanner */
    if (modo == MODO_TOKENS)
    {
        listar_tokens();
        fclose(f);
        erros_fase[FASE_LEXICA] = erros_lexicos;
        return encerrar(erros_lexicos > 0 ? SAIDA_LEXICO : 0);
    }

    /* Fases 1 e 2: o parser chama o scanner, entao as duas rodam juntas */
    int resultado_sintatico = yyparse();
    fclose(f);
    erros_fase[FASE_LEXICA] = erros_lexicos;
    erros_fase[FASE_SINTATICA] = erros_sintaticos > 0 ? erros_sintaticos : (resultado_sintatico != 0);

    if (erros_lexicos > 0)
        return encerrar(SAIDA_LEXICO);
    if (erros_fase[FASE_SINTATICA] > 0)
        return encerrar(SAIDA_SINTATICO);

    /* Fase 3: analise semantica (tabela de simbolos + tipos) */
    int semantica_ok = analisar_semantica(raiz_ast);
    erros_fase[FASE_SEMANTICA] = erros_semanticos;

    if (modo == MODO_TABELA)
        imprimir_simbolos_declarados();
    if (!semantica_ok)
        return encerrar(SAIDA_SEMANTICO);

    if (modo == MODO_AST)
        imprimir_ast(raiz_ast, 0);
    if (modo != MODO_COMPLETO)
        return encerrar(0);

    /* Fase 4: execucao */
    interpretar(raiz_ast);
    erros_fase[FASE_EXECUCAO] = erros_execucao;

    return encerrar(erros_execucao > 0 ? SAIDA_EXECUCAO : 0);
}
