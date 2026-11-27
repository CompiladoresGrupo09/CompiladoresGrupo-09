#ifndef ERROS_H
#define ERROS_H

typedef enum
{
    FASE_LEXICA,
    FASE_SINTATICA,
    FASE_SEMANTICA,
    FASE_EXECUCAO,
    NUM_FASES
} Fase;

#ifdef __GNUC__
__attribute__((format(printf, 3, 4)))
#endif
void reportar_erro(Fase fase, int linha, const char *formato, ...);

int erros_na_fase(Fase fase);

#endif
