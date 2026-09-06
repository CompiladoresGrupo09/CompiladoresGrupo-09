%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "parser.tab.h"

int erros_lexicos = 0;

static void erro_lexico(const char *msg, const char *lexema, int linha) {
    fprintf(stderr, "Erro lexico [linha %d]: %s '%s'\n", linha, msg, lexema);
    erros_lexicos++;
}

/* texto inclui as aspas simples, ex.: 'a' ou '\n' */
static char decodificar_char(const char *texto) {
    if (texto[1] == '\\') {
        switch (texto[2]) {
            case 'n': return '\n';
            case 't': return '\t';
            case '0': return '\0';
            case '\\': return '\\';
            case '\'': return '\'';
            case '"': return '"';
            default: return texto[2];
        }
    }
    return texto[1];
}

/* remove as aspas duplas; as sequencias de escape ficam literais e sao
   decodificadas depois, na fase de interpretacao do printf/scanf */
static char *extrair_string(const char *texto) {
    int tam = strlen(texto);
    char *resultado = malloc(tam - 1);
    strncpy(resultado, texto + 1, tam - 2);
    resultado[tam - 2] = '\0';
    return resultado;
}
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno;
%}

%option noyywrap
%option yylineno

DIGITO      [0-9]
LETRA       [a-zA-Z_]
NUM_INT     {DIGITO}+
NUM_FLOAT   {DIGITO}+"."{DIGITO}+
IDENT       {LETRA}({LETRA}|{DIGITO})*
LIT_CHAR    '(\\.|[^\\'])'
LIT_STRING  \"(\\.|[^"\\])*\"

%%

"//".*                          { /* comentario de linha: ignora */ }
"/*"([^*]|\*+[^*/])*\*+"/"      { /* comentario de bloco: ignora */ }
"#".*                           { /* diretiva de pre-processador: ignorada, fora de escopo do nucleo */ }

[ \t\r]+                        { /* espaco em branco: ignora */ }
\n                               { /* nova linha: yylineno ja conta automaticamente */ }

"int"                            { return INT; }
"float"                          { return FLOAT; }
"char"                           { return CHAR; }
"void"                           { return VOID; }
"if"                             { return IF; }
"else"                           { return ELSE; }
"while"                          { return WHILE; }
"for"                            { return FOR; }
"return"                         { return RETURN; }

{NUM_FLOAT}                      { yylval.floatval = atof(yytext); return FLOAT_LIT; }
{NUM_INT}                        { yylval.intval = atoi(yytext); return INT_LIT; }
{LIT_CHAR}                       { yylval.charval = decodificar_char(yytext); return CHAR_LIT; }
{LIT_STRING}                     { yylval.strval = extrair_string(yytext); return STRING_LIT; }
{IDENT}                          { yylval.strval = strdup(yytext); return ID; }

"=="                             { return EQ; }
"!="                             { return NE; }
"<="                             { return LE; }
">="                             { return GE; }
"&&"                             { return AND; }
"||"                             { return OR; }

"<"                              { return '<'; }
">"                              { return '>'; }
"!"                              { return '!'; }
"="                              { return '='; }
"+"                              { return '+'; }
"-"                              { return '-'; }
"*"                              { return '*'; }
"/"                              { return '/'; }
"%"                              { return '%'; }

"("                              { return '('; }
")"                              { return ')'; }
"{"                              { return '{'; }
"}"                              { return '}'; }
";"                              { return ';'; }
","                              { return ','; }
"&"                              { return '&'; }

.                                { erro_lexico("caractere invalido", yytext, yylineno); }

%%
