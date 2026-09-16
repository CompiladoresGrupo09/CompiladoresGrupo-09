#include <stdio.h>

#include "semantic.h"
#include "tabela.h"

int erros_semanticos = 0;

static void erro_simbolo(const ASTNode *no, const char *mensagem, const char *nome) {
	fprintf(stderr, "Erro semantico [linha %d]: %s '%s'\n", no->line, mensagem, nome);
	erros_semanticos++;
}

static void visitar_no(ASTNode *no);

static const char *nome_declarado(ASTNode *declarador) {
	if (declarador->type == NODE_ID) {
		return declarador->strval;
	}

	if (declarador->type == NODE_BINOP && declarador->num_children > 0 &&
		declarador->children[0] != NULL && declarador->children[0]->type == NODE_ID) {
		return declarador->children[0]->strval;
	}

	return NULL;
}

static void visitar_declaracao_variavel(ASTNode *declaracao) {
	for (int i = 0; i < declaracao->num_children; i++) {
		ASTNode *declarador = declaracao->children[i];
		const char *nome = nome_declarado(declarador);

		if (nome == NULL) {
			continue;
		}

		if (!inserir_simbolo(nome, (TipoDado)declaracao->intval,
							 declarador->line, 0, 0)) {
			erros_semanticos++;
		}

		if (declarador->type == NODE_BINOP && declarador->num_children > 1) {
			visitar_no(declarador->children[1]);
		}
	}
}

static void visitar_bloco(ASTNode *bloco, int criar_escopo) {
	if (criar_escopo) {
		entrar_escopo();
	}

	for (int i = 0; i < bloco->num_children; i++) {
		visitar_no(bloco->children[i]);
	}

	if (criar_escopo) {
		sair_escopo();
	}
}

static void visitar_funcao(ASTNode *funcao) {
	ASTNode *parametros = funcao->num_children > 0 ? funcao->children[0] : NULL;
	ASTNode *bloco = funcao->num_children > 1 ? funcao->children[1] : NULL;
	int aridade = parametros == NULL ? 0 : parametros->num_children;

	if (!inserir_simbolo(funcao->strval, (TipoDado)funcao->intval,
						 funcao->line, 1, aridade)) {
		erros_semanticos++;
	}

	entrar_escopo();

	if (parametros != NULL) {
		for (int i = 0; i < parametros->num_children; i++) {
			ASTNode *parametro = parametros->children[i];
			if (!inserir_simbolo(parametro->strval, (TipoDado)parametro->intval,
								 parametro->line, 0, 0)) {
				erros_semanticos++;
			}
		}
	}

	if (bloco != NULL) {
		visitar_bloco(bloco, 0);
	}

	sair_escopo();
}

static void visitar_no(ASTNode *no) {
	if (no == NULL) {
		return;
	}

	switch (no->type) {
		case NODE_VAR_DECL:
			visitar_declaracao_variavel(no);
			break;

		case NODE_FUNC_DECL:
			visitar_funcao(no);
			break;

		case NODE_BLOCK:
			visitar_bloco(no, 1);
			break;

		case NODE_ID:
		case NODE_ADDR:
			if (buscar_simbolo(no->strval) == NULL) {
				erro_simbolo(no, "identificador nao declarado:", no->strval);
			}
			break;

		case NODE_CALL: {
			Simbolo *simbolo = buscar_simbolo(no->strval);
			if (simbolo == NULL) {
				erro_simbolo(no, "funcao nao declarada:", no->strval);
			} else if (!simbolo->e_funcao) {
				erro_simbolo(no, "simbolo nao e uma funcao:", no->strval);
			} else if (simbolo->aridade >= 0 && simbolo->aridade != no->num_children) {
				fprintf(stderr, "Erro semantico [linha %d]: funcao '%s' esperava %d argumento(s), recebeu %d\n",
						no->line, no->strval, simbolo->aridade, no->num_children);
				erros_semanticos++;
			}
			for (int i = 0; i < no->num_children; i++) {
				visitar_no(no->children[i]);
			}
			break;
		}

		default:
			for (int i = 0; i < no->num_children; i++) {
				visitar_no(no->children[i]);
			}
			break;
	}
}

int analisar_semantica(ASTNode *raiz) {
	erros_semanticos = 0;
	inicializar_tabela();
	inserir_simbolo("printf", TIPO_INT, 0, 1, -1);
	inserir_simbolo("scanf", TIPO_INT, 0, 1, -1);

	if (raiz != NULL) {
		for (int i = 0; i < raiz->num_children; i++) {
			visitar_no(raiz->children[i]);
		}
	}

	liberar_tabela();
	return erros_semanticos == 0;
}
