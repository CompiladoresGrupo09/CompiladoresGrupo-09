%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylineno;
extern char *yytext;
int yylex(void);
void yyerror(const char *msg);

ASTNode *raiz_ast = NULL;
%}

%union {
    int intval;
    float floatval;
    char charval;
    char *strval;
    struct ASTNode *node;
}

%token <intval> INT_LIT
%token <floatval> FLOAT_LIT
%token <charval> CHAR_LIT
%token <strval> STRING_LIT
%token <strval> ID

%token INT FLOAT CHAR VOID IF ELSE WHILE FOR RETURN
%token EQ NE LE GE AND OR

%locations

%type <node> programa lista_declaracoes declaracao declaracao_funcao declaracao_variavel
%type <node> lista_declaradores declarador lista_parametros lista_parametros_ne parametro
%type <node> bloco lista_comandos comando comando_expressao comando_if comando_while comando_for comando_return
%type <node> expressao_opt expressao atribuicao expr_or expr_and expr_eq expr_rel expr_add expr_mul expr_unario expr_primaria
%type <node> lista_argumentos lista_argumentos_ne argumento
%type <intval> tipo

%right '='
%left OR
%left AND
%left EQ NE
%left '<' '>' LE GE
%left '+' '-'
%left '*' '/' '%'
%right UMINUS NOT

%start programa

%%

programa:
      lista_declaracoes { raiz_ast = $1; $$ = $1; }
    ;

lista_declaracoes:
      lista_declaracoes declaracao { adicionar_filho($1, $2); $$ = $1; }
    | /* vazio */ { $$ = criar_no(NODE_PROGRAM, yylineno); }
    ;

declaracao:
      declaracao_funcao { $$ = $1; }
    | declaracao_variavel { $$ = $1; }
    ;

tipo:
      INT   { $$ = TIPO_INT; }
    | FLOAT { $$ = TIPO_FLOAT; }
    | CHAR  { $$ = TIPO_CHAR; }
    | VOID  { $$ = TIPO_VOID; }
    ;

declaracao_variavel:
      tipo lista_declaradores ';' {
          $2->type = NODE_VAR_DECL;
          $2->intval = $1;
          $2->line = @$.first_line;
          $$ = $2;
      }
    ;

lista_declaradores:
      lista_declaradores ',' declarador { adicionar_filho($1, $3); $$ = $1; }
    | declarador {
          $$ = criar_no(NODE_LIST, yylineno);
          adicionar_filho($$, $1);
      }
    ;

declarador:
      ID { $$ = criar_no_id($1, yylineno); }
    | ID '=' expressao {
          ASTNode *id = criar_no_id($1, yylineno);
          $$ = criar_no_binop("=", id, $3, yylineno);
      }
    ;

declaracao_funcao:
      tipo ID '(' lista_parametros ')' bloco {
          ASTNode *no = criar_no(NODE_FUNC_DECL, @$.first_line);
          no->intval = $1;
          no->strval = strdup($2);
          adicionar_filho(no, $4);
          adicionar_filho(no, $6);
          $$ = no;
      }
    ;

lista_parametros:
      lista_parametros_ne { $$ = $1; }
    | /* vazio */ { $$ = criar_no(NODE_LIST, yylineno); }
    ;

lista_parametros_ne:
      lista_parametros_ne ',' parametro { adicionar_filho($1, $3); $$ = $1; }
    | parametro {
          $$ = criar_no(NODE_LIST, yylineno);
          adicionar_filho($$, $1);
      }
    ;

parametro:
      tipo ID {
          ASTNode *no = criar_no(NODE_PARAM, yylineno);
          no->intval = $1;
          no->strval = strdup($2);
          $$ = no;
      }
    ;

bloco:
      '{' lista_comandos '}' { $$ = $2; }
    ;

lista_comandos:
      lista_comandos comando { adicionar_filho($1, $2); $$ = $1; }
    | /* vazio */ { $$ = criar_no(NODE_BLOCK, yylineno); }
    ;

comando:
      declaracao_variavel { $$ = $1; }
    | comando_expressao   { $$ = $1; }
    | comando_if          { $$ = $1; }
    | comando_while       { $$ = $1; }
    | comando_for         { $$ = $1; }
    | comando_return      { $$ = $1; }
    | bloco               { $$ = $1; }
    ;

comando_expressao:
      expressao ';' {
          $$ = criar_no(NODE_EXPR_STMT, yylineno);
          adicionar_filho($$, $1);
      }
    | ';' { $$ = criar_no(NODE_EXPR_STMT, yylineno); adicionar_filho($$, NULL); }
    ;

comando_if:
      IF '(' expressao ')' comando ELSE comando {
          $$ = criar_no(NODE_IF, @$.first_line);
          adicionar_filho($$, $3);
          adicionar_filho($$, $5);
          adicionar_filho($$, $7);
      }
    | IF '(' expressao ')' comando {
          $$ = criar_no(NODE_IF, @$.first_line);
          adicionar_filho($$, $3);
          adicionar_filho($$, $5);
          adicionar_filho($$, NULL);
      }
    ;

