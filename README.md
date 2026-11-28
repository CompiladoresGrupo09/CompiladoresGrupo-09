# Compiladores Grupo 09
# Interpretador de C (subconjunto): Equipe 09

[![Build e testes](https://github.com/CompiladoresGrupo09/CompiladoresGrupo-09/actions/workflows/build.yml/badge.svg?branch=Dev)](https://github.com/CompiladoresGrupo09/CompiladoresGrupo-09/actions/workflows/build.yml)

## Integrantes e papéis
| Integrante | GitHub | Papel |
|---|---|---|
| Brenda Beatriz | [@Brwnds](https://github.com/Brwnds) | Léxico (`scanner.l`) e Sintático/AST (`parser.y`); integração, testes e documentação; **líder** (envio dos formulários P1/P2) |
| Giovana Ferreira | [@gih7915](https://github.com/gih7915) | Semântico: tabela de símbolos com escopos; integração (`main.c`) |
| Mariana Martins | [@Marianamrts](https://github.com/Marianamrts) | Semântico: tipos na AST e verificação de compatibilidade |
| Eduardo Ribeiro Xavier | [@EduardoRibeiroXavier](https://github.com/EduardoRibeiroXavier) | Interpretação |
| Henrique Fontenelle | [@HenriqueFontenelle](https://github.com/HenriqueFontenelle) | Scrum Master: revisão, integração `Dev`/`main` e decisões técnicas; **suplente** do líder |

Detalhes das entregas de cada um em `docs/documentacao/planejamento-sprints.md` (tabela de papéis).

## Aplicação
Organizador de compras no mercado: o programa recebe itens e preços via `scanf` em loop, soma o total gasto e avisa se o valor ultrapassou o orçamento informado.

## Escopo suportado
Núcleo obrigatório: tipos `int`/`float`/`char`, variáveis, expressões aritméticas/relacionais/lógicas com precedência, `if`/`else`/`while`/`for`, blocos com escopo, funções com parâmetros/retorno/recursão, `printf`/`scanf` básicos (`%d %f %c %s`).

Diferenciais: fizemos os testes automatizados mais extensos (88 testes) e o relatório de erros unificado. A coluna nas mensagens de erro e os vetores ficaram de fora, de propósito, para não arriscar o núcleo no dia da entrega.

Fora de escopo: otimizações, ponteiros, structs/unions, pré-processador, strings dinâmicas, arquivos.

Ver `docs/documentacao/definicao-da-linguagem.md` para detalhes e exemplos.

## Como compilar/executar
```
cd src
make
./interpretador ../tests/validos/organizador_compras.c
```
Flags para inspecionar cada fase: `--tokens` (só o léxico), `--ast` (imprime a AST), `--tabela` (imprime a tabela de símbolos) e `--no-run` (faz todas as análises, sem executar). No fim, o programa sempre mostra um relatório com o total de erros por fase. Ver `docs/documentacao/demonstracao.md`.

Código de saída do processo:

| Código | Significado |
|---|---|
| valor do `return` do `main` | execução sem erros em nenhuma fase |
| 1 | uso incorreto (arquivo ausente, flag desconhecida) |
| 2 | erro sintático |
| 3 | erro léxico |
| 4 | erro semântico |
| 5 | erro de execução |


## Exemplos de entrada e saída

### Organizador de compras (estudo de caso)
Programa completo em `tests/validos/organizador_compras.c`. Rodando no terminal e digitando orçamento 100 e os itens 30, 40 e 50, encerrando com -1:

```
$ ./interpretador ../tests/validos/organizador_compras.c
Digite o orcamento disponivel: 100
Digite o preco de cada item (digite -1 para finalizar):
Preco do item: 30
Preco do item: 40
Preco do item: 50
Preco do item: -1
Itens registrados: 3
Total gasto: 120.000000
Atencao: voce ultrapassou o orcamento!

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   0 erro(s)
```

### Total por quantidade
Programa mais simples, em `tests/validos/total_por_quantidade.c`: lê a quantidade e o preço unitário, mostra o total e diz se a compra é no atacado (10 itens ou mais) ou no varejo.

```
$ ./interpretador ../tests/validos/total_por_quantidade.c
Quantidade: 12
Preco unitario: 2.5
Total: 30.000000
Compra no atacado

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   0 erro(s)
```

Mais detalhes do estudo de caso em `docs/documentacao/definicao-da-linguagem.md`. Os outros quatro programas que usamos para validar o interpretador (boletim escolar, caixa eletrônico, calculadora de números e controle de temperatura) estão em `docs/documentacao/estudos-de-casos.md`.

## Arquitetura
Fonte → léxico (Flex) → sintático (Bison, já construindo a AST) → semântico (tabela de símbolos, checagem de tipos/escopo) → interpretação recursiva da AST → resultado.

As quatro fases reportam os erros pelo mesmo módulo (`src/erros.c`), no formato `Erro <fase> [linha N]: <descricao>`, e o relatório no fim mostra o total de erros de cada fase.

## Testes
```
cd src
make test
```
São 88 testes: casos válidos em `tests/validos/` e casos inválidos em `tests/invalidos/`. Cada caso tem um `.esperado` com a saída correta, e os que leem do teclado têm um `.entrada`. A suíte confere o código de saída e compara a saída inteira, e a CI roda a mesma suíte em todo PR. A tabela que mostra qual teste cobre cada parte do núcleo está em `tests/README.md`.

## Decisões técnicas, planejamento e problemas/soluções
Ver pasta `docs/` ou o site da documentação no GitHub Pages: decisões técnicas, planejamento das sprints, atas das reuniões, problemas e soluções, gramática e roteiro de demonstração.

## Limitações conhecidas
- As mensagens de erro mostram a linha, mas não a coluna.
- O `printf` não aceita precisão (`%.2f`), e o `%f` sempre mostra 6 casas.
- A recursão é limitada a 1000 chamadas aninhadas.
- A divisão de `float` por zero não é erro e imprime `inf`, como em C.
- Uma string sem as aspas de fechamento aparece como "caractere invalido", sem mensagem própria.
- Vetores, ponteiros, structs e unions são recusados com a mensagem "construcao fora do escopo".

Ver também a seção "Fora de escopo" acima e `docs/documentacao/demonstracao.md`.
