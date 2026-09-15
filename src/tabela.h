#ifndef TABELA_H
#define TABELA_H

#include "ast.h"

typedef struct Simbolo {
    char *nome;
    TipoDado tipo;
    int linha;
    int e_funcao;
    int aridade;
} Simbolo;

typedef struct Escopo {
    Simbolo *simbolos;
    int quantidade;
    int capacidade;
} Escopo;

typedef struct TabelaSimbolos {
    Escopo *escopos;
    int numero_escopos;
    int capacidade_escopos;
    int inicializada;
} TabelaSimbolos;

void inicializar_tabela(void);
void liberar_tabela(void);

void entrar_escopo(void);
void sair_escopo(void);

int inserir_simbolo(const char *nome, TipoDado tipo, int linha, int e_funcao, int aridade);
Simbolo *buscar_simbolo(const char *nome);
Simbolo *buscar_no_escopo_atual(const char *nome);

void imprimir_tabela(void);

#endif