comando_while:
      WHILE '(' expressao ')' comando {
          $$ = criar_no(NODE_WHILE, @$.first_line);
          adicionar_filho($$, $3);
          adicionar_filho($$, $5);
      }
    ;

comando_for:
      FOR '(' expressao_opt ';' expressao_opt ';' expressao_opt ')' comando {
          $$ = criar_no(NODE_FOR, @$.first_line);
          adicionar_filho($$, $3);
          adicionar_filho($$, $5);
          adicionar_filho($$, $7);
          adicionar_filho($$, $9);
      }
    ;

expressao_opt:
      expressao { $$ = $1; }
    | /* vazio */ { $$ = NULL; }
    ;

comando_return:
      RETURN expressao ';' {
          $$ = criar_no(NODE_RETURN, yylineno);
          adicionar_filho($$, $2);
      }
    | RETURN ';' {
          $$ = criar_no(NODE_RETURN, yylineno);
          adicionar_filho($$, NULL);
      }
    ;

expressao:
      atribuicao { $$ = $1; }
    ;

atribuicao:
      ID '=' atribuicao {
          ASTNode *id = criar_no_id($1, yylineno);
          $$ = criar_no_binop("=", id, $3, yylineno);
      }
    | expr_or { $$ = $1; }
    ;

expr_or:
      expr_or OR expr_and { $$ = criar_no_binop("||", $1, $3, yylineno); }
    | expr_and { $$ = $1; }
    ;

expr_and:
      expr_and AND expr_eq { $$ = criar_no_binop("&&", $1, $3, yylineno); }
    | expr_eq { $$ = $1; }
    ;

expr_eq:
      expr_eq EQ expr_rel { $$ = criar_no_binop("==", $1, $3, yylineno); }
    | expr_eq NE expr_rel { $$ = criar_no_binop("!=", $1, $3, yylineno); }
    | expr_rel { $$ = $1; }
    ;

expr_rel:
      expr_rel '<' expr_add { $$ = criar_no_binop("<", $1, $3, yylineno); }
    | expr_rel '>' expr_add { $$ = criar_no_binop(">", $1, $3, yylineno); }
    | expr_rel LE expr_add  { $$ = criar_no_binop("<=", $1, $3, yylineno); }
    | expr_rel GE expr_add  { $$ = criar_no_binop(">=", $1, $3, yylineno); }
    | expr_add { $$ = $1; }
    ;

expr_add:
      expr_add '+' expr_mul { $$ = criar_no_binop("+", $1, $3, yylineno); }
    | expr_add '-' expr_mul { $$ = criar_no_binop("-", $1, $3, yylineno); }
    | expr_mul { $$ = $1; }
    ;

expr_mul:
      expr_mul '*' expr_unario { $$ = criar_no_binop("*", $1, $3, yylineno); }
    | expr_mul '/' expr_unario { $$ = criar_no_binop("/", $1, $3, yylineno); }
    | expr_mul '%' expr_unario { $$ = criar_no_binop("%", $1, $3, yylineno); }
    | expr_unario { $$ = $1; }
    ;

expr_unario:
      '-' expr_unario %prec UMINUS { $$ = criar_no_unop("-", $2, yylineno); }
    | '!' expr_unario %prec NOT    { $$ = criar_no_unop("!", $2, yylineno); }
    | expr_primaria { $$ = $1; }
    ;

expr_primaria:
      INT_LIT    { $$ = criar_no_int($1, yylineno); }
    | FLOAT_LIT  { $$ = criar_no_float($1, yylineno); }
    | CHAR_LIT   { $$ = criar_no_char($1, yylineno); }
    | STRING_LIT { $$ = criar_no_string($1, yylineno); }
    | ID         { $$ = criar_no_id($1, yylineno); }
    | ID '(' lista_argumentos ')' {
          $3->type = NODE_CALL;
          $3->strval = strdup($1);
          $$ = $3;
      }
    | '(' expressao ')' { $$ = $2; }
    ;

lista_argumentos:
      lista_argumentos_ne { $$ = $1; }
    | /* vazio */ { $$ = criar_no(NODE_LIST, yylineno); }
    ;

lista_argumentos_ne:
      lista_argumentos_ne ',' argumento { adicionar_filho($1, $3); $$ = $1; }
    | argumento {
          $$ = criar_no(NODE_LIST, yylineno);
          adicionar_filho($$, $1);
      }
    ;

argumento:
      expressao { $$ = $1; }
    | '&' ID    { $$ = criar_no_addr($2, yylineno); }
    ;

%%

void yyerror(const char *msg) {
    fprintf(stderr, "Erro sintatico [linha %d]: %s (proximo a '%s')\n", yylineno, msg, yytext);
}

#ifdef PARSER_TEST_MAIN
extern FILE *yyin;

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            fprintf(stderr, "Nao foi possivel abrir o arquivo: %s\n", argv[1]);
            return 1;
        }
        yyin = f;
    }

    int resultado = yyparse();

    if (argc > 1) {
        fclose(yyin);
    }

    if (resultado == 0) {
        printf("Analise sintatica concluida sem erros.\n\n");
        imprimir_ast(raiz_ast, 0);
        liberar_ast(raiz_ast);
    }

    return resultado;
}
#endif
