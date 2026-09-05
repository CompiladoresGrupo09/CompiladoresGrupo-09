# Definição da linguagem — núcleo de C interpretado

Mesmo com cerca de um mês de prazo, C completo (ponteiros, structs, pré-processador etc.) não cabe com qualidade. Este documento fixa o escopo que a equipe decidiu manter fechado.

## Núcleo obrigatório (para o interpretador "rodar")

- Tipos: `int`, `float`, `char`
- Declaração e atribuição de variáveis
- Expressões aritméticas, relacionais e lógicas, com precedência correta
- Estruturas de controle: `if`/`else`, `while`, `for`
- Blocos `{ }` e escopo de variáveis
- Funções com parâmetros, retorno e chamadas (inclusive recursivas)
- Entrada/saída mínima: `printf` (formatos básicos `%d %f %c %s`) e `scanf`

## Diferenciais — só se o núcleo fechar adiantado, nesta ordem de prioridade

1. Mensagens de erro com linha/coluna (barato: basta propagar `yylineno` do Flex pelas fases; alto retorno em robustez percebida)
2. Testes automatizados mais extensos (cobrir mais casos-limite do núcleo — tratar como parte da qualidade do núcleo, não como "extra")
3. Vetores/arrays 1D (só se sobrar bastante tempo; exige mudanças em parser, tabela de símbolos e semântica)

## Deliberadamente fora de escopo, mesmo com um mês

- Otimizações (dobra de constantes etc.)
- Ponteiros
- Structs/unions
- Pré-processador
- Strings dinâmicas
- Arquivos

## Exemplos de programas suportados

_(preencher conforme o núcleo for sendo implementado — pelo menos 2-3 exemplos de código `.c` de entrada com a saída esperada)_
