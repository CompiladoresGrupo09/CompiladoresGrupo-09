# Planejamento de sprints

Planejado × entregue, por sprint. Atualizar ao final de cada sprint (exigência de repositório).

## Replanejamento (registrado em referência à issue #33)
Devido ao prazo real do formulário P1 (23/09) e à Semana Universitária no meio do caminho, a Sprint 2 passou a ir até **16/09** e a Sprint 3 até **23/09**, em vez do fechamento original previsto. Datas exatas e motivo detalhado a confirmar com a Brenda para deixar o registro completo.

## Tabela de papéis (status atual, sujeito a confirmação do time)

| Papel | Responsável | Status |
|---|---|---|
| P1 — Léxico | mesma pessoa que entregou P2 (autoria a confirmar formalmente) | ✅ Implementado |
| P2 — Sintático/AST | idem acima | ✅ Implementado |
| Integração (`main.c`) | Giovana Ferreira Santos | ✅ Implementado (issue #18 / PR #39) |
| P3 — Semântico | Eduardo Ribeiro Xavier e Mariana Martins (atribuídos à issue #22; sem atividade visível até o momento) | ❌ Não iniciado |
| P4 — Interpretação | sem responsável definido (issue #31 sugere pareamento com quem entregou a integração) | ❌ Não iniciado |
| Líder / P5 — Integração, Testes, Documentação | Brenda Beatriz | Ativo |
| Scrum Master | Henrique Fontenelle | Ativo |

## Sprint 1 — Ambiente, linguagem e léxico

**Planejado:**
- [x] Confirmar escopo por escrito em `docs/documentacao/definicao-da-linguagem.md`
- [x] Desenhar gramática em alto nível, definir tokens
- [x] Criar repositório com estrutura `docs/` + `src/`
- [x] Configurar ambiente em todas as máquinas
- [ ] Designar o líder para os formulários — combinado informalmente (Brenda), mas o registro formal por escrito com suplente ainda está em aberto (ver issue #30)
- [x] Iniciar `scanner.l` em par (P1 + P5); primeiros testes de tokens
- [x] Fechar `scanner.l` completo (tokens, reservadas, literais, erro léxico básico), testado com `flex`/`gcc -lfl`

**Entregue:** Ambiente configurado e testado em todas as máquinas. Repositório criado (estrutura reorganizada depois, na Sprint 2, para `docs/` + `src/` + `tests/` alinhados ao projeto). Escopo do núcleo documentado em `definicao-da-linguagem.md`. Gramática de alto nível definida em `gramatica.md`. `scanner.l` completo, testado e mesclado (PR #15). Pendente: registro formal por escrito do líder responsável pelos formulários e do suplente (issue #30, em aberto).

## Sprint 2 — Sintático, AST e início do semântico

**Planejado:**
- [x] Revisar gramática, discutir estrutura dos nós da AST
- [x] Pair programming P1+P2: conectar scanner ao `parser.y`, gramática de expressões com precedência, primeiras ações semânticas construindo AST
- [x] Completar `parser.y` (comandos, blocos) construindo AST
- [ ] Iniciar tabela de símbolos e verificação de declaração/uso de variáveis (P3, par com P4)
- [x] Testes de declarações, expressões, `if`/`while`; atualizar `docs/` e README

**Marco P1:** Front-end (léxico + sintático/AST) compilando, versionado, com `docs/` atualizado — **atingido parcialmente**. O início do semântico previsto para esta sprint **não ocorreu**: a issue #22 (P3) permanece com Eduardo e Mariana atribuídos, mas sem atividade visível (nenhuma branch ou PR até o momento). Formulário P1 ainda não enviado (a confirmar prazo com o replanejamento).

**Entregue:** `parser.y` completo com construção de AST (PR #16). Integração implementada além do previsto para esta sprint: `main.c` real e binário `interpretador` funcionando (issue #18 / PR #39, validado manualmente). Testes de tokens e estruturas de controle criados (`tokens_basico.c`, `organizador_compras.c`). README atualizado com o comando de build/execução real. Pendente: início da tabela de símbolos (P3) — atribuída, mas sem progresso visível; lacuna identificada de testes para função com parâmetros/retorno/recursão (ver issue #25).

## Sprint 3 — Semântica completa e início da interpretação

**Planejado:**
- [ ] Fechar verificação semântica pendente; decidir estrutura do interpretador (`interpretNode()`)
- [ ] Pair programming P3+P4: escopo de blocos/funções, avaliação de expressões e comandos sobre a AST
- [ ] Funções e I/O: chamadas de função (inclusive recursão), retorno, `printf`/`scanf` básicos
- [ ] Checkpoint: rodar programas de teste ponta a ponta; decidir se cabe algum diferencial na sprint 4

**Entregue:** _(preencher ao final da sprint)_

## Sprint 4 — Robustez, testes, documentação

**Planejado:**
- [ ] Revisar o que falta, priorizar robustez do núcleo > diferencial opcional
- [ ] Ajustes finais, revisão cruzada de módulos
- [ ] Suíte de testes cobrindo o núcleo inteiro (casos válidos e de erro)
- [ ] Fechar `docs/documentacao/decisoes-tecnicas.md` e `docs/documentacao/problemas-e-solucoes.md`; atualizar README com exemplos
- [ ] Ensaio da entrevista

**Marco P2:** Interpretador completo (núcleo), testado, documentado; formulário P2 enviado pelo líder.

**Entregue:** _(preencher ao final da sprint)_

## Sprint 5 — Otimizações, recursos extras e testes integrados

**Planejado:**
- [ ] Otimizações opcionais na interpretação (ex.: simplificação de expressões constantes), só se o núcleo já estiver estável
- [ ] Recursos adicionais na linguagem, caso haja tempo (respeitando a ordem de prioridade de diferenciais definida em `definicao-da-linguagem.md`)
- [ ] Testes de integração cobrindo programas mais completos, não só casos isolados
- [ ] Preparar a versão candidata à entrega final (repositório atualizado, README completo)

**Entregue:** _(preencher ao final da sprint)_

## Sprint 6 — Entrevistas finais e encerramento

**Planejado:**
- [ ] Participar das entrevistas finais com o professor (equipe completa)
- [ ] Corrigir pendências/bugs apontados durante as entrevistas ou testes finais
- [ ] Finalizar documentação (README, `docs/`, exemplos de uso)
- [ ] Conferir datas de entrevista no plano de ensino — falta de comparecimento pode zerar a nota da apresentação final

**Entregue:** _(preencher ao final da sprint)_
