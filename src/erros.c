#include <stdarg.h>
#include <stdio.h>
#include "erros.h"

static const char *prefixos[NUM_FASES] = {
    "Erro lexico",
    "Erro sintatico",
    "Erro semantico",
    "Erro de execucao",
};

static int contagem[NUM_FASES];

void reportar_erro(Fase fase, int linha, const char *formato, ...)
{
    va_list argumentos;

    fprintf(stderr, "%s [linha %d]: ", prefixos[fase], linha);
    va_start(argumentos, formato);
    vfprintf(stderr, formato, argumentos);
    va_end(argumentos);
    fputc('\n', stderr);

    contar_erro(fase);
}

void contar_erro(Fase fase)
{
    contagem[fase]++;
}

int erros_na_fase(Fase fase)
{
    return contagem[fase];
}
