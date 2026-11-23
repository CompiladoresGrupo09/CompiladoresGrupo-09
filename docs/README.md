# Compiladores Grupo 09
# Interpretador de C (subconjunto) — Equipe 09

## Integrantes e papéis
- Brenda Beatriz — Léxico (scanner.l) e Sintático/AST (parser.y);
- Giovana — Semântico: tabela de símbolos com escopos; integração (main.c);
- Mariana — Semântico: tipos na AST e verificação de compatibilidade;
- Eduardo — Interpretação;
- Brenda Beatriz — Integração, testes, documentação e líder (formulários P1/P2);

## Escopo suportado
Núcleo obrigatório: tipos `int`/`float`/`char`, variáveis, expressões aritméticas/relacionais/lógicas com precedência, `if`/`else`/`while`/`for`, blocos com escopo, funções com parâmetros/retorno/recursão, `printf`/`scanf` básicos (`%d %f %c %s`).

Diferenciais (só se sobrar tempo, nesta ordem): mensagens de erro com linha/coluna, testes automatizados mais extensos, vetores/arrays 1D.

Fora de escopo: otimizações, ponteiros, structs/unions, pré-processador, strings dinâmicas, arquivos.

Ver [Definição da Linguagem](/documentacao/definicao-da-linguagem.md) para detalhes e exemplos.

## Como compilar
cd src
make


## Como executar
./interpretador exemplo.c


## Exemplos de entrada e saída

#### 1. Boletim escolar (`tests/validos/estudo_boletim.c`)

Lê três notas em `float` com `scanf("%f", ...)`, calcula a média na função `float media(float a, float b, float c)` e classifica a situação com `if / else` aninhado (`>= 7` aprovado, `>= 5` recuperação ou reprovado), exibindo os resultados com `printf` (`%f` e `%s`).

**Entrada:**
```text
8.0
7.0
9.0
```

**Saída esperada (conferida em C com `gcc`, `%f` com 6 casas decimais):**
```text
Nota 1: 9.000000
Nota 2: 7.000000
Nota 3: 8.000000
Média: 8.000000
Situacao: APROVADO
```

#### 2. Controle de temperatura (`tests/validos/estudo_temperatura.c`)

Lê a escala (`C` ou `F`) com `%c`, lê temperaturas até a sentinela `-1` em um laço `while`, converte na função `float converter(float celsius, char escala)` e exibe quantidade, maior, menor, média e o efeito de escopo/*shadowing* em bloco interno.

**Entrada:**
```text
F
20
25
30
-1
```

**Saída esperada:**
```text
Escala de saida (C ou F): Digite as temperaturas em Celsius (-1 para terminar):
Escala: C
Leituras: 3
Maior: 25.500000
Menor: 18.500000
Media: 22.000000
Leituras dentro do bloco: 0
Leituras fora do bloco: 3
```

#### 3. Erro semântico de conversão implícita (`tests/invalidos/estudo_boletim_erro_tipo.c`)

Demonstra a rejeição na análise semântica quando o retorno `float` de `media(...)` é atribuído a uma variável declarada como `int` (`EXIT_CODE: 4`), impedindo a fase de execução:
```text
Erro semantico [linha 20]: atribuicao incompativel: conversao implicita de float para int nao permitida

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  1 erro(s)
Execucao   nao executada
```

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
Ver seção "Fora de escopo" acima e [Definição da Linguagem](/documentacao/definicao-da-linguagem.md).

## Histórico de Versão

| Data | Versão | Descrição | Autor |
|------|--------|-----------|-------|
| 05/09/2026 | 1.0 | Criação da página de Visão Geral | Brenda |
| 23/11/2026 | 1.1 | Atualização da página de Visão Geral | Mariana |