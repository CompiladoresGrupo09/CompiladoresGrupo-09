## Estudo de Caso: Boletim Escolar (`S7-08`)

### Objetivo e recursos exercitados
Programa em C (`tests/validos/estudo_boletim.c`) voltado à validação de ponta a ponta do interpretador utilizando apenas recursos do núcleo da linguagem (sem vetores, ponteiros, `structs` ou `%.2f`):
* Leitura de três notas em `float` com `scanf("%f", ...)`
* Cálculo da média na função `float media(float a, float b, float c)` e promoção implícita `int -> float`
* Classificação por `if / else` aninhado com operadores relacionais (`>= 7` para `APROVADO`, `>= 5` para `RECUPERACAO` e `REPROVADO` caso contrário)
* Impressão de notas, média e situação com `printf` (`%f` e `%s`)

### Cenários e arquivos de teste
* **Válido principal (`tests/validos/estudo_boletim.c`):**
  * Cenário aprovado (`8.0`, `7.0`, `9.0` -> média `8.000000`, `APROVADO`) com `.entrada` e `.esperado` correspondentes (incluindo `estudo_boletim.entrada` e `estudo_boletim.esperado` para execução automática via `./run_tests.sh`)
  * Cenário recuperação (`5.0`, `6.0`, `4.0` -> média `5.000000`, `RECUPERACAO`) com `.entrada` e `.esperado`
  * Cenário reprovado (`5.0`, `4.0`, `4.0` -> média `4.333333`, `REPROVADO`) com `.entrada` e `.esperado`
* **Inválido (`tests/invalidos/estudo_boletim_erro_tipo.c`):**
  * Atribuição do retorno `float` de `media(...)` a uma variável declarada como `int`, validada com `estudo_boletim_erro_tipo.esperado` (erro semântico e código de saída diferente de `0`)

### Validação e execução
1. Saídas dos cenários conferidas a partir da execução de um programa C real compilado com `gcc` (usando `%f` padrão com 6 casas decimais).
2. Execução da suíte automatizada via `./run_tests.sh` na branch `test/estudo-boletim`, com resultado final de **55 passaram, 0 falharam** (`validos/estudo_boletim.c` e `invalidos/estudo_boletim_erro_tipo.c` aprovados em código e saída).

### Resumo das fases do interpretador no caso
* **Análise léxica:** identifica palavras-chave (`float`, `int`, `if`, `else`, `return`), identificadores, literais, operadores e chamadas `scanf`/`printf`.
* **Análise sintática e AST:** valida a gramática da função `media`, `main`, entradas/saídas e `if`/`else` aninhado, construindo a AST.
* **Análise semântica:** verifica declarações, escopos e compatibilidade de tipos (rejeitando a atribuição inválida no caso em `tests/invalidos/`).
* **Execução:** avalia expressões e conversões implícitas, executa `media`, lê as notas, avalia as faixas de média e exibe o resultado formatado.