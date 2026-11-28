# Roteiro de Demonstração do Pipeline

Este é o roteiro que usamos para mostrar o interpretador funcionando de ponta a ponta na apresentação. O programa principal da demonstração é o organizador de compras (`tests/validos/organizador_compras.c`), o estudo de caso que escolhemos no início do projeto. Os outros programas que usamos para validar o interpretador estão em [Estudos de Casos](/documentacao/estudos-de-casos.md).

## Preparação (antes da apresentação)

Num clone limpo do repositório, com `gcc`, `flex`, `bison` e `make` instalados:

```bash
cd src
make clean && make
make test
```

O `make test` tem que terminar com `0 falharam`. Se alguma coisa der errado na hora, usamos as saídas salvas no fim desta página como plano B.

## Como o pipeline funciona

```
arquivo .c → léxico (Flex) → sintático (Bison + AST) → semântico (tabela de símbolos + tipos) → execução
```

- O `main.c` chama as fases nessa ordem e **para na primeira que encontrar erro**. Não faz sentido analisar a semântica de uma AST que veio de um parse com erro, nem executar um programa com erro de tipo.
- O léxico e o sintático rodam juntos, porque o parser pede cada token ao scanner.
- No fim sempre aparece o **relatório de erros**, com o total de erros de cada fase. As fases que não chegaram a rodar aparecem como `nao executada`.
- A execução percorre a AST diretamente. Como o projeto é um interpretador, não geramos código: o último passo é executar o programa.

| Comando | O que faz |
|---|---|
| `./interpretador arquivo.c` | Pipeline completo, com as análises e a execução |
| `./interpretador arquivo.c --tokens` | Só a análise léxica, listando linha, token e lexema |
| `./interpretador arquivo.c --ast` | Análises completas e impressão da AST, sem executar |
| `./interpretador arquivo.c --tabela` | Análises completas e impressão da tabela de símbolos, sem executar |
| `./interpretador arquivo.c --no-run` | Análises completas, sem executar |
| `./interpretador arquivo.c --limite-recursao N` | Executa com um limite menor de chamadas aninhadas (o padrão e o máximo são 1000) |

| Código de saída | Significado |
|---|---|
| 0 | Sucesso, ou o valor do `return` do `main` |
| 1 | Uso incorreto ou arquivo não encontrado |
| 2 | Parou na fase sintática |
| 3 | Parou na fase léxica |
| 4 | Parou na fase semântica |
| 5 | Erro durante a execução (divisão por zero, entrada inválida no `scanf`, recursão além do limite...) |

## Roteiro (cerca de 5 minutos)

Rodamos todos os comandos dentro de `src/`.

**1. Análise léxica:** o scanner transforma o texto em tokens e guarda a linha de cada um.
```bash
./interpretador ../tests/validos/organizador_compras.c --tokens
```

**2. Tabela de símbolos:** o semântico registra cada declaração com tipo, escopo e linha. O `printf` e o `scanf` já vêm declarados no escopo global.
```bash
./interpretador ../tests/validos/organizador_compras.c --tabela
```

**3. AST:** o parser monta a árvore, e o semântico anota o tipo de cada expressão (`<tipo: ...>`).
```bash
./interpretador ../tests/validos/organizador_compras.c --ast
```

**4. Pipeline completo:** todas as fases encadeadas e o programa executando. No terminal, digitamos o orçamento e os preços, e terminamos com `-1`.
```bash
./interpretador ../tests/validos/organizador_compras.c
```

**5. Funções e recursão:** cada chamada tem o próprio escopo, e uma recursão sem fim para com erro em vez de travar o programa.
```bash
./interpretador ../tests/validos/funcao_recursiva.c
./interpretador ../tests/invalidos/recursao_infinita.c
```

**6. Parada na primeira fase com erro:** um erro semântico impede a execução, e um erro sintático impede o semântico.
```bash
./interpretador ../tests/invalidos/variavel_nao_declarada.c
./interpretador ../tests/invalidos/erro_sintatico.c
```

**7. Suíte automatizada:** todos os casos de `tests/validos/` e `tests/invalidos/` rodam com `make test`, comparando a saída com os arquivos `.esperado`. A CI roda a mesma suíte em todo PR.
```bash
make test
```

