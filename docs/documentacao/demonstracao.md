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

**6. Suíte automatizada:** os 26 casos rodam com `make test`, e a CI roda a mesma suíte em todo PR.
```bash
make test
```

## Limitações conhecidas (para não ser pego de surpresa)

- O interpretador executa por enquanto **só declarações e expressões do bloco da `main`**. `if`, `while`, `for`, `return` e chamadas de função passam pelas análises, mas ainda não são executados. Por isso o organizador de compras imprime os textos sem entrar no laço.
- O `printf` ainda **não traduz `\n`**, que aparece literalmente, e **não implementa `%s`**.
- O `scanf` passa pelas análises, mas não lê valores na execução.

Esses pontos fazem parte do planejamento da próxima sprint.

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
