# Compiladores Grupo 09
## Interpretador de C (subconjunto) — Equipe 09

## Integrantes e papéis
- Nome — Léxico (scanner.l)
- Nome — Sintático/AST (parser.y)
- Nome — Semântico
- Nome — Interpretação
- Brenda — Integração, testes, documentação e líder (formulários P1/P2)

## Escopo suportado
Núcleo obrigatório: tipos `int`/`float`/`char`, variáveis, expressões aritméticas/relacionais/lógicas com precedência, `if`/`else`/`while`/`for`, blocos com escopo, funções com parâmetros/retorno/recursão, `printf`/`scanf` básicos (`%d %f %c %s`).

Diferenciais (só se sobrar tempo, nesta ordem): mensagens de erro com linha/coluna, testes automatizados mais extensos, vetores/arrays 1D.

Fora de escopo: otimizações, ponteiros, structs/unions, pré-processador, strings dinâmicas, arquivos.

Ver `docs/documentacao/definicao-da-linguagem.md` para detalhes e exemplos.

## Como compilar
cd src
make


## Como executar
./interpretador exemplo.c


## Exemplos de entrada e saída
[pelo menos 2-3 exemplos de programas .c e a saída esperada — adicionar conforme o núcleo for funcionando]

## Arquitetura
Fonte → léxico (Flex) → sintático (Bison, já construindo a AST) → semântico (tabela de símbolos, checagem de tipos/escopo) → interpretação recursiva da AST → resultado.

## Testes

Casos válidos em `tests/validos/`, casos inválidos em `tests/invalidos/`.

### Rodando a suíte completa
cd src
make test

Isso compila o interpretador (se necessário) e roda **todos** os arquivos `.c` de `tests/validos/` e `tests/invalidos/` automaticamente, imprimindo a AST (para os válidos) ou a mensagem de erro esperada (para os inválidos).

### Testes específicos da tabela de símbolos e análise semântica (issue #22)
Além dos testes sintáticos/léxicos originais, a suíte cobre os critérios de aceitação da tabela de símbolos com escopos aninhados:

| Arquivo | O que valida |
|---|---|
| `tests/validos/escopo_shadowing.c` | Shadowing em 3 níveis (global → parâmetro → bloco interno) com o mesmo nome de variável — deve compilar **sem** erro semântico. |
| `tests/invalidos/redeclaracao_mesmo_escopo.c` | Duas declarações da mesma variável no **mesmo** escopo — deve gerar erro semântico de redeclaração. |
| `tests/invalidos/variavel_nao_declarada.c` | Uso de identificador nunca declarado — deve gerar erro semântico de identificador não declarado. |

Esses três já rodam automaticamente dentro de `make test`, junto com os demais.

### Rodando um arquivo isolado, fora da suíte

cd src
./interpretador ../tests/validos/escopo_shadowing.c

Útil pra depurar um caso específico sem rodar a suíte inteira.

### Verificando vazamento de memória (opcional, recomendado antes de aprovar PRs da tabela de símbolos)

cd src
gcc -Wall -fsanitize=address,undefined -g main.c parser.tab.c lex.yy.c ast.c tabela.c semantic.c -o interpretador_asan
./interpretador_asan ../tests/validos/organizador_compras.c

Se o AddressSanitizer não imprimir nenhum relatório ao final, não há vazamento nem uso indevido de memória.

## Decisões técnicas, planejamento e problemas/soluções
Ver pasta `docs/`.

## Limitações conhecidas
Ver seção "Fora de escopo" acima e `docs/documentacao/definicao-da-linguagem.md`.