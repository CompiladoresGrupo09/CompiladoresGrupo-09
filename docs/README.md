# Compiladores Grupo 09
# Interpretador de C (subconjunto), Equipe 09

## Integrantes e papéis
- Brenda Beatriz: Léxico (scanner.l) e Sintático/AST (parser.y);
- Giovana: Semântico, com a tabela de símbolos e os escopos; integração (main.c);
- Mariana: Semântico, com os tipos na AST e a verificação de compatibilidade;
- Eduardo: Interpretação;
- Brenda Beatriz: Integração, testes, documentação e líder (formulários P1/P2);

## Escopo suportado
Núcleo obrigatório: tipos `int`/`float`/`char`, variáveis, expressões aritméticas/relacionais/lógicas com precedência, `if`/`else`/`while`/`for`, blocos com escopo, funções com parâmetros/retorno/recursão, `printf`/`scanf` básicos (`%d %f %c %s`).

Diferenciais (só se sobrasse tempo, nesta ordem): mensagens de erro com linha/coluna, testes automatizados mais extensos, vetores/arrays 1D.

Fora de escopo: otimizações, ponteiros, structs/unions, pré-processador, strings dinâmicas, arquivos.

Os detalhes e exemplos estão em [Definição da Linguagem](/documentacao/definicao-da-linguagem.md).

## Como compilar
```bash
cd src
make
```

## Como executar
```bash
./interpretador exemplo.c
```

## Exemplos de entrada e saída

#### 1. Boletim escolar (`tests/validos/estudo_boletim.c`)

Lê três notas em `float` com `scanf("%f", ...)`, calcula a média na função `float media(float a, float b, float c)` e classifica a situação com `if / else` aninhado (`>= 7` aprovado, `>= 5` recuperação, abaixo disso reprovado), mostrando os resultados com `printf` (`%f` e `%s`).

**Entrada:**
```text
8.0
7.0
9.0
```

**Saída esperada (conferimos em C com `gcc`, `%f` com 6 casas decimais):**
```text
Nota 1: 8.000000
Nota 2: 7.000000
Nota 3: 9.000000
Média: 8.000000
Situacao: APROVADO
```

#### 2. Controle de temperatura (`tests/validos/estudo_temperatura.c`)

Lê a escala (`C` ou `F`) com `%c`, lê temperaturas até a sentinela `-1` num laço `while`, converte cada uma na função `float converter(float celsius, char escala)` e mostra a quantidade, a maior, a menor, a média e o efeito do escopo (*shadowing*) num bloco interno.

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
Escala: F
Leituras: 3
Maior: 86.000000
Menor: 68.000000
Media: 77.000000
Leituras dentro do bloco: 0
Leituras fora do bloco: 3
```

#### 3. Erro semântico de conversão implícita (`tests/invalidos/estudo_boletim_erro_tipo.c`)

Mostra a análise semântica recusando o programa quando o retorno `float` de `media(...)` é guardado numa variável `int` (código de saída 4). A execução nem chega a acontecer:
```text
Erro semantico [linha 20]: atribuicao incompativel: conversao implicita de float para int nao permitida

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  1 erro(s)
Execucao   nao executada
```

Os outros programas que usamos para validar o interpretador estão em [Estudos de Casos](/documentacao/estudos-de-casos.md).

## Arquitetura
Fonte → léxico (Flex) → sintático (Bison, que já monta a AST) → semântico (tabela de símbolos, checagem de tipos e de escopo) → interpretação recursiva da AST → resultado.

## Testes

Os casos válidos ficam em `tests/validos/` e os inválidos em `tests/invalidos/`.

### Rodando a suíte completa
```bash
cd src
make test
```

Isso compila o interpretador (se precisar) e roda **todos** os arquivos `.c` de `tests/validos/` e `tests/invalidos/`. Para cada caso, conferimos o código de saída e, quando existe um arquivo `.esperado` com o mesmo nome, também comparamos a saída do programa com ele. Quando existe um `.entrada`, ele é usado como o que o usuário digitaria.

### Testes da tabela de símbolos e da análise semântica (issue #22)
Além dos testes léxicos e sintáticos, a suíte cobre os critérios de aceitação da tabela de símbolos com escopos aninhados:

| Arquivo | O que valida |
|---|---|
| `tests/validos/escopo_shadowing.c` | *Shadowing* em 3 níveis (global, parâmetro e bloco interno) com o mesmo nome de variável. Tem que passar **sem** erro semântico. |
| `tests/invalidos/redeclaracao_mesmo_escopo.c` | Duas declarações da mesma variável no **mesmo** escopo. Tem que dar erro semântico de redeclaração. |
| `tests/invalidos/variavel_nao_declarada.c` | Uso de um identificador que nunca foi declarado. Tem que dar erro semântico de identificador não declarado. |

Esses três rodam automaticamente no `make test`, junto com os outros.

### Rodando um arquivo isolado, fora da suíte
```bash
cd src
./interpretador ../tests/validos/escopo_shadowing.c
```

Serve para depurar um caso específico sem rodar a suíte inteira.

### Verificando vazamento de memória (opcional, recomendado antes de aprovar PRs que mexem na memória)
```bash
cd src
gcc -Wall -fsanitize=address,undefined -g main.c parser.tab.c lex.yy.c ast.c tabela.c semantic.c interpreter.c -o interpretador_asan
./interpretador_asan ../tests/validos/organizador_compras.c
```

Se o AddressSanitizer não mostrar nenhum relatório no fim, não há vazamento nem uso indevido de memória.

## Decisões técnicas, planejamento e problemas/soluções
Estão na pasta `docs/`.

## Limitações conhecidas
Ver a seção "Fora de escopo" acima, a [Definição da Linguagem](/documentacao/definicao-da-linguagem.md) e as limitações no [Roteiro de Demonstração](/documentacao/demonstracao.md).

## Histórico de Versão

| Data | Versão | Descrição | Autor |
|------|--------|-----------|-------|
| 05/09/2026 | 1.0 | Criação da página de Visão Geral | Brenda |
| 23/11/2026 | 1.1 | Atualização da página de Visão Geral | Mariana |
| 26/11/2026 | 1.2 | Correção dos exemplos, da descrição da suíte e do comando de verificação de memória | Brenda |
