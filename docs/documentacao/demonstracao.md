# Roteiro de Demonstração do Pipeline

Roteiro para mostrar o interpretador funcionando de ponta a ponta na apresentação (S3-01). O estudo de caso demonstrado é o organizador de compras (`tests/validos/organizador_compras.c`).

## Preparação (antes da apresentação)

Num clone limpo do repositório, com `gcc`, `flex`, `bison` e `make` instalados:

```bash
cd src
make clean && make
make test
```

O `make test` deve terminar com `Resumo: 26 passaram, 0 falharam (total 26)`. Se algo falhar, use as saídas salvas no fim desta página como plano B.

## Como o pipeline funciona

```
arquivo .c → léxico (Flex) → sintático (Bison + AST) → semântico (tabela de símbolos + tipos) → execução
```

- O `main.c` chama as fases nessa ordem e **para na primeira que acusar erro**: não faz sentido analisar semanticamente uma AST que veio de um parse com erro.
- O léxico e o sintático rodam juntos, porque o parser pede cada token ao scanner.
- No fim, sempre aparece o **relatório de erros**, com o total por fase. As fases que não chegaram a rodar aparecem como `nao executada`.

| Comando | O que faz |
|---|---|
| `./interpretador arquivo.c` | Pipeline completo: análises e execução |
| `./interpretador arquivo.c --tokens` | Só a análise léxica, listando linha, token e lexema |
| `./interpretador arquivo.c --ast` | Análises completas e impressão da AST, sem executar |
| `./interpretador arquivo.c --tabela` | Análises completas e impressão da tabela de símbolos, sem executar |

| Código de saída | Significado |
|---|---|
| 0 | Sucesso |
| 1 | Uso incorreto ou arquivo não encontrado |
| 2 | Parou na fase sintática |
| 3 | Parou na fase léxica |
| 4 | Parou na fase semântica |
| 5 | Erro durante a execução (por exemplo, divisão por zero) |

## Roteiro (cerca de 5 minutos)

Todos os comandos são rodados dentro de `src/`.

**1. Análise léxica:** o scanner transforma o texto em tokens, guardando a linha de cada um.
```bash
./interpretador ../tests/validos/organizador_compras.c --tokens
```

**2. Tabela de símbolos:** o semântico registra cada declaração com tipo, escopo e linha. `printf` e `scanf` já vêm declaradas no escopo global.
```bash
./interpretador ../tests/validos/organizador_compras.c --tabela
```

**3. AST:** o parser constrói a árvore, e o semântico anota o tipo de cada expressão (`<tipo: ...>`).
```bash
./interpretador ../tests/validos/organizador_compras.c --ast
```

**4. Pipeline completo:** todas as fases encadeadas, com o relatório no fim.
```bash
./interpretador ../tests/validos/organizador_compras.c
./interpretador ../tests/validos/interpretador_precedencia.c
```

**5. Parada na primeira fase com erro:** um erro semântico impede a execução, e um erro sintático impede o semântico.
```bash
./interpretador ../tests/invalidos/variavel_nao_declarada.c
./interpretador ../tests/invalidos/erro_sintatico.c
```

**6. Suíte automatizada:** todos os casos de `tests/validos/` e `tests/invalidos/` rodam com `make test`, comparando a saída com os arquivos `.esperado`, e a CI roda a mesma suíte em todo PR.
```bash
make test
```

## Estudos de caso (validação final)

Cada integrante escreveu um programa que usa uma parte diferente do núcleo e validou o interpretador com ele de ponta a ponta. Os cenários estão na suíte, com `.entrada` e `.esperado`.

### Controle de temperatura (S7-11)

Programa em `tests/validos/estudo_temperatura.c`. Lê a escala de saída (`C` ou `F`) com `%c`, lê temperaturas em Celsius até a sentinela `-1`, converte cada uma numa função `float converter(float celsius, char escala)` e mostra a quantidade de leituras, a maior, a menor e a média. No fim, um bloco interno declara outra variável `leituras`, que esconde a de fora (*shadowing*).

Recursos do núcleo que ele valida: `char` e `%c`, função com parâmetro `char` e retorno `float`, `while` com sentinela, `if`/`else` aninhado, `||`, blocos e escopo, `scanf` com `%c` e `%f`.

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
| Saída em Celsius (18.5, 22, 25.5) | `validos/estudo_temperatura.c` | maior 25.5, menor 18.5, média 22 |
| Saída em Fahrenheit (20, 25, 30 °C) | `validos/estudo_temperatura_fahrenheit.c` | 68, 77 e 86 °F, média 77 |
| Texto no lugar da temperatura | `invalidos/estudo_temperatura_entrada_invalida.c` | `Erro de execucao [linha 41]: entrada invalida para %f no scanf`, código 5 |

Como o `run_tests.sh` associa cada `.entrada` ao `.c` de mesmo nome, os cenários de Fahrenheit e de entrada inválida usam cópias do mesmo programa. Também foram conferidos à mão, fora da suíte, dois casos de borda: sem nenhuma leitura (só `-1`) o programa mostra "Nenhuma leitura registrada" em vez de dividir por zero, e com a entrada terminando antes do `-1` a execução para com "fim da entrada no scanf", sem travar.

