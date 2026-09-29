#include <stdio.h>
#include "ast.h"
#include "semantic.h"
#include "interpreter.h"

extern FILE *yyin;
extern ASTNode *raiz_ast;
extern int erros_lexicos;
extern int erros_sintaticos;
int yyparse(void);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s <arquivo.c>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        fprintf(stderr, "Nao foi possivel abrir o arquivo: %s\n", argv[1]);
        return 1;
    }
    yyin = f;

    int resultado_sintatico = yyparse();
    fclose(f);

    if (resultado_sintatico != 0 || erros_sintaticos > 0)
    {
        if (erros_sintaticos > 0)
        {
            fprintf(stderr, "Total de erros sintaticos: %d\n", erros_sintaticos);
        }
        liberar_ast(raiz_ast);
        return 2;
    }

    if (erros_lexicos > 0)
    {
        fprintf(stderr, "\nTotal de erros lexicos: %d\n", erros_lexicos);
        liberar_ast(raiz_ast);
        return 3;
    }

    if (!analisar_semantica(raiz_ast))
    {
        liberar_ast(raiz_ast);
        return 4;
    }

    printf("Analise lexica, sintatica e semantica concluida sem erros.\n\n");
    imprimir_ast(raiz_ast, 0);

    printf("\n");
    interpretar(raiz_ast);

    if (erros_execucao > 0)
    {
        fprintf(stderr, "\nTotal de erros de execucao: %d\n", erros_execucao);
        liberar_ast(raiz_ast);
        return 5;
    }

    liberar_ast(raiz_ast);
    return 0;
}