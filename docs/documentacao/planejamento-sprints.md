# Planejamento de sprints

Aqui registramos o planejado × entregue de cada sprint. Atualizamos ao fim de cada uma, como pede a Proposta de Trabalho.

## Replanejamento (issue #33)

**O que mudou:** o planejamento original fechava a Sprint 2 com o marco P1. A Sprint 2 original foi dividida em duas:

| Sprint | Período | Foco |
|---|---|---|
| Sprint 2 | até **16/09** | Sintático, AST e início do semântico |
| Sprint 3 | até **23/09** (prazo do formulário P1), com a apresentação do P1 em **05/10** | Semântica de tipos, início da interpretação, testes, integração e documentação do P1 |

**Por quê:** o prazo real do formulário P1 foi 23/09, e a Semana Universitária caiu no meio do período, reduzindo o tempo útil. Em vez de espremer num único bloco o fechamento do front-end, a verificação semântica e a preparação do P1, separamos o trabalho em duas sprints e deixamos claro o que precisava estar pronto em cada data.

**Consequências:**
- As sprints seguintes foram **renumeradas**: a Sprint 3 original (interpretação) passou a ser a Sprint 4, e assim por diante. O marco P2 passou da Sprint 4 para a Sprint 5.
- A interpretação de comandos, funções e E/S, prevista para a Sprint 3 original, foi para a Sprint 4. A Sprint 3 priorizou o que o P1 cobra: front-end fechado, testes automatizados, pipeline integrado para a demonstração e documentação.
- As entregas de testes, CI, pipeline e documentação da Sprint 3 foram concluídas entre 30/09 e 01/10, antes da apresentação do P1 em 05/10 (a partir do PR #56).

## Tabela de papéis (confirmada pelo grupo em 01/10/2026, issue #28)

| Papel | Responsável | Status |
|---|---|---|
| P1 (Léxico) | Brenda Beatriz implementou `scanner.l` (PR #15) | ✅ Implementado |
| P2 (Sintático/AST) | Brenda Beatriz implementou `parser.y` e a AST (PRs #16 e #42) | ✅ Implementado |
| Integração (`main.c`) | Giovana Ferreira Santos (issue #18 / PR #39); pipeline com flags e relatório por Brenda Beatriz (S3-01, PR #62) | ✅ Implementado |
| P3 (Semântico) | Giovana Ferreira Santos (tabela de símbolos, issue #22, PR #46; recuperação de erros sintáticos, PR #40); Mariana Martins (tipos na AST e compatibilidade, PR #53) | ✅ Implementado (PRs #46 e #53) |
| P4 (Interpretação) | Eduardo Ribeiro Xavier (expressões, PR #52; ambiente de execução e controle de fluxo, PRs #81 e #83; `scanf` e `printf` com `%s`, PRs #111 e #114); Mariana Martins (correções do `printf`, PR #87); Brenda Beatriz (chamada de função e retorno, PR #115) | ✅ Implementado, incluindo o limite de recursão (Giovana Ferreira Santos, PR #120) |
| Líder / P5 (Integração, Testes, Documentação) | Brenda Beatriz (testes, CI e documentação do P1: PRs #56 a #64) | Ativo |
| Scrum Master | Henrique Fontenelle (revisão e integração, decisões técnicas, sincronização `Dev`/`main`) | Ativo |

## Sprint 1: Ambiente, linguagem e léxico

**Planejado:**
- [x] Confirmar escopo por escrito em `docs/documentacao/definicao-da-linguagem.md`
- [x] Desenhar gramática em alto nível, definir tokens
- [x] Criar repositório com estrutura `docs/` + `src/`
- [x] Configurar ambiente em todas as máquinas
- [x] Designar o líder para os formulários: Brenda (issue #6, fechada em 07/09); registro formal por escrito, com Henrique como suplente, feito em 01/10 (issue #30, ver `decisoes-tecnicas.md`)
- [x] Iniciar `scanner.l` em par (P1 + P5); primeiros testes de tokens
- [x] Fechar `scanner.l` completo (tokens, reservadas, literais, erro léxico básico), testado com `flex`/`gcc -lfl`

**Entregue:** Ambiente configurado e testado em todas as máquinas. Repositório criado (estrutura reorganizada depois, na Sprint 2, para `docs/` + `src/` + `tests/` alinhados ao projeto). Escopo do núcleo documentado em `definicao-da-linguagem.md`. Gramática de alto nível definida em `gramatica.md`. `scanner.l` completo, testado e mesclado (PR #15). Padrão de commits definido e site da documentação publicado no GitHub Pages. Pendente: registro formal por escrito do líder responsável pelos formulários e do suplente (issue #30, concluído em 01/10).

**Issues fechadas:**

| Issue | Descrição | PR / commit | Responsáveis |
|---|---|---|---|
| #4 | Padronizar commits | `bb380b9` | Giovana |
| #5 | Criação do GitHub Pages | `c877a6f`, `963ff34` | Mariana, Brenda |
| #6 | Decidir líder dos formulários | - | Equipe (Brenda definida como líder) |
| #7 | Desenhar a gramática em alto nível | PR #16 | Brenda |
| #8 | Iniciar `scanner.l` em par; primeiros testes de tokens | PR #15 | Brenda |
| #9 | Fechar `scanner.l` completo | PR #15 | Brenda |
| #14 | Documentar escopo da linguagem e exemplo de programa | `61cd488` | Brenda |

## Sprint 2: Sintático, AST e início do semântico (até 16/09)

**Planejado:**
- [x] Revisar gramática, discutir estrutura dos nós da AST
- [x] Pair programming P1+P2: conectar scanner ao `parser.y`, gramática de expressões com precedência, primeiras ações semânticas construindo AST
- [x] Completar `parser.y` (comandos, blocos) construindo AST
- [x] Iniciar tabela de símbolos e verificação de declaração/uso de variáveis (P3, par com P4)
- [x] Testes de declarações, expressões, `if`/`while`; atualizar `docs/` e README

**Entregue:** `parser.y` completo com construção de AST (PR #16), recuperação de erros sintáticos (PR #40) e linha correta nos nós de expressão (PR #42). Integração implementada além do previsto para esta sprint: `main.c` real e binário `interpretador` funcionando (issue #18 / PR #39, validado manualmente). Testes de tokens e estruturas de controle criados (`tokens_basico.c`, `organizador_compras.c`). README atualizado com o comando de build/execução real. Tabela de símbolos com escopos aninhados e verificação de declaração/uso integradas à análise semântica (PR #46), com testes de escopo, shadowing, redeclaração e variável não declarada (PR #50). Pendente na época: faltavam testes para função com parâmetros, retorno e recursão (issue #25), o que resolvemos na Sprint 3 com o PR #56. Ficaram abertas desta sprint: README com os integrantes e papéis reais (S2-11, issue #28; na época o README ainda mostrava "Nome: ...") e registro por escrito do líder e do suplente (S2-12, issue #30, concluído em 01/10).

**Issues fechadas:**

| Issue | Descrição | PR / commit | Responsáveis |
|---|---|---|---|
| #18 | S2-01: `main.c` e binário `interpretador` | PR #39 | Giovana |
| #19 | S2-02: Recuperação de erros sintáticos com o token `error` | PR #40 | Giovana |
| #41 | Bug: recuperação de erros não integrada ao `main.c` real | `f77e74f` | Brenda |
| #21 | S2-04: Linha correta nas expressões (`@$` no lugar de `yylineno`) | PR #42 | Brenda (aprovação de Mariana) |
| #45 | Reunião de Alinhamento 01 | - | Equipe |
| #22 | S2-05: Tabela de símbolos com escopos aninhados | PR #46, testes no PR #50 | Giovana; testes de Henrique |
| #23 | S2-06: Variável não declarada e redeclaração no mesmo escopo | PR #46 (validada em 24/09) | Giovana; validação de Henrique |

## Sprint 3: Semântica de tipos, início da interpretação e preparação do P1 (até 23/09; apresentação do P1 em 05/10)

**Planejado:**
- [x] Campo de tipo na AST e verificação de compatibilidade de tipos (S2-07, issue #24, PR #53), com a padronização das mensagens e testes (issue #55, PR #57)
- [x] Decidir a estrutura do interpretador e avaliar expressões sobre a AST (`interpretarNo()`, issue #31, PR #52)
- [x] Ampliar a suíte de testes léxicos e sintáticos (S2-08, issue #25, PR #56)
- [x] `run_tests.sh` com comparação de saída esperada, integrado ao `make test` (S2-09, issue #26, PR #58)
- [x] CI que compila e roda os testes em cada PR (S2-10, issue #27, PR #59)
- [x] Checkpoint: pipeline integrado de ponta a ponta, com flags de inspeção e relatório de erros (S3-01, issue #32, PR #62)
- [x] Registrar os problemas reais enfrentados (S3-03, issue #34, PR #63)
- [x] Registrar as decisões técnicas da Sprint 2 (S3-04, issue #35, PR #64)
- [x] Atualizar o planejamento: planejado × entregue e replanejamento (S3-02, issue #33)
- [x] Enviar o formulário P1: enviado pela líder em 22/09/2026, dentro do prazo (S3-05, issue #17)
- [ ] Ensaio da apresentação e da entrevista do P1 (P1-01, issue #37)
- [ ] Atualizar o site da documentação no GitHub Pages (S3-07, issue #51), passada para a Sprint 4
- [ ] Congelar a `main` e criar a tag `v0.1-p1` (S3-06, issue #36)
- [x] Registrar por escrito o líder dos formulários e o suplente (S2-12, issue #30): Brenda líder, Henrique suplente

**Marco P1:** Front-end (léxico + sintático/AST + início do semântico) compilando, versionado, com `docs/` atualizado. **Atingido.** O início do semântico, que estava atrasado, foi concluído com a tabela de símbolos e a verificação de declaração/uso (issue #22 / PR #46; S2-06, issue #23, validada e encerrada em 24/09/2026). Formulário P1 enviado pela líder em 22/09/2026, dentro do prazo (23/09/2026).

**Entregue:**
- **Semântico:** tipos sintetizados na AST e recusa de conversão implícita `float` → `int` (PR #53), com mensagem única e testes de atribuição e inicialização (PR #57).
- **Interpretação:** avaliação de expressões aritméticas, relacionais e lógicas com precedência e promoção `int` → `float`, `printf` com `%d`/`%f`/`%c` e erro de divisão por zero (PR #52).
- **Testes:** suíte organizada em um arquivo por construção, com 13 casos válidos e 13 inválidos (PRs #56 e #57); `run_tests.sh` comparando a saída de 15 casos com arquivos `.esperado` (PR #58); CI no GitHub Actions rodando build e testes em todo PR, verificada com PRs propositais de código quebrado e de teste falhando (PR #59).
- **Integração:** `main.c` encadeando as fases com parada na primeira que falhar, flags `--tokens`/`--ast`/`--tabela`, relatório de erros por fase e roteiro de demonstração com plano B (PR #62).
- **Documentação:** 19 problemas registrados com causa e lição (PR #63); decisões S2-02, S2-03, S2-05 e S2-07 com justificativa, mais as decisões da Sprint 3 (PR #64); este planejamento atualizado.

**Pendente, e por quê:**
- **Execução de comandos, funções e E/S no interpretador** (`if`/`while`/`for`/`return`, chamadas, `scanf`): era da Sprint 3 original e foi para a Sprint 4 no replanejamento, porque a Sprint 3 encurtada priorizou o que o P1 avalia.
- **Tag `v0.1-p1` (S3-06):** é o último passo, depende de todos os PRs do P1 estarem mesclados na `Dev` para então mesclar a `Dev` na `main`. A `main` não recebe a `Dev` desde 15/09 (PR #48), então esse merge vai trazer tudo o que foi feito depois disso.
- **GitHub Pages (S3-07):** movida para a Sprint 4 por decisão da equipe. O site deve publicar a documentação já atualizada desta sprint (problemas, decisões, planejamento e roteiro de demonstração), então faz mais sentido fazê-lo depois que esses documentos forem mesclados e a `Dev` chegar à `main`.
- **Ensaio (P1-01):** depende de reunião do grupo.
- **Aprovação dos PRs:** os PRs #56 a #64 foram mesclados sem revisão de outro integrante, contrariando a regra da equipe; ver processo na Sprint 4.

**Issues fechadas:**

| Issue | Descrição | PR | Responsáveis |
|---|---|---|---|
| #24 | S2-07: Campo de tipo na AST e verificação de compatibilidade | PR #53 | Mariana |
| #31 | S2-13: Esqueleto do interpretador avaliando expressões | PR #52 | Eduardo |
| #20 | S2-03: Conflito shift/reduce do *dangling else* | PRs #43 e #56 | Brenda, Henrique |
| #25 | S2-08: Ampliar a suíte de testes léxicos e sintáticos | PR #56 | Brenda |
| #55 | S2-07: Testes de `float` → `int` e padronização das mensagens | PR #57 | Brenda (verificação de Henrique) |
| #26 | S2-09: `run_tests.sh` com asserções e integração ao `make test` | PR #58 | Brenda |
| #27 | S2-10: CI que compila e roda os testes em cada PR | PR #59 | Brenda |
| #32 | S3-01: Integração ponta a ponta com relatório de erros | PR #62 | Brenda |
| #34 | S3-03: `problemas-e-solucoes.md` com os problemas reais | PR #63 | Brenda, Henrique |
| #35 | S3-04: `decisoes-tecnicas.md` | PRs #43, #54 e #64 | Henrique, Brenda |

As issues S2-03 a S2-10 eram da Sprint 2 no planejamento original e foram concluídas na Sprint 3, depois do replanejamento.

## Sprint 4: Interpretação completa: comandos, funções e E/S

**Planejado:**
- [x] Executar comandos no interpretador: blocos, `if`/`else`, `while`, `for` e `return` (S4-04, issue #73, PR #83)
- [x] Chamadas de função com parâmetros, retorno e recursão. Ficou para a Sprint 5 (S5-01, issue #80, PR #115; limite de recursão na S5-02, issue #88, PR #120)
- [x] Migrar o armazenamento de variáveis do interpretador para cima da `TabelaSimbolos`, com escopos. Mudamos de ideia: o interpretador ganhou um ambiente de execução próprio, separado da tabela (S4-03, issue #72, PR #81; o motivo está em `decisoes-tecnicas.md`)
- [x] `printf` decodificando sequências de escape (`\n`, `\t`) e implementando `%s`; `scanf` lendo valores na execução. As escapes entraram na Sprint 4 (S4-02, issue #71, PR #87), e o `%s` e o `scanf` na Sprint 5 (PRs #114 e #111)
- [ ] Mensagem de erro léxico específica para string não fechada
- [x] Novos casos de teste com saída esperada para programas que dependem de comandos (S4-01, issue #70, PRs #82 e #84)
- [ ] Atualizar o site da documentação no GitHub Pages com os documentos da Sprint 3 e o histórico de sprints (S3-07, issue #51, vinda da Sprint 3)
- [x] Concluir as pendências do P1: tag `v0.1-p1` (S3-06, PR #69) e README com integrantes e papéis reais (S2-11, PR #68)
- [x] Confirmar as decisões provisórias: os papéis foram confirmados em 01/10, as políticas de escopo (S2-05) e de conversão implícita (S2-07) ficaram como estavam até a entrega final, e o fluxo de branches ficou com todos os PRs indo para a `Dev`, que vai para a `main` nos marcos de entrega
- [ ] Cada integrante conferir o próprio registro em `problemas-e-solucoes.md`
- [ ] Processo: exigir aprovação antes do merge e ativar *Require status checks* (`build-e-testes`) nas regras de branch (S4-09, issue #78)

**Checkpoint:** organizador de compras executando de ponta a ponta, lendo os preços com `scanf` e decidindo dentro do orçamento. **Atingido na Sprint 5** (S5-05, issue #91, PR #113), porque dependia do `scanf` e das chamadas de função.

**Replanejamento de 26/10:** depois do P1, ficamos de 01/10 a 08/11 sem mesclar nenhum PR. Em 26/10 revisamos o código da `main` e descobrimos que a suíte passava sem o interpretador executar a maior parte dos programas: atribuição, `if`, `while`, `for` e `return` eram pulados sem aviso, e os testes válidos só conferiam o código de saída. A partir dessa revisão refizemos o backlog, com as issues S4-01 a S4-10 nos milestones do GitHub, e passamos as chamadas de função, o `scanf` e o resto do `printf` para a Sprint 5.

**Entregue:**
- **Testes:** escrevemos à mão os `.esperado` dos testes válidos e passamos a redirecionar um `.entrada` para a entrada padrão. O `tests/pendentes.txt` marcava os casos que ainda dependiam de implementação (S4-01, PRs #82 e #84).
- **Interpretação:** ambiente de execução com pilha de escopos, atribuição e conversão para o tipo declarado (S4-03, PR #81); `if`/`else`, `while`, `for`, `return`, curto-circuito de `&&` e `||`, limite de iterações e erro interno no lugar dos comandos pulados sem aviso (S4-04, PR #83); `printf` sem ler fora da memória, com `%` no fim do texto e escapes decodificadas (S4-02, PR #87).
- **Semântico:** passamos a recusar programa sem `main`, variável ou parâmetro `void`, `&` fora do `scanf` e `return` de tipo errado (S4-06, PR #85), e a conferir o tipo dos argumentos nas chamadas de função (S4-05, PR #112).
- **Sintático:** ponteiros, vetores, structs, unions e `for` com declaração passaram a ser recusados com a mensagem "construcao fora do escopo", e a `gramatica.md` ficou igual ao `parser.y` (S4-07, PR #86).
- **Integração:** flag `--no-run` e código de saída vindo do `return` do `main` (S4-08, PR #107).

**Pendente, e por quê:**
- **Mensagem para string não fechada:** não chegou a virar issue; está registrada como problema em aberto em `problemas-e-solucoes.md`.
- **GitHub Pages (S3-07, issue #51):** vai junto com o fechamento dos documentos (S7-05, issue #102).

**Issues fechadas:**

| Issue | Descrição | PR | Responsáveis |
|---|---|---|---|
| #70 | S4-01: Testes com saída esperada e entrada redirecionada | PRs #82 e #84 | Mariana |
| #71 | S4-02: `printf` com argumentos, `%` no fim e escapes | PR #87 | Mariana, Brenda |
| #72 | S4-03: Ambiente de execução com escopos, atribuição e conversão | PR #81 | Eduardo |
| #73 | S4-04: `if`/`else`, `while`, `for`, `return` e curto-circuito | PR #83 | Eduardo |
| #74 | S4-05: Semântica de chamadas (existência, aridade e tipos) | PR #112 | Eduardo |
| #75 | S4-06: Programa sem `main`, `void`, `&` e `return` | PR #85 | Giovana |
| #76 | S4-07: Núcleo contra a gramática e construções fora do escopo | PR #86 | Giovana, Brenda |
| #77 | S4-08: `--no-run` e código de saída do `return` do `main` | PR #107 | Eduardo |

Parte desse trabalho ficou pronta em 08/11, mas as issues só foram fechadas em 18/11. Um PR mesclado na `Dev` não fecha a issue sozinho (ver `problemas-e-solucoes.md`), então fechamos pelo PR `Dev` para `main` (#110) ou à mão.

## Sprint 5: Robustez, testes, documentação

**Planejado:**
- [x] Revisar o que falta, priorizar robustez do núcleo > diferencial opcional. Fizemos isso na revisão de código de 26/10, que gerou o backlog das Sprints 4 a 8
- [x] Ajustes finais, revisão cruzada de módulos (S6-01, issue #94). A revisão foi feita durante o trabalho: cada integrante testa o PR de outro antes de aprovar e revisa a documentação conforme ela é escrita
- [x] Suíte de testes cobrindo o núcleo inteiro (casos válidos e de erro). Desde o PR #115 todos os casos válidos executam e comparam a saída; falta a tabela de cobertura (S5-06, issue #92)
- [x] Fechar `docs/documentacao/decisoes-tecnicas.md` e `docs/documentacao/problemas-e-solucoes.md`; atualizar README com exemplos. O README ganhou dois exemplos reais (PR #113), e os documentos foram fechados na S7-05 (issue #102, PR #132)
- [x] Ensaio da entrevista (S8-01, issue #104). Como a preparação para a entrevista é individual, ficou um material de apoio para o estudo de cada um

**Marco P2:** Interpretador completo (núcleo), testado, documentado; formulário P2 enviado pelo líder. **Atingido em parte:** a Giovana enviou o formulário do P2, e o núcleo ficou completo e testado em 18/11, com chamadas de função, `scanf`, `printf` com `%s` e o organizador de compras rodando de ponta a ponta. O limite de recursão entrou depois, em 27/11.

**Entregue:**
- **Interpretação:** chamada de função com escopo próprio, parâmetros por valor, retorno convertido para o tipo da função, variáveis globais e recursão, com marcações separadas para `return` e para erro (S5-01, PR #115); `printf` com `%s`, escapes dentro do `%s` e erro quando o tipo do argumento não combina, mostrando o relatório no lugar do antigo `exit(5)` (S5-03, PR #114); `scanf` com `%d`, `%f` e `%c`, erro sem `&` e tratamento do fim da entrada (S5-04, PR #111); limite de 1000 chamadas aninhadas, com erro claro em vez de travar o programa (S5-02, PR #120).
- **Estudo de caso:** o organizador de compras rodando de ponta a ponta na suíte, com `.entrada` e `.esperado`, um segundo exemplo (total por quantidade) e os dois no README (S5-05, PR #113).
- **Testes:** o `tests/pendentes.txt` ficou vazio, e corrigimos três `.esperado` antigos que estavam errados (PR #115).
- **Documentação:** registramos as decisões da S5-01, S5-02 e S5-03 e atualizamos as limitações e os problemas resolvidos (PR #122).

**Pendente, e por quê:**
- **Tabela de cobertura (S5-06, issue #92):** os testes existem e passam; falta escrever qual teste cobre cada item do núcleo. Entregue depois, na Sprint 6 (PR #126).
- **Decisão do `scanf` (S5-04):** o que fazer no fim da entrada e com entrada inválida está implementado, mas ainda não está escrito em `decisoes-tecnicas.md`.

**Issues fechadas:**

| Issue | Descrição | PR | Responsáveis |
|---|---|---|---|
| #80 | S5-01: Chamada de função, parâmetros e retorno | PR #115 | Brenda |
| #89 | S5-03: `printf` com `%s` e erros de tipo | PR #114 | Eduardo |
| #90 | S5-04: `scanf` com `%d`, `%f` e `%c` | PR #111 | Eduardo |
| #91 | S5-05: Estudo de caso do organizador de compras | PR #113 | Eduardo |

A issue #88 (S5-02, limite de recursão, PR #120, Giovana) já está na `Dev` e fecha quando a `Dev` for para a `main`.

## Sprint 6: Otimizações, recursos extras e testes integrados

**Planejado:**
- [ ] Otimizações opcionais na interpretação (ex.: simplificação de expressões constantes), só se o núcleo já estiver estável
- [ ] Recursos adicionais na linguagem, caso haja tempo (respeitando a ordem de prioridade de diferenciais definida em `definicao-da-linguagem.md`)
- [x] Testes de integração cobrindo programas mais completos, não só casos isolados
- [ ] Preparar a versão candidata à entrega final (repositório atualizado, README completo)

**Entregue:**
- **Estudos de caso:** em vez de testes de integração soltos, cada integrante escreveu um programa completo e validou o interpretador com ele: boletim escolar (Mariana, PR #123), calculadora de números (Eduardo, PR #125), controle de temperatura (Brenda, PR #121) e caixa eletrônico (Giovana, issue #117). Os programas e os resultados estão em [Estudos de Casos](/documentacao/estudos-de-casos.md).
- **Bug achado pelos estudos de caso:** a calculadora de números mostrou que a divisão por zero exibia o erro mas deixava o programa continuar. Corrigimos para a divisão por zero interromper a execução, como os outros erros (issue #124, PR #125).
- **Testes mais extensos (diferencial 2):** a suíte do núcleo com saída esperada em todos os casos e a tabela de cobertura em `tests/README.md` (S5-06, PR #126), os casos-limite (S7-03, PR #129) e a lista completa de erros de execução, com o resto por zero (S6-02, PR #130). São 88 testes.
- **Relatório de erros unificado:** as quatro fases passaram a reportar os erros por um único módulo, `erros.c`, sem mudar o texto das mensagens (S6-08, issue #97, PR #131, Eduardo).
- **Documentação:** limitações, problemas resolvidos e roteiro de demonstração atualizados (PR #122 e branch `docs-fechamento-final`).

**Pendente, e por quê:**
- **Otimizações e diferenciais:** decidimos priorizar um núcleo completo e testado, e nenhuma otimização foi implementada. Dos diferenciais, decidimos em grupo em 27/11 fazer os testes mais extensos e o relatório unificado; a coluna nas mensagens de erro e os vetores ficaram de fora, para não arriscar o núcleo no dia da entrega (S7-01, issue #98, explicado em [Decisões Técnicas](/documentacao/decisoes-tecnicas.md)).
- **Versão candidata à entrega:** depende de levar a `Dev` para a `main` e de fechar o README (S7-04, issue #101).

## Sprint 7: Entrevistas finais e encerramento

**Planejado:**
- [ ] Participar das entrevistas finais com o professor (equipe completa)
- [ ] Corrigir pendências/bugs apontados durante as entrevistas ou testes finais
- [x] Finalizar documentação (README, `docs/`, exemplos de uso)
- [x] Conferir datas de entrevista no plano de ensino, porque faltar pode zerar a nota da apresentação final. O professor confirmou a entrevista em 30/11

**Entregue (até 27/11):**
- **Diferenciais:** decidimos em grupo fazer os testes mais extensos e o relatório de erros unificado, e deixar de fora a coluna nas mensagens de erro e os vetores (S7-01, issue #98; ver [Decisões Técnicas](/documentacao/decisoes-tecnicas.md)).
- **Testes:** casos-limite na suíte (S7-03, PR #129), chegando a 88 testes.
- **Documentação:** todos os documentos de `docs/` fechados, com os estudos de caso, as atas das reuniões e o design novo do site (S7-05, PR #132).
- **Conferência final:** checklist feito num clone limpo da `Dev`, com o build do zero e os 88 testes passando (S8-02, issue #105).

**Pendente:** levar a `Dev` para a `main` e criar a tag `v1.0` (S7-06, issue #103). O que o professor apontar nas entrevistas de 30/11 e 02/12 registramos depois.

## Histórico de entregas até o P1

Pull Requests mesclados até 01/10/2026, na ordem de número.

| PR | Data do merge | Conteúdo | Issue |
|---|---|---|---|
| #2 | 02/09 | Estrutura do projeto (`docs/` + `src/` + `tests/`) | - |
| #3 | 02/09 | Checkboxes no planejamento | - |
| #10 | 04/09 | Estrutura do projeto do interpretador | - |
| #13 | 06/09 | Templates de issue/PR, workflow de deploy e estrutura da documentação | - |
| #15 | 06/09 | `scanner.l` com tokens, palavras reservadas e erro léxico | - |
| #16 | 06/09 | Gramática e `parser.y` com construção da AST | #7 |
| #39 | 09/09 | `main.c` e binário `interpretador` | #18 |
| #40 | 09/09 | Recuperação de erros sintáticos com o token `error` (S2-02) | - |
| #42 | 10/09 | Linha correta nas expressões com `@$.first_line` (S2-04) | #21 |
| #43 | 14/09 | Decisões técnicas: dangling else, resposta do professor | #35 |
| #46 | 16/09 | Tabela de símbolos com escopos aninhados e verificação de declaração/uso | #22, #23 |
| #47 / #48 | 16/09 | Revert e reaplicação da sincronização `Dev` → `main` | - |
| #50 | 16/09 | Testes de shadowing, redeclaração e identificador não declarado | #22 |
| #53 | 24/09 | Campo de tipo na AST e verificação de compatibilidade (S2-07) | #24 |
| #44 | 29/09 | Planejamento: checkboxes, entregue e replanejamento | #33 |
| #52 | 29/09 | Interpretador avaliando expressões | #31 |
| #54 | 29/09 | Decisões técnicas: correção da S2-07 e sincronização com a `main` | - |
| #56 | 01/10 | Suíte de testes léxicos e sintáticos (S2-08) | #25 |
| #57 | 01/10 | Mensagem padronizada e testes de `float` → `int` (S2-07) | #55 |
| #58 | 01/10 | `run_tests.sh` com saída esperada (S2-09) | #26 |
| #59 | 01/10 | CI de build e testes em cada PR (S2-10) | #27 |
| #62 | 01/10 | Pipeline integrado com flags e relatório de erros (S3-01) | #32 |
| #63 | 01/10 | Problemas e soluções (S3-03) | #34 |
| #64 | 01/10 | Decisões técnicas da Sprint 2 e 3 (S3-04) | #35 |

As datas são as exibidas pelo GitHub; os PRs #56 a #64 aparecem com 01/10 no GitHub e com a noite de 30/09 no histórico local, por causa do fuso horário.

**PRs fechados sem merge:** #1 (estrutura inicial das semanas 01 a 03), #11 (guia de padrão para mensagens de commit), #12 (templates de issue/PR e workflow de deploy), #38 (pendência do professor e papéis P1 a P4), #49 (testes da tabela de símbolos), #60 e #61 (rascunhos propositais para verificar a CI).

**Situação das issues em 01/10/2026:** 24 fechadas e 7 abertas: #17 (S3-05, formulário já enviado), #28 (S2-11), #30 (S2-12), #33 (S3-02, este documento), #36 (S3-06), #37 (P1-01) e #51 (S3-07, movida para a Sprint 4).

## Histórico de versões deste documento

| Data | Versão | Descrição | Autor |
|---|---|---|---|
| 05/09/2026 | 1.0 | Criação da página | Brenda |
| 11/09/2026 | 1.1 | Checkboxes, campos "Entregue" e primeiro registro do replanejamento | Henrique |
| 24/09/2026 | 1.2 | Atualização da Sprint 2 (PR #46, S2-06, envio do P1) | Henrique |
| 24/09/2026 | - | Proposta de tabelas de issues fechadas por sprint e deste histórico de versões (branch `docs-atualizacao-p1-pages`, não mesclada; estrutura incorporada na versão 2.0) | Mariana |
| 29/09/2026 | 1.3 | Tabela de papéis com as entregas reais de P3 e P4 | Brenda |
| 01/10/2026 | 2.0 | Replanejamento completo e renumeração das sprints, Sprint 3 preenchida, plano da Sprint 4, issues fechadas por sprint e histórico de PRs (S3-02) | Brenda |
| 26/11/2026 | 2.1 | Sprints 4, 5 e 6 preenchidas, com o replanejamento de 26/10, as pendências e as issues fechadas (S4-10 e S5-07) | Brenda |
| 27/11/2026 | 2.2 | Sprint 6 com os testes mais extensos, o relatório unificado e a decisão dos diferenciais | Brenda |
| 27/11/2026 | 2.3 | Sprint 7 preenchida e pendências antigas resolvidas para a entrega final | Brenda |