## Limitações conhecidas (para não sermos pegos de surpresa)

Atualizado em 26/11/2026. O núcleo executa declarações, expressões, `if`/`else`, `while`, `for`, `return`, chamadas de função (inclusive recursivas), `printf` (`%d`, `%f`, `%c`, `%s`) e `scanf` (`%d`, `%f`, `%c`). O que ainda pode surpreender:

- **Erros de execução param o programa.** Divisão inteira ou resto por zero, entrada inválida ou fim da entrada no `scanf`, argumento faltando no `printf` e recursão além do limite mostram o erro com a linha e encerram com código 5.
- **Divisão de `float` por zero** não é erro e imprime `inf`, como em C.
- **Recursão:** o limite é de 1000 chamadas aninhadas, sem contar o `main`. A opção `--limite-recursao` só deixa o limite menor, porque acima disso a pilha do próprio interpretador pode estourar no Windows.
- **`printf`:** não aceita precisão (`%.2f`), e o `%f` sempre mostra 6 casas. O `%s` só aceita texto entre aspas, porque a linguagem não tem variável de texto. O `printf` também não pode ser usado dentro de uma expressão, como `x = printf(...)`.
- **Entrada vinda de arquivo:** quando rodamos com `< entrada.txt`, como faz a suíte, os valores lidos não aparecem na tela e as mensagens ficam coladas umas nas outras. No terminal, digitando, a saída fica normal.
- **String sem aspas de fechamento:** aparece como "caractere invalido", e não com uma mensagem própria (ver [Problemas e Soluções](/documentacao/problemas-e-solucoes.md)).
- **Fora do escopo, de propósito:** vetores, ponteiros, structs, unions, variáveis de texto e pré-processador. Vetores, ponteiros, structs e unions são recusados com a mensagem "construcao fora do escopo", e as linhas que começam com `#` (como `#include`) são ignoradas.

## Plano B: saídas salvas

Saídas reais dos comandos acima, geradas em 26/11/2026 a partir da `Dev`. Cortamos as listas longas (`...`). No terminal, o relatório de erros aparece sempre no fim.

```
$ ./interpretador ../tests/validos/organizador_compras.c --tokens
LINHA  TOKEN       LEXEMA
4      INT         int
4      ID          main
4      SIMBOLO     (
4      SIMBOLO     )
4      SIMBOLO     {
5      FLOAT       float
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
  FUNC_DECL 'main' (retorno=INT) <tipo: VOID> [linha 4]
    LIST <tipo: VOID> [linha 4]
    BLOCK <tipo: VOID> [linha 4]
      VAR_DECL (tipo=FLOAT) <tipo: VOID> [linha 5]
        ID 'orcamento' <tipo: VOID> [linha 5]
      VAR_DECL (tipo=FLOAT) <tipo: VOID> [linha 6]
        ID 'preco' <tipo: VOID> [linha 6]
      VAR_DECL (tipo=FLOAT) <tipo: VOID> [linha 7]
        BINOP '=' <tipo: VOID> [linha 7]
          ID 'total' <tipo: VOID> [linha 7]
          INT_LIT 0 <tipo: INT> [linha 7]
...

$ ./interpretador ../tests/validos/organizador_compras.c < ../tests/validos/organizador_compras.entrada
Digite o orcamento disponivel: Digite o preco de cada item (digite -1 para finalizar):
Preco do item: Preco do item: Preco do item: Preco do item: Itens registrados: 3
Total gasto: 120.000000
Atencao: voce ultrapassou o orcamento!

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   0 erro(s)

$ ./interpretador ../tests/validos/funcao_recursiva.c
Fatorial: 120
Fibonacci: 55

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   0 erro(s)

$ ./interpretador ../tests/invalidos/recursao_infinita.c
Erro de execucao [linha 2]: limite de profundidade de 1000 chamadas excedido na funcao 'loop_infinito'

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   1 erro(s)

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

A saída do organizador acima veio com a entrada de `tests/validos/organizador_compras.entrada` (orçamento 100 e itens 30, 40 e 50). Por isso os números digitados não aparecem e as mensagens ficam coladas.
