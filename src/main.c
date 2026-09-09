#include <stdio.h>
#include "ast.h"

extern FILE *yyin;
extern ASTNode *raiz_ast;
extern int erros_lexicos;
int yyparse(void);

/* codigos de saida: 0 sucesso, 1 erro de uso/arquivo, 2 erro sintatico, 3 erro lexico */
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo.c>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Nao foi possivel abrir o arquivo: %s\n", argv[1]);
        return 1;
    }
    yyin = f;

    int resultado_sintatico = yyparse();
    fclose(f);

    if (resultado_sintatico != 0) {
        liberar_ast(raiz_ast);
        return 2;
    }

    if (erros_lexicos > 0) {
        fprintf(stderr, "\nTotal de erros lexicos: %d\n", erros_lexicos);
        liberar_ast(raiz_ast);
        return 3;
    }

    printf("Analise sintatica concluida sem erros.\n\n");
    imprimir_ast(raiz_ast, 0);
    liberar_ast(raiz_ast);
    return 0;
}
