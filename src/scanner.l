%{
#include <stdio.h>
#include <stdlib.h>

static int erros_lexicos = 0;

static void erro_lexico(const char *msg, const char *lexema, int linha) {
    fprintf(stderr, "Erro lexico [linha %d]: %s '%s'\n", linha, msg, lexema);
    erros_lexicos++;
}
%}

%option noyywrap
%option yylineno

DIGITO      [0-9]
LETRA       [a-zA-Z_]
INTEIRO     {DIGITO}+
FLOAT       {DIGITO}+"."{DIGITO}+
ID          {LETRA}({LETRA}|{DIGITO})*
CHAR_LIT    '(\\.|[^\\'])'
STRING_LIT  \"(\\.|[^"\\])*\"

%%

"//".*                          { /* comentario de linha: ignora */ }
"/*"([^*]|\*+[^*/])*\*+"/"      { /* comentario de bloco: ignora */ }
"#".*                           { /* diretiva de pre-processador: ignorada, fora de escopo do nucleo */ }

[ \t\r]+                        { /* espaco em branco: ignora */ }
\n                               { /* nova linha: yylineno ja conta automaticamente */ }

"int"                            { printf("PALAVRA_RESERVADA(int) linha %d\n", yylineno); }
"float"                          { printf("PALAVRA_RESERVADA(float) linha %d\n", yylineno); }
"char"                           { printf("PALAVRA_RESERVADA(char) linha %d\n", yylineno); }
"void"                           { printf("PALAVRA_RESERVADA(void) linha %d\n", yylineno); }
"if"                             { printf("PALAVRA_RESERVADA(if) linha %d\n", yylineno); }
"else"                           { printf("PALAVRA_RESERVADA(else) linha %d\n", yylineno); }
"while"                          { printf("PALAVRA_RESERVADA(while) linha %d\n", yylineno); }
"for"                            { printf("PALAVRA_RESERVADA(for) linha %d\n", yylineno); }
"return"                         { printf("PALAVRA_RESERVADA(return) linha %d\n", yylineno); }

{FLOAT}                          { printf("FLOAT_LIT(%s) linha %d\n", yytext, yylineno); }
{INTEIRO}                        { printf("INT_LIT(%s) linha %d\n", yytext, yylineno); }
{CHAR_LIT}                       { printf("CHAR_LIT(%s) linha %d\n", yytext, yylineno); }
{STRING_LIT}                     { printf("STRING_LIT(%s) linha %d\n", yytext, yylineno); }
{ID}                             { printf("ID(%s) linha %d\n", yytext, yylineno); }

"=="                             { printf("OP_REL(==) linha %d\n", yylineno); }
"!="                             { printf("OP_REL(!=) linha %d\n", yylineno); }
"<="                             { printf("OP_REL(<=) linha %d\n", yylineno); }
">="                             { printf("OP_REL(>=) linha %d\n", yylineno); }
"<"                              { printf("OP_REL(<) linha %d\n", yylineno); }
">"                              { printf("OP_REL(>) linha %d\n", yylineno); }
"&&"                             { printf("OP_LOG(&&) linha %d\n", yylineno); }
"||"                             { printf("OP_LOG(||) linha %d\n", yylineno); }
"!"                              { printf("OP_LOG(!) linha %d\n", yylineno); }
"="                              { printf("OP_ATRIB(=) linha %d\n", yylineno); }
"+"                              { printf("OP_ARIT(+) linha %d\n", yylineno); }
"-"                              { printf("OP_ARIT(-) linha %d\n", yylineno); }
"*"                              { printf("OP_ARIT(*) linha %d\n", yylineno); }
"/"                              { printf("OP_ARIT(/) linha %d\n", yylineno); }
"%"                              { printf("OP_ARIT(%%) linha %d\n", yylineno); }

"("                              { printf("DELIM(() linha %d\n", yylineno); }
")"                              { printf("DELIM()) linha %d\n", yylineno); }
"{"                              { printf("DELIM({) linha %d\n", yylineno); }
"}"                              { printf("DELIM(}) linha %d\n", yylineno); }
";"                              { printf("DELIM(;) linha %d\n", yylineno); }
","                              { printf("DELIM(,) linha %d\n", yylineno); }
"&"                              { printf("DELIM(&) linha %d\n", yylineno); }

.                                { erro_lexico("caractere invalido", yytext, yylineno); }

%%

#ifdef SCANNER_TEST_MAIN
int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            fprintf(stderr, "Nao foi possivel abrir o arquivo: %s\n", argv[1]);
            return 1;
        }
        yyin = f;
    }

    yylex();

    if (argc > 1) {
        fclose(yyin);
    }

    if (erros_lexicos > 0) {
        fprintf(stderr, "\nTotal de erros lexicos: %d\n", erros_lexicos);
        return 1;
    }

    return 0;
}
#endif
