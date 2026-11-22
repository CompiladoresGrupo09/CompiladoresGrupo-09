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

## Limitações conhecidas (para não ser pego de surpresa)

Atualizado em 22/11/2026. O núcleo executa declarações, expressões, `if`/`else`, `while`, `for`, `return`, chamadas de função (inclusive recursivas), `printf` (`%d`, `%f`, `%c`, `%s`) e `scanf` (`%d`, `%f`, `%c`). O que ainda pode surpreender:

- **Recursão infinita:** o limite de profundidade está em revisão no PR #120 (issue #88). Até ele entrar, uma recursão sem fim derruba o processo sem mensagem.
- **Divisão por zero:** a divisão inteira (e o `%`) por zero mostra o erro, mas a execução continua com o valor 0; a divisão de `float` por zero não é erro e imprime `inf`, como em C. A decisão de parar ou não no primeiro erro de execução está na issue #95.
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
