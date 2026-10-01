# Planejamento de sprints

Planejado × entregue, por sprint. Atualizar ao final de cada sprint (exigência de repositório).

## Replanejamento (issue #33)

**O que mudou:** o planejamento original fechava a Sprint 2 com o marco P1. A Sprint 2 original foi dividida em duas:

| Sprint | Período | Foco |
|---|---|---|
| Sprint 2 | até **16/09** | Sintático, AST e início do semântico |
| Sprint 3 | até **23/09** (prazo do formulário P1), com a apresentação do P1 em **05/10** | Semântica de tipos, início da interpretação, testes, integração e documentação do P1 |

**Por quê:** o prazo real do formulário P1 foi 23/09, e a Semana Universitária caiu no meio do período, reduzindo o tempo útil. Em vez de comprimir num único bloco o fechamento do front-end, a verificação semântica e a preparação do P1, a equipe separou o trabalho em duas sprints e deixou explícito o que precisava estar pronto em cada data.

**Consequências:**
- As sprints seguintes foram **renumeradas**: a Sprint 3 original (interpretação) passou a ser a Sprint 4, e assim por diante. O marco P2 passou da Sprint 4 para a Sprint 5.
- A interpretação de comandos, funções e E/S, prevista para a Sprint 3 original, foi para a Sprint 4. A Sprint 3 priorizou o que o P1 cobra: front-end fechado, testes automatizados, pipeline integrado para a demonstração e documentação.
- As entregas de testes, CI, pipeline e documentação da Sprint 3 foram concluídas entre 30/09 e 01/10, antes da apresentação do P1 em 05/10 (a partir do PR #56).

## Tabela de papéis (status atual, sujeito a confirmação do time)

| Papel | Responsável | Status |
|---|---|---|
| P1 — Léxico | Brenda Beatriz implementou `scanner.l` (PR #15); atribuição formal a confirmar | ✅ Implementado |
| P2 — Sintático/AST | Brenda Beatriz implementou `parser.y` e a AST (PRs #16 e #42); atribuição formal a confirmar | ✅ Implementado |
| Integração (`main.c`) | Giovana Ferreira Santos (issue #18 / PR #39); pipeline com flags e relatório por Brenda Beatriz (S3-01, PR #62) | ✅ Implementado |
| P3 — Semântico | Giovana Ferreira Santos (tabela de símbolos, issue #22, PR #46; recuperação de erros sintáticos, PR #40); Mariana Martins (tipos na AST e compatibilidade, PR #53); autoria formal a confirmar | ✅ Implementado (PRs #46 e #53) |
| P4 — Interpretação | Eduardo Ribeiro Xavier (esqueleto avaliando expressões, issue #31, PR #52); autoria formal a confirmar | 🟡 Expressões implementadas; comandos e funções na Sprint 4 |
| Líder / P5 — Integração, Testes, Documentação | Brenda Beatriz (testes, CI e documentação do P1: PRs #56–#64) | Ativo |
| Scrum Master | Henrique Fontenelle (revisão e integração, decisões técnicas, sincronização `Dev`/`main`) | Ativo |

## Sprint 1 — Ambiente, linguagem e léxico

**Planejado:**
- [x] Confirmar escopo por escrito em `docs/documentacao/definicao-da-linguagem.md`
- [x] Desenhar gramática em alto nível, definir tokens
- [x] Criar repositório com estrutura `docs/` + `src/`
- [x] Configurar ambiente em todas as máquinas
- [x] Designar o líder para os formulários — Brenda (issue #6, fechada em 07/09); registro formal por escrito, com Henrique como suplente, feito em 01/10 (issue #30, ver `decisoes-tecnicas.md`)
- [x] Iniciar `scanner.l` em par (P1 + P5); primeiros testes de tokens
- [x] Fechar `scanner.l` completo (tokens, reservadas, literais, erro léxico básico), testado com `flex`/`gcc -lfl`

**Entregue:** Ambiente configurado e testado em todas as máquinas. Repositório criado (estrutura reorganizada depois, na Sprint 2, para `docs/` + `src/` + `tests/` alinhados ao projeto). Escopo do núcleo documentado em `definicao-da-linguagem.md`. Gramática de alto nível definida em `gramatica.md`. `scanner.l` completo, testado e mesclado (PR #15). Padrão de commits definido e site da documentação publicado no GitHub Pages. Pendente: registro formal por escrito do líder responsável pelos formulários e do suplente (issue #30, concluído em 01/10).

**Issues fechadas:**

| Issue | Descrição | PR / commit | Responsáveis |
|---|---|---|---|
| #4 | Padronizar commits | `bb380b9` | Giovana |
| #5 | Criação do GitHub Pages | `c877a6f`, `963ff34` | Mariana, Brenda |
| #6 | Decidir líder dos formulários | — | Equipe (Brenda definida como líder) |
| #7 | Desenhar a gramática em alto nível | PR #16 | Brenda |
| #8 | Iniciar `scanner.l` em par; primeiros testes de tokens | PR #15 | Brenda |
| #9 | Fechar `scanner.l` completo | PR #15 | Brenda |
| #14 | Documentar escopo da linguagem e exemplo de programa | — | a confirmar |

## Sprint 2 — Sintático, AST e início do semântico (até 16/09)

**Planejado:**
- [x] Revisar gramática, discutir estrutura dos nós da AST
- [x] Pair programming P1+P2: conectar scanner ao `parser.y`, gramática de expressões com precedência, primeiras ações semânticas construindo AST
- [x] Completar `parser.y` (comandos, blocos) construindo AST
- [x] Iniciar tabela de símbolos e verificação de declaração/uso de variáveis (P3, par com P4)
- [x] Testes de declarações, expressões, `if`/`while`; atualizar `docs/` e README

**Entregue:** `parser.y` completo com construção de AST (PR #16), recuperação de erros sintáticos (PR #40) e linha correta nos nós de expressão (PR #42). Integração implementada além do previsto para esta sprint: `main.c` real e binário `interpretador` funcionando (issue #18 / PR #39, validado manualmente). Testes de tokens e estruturas de controle criados (`tokens_basico.c`, `organizador_compras.c`). README atualizado com o comando de build/execução real. Tabela de símbolos com escopos aninhados e verificação de declaração/uso integradas à análise semântica (PR #46), com testes de escopo, shadowing, redeclaração e variável não declarada (PR #50). Pendente na época: lacuna de testes para função com parâmetros/retorno/recursão (issue #25) — resolvida na Sprint 3 pelo PR #56. Continuam abertas desta sprint: README com os integrantes e papéis reais (S2-11, issue #28 — o README ainda mostra "Nome — ...") e registro por escrito do líder e do suplente (S2-12, issue #30, concluído em 01/10).

**Issues fechadas:**

| Issue | Descrição | PR / commit | Responsáveis |
|---|---|---|---|
| #18 | S2-01 — `main.c` e binário `interpretador` | PR #39 | Giovana |
| #19 | S2-02 — Recuperação de erros sintáticos com o token `error` | PR #40 | Giovana |
| #41 | Bug: recuperação de erros não integrada ao `main.c` real | `f77e74f` | Brenda |
| #21 | S2-04 — Linha correta nas expressões (`@$` no lugar de `yylineno`) | PR #42 | Brenda (aprovação de Mariana) |
| #45 | Reunião de Alinhamento 01 | — | Equipe |
| #22 | S2-05 — Tabela de símbolos com escopos aninhados | PR #46, testes no PR #50 | Giovana; testes de Henrique |
| #23 | S2-06 — Variável não declarada e redeclaração no mesmo escopo | PR #46 (validada em 24/09) | Giovana; validação de Henrique |

## Sprint 3 — Semântica de tipos, início da interpretação e preparação do P1 (até 23/09; apresentação do P1 em 05/10)

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
- [x] Enviar o formulário P1 — enviado pela líder em 22/09/2026, dentro do prazo (S3-05, issue #17)
- [ ] Ensaio da apresentação e da entrevista do P1 (P1-01, issue #37)
- [ ] Atualizar o site da documentação no GitHub Pages (S3-07, issue #51) — movida para a Sprint 4
- [ ] Congelar a `main` e criar a tag `v0.1-p1` (S3-06, issue #36)
- [x] Registrar por escrito o líder dos formulários e o suplente (S2-12, issue #30) — Brenda líder, Henrique suplente

**Marco P1:** Front-end (léxico + sintático/AST + início do semântico) compilando, versionado, com `docs/` atualizado — **atingido**. O início do semântico, que estava atrasado, foi concluído com a tabela de símbolos e a verificação de declaração/uso (issue #22 / PR #46; S2-06, issue #23, validada e encerrada em 24/09/2026). Formulário P1 enviado pela líder em 22/09/2026, dentro do prazo (23/09/2026).

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
| #24 | S2-07 — Campo de tipo na AST e verificação de compatibilidade | PR #53 | Mariana |
| #31 | S2-13 — Esqueleto do interpretador avaliando expressões | PR #52 | Eduardo |
| #20 | S2-03 — Conflito shift/reduce do *dangling else* | PRs #43 e #56 | Brenda, Henrique |
| #25 | S2-08 — Ampliar a suíte de testes léxicos e sintáticos | PR #56 | Brenda |
| #55 | S2-07 — Testes de `float` → `int` e padronização das mensagens | PR #57 | Brenda (verificação de Henrique) |
| #26 | S2-09 — `run_tests.sh` com asserções e integração ao `make test` | PR #58 | Brenda |
| #27 | S2-10 — CI que compila e roda os testes em cada PR | PR #59 | Brenda |
| #32 | S3-01 — Integração ponta a ponta com relatório de erros | PR #62 | Brenda |
| #34 | S3-03 — `problemas-e-solucoes.md` com os problemas reais | PR #63 | Brenda, Henrique |
| #35 | S3-04 — `decisoes-tecnicas.md` | PRs #43, #54 e #64 | Henrique, Brenda |

As issues S2-03 a S2-10 eram da Sprint 2 no planejamento original e foram concluídas na Sprint 3, depois do replanejamento.

## Sprint 4 — Interpretação completa: comandos, funções e E/S

**Planejado:**
- [ ] Executar comandos no interpretador: blocos, `if`/`else`, `while`, `for` e `return`
- [ ] Chamadas de função com parâmetros, retorno e recursão
- [ ] Migrar o armazenamento de variáveis do interpretador para cima da `TabelaSimbolos`, com escopos (limitação registrada em `decisoes-tecnicas.md`)
- [ ] `printf` decodificando sequências de escape (`\n`, `\t`) e implementando `%s`; `scanf` lendo valores na execução
- [ ] Mensagem de erro léxico específica para string não fechada
- [ ] Novos casos de teste com saída esperada para programas que dependem de comandos (fatorial, organizador de compras com entrada)
- [ ] Atualizar o site da documentação no GitHub Pages com os documentos da Sprint 3 e o histórico de sprints (S3-07, issue #51, vinda da Sprint 3)
- [ ] Concluir as pendências do P1: tag `v0.1-p1` (S3-06) e README com integrantes e papéis reais (S2-11)
- [ ] Confirmar em reunião as decisões provisórias: conversão implícita (S2-07), papéis P1/P2/P4 e fluxo de branches
- [ ] Cada integrante conferir o próprio registro em `problemas-e-solucoes.md`
- [ ] Processo: exigir aprovação antes do merge e ativar *Require status checks* (`build-e-testes`) nas regras de branch

**Checkpoint:** organizador de compras executando de ponta a ponta, lendo os preços com `scanf` e decidindo dentro do orçamento.

**Entregue:** _(preencher ao final da sprint)_

## Sprint 5 — Robustez, testes, documentação

**Planejado:**
- [ ] Revisar o que falta, priorizar robustez do núcleo > diferencial opcional
- [ ] Ajustes finais, revisão cruzada de módulos
- [ ] Suíte de testes cobrindo o núcleo inteiro (casos válidos e de erro)
- [ ] Fechar `docs/documentacao/decisoes-tecnicas.md` e `docs/documentacao/problemas-e-solucoes.md`; atualizar README com exemplos
- [ ] Ensaio da entrevista

**Marco P2:** Interpretador completo (núcleo), testado, documentado; formulário P2 enviado pelo líder.

**Entregue:** _(preencher ao final da sprint)_

## Sprint 6 — Otimizações, recursos extras e testes integrados

**Planejado:**
- [ ] Otimizações opcionais na interpretação (ex.: simplificação de expressões constantes), só se o núcleo já estiver estável
- [ ] Recursos adicionais na linguagem, caso haja tempo (respeitando a ordem de prioridade de diferenciais definida em `definicao-da-linguagem.md`)
- [ ] Testes de integração cobrindo programas mais completos, não só casos isolados
- [ ] Preparar a versão candidata à entrega final (repositório atualizado, README completo)

**Entregue:** _(preencher ao final da sprint)_

## Sprint 7 — Entrevistas finais e encerramento

**Planejado:**
- [ ] Participar das entrevistas finais com o professor (equipe completa)
- [ ] Corrigir pendências/bugs apontados durante as entrevistas ou testes finais
- [ ] Finalizar documentação (README, `docs/`, exemplos de uso)
- [ ] Conferir datas de entrevista no plano de ensino — falta de comparecimento pode zerar a nota da apresentação final

**Entregue:** _(preencher ao final da sprint)_

## Histórico de entregas até o P1

Pull Requests mesclados até 01/10/2026, na ordem de número.

| PR | Data do merge | Conteúdo | Issue |
|---|---|---|---|
| #2 | 02/09 | Estrutura do projeto (`docs/` + `src/` + `tests/`) | — |
| #3 | 02/09 | Checkboxes no planejamento | — |
| #10 | 04/09 | Estrutura do projeto do interpretador | — |
| #13 | 06/09 | Templates de issue/PR, workflow de deploy e estrutura da documentação | — |
| #15 | 06/09 | `scanner.l` com tokens, palavras reservadas e erro léxico | — |
| #16 | 06/09 | Gramática e `parser.y` com construção da AST | #7 |
| #39 | 09/09 | `main.c` e binário `interpretador` | #18 |
| #40 | 09/09 | Recuperação de erros sintáticos com o token `error` (S2-02) | — |
| #42 | 10/09 | Linha correta nas expressões com `@$.first_line` (S2-04) | #21 |
| #43 | 14/09 | Decisões técnicas: dangling else, resposta do professor | #35 |
| #46 | 16/09 | Tabela de símbolos com escopos aninhados e verificação de declaração/uso | #22, #23 |
| #47 / #48 | 16/09 | Revert e reaplicação da sincronização `Dev` → `main` | — |
| #50 | 16/09 | Testes de shadowing, redeclaração e identificador não declarado | #22 |
| #53 | 24/09 | Campo de tipo na AST e verificação de compatibilidade (S2-07) | #24 |
| #44 | 29/09 | Planejamento: checkboxes, entregue e replanejamento | #33 |
| #52 | 29/09 | Interpretador avaliando expressões | #31 |
| #54 | 29/09 | Decisões técnicas: correção da S2-07 e sincronização com a `main` | — |
| #56 | 01/10 | Suíte de testes léxicos e sintáticos (S2-08) | #25 |
| #57 | 01/10 | Mensagem padronizada e testes de `float` → `int` (S2-07) | #55 |
| #58 | 01/10 | `run_tests.sh` com saída esperada (S2-09) | #26 |
| #59 | 01/10 | CI de build e testes em cada PR (S2-10) | #27 |
| #62 | 01/10 | Pipeline integrado com flags e relatório de erros (S3-01) | #32 |
| #63 | 01/10 | Problemas e soluções (S3-03) | #34 |
| #64 | 01/10 | Decisões técnicas da Sprint 2 e 3 (S3-04) | #35 |

As datas são as exibidas pelo GitHub; os PRs #56 a #64 aparecem com 01/10 no GitHub e com a noite de 30/09 no histórico local, por causa do fuso horário.

**PRs fechados sem merge:** #1 (estrutura inicial das semanas 01 a 03), #11 (guia de padrão para mensagens de commit), #12 (templates de issue/PR e workflow de deploy), #38 (pendência do professor e papéis P1–P4), #49 (testes da tabela de símbolos), #60 e #61 (rascunhos propositais para verificar a CI).

**Situação das issues em 01/10/2026:** 24 fechadas e 7 abertas — #17 (S3-05, formulário já enviado), #28 (S2-11), #30 (S2-12), #33 (S3-02, este documento), #36 (S3-06), #37 (P1-01) e #51 (S3-07, movida para a Sprint 4).

## Histórico de versões deste documento

| Data | Versão | Descrição | Autor |
|---|---|---|---|
| 05/09/2026 | 1.0 | Criação da página | Brenda |
| 11/09/2026 | 1.1 | Checkboxes, campos "Entregue" e primeiro registro do replanejamento | Henrique |
| 24/09/2026 | 1.2 | Atualização da Sprint 2 (PR #46, S2-06, envio do P1) | Henrique |
| 24/09/2026 | — | Proposta de tabelas de issues fechadas por sprint e deste histórico de versões (branch `docs-atualizacao-p1-pages`, não mesclada; estrutura incorporada na versão 2.0) | Mariana |
| 29/09/2026 | 1.3 | Tabela de papéis com as entregas reais de P3 e P4 | Brenda |
| 01/10/2026 | 2.0 | Replanejamento completo e renumeração das sprints, Sprint 3 preenchida, plano da Sprint 4, issues fechadas por sprint e histórico de PRs (S3-02) | Brenda |
