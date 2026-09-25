#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tabela.h"

static TabelaSimbolos tabela = {0};

static void garantir_capacidade_escopos(void) {
    if (tabela.numero_escopos < tabela.capacidade_escopos) {
        return;
    }

    int nova_capacidade = tabela.capacidade_escopos == 0 ? 4 : tabela.capacidade_escopos * 2;
    Escopo *novo_escopos = realloc(tabela.escopos, nova_capacidade * sizeof(Escopo));
    if (!novo_escopos) {
        fprintf(stderr, "Erro: falha ao alocar escopos da tabela de simbolos\n");
        exit(EXIT_FAILURE);
    }

    tabela.escopos = novo_escopos;
    tabela.capacidade_escopos = nova_capacidade;
}

static void garantir_capacidade_simbolos(Escopo *escopo) {
    if (escopo->quantidade < escopo->capacidade) {
        return;
    }

    int nova_capacidade = escopo->capacidade == 0 ? 4 : escopo->capacidade * 2;
    Simbolo *novos_simbolos = realloc(escopo->simbolos, nova_capacidade * sizeof(Simbolo));
    if (!novos_simbolos) {
        fprintf(stderr, "Erro: falha ao alocar simbolos do escopo\n");
        exit(EXIT_FAILURE);
    }

    escopo->simbolos = novos_simbolos;
    escopo->capacidade = nova_capacidade;
}

void inicializar_tabela(void) {
    tabela.numero_escopos = 0;
    tabela.capacidade_escopos = 0;
    tabela.escopos = NULL;
    tabela.inicializada = 1;
    entrar_escopo();
}

void liberar_tabela(void) {
    if (!tabela.inicializada) {
        return;
    }

    while (tabela.numero_escopos > 0) {
        sair_escopo();
    }

    free(tabela.escopos);
    tabela.escopos = NULL;
    tabela.capacidade_escopos = 0;
    tabela.numero_escopos = 0;
    tabela.inicializada = 0;
}

void entrar_escopo(void) {
    garantir_capacidade_escopos();

    Escopo *escopo = &tabela.escopos[tabela.numero_escopos++];
    escopo->simbolos = NULL;
    escopo->quantidade = 0;
    escopo->capacidade = 0;
}

void sair_escopo(void) {
    if (tabela.numero_escopos == 0) {
        return;
    }

    Escopo *escopo = &tabela.escopos[tabela.numero_escopos - 1];

    for (int i = 0; i < escopo->quantidade; i++) {
        free(escopo->simbolos[i].nome);
    }

    free(escopo->simbolos);
    escopo->simbolos = NULL;
    escopo->quantidade = 0;
    escopo->capacidade = 0;

    tabela.numero_escopos--;
}

int inserir_simbolo(const char *nome, TipoDado tipo, int linha, int e_funcao, int aridade) {
    if (!tabela.inicializada) {
        inicializar_tabela();
    }

    Escopo *escopo_atual = &tabela.escopos[tabela.numero_escopos - 1];

    if (buscar_no_escopo_atual(nome) != NULL) {
        fprintf(stderr, "Erro semantico [linha %d]: simbolo '%s' ja declarado no escopo atual\n",
                linha, nome);
        return 0;
    }

    garantir_capacidade_simbolos(escopo_atual);

    Simbolo *simbolo = &escopo_atual->simbolos[escopo_atual->quantidade++];
    simbolo->nome = strdup(nome);
    simbolo->tipo = tipo;
    simbolo->linha = linha;
    simbolo->e_funcao = e_funcao;
    simbolo->aridade = aridade;

    return 1;
}

Simbolo *buscar_simbolo(const char *nome) {
    if (!tabela.inicializada || tabela.numero_escopos == 0) {
        return NULL;
    }

    for (int i = tabela.numero_escopos - 1; i >= 0; i--) {
        for (int j = 0; j < tabela.escopos[i].quantidade; j++) {
            if (strcmp(tabela.escopos[i].simbolos[j].nome, nome) == 0) {
                return &tabela.escopos[i].simbolos[j];
            }
        }
    }

    return NULL;
}

Simbolo *buscar_no_escopo_atual(const char *nome) {
    if (!tabela.inicializada || tabela.numero_escopos == 0) {
        return NULL;
    }

    Escopo *escopo_atual = &tabela.escopos[tabela.numero_escopos - 1];

    for (int i = 0; i < escopo_atual->quantidade; i++) {
        if (strcmp(escopo_atual->simbolos[i].nome, nome) == 0) {
            return &escopo_atual->simbolos[i];
        }
    }

    return NULL;
}

void imprimir_tabela(void) {
    if (!tabela.inicializada || tabela.numero_escopos == 0) {
        printf("Tabela de simbolos vazia.\n");
        return;
    }

    printf("Tabela de simbolos:\n");
    for (int i = 0; i < tabela.numero_escopos; i++) {
        printf("  Escopo %d\n", i);
        for (int j = 0; j < tabela.escopos[i].quantidade; j++) {
            Simbolo *simbolo = &tabela.escopos[i].simbolos[j];
            printf("    - %s | tipo=%d | linha=%d | funcao=%d | aridade=%d\n",
                   simbolo->nome,
                   simbolo->tipo,
                   simbolo->linha,
                   simbolo->e_funcao,
                   simbolo->aridade);
        }
    }
}
