# Gramática do núcleo (alto nível)

Especificação em BNF informal da gramática do núcleo obrigatório (ver [Definição da Linguagem](definicao-da-linguagem.md)), usada como base para o `src/parser.y` (Bison). Os tokens vêm do `src/scanner.l` (Flex).

## Tokens

| Categoria | Tokens |
|---|---|
| Palavras reservadas | `int` `float` `char` `void` `if` `else` `while` `for` `return` |
| Literais | `INT_LIT` `FLOAT_LIT` `CHAR_LIT` `STRING_LIT` |
| Identificador | `ID` |
| Operadores relacionais | `==` `!=` `<` `>` `<=` `>=` |
| Operadores lógicos | `&&` `\|\|` `!` |
| Operadores aritméticos | `+` `-` `*` `/` `%` |
| Atribuição | `=` |
| Delimitadores | `(` `)` `{` `}` `;` `,` `&` |

`STRING_LIT` só é usado como primeiro argumento de `printf`/`scanf` (formato) — a linguagem não tem tipo string para variáveis. `&ID` é usado apenas como argumento de `scanf`.

## Regras de produção

```
programa            → lista_declaracoes

lista_declaracoes   → lista_declaracoes declaracao
                     | ε

declaracao          → declaracao_funcao
                     | declaracao_variavel

tipo                → 'int' | 'float' | 'char' | 'void'

declaracao_variavel → tipo lista_declaradores ';'
lista_declaradores  → lista_declaradores ',' declarador
                     | declarador
declarador          → ID
                     | ID '=' expressao

declaracao_funcao   → tipo ID '(' lista_parametros ')' bloco
lista_parametros    → lista_parametros_ne | ε
lista_parametros_ne → lista_parametros_ne ',' parametro
                     | parametro
parametro           → tipo ID

bloco               → '{' lista_comandos '}'
lista_comandos      → lista_comandos comando | ε

comando             → declaracao_variavel
                     | comando_expressao
                     | comando_if
                     | comando_while
                     | comando_for
                     | comando_return
                     | bloco

comando_expressao   → expressao ';' | ';'
comando_if          → 'if' '(' expressao ')' comando
                     | 'if' '(' expressao ')' comando 'else' comando
comando_while       → 'while' '(' expressao ')' comando
comando_for         → 'for' '(' expressao_opt ';' expressao_opt ';' expressao_opt ')' comando
comando_return      → 'return' expressao ';' | 'return' ';'
expressao_opt       → expressao | ε

expressao           → atribuicao
atribuicao          → ID '=' atribuicao
                     | expr_or
expr_or             → expr_or '||' expr_and | expr_and
expr_and            → expr_and '&&' expr_eq | expr_eq
expr_eq             → expr_eq '==' expr_rel | expr_eq '!=' expr_rel | expr_rel
expr_rel            → expr_rel ('<'|'>'|'<='|'>=') expr_add | expr_add
expr_add            → expr_add ('+'|'-') expr_mul | expr_mul
expr_mul            → expr_mul ('*'|'/'|'%') expr_unario | expr_unario
expr_unario         → '-' expr_unario | '!' expr_unario | expr_primaria
expr_primaria       → INT_LIT | FLOAT_LIT | CHAR_LIT | STRING_LIT
                     | ID
                     | ID '(' lista_argumentos ')'
                     | '(' expressao ')'

lista_argumentos    → lista_argumentos_ne | ε
lista_argumentos_ne → lista_argumentos_ne ',' argumento | argumento
argumento           → expressao | '&' ID
```

## Precedência de operadores (da menor para a maior)

1. `=` (associativo à direita)
2. `||`
3. `&&`
4. `==` `!=`
5. `<` `>` `<=` `>=`
6. `+` `-`
7. `*` `/` `%`
8. `-` unário, `!` (associativo à direita)

## Decisões de projeto

- **`if`/`else` pendurado ("dangling else")**: resolvido pela precedência natural do Bison, que prefere deslocar (`shift`) o `else` para o `if` mais próximo sem `else` — é o comportamento padrão do C e não exige regra extra na gramática (o Bison emite 1 aviso de conflito shift/reduce esperado para isso).
- **Pré-processador**: linhas começando com `#` (ex.: `#include`) são reconhecidas e ignoradas pelo scanner, sem nenhum processamento real — o núcleo não implementa pré-processador (ver [Definição da Linguagem](definicao-da-linguagem.md)).
- **`printf`/`scanf`**: não são palavras reservadas — são reconhecidos como `ID` comuns na análise sintática, e recebem tratamento especial apenas na análise semântica/interpretação (fases futuras).
