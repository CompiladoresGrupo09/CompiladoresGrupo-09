# Compiladores Grupo 09
# Interpretador de C (subconjunto) — Equipe 09

## Integrantes e papéis
- Nome — Léxico (scanner.l)
- Nome — Sintático/AST (parser.y)
- Nome — Semântico
- Nome — Interpretação
- Nome — Integração, testes, documentação e líder (formulários P1/P2)

## Aplicação
Organizador de compras no mercado: o programa recebe itens e preços via `scanf` em loop, soma o total gasto e avisa se o valor ultrapassou o orçamento informado.

## Escopo suportado
Núcleo obrigatório: tipos `int`/`float`/`char`, variáveis, expressões aritméticas/relacionais/lógicas com precedência, `if`/`else`/`while`/`for`, blocos com escopo, funções com parâmetros/retorno/recursão, `printf`/`scanf` básicos (`%d %f %c %s`).

Diferenciais (só se sobrar tempo, nesta ordem): mensagens de erro com linha/coluna, testes automatizados mais extensos, vetores/arrays 1D.

Fora de escopo: otimizações, ponteiros, structs/unions, pré-processador, strings dinâmicas, arquivos.

Ver `docs/definicao-da-linguagem.md` para detalhes e exemplos.

## Como compilar
```
make
```

## Como executar
```
./interpretador exemplo.c
```

## Exemplos de entrada e saída
Ver `docs/definicao-da-linguagem.md` para o exemplo completo do organizador de compras (entrada `.c` e saída esperada). Mais exemplos serão adicionados conforme o núcleo for funcionando.

## Arquitetura
Fonte → léxico (Flex) → sintático (Bison, já construindo a AST) → semântico (tabela de símbolos, checagem de tipos/escopo) → interpretação recursiva da AST → resultado.

## Testes
Casos válidos em `tests/validos/`, casos inválidos em `tests/invalidos/`.

## Decisões técnicas, planejamento e problemas/soluções
Ver pasta `docs/`.

## Limitações conhecidas
Ver seção "Fora de escopo" acima e `docs/definicao-da-linguagem.md`.