### Calculadora de números (S7-10)

Programa em `tests/validos/estudo_numeros.c`. Lê dois inteiros `a` e `b` e mostra o MDC (função recursiva, algoritmo de Euclides), o MMC, o fatorial de `a` (função recursiva), os números primos de 2 até `b` e a tabuada de `a`.

Recursos do núcleo que ele valida: recursão em duas funções, `for` aninhado, `for` com corpo de uma linha, operador `%`, divisão inteira, função chamada dentro do `printf` e dentro de uma expressão, `scanf` com dois `%d` no mesmo formato.

Rodando no terminal e digitando `12 18`:

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
12 x 3 = 36
12 x 4 = 48
12 x 5 = 60
12 x 6 = 72
12 x 7 = 84
12 x 8 = 96
12 x 9 = 108
12 x 10 = 120

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   0 erro(s)
```

| Cenário | Arquivo | Resultado |
|---|---|---|
| `12 18` | `validos/estudo_numeros.c` | MDC 6, MMC 36, 12! = 479001600, 7 primos |
| `7 30` (coprimos) | `validos/estudo_numeros_coprimos.c` | MDC 1, MMC 210, 7! = 5040, 10 primos |
| `0 0` | `invalidos/estudo_numeros_divisao_zero.c` | `Erro de execucao [linha 27]: divisao por zero` no MMC, código 5 |

Os resultados foram conferidos à mão. O cenário `0 0` revelou um defeito: a divisão por zero mostrava o erro mas o programa continuava, imprimindo 0 no lugar da conta. Ele foi registrado como issue `bug` `(#124)` e corrigido no mesmo PR; agora a execução para no erro.

## Limitações conhecidas (para não ser pego de surpresa)

Atualizado em 24/11/2026. O núcleo executa declarações, expressões, `if`/`else`, `while`, `for`, `return`, chamadas de função (inclusive recursivas), `printf` (`%d`, `%f`, `%c`, `%s`) e `scanf` (`%d`, `%f`, `%c`). O que ainda pode surpreender:

- **Recursão infinita:** o limite de profundidade está em revisão no PR #120 (issue #88). Até ele entrar, uma recursão sem fim derruba o processo sem mensagem.
- **Divisão por zero:** a divisão inteira (e o `%`) por zero mostra o erro e interrompe a execução com código 5, como os outros erros de execução (issue #124). Já a divisão de `float` por zero não é erro e imprime `inf`, como em C.
- **`printf`:** não aceita precisão (`%.2f`), e o `%f` sempre mostra 6 casas. O `%s` só aceita texto entre aspas, porque a linguagem não tem variável de texto. O `printf` também não pode ser usado dentro de uma expressão, como `x = printf(...)`.
- **String sem aspas de fechamento:** é reportada como "caractere invalido" em vez de uma mensagem própria (ver `problemas-e-solucoes.md`).
- **Fora do escopo, de propósito:** vetores, ponteiros, structs, unions, variáveis de texto e pré-processador. Vetores, ponteiros, structs e unions são recusados com a mensagem "construcao fora do escopo"; as linhas com `#` (como `#include`) são ignoradas.

## Plano B: saídas salvas

Saídas reais dos comandos acima, geradas em 01/10/2026 a partir da branch `feat/pipeline-integrado`. As listas longas foram cortadas (`...`).

```
$ ./interpretador ../tests/validos/organizador_compras.c --tokens
LINHA  TOKEN       LEXEMA
4      INT         int
4      ID          main
4      SIMBOLO     (
4      SIMBOLO     )
4      SIMBOLO     {
5      FLOAT       float
5      ID          orcamento
5      SIMBOLO     ;
6      FLOAT       float
6      ID          preco
6      SIMBOLO     ;
7      FLOAT       float
...

$ ./interpretador ../tests/validos/organizador_compras.c --tabela
Tabela de simbolos (na ordem de declaracao; escopo 0 = global)

ESCOPO  NOME                 TIPO   CATEGORIA LINHA  PARAMETROS
0       printf               INT    funcao    -      variavel
0       scanf                INT    funcao    -      variavel
0       main                 INT    funcao    4      0
1       orcamento            FLOAT  variavel  5      -
1       preco                FLOAT  variavel  6      -
1       total                FLOAT  variavel  7      -
1       quantidadeItens      INT    variavel  8      -

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   nao executada

$ ./interpretador ../tests/validos/organizador_compras.c --ast
PROGRAM <tipo: VOID> [linha 1]
  FUNC_DECL 'main' <tipo: VOID> [linha 4]
    LIST <tipo: VOID> [linha 4]
    BLOCK <tipo: VOID> [linha 4]
      VAR_DECL (tipo=1) <tipo: VOID> [linha 5]
        ID 'orcamento' <tipo: VOID> [linha 5]
      VAR_DECL (tipo=1) <tipo: VOID> [linha 6]
        ID 'preco' <tipo: VOID> [linha 6]
      VAR_DECL (tipo=1) <tipo: VOID> [linha 7]
        BINOP '=' <tipo: VOID> [linha 7]
          ID 'total' <tipo: VOID> [linha 7]
...

$ ./interpretador ../tests/validos/organizador_compras.c
Digite o orcamento disponivel: Digite o preco de cada item (digite -1 para finalizar):\nItens registrados: 0\nTotal gasto: 0.000000\n

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   0 erro(s)

$ ./interpretador ../tests/validos/interpretador_precedencia.c
7

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   0 erro(s)

$ ./interpretador ../tests/invalidos/variavel_nao_declarada.c
Erro semantico [linha 3]: identificador nao declarado: 'y'

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  1 erro(s)
Execucao   nao executada

$ ./interpretador ../tests/invalidos/erro_sintatico.c
Erro sintatico [linha 4]: syntax error (proximo a 'return')

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  1 erro(s)
Semantica  nao executada
Execucao   nao executada
```
### Boletim escolar (S7-08)

Programa em `tests/validos/estudo_boletim.c`. Lê as notas de três provas com `scanf("%f", ...)`, calcula a média na função `float media(float a, float b, float c)`, classifica com `if / else` aninhado (`>= 7` APROVADO, `>= 5` RECUPERAÇÃO OU REPROVADO) e imprime as notas, a média e a situação com `printf` usando `%f` e `%s`.

Recursos do núcleo que ele valida: `float` e conversão `int -> float`, função com parâmetros e retorno `float`, `if / else` aninhado e operadores relacionais, `scanf` com `%f` e `printf` com `%f` e `%s`.

Rodando no terminal e digitando `8.0`, `7.0` e `9.0`:

```text
$ ./src/interpretador tests/validos/estudo_boletim.c
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

| Cenário | Arquivo | Resultado |
| :--- | :--- | :--- |
| Aprovado (`8.0`, `7.0`, `9.0`) | `validos/estudo_boletim.c` (`estudo_boletim_aprovado.*`) | média `8.000000`, situação `APROVADO`, código `0` |
| Recuperação (`6.0`, `5.0`, `4.0`) | `validos/estudo_boletim.c` (`estudo_boletim_recuperacao.*`) | média `5.000000`, situação `RECUPERACAO`, código `0` |
| Reprovado (`3.0`, `4.0`, `5.0`) | `validos/estudo_boletim.c` (`estudo_boletim_reprovado.*`) | média `4.000000`, situação `REPROVADO`, código `0` |
| Atribuição inválida `float` -> `int` | `invalidos/estudo_boletim_erro_tipo.c` | `Erro semantico [linha 20]: atribuicao incompativel: conversao implicita de float para int nao permitida`.

As saídas esperadas foram conferidas contra a execução em programa C real compilado com `gcc` (usando `%f` com 6 casas decimais, sem `%.2f`, e sem vetores, ponteiros ou `structs`). Na execução da suíte `run_tests.sh` na branch `test/estudo-boletim`, todos os 55 casos passaram (`55 passaram, 0 falharam`).

### Caixa eletrônico (S7-09)

O programa `tests/validos/estudo_caixa.c` mantém o saldo em uma variável global e oferece um menu em `while` para depositar, sacar, consultar o saldo ou sair. As funções `void depositar(float valor)` e `void sacar(float valor)` alteram o saldo global; `sacar` recusa valores negativos ou acima do saldo e usa `return;` para encerrar antecipadamente. A condição do laço combina o resultado de `scanf` com a opção escolhida (`&&`), e a validação do saque usa `||`.

O interpretador cria um frame de chamada para cada função. A função enxerga seus parâmetros e variáveis locais nesse frame e, depois, consulta o escopo global; ela não percorre o frame de quem a chamou. Por isso `depositar` e `sacar` acessam `saldo`, mas não as variáveis locais `opcao` e `valor` de `main`.

Como a suíte associa cada `.entrada` ao `.c` de mesmo nome, os cenários usam cópias do programa:

| Cenário | Arquivo | Resultado esperado |
|---|---|---|
| Depósito de 100, saque de 30 e consulta | `validos/estudo_caixa.c` | Saldo `70.000000`; saída com código 0 |
| Saque de 60 após depósito de 50 e saque de `-10` | `validos/estudo_caixa_tentativas_invalidas.c` | Dois avisos de recusa; saldo permanece `50.000000`; código 0 |
| Fim da entrada após um depósito, antes de `0` | `validos/estudo_caixa_fim_entrada.c` | O `scanf` da condição do `while` retorna EOF, o laço termina e o programa encerra sem erro; código 0 |
| Chamada de `sacar` com dois argumentos | `invalidos/estudo_caixa_argumentos_invalidos.c` | `Erro semantico [linha 14]: funcao 'sacar' esperava 1 argumento(s), recebeu 2`; código 4 |

Saída esperada do cenário normal, rodado em `src/`:

```text
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

Os arquivos `.esperado` foram escritos manualmente a partir dos cenários e conferidos pela suíte, não apenas gerados com `--gerar`. Em `src/`, com `make`, `gcc`, `flex` e `bison` do MSYS2 no `PATH`, foi executado:

```bash
make clean && make test
```

Resultado: `67 passaram, 0 falharam (total 67)`.