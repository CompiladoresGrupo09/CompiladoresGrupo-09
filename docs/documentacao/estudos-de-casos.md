# Estudos de Casos

Para validar o interpretador de ponta a ponta, cada integrante escreveu um programa C que usa uma parte diferente do núcleo da linguagem. Juntos, os programas passam por todas as construções que prometemos em [Definição da Linguagem](/documentacao/definicao-da-linguagem.md): tipos `int`, `float` e `char`, decisões, laços, funções, recursão, escopo, `printf` e `scanf`.

Cada cenário tem um arquivo `.entrada`, com o que o usuário digitaria, e um `.esperado`, com a saída correta. Conferimos os valores à mão, ou com o mesmo programa compilado com `gcc`, antes de colocar o caso na suíte. Também escrevemos, para cada programa, um caso inválido que mostra o interpretador acusando um erro.

O estudo de caso principal, usado na demonstração, continua sendo o **organizador de compras** (`tests/validos/organizador_compras.c`), descrito em [Definição da Linguagem](/documentacao/definicao-da-linguagem.md) e no [Roteiro de Demonstração](/documentacao/demonstracao.md).

| Estudo de caso | Responsável | O que ele valida principalmente | Situação |
|---|---|---|---|
| Boletim escolar (S7-08) | Mariana | `float`, `if`/`else` aninhado, função com retorno `float` | Concluído |
| Caixa eletrônico (S7-09) | Giovana | Menu com `while`, variáveis globais, funções `void` | Concluído |
| Calculadora de números (S7-10) | Eduardo | Recursão, `for` aninhado, operador `%` | Concluído |
| Controle de temperatura (S7-11) | Brenda | `char` e `%c`, sentinela, escopo e *shadowing* | Concluído |

Como o `run_tests.sh` liga cada `.entrada` ao `.c` de mesmo nome, quando um programa tem mais de um cenário usamos uma cópia do `.c` com outro nome para cada cenário.

# Boletim escolar (S7-08)

Programa em `tests/validos/estudo_boletim.c`. Ele lê as notas de três provas com `scanf("%f", ...)`, calcula a média na função `float media(float a, float b, float c)` e classifica o aluno com `if`/`else` aninhado: média 7 ou mais é `APROVADO`, de 5 a 7 é `RECUPERACAO` e abaixo de 5 é `REPROVADO`. No fim, imprime as notas, a média e a situação com `printf`, usando `%f` e `%s`.

O que ele valida: `float` e a conversão de `int` para `float` na divisão da média, função com parâmetros e retorno `float`, `if`/`else` aninhado com operadores relacionais, `scanf` com `%f` e `printf` com `%f` e `%s`.

Rodando no terminal e digitando `8.0`, `7.0` e `9.0`:

```
$ ./interpretador ../tests/validos/estudo_boletim.c
8.0
7.0
9.0
Nota 1: 8.000000
Nota 2: 7.000000
Nota 3: 9.000000
Média: 8.000000
Situacao: APROVADO

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   0 erro(s)
```

| Cenário | Entrada | Resultado |
|---|---|---|
| Aprovado | `8.0`, `7.0`, `9.0` | média `8.000000`, `APROVADO` |
| Recuperação | `5.0`, `6.0`, `4.0` | média `5.000000`, `RECUPERACAO` |
| Reprovado | `5.0`, `4.0`, `4.0` | média `4.333333`, `REPROVADO` |
| Inválido: média `float` guardada numa variável `int` (`invalidos/estudo_boletim_erro_tipo.c`) | | `Erro semantico [linha 20]: atribuicao incompativel: conversao implicita de float para int nao permitida`, código 4 |

# Caixa eletrônico (S7-09)

Programa em `tests/validos/estudo_caixa.c`. Ele guarda o saldo numa variável global e oferece um menu em `while` para depositar, sacar, ver o saldo ou sair. As funções `void depositar(float valor)` e `void sacar(float valor)` alteram o saldo global, e `sacar` recusa valores negativos ou acima do saldo, usando `return;` para sair antes do fim da função. A condição do laço combina o resultado do `scanf` com a opção escolhida (`&&`), e a validação do saque usa `||`.

O que ele valida: variável global alterada por funções, funções `void` com `return;` sem valor, `while` com menu, `&&` e `||`, e o valor de retorno do `scanf` usado numa condição. Ele também mostra como funciona o escopo das chamadas: cada função tem o próprio frame e, fora dele, só enxerga o escopo global. Por isso `depositar` e `sacar` acessam `saldo`, mas não as variáveis `opcao` e `valor` do `main`.

Rodando com a entrada de `estudo_caixa.entrada` (depósito de 100, saque de 30, ver saldo e sair):

```
$ ./interpretador ../tests/validos/estudo_caixa.c < ../tests/validos/estudo_caixa.entrada
1 Depositar, 2 Sacar, 3 Ver saldo, 0 Sair: Deposito realizado.
1 Depositar, 2 Sacar, 3 Ver saldo, 0 Sair: Saque realizado.
1 Depositar, 2 Sacar, 3 Ver saldo, 0 Sair: Saldo: 70.000000
1 Depositar, 2 Sacar, 3 Ver saldo, 0 Sair: Caixa encerrado.

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   0 erro(s)
```

| Cenário | Arquivo | Resultado |
|---|---|---|
| Depósito de 100, saque de 30 e consulta | `validos/estudo_caixa.c` | saldo `70.000000` |
| Depósito de 50, saque de 60 e saque de `-10` | `validos/estudo_caixa_tentativas_invalidas.c` | os dois saques são recusados e o saldo continua `50.000000` |
| Entrada termina depois de um depósito, sem o `0` | `validos/estudo_caixa_fim_entrada.c` | o `scanf` da condição devolve fim de entrada, o laço termina e o programa encerra sem erro |
| `sacar` chamada com dois argumentos (inválido) | `invalidos/estudo_caixa_argumentos_invalidos.c` | `Erro semantico [linha 14]: funcao 'sacar' esperava 1 argumento(s), recebeu 2`, código 4 |

Os `.esperado` foram escritos à mão a partir dos cenários e conferidos pela suíte, e o caso também passou num clone limpo da branch `test/estudo-caixa`.

# Calculadora de números (S7-10)

Programa em `tests/validos/estudo_numeros.c`. Ele lê dois números inteiros e mostra o MDC, calculado por uma função recursiva com o algoritmo de Euclides, o MMC, o fatorial do primeiro número, também recursivo, os números primos até o segundo número e a tabuada do primeiro.

O que ele valida: duas funções recursivas, chamada de função dentro de uma expressão e dentro de um argumento do `printf`, `for` aninhado, `for` com corpo de uma linha, operador `%` e divisão inteira, e `scanf` lendo dois valores numa chamada só (`"%d %d"`).

Rodando com a entrada `12 18`:

```
$ ./interpretador ../tests/validos/estudo_numeros.c
Digite dois numeros inteiros: 12 18
MDC(12, 18) = 6
MMC(12, 18) = 36
12! = 479001600
Primos ate 18: 2 3 5 7 11 13 17
Total de primos: 7
Tabuada do 12:
12 x 1 = 12
12 x 2 = 24
...
12 x 10 = 120

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   0 erro(s)
```

| Cenário | Arquivo | Resultado |
|---|---|---|
| `12 18` | `validos/estudo_numeros.c` | MDC 6, MMC 36, 7 primos até 18 |
| `7 30`, números primos entre si | `validos/estudo_numeros_coprimos.c` | MDC 1, MMC 210, `7! = 5040`, 10 primos até 30 |
| `0 0` (inválido) | `invalidos/estudo_numeros_divisao_zero.c` | o MMC divide por `mdc(0, 0)`, que é 0: `Erro de execucao [linha 27]: divisao por zero`, código 5 |

Foi esse estudo de caso que mostrou que a divisão por zero mostrava o erro e deixava o programa continuar, imprimindo fatorial, primos e tabuada depois. Corrigimos isso na issue #124: agora a divisão por zero interrompe a execução, como os outros erros.

# Controle de temperatura (S7-11)

Programa em `tests/validos/estudo_temperatura.c`. Ele lê a escala de saída (`C` ou `F`) com `%c`, lê temperaturas em Celsius até o usuário digitar `-1`, converte cada uma na função `float converter(float celsius, char escala)` e mostra quantas leituras foram feitas, a maior, a menor e a média. No fim, um bloco interno declara outra variável `leituras`, com o mesmo nome de uma de fora, para mostrar o *shadowing*.

O que ele valida: `char`, literal de caractere e `%c`, função com parâmetro `char` e retorno `float`, `while` com sentinela, `if`/`else` aninhado, `||`, blocos e escopo, e `scanf` com `%c` e `%f`.

Rodando no terminal e digitando `F`, `20`, `25`, `30` e `-1`:

```
$ ./interpretador ../tests/validos/estudo_temperatura.c
Escala de saida (C ou F): F
Digite as temperaturas em Celsius (-1 para terminar):
20
25
30
-1
Escala: F
Leituras: 3
Maior: 86.000000
Menor: 68.000000
Media: 77.000000
Leituras dentro do bloco: 0
Leituras fora do bloco: 3

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   0 erro(s)
```

| Cenário | Arquivo | Resultado |
|---|---|---|
| Saída em Celsius (18.5, 22 e 25.5) | `validos/estudo_temperatura.c` | maior 25.5, menor 18.5, média 22 |
| Saída em Fahrenheit (20, 25 e 30 °C) | `validos/estudo_temperatura_fahrenheit.c` | 68, 77 e 86 °F, média 77 |
| Texto no lugar da temperatura (inválido) | `invalidos/estudo_temperatura_entrada_invalida.c` | `Erro de execucao [linha 41]: entrada invalida para %f no scanf`, código 5 |

Também testamos dois casos de borda fora da suíte. Sem nenhuma leitura (só `-1`), o programa mostra "Nenhuma leitura registrada" em vez de dividir por zero. E se a entrada acaba antes do `-1`, a execução para com "fim da entrada no scanf", sem travar.

## O que cada fase faz com esses programas

- **Léxico:** separa palavras reservadas, identificadores, literais (`8.0`, `'F'`, `"APROVADO"`), operadores e as chamadas de `printf` e `scanf`. A linha `#include <stdio.h>` é ignorada.
- **Sintático:** confere a estrutura das funções, dos `if`/`else` aninhados e dos laços, e monta a AST.
- **Semântico:** confere declarações, escopos, tipos e chamadas de função. É ele que recusa o caso inválido do boletim, antes de qualquer execução.
- **Execução:** percorre a AST, executa as funções com escopo próprio, lê a entrada com `scanf` e imprime os resultados. É ela que acusa a divisão por zero da calculadora e a entrada inválida da temperatura.
