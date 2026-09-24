#include <stdio.h>
#include <string.h>

#include "semantic.h"
#include "tabela.h"

int erros_semanticos = 0;

static void erro_semantico(int linha, const char *mensagem) {
    fprintf(stderr, "Erro semantico [linha %d]: %s\n", linha, mensagem);
    erros_semanticos++;
}

static void erro_simbolo(const ASTNode *no, const char *mensagem, const char *nome) {
	fprintf(stderr, "Erro semantico [linha %d]: %s '%s'\n", no->line, mensagem, nome);
	erros_semanticos++;
}

static TipoDado inferir_tipo(ASTNode *no);
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
	TipoDado tipo_var = (TipoDado)declaracao->intval;

	for (int i = 0; i < declaracao->num_children; i++) {
		ASTNode *declarador = declaracao->children[i];
		const char *nome = nome_declarado(declarador);

		if (nome == NULL) {
			continue;
		}

		if (!inserir_simbolo(nome, tipo_var,
							 declarador->line, 0, 0)) {
			erros_semanticos++;
		}
        
		// caso haja iniciialização na declaração//
		if (declarador->type == NODE_BINOP && declarador->num_children > 1) {
			TipoDado tipo_expr = inferir_tipo(declarador->children[1]);

			if (tipo_var == TIPO_INT && tipo_expr == TIPO_FLOAT) {
				erro_semantico(declarador->line, "atribuicao incompativel: conversao implicita de float para int não permitida");
			}
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

static TipoDado inferir_tipo(ASTNode *no) {
    if (no == NULL) {
        return TIPO_VOID;
    }

    switch (no->type) {
        case NODE_INT_LIT:
            no->tipo = TIPO_INT;
            return no->tipo;

        case NODE_FLOAT_LIT:
            no->tipo = TIPO_FLOAT;
            return no->tipo;

        case NODE_CHAR_LIT:
            no->tipo = TIPO_CHAR;
            return no->tipo;

        case NODE_ID:
        case NODE_ADDR: {
            Simbolo *simbolo = buscar_simbolo(no->strval);
            if (simbolo == NULL) {
                erro_simbolo(no, "identificador nao declarado:", no->strval);
                no->tipo = TIPO_VOID;
            } else {
                no->tipo = simbolo->tipo;
            }
            return no->tipo;
        }

        case NODE_CALL: {
            Simbolo *simbolo = buscar_simbolo(no->strval);
            if (simbolo == NULL) {
                erro_simbolo(no, "funcao nao declarada:", no->strval);
                no->tipo = TIPO_VOID;
            } else {
                no->tipo = simbolo->tipo;
                if (!simbolo->e_funcao) {
                    erro_simbolo(no, "simbolo nao e uma funcao:", no->strval);
                } else if (simbolo->aridade >= 0 && simbolo->aridade != no->num_children) {
                    fprintf(stderr, "Erro semantico [linha %d]: funcao '%s' esperava %d argumento(s), recebeu %d\n",
                            no->line, no->strval, simbolo->aridade, no->num_children);
                    erros_semanticos++;
                }
            }
            // tipos de arguentos //
            for (int i = 0; i < no->num_children; i++) {
                inferir_tipo(no->children[i]);
            }
            return no->tipo;
        }

        case NODE_UNOP: {
            if (no->num_children > 0) {
                TipoDado t_op = inferir_tipo(no->children[0]);
                // negação, sem´pre 1 ou 0 //
                if (no->strval != NULL && strcmp(no->strval, "!") == 0) {
                    no->tipo = TIPO_INT;
                } else {
                    no->tipo = t_op;
                }
            } else {
                no->tipo = TIPO_VOID;
            }
            return no->tipo;
        }

        case NODE_BINOP: {
            if (no->num_children < 2) {
                no->tipo = TIPO_VOID;
                return no->tipo;
            }

            const char *op = no->strval;

            if (op != NULL && strcmp(op, "=") == 0) {
                TipoDado t_esq = inferir_tipo(no->children[0]);
                TipoDado t_dir = inferir_tipo(no->children[1]);

                // recusar float para int //
                if (t_esq == TIPO_INT && t_dir == TIPO_FLOAT) {
                    erro_semantico(no->line, "atribuicao incompativel: conversao de float para int recusada");
                }
                no->tipo = t_esq;
                return no->tipo;
            }

            TipoDado t_esq = inferir_tipo(no->children[0]);
            TipoDado t_dir = inferir_tipo(no->children[1]);

            // resto da divisão apenas para inteiros //
            if (op != NULL && strcmp(op, "%") == 0) {
                if (t_esq != TIPO_INT || t_dir != TIPO_INT) {
                    erro_semantico(no->line, "operador '%' invalido: esperado tipos inteiros");
                }
                no->tipo = TIPO_INT;
                return no->tipo;
            }

            if (op != NULL && (
                strcmp(op, "==") == 0 || strcmp(op, "!=") == 0 ||
                strcmp(op, "<")  == 0 || strcmp(op, ">")  == 0 ||
                strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0 ||
                strcmp(op, "&&") == 0 || strcmp(op, "||") == 0)) {
                no->tipo = TIPO_INT;
                return no->tipo;
            }

            if (t_esq == TIPO_FLOAT || t_dir == TIPO_FLOAT) {
                no->tipo = TIPO_FLOAT;
            } else if (t_esq == TIPO_INT || t_dir == TIPO_INT) {
                no->tipo = TIPO_INT;
            } else {
                no->tipo = t_esq;
            }
            return no->tipo;
        }

        default:
            for (int i = 0; i < no->num_children; i++) {
                inferir_tipo(no->children[i]);
            }
            no->tipo = TIPO_VOID;
            return no->tipo;
    }
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
