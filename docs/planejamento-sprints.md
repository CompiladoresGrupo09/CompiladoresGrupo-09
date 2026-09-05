# Planejamento de sprints

Planejado × entregue, por sprint. Atualizar ao final de cada sprint (exigência de repositório).

## Sprint 1 — Ambiente, linguagem e léxico

**Planejado:**
- [x] Confirmar escopo por escrito em `docs/definicao-da-linguagem.md`
- [ ] Desenhar gramática em alto nível, definir tokens
- [x] Criar repositório com estrutura `docs/` + `src/`
- [x] Configurar ambiente em todas as máquinas
- [ ] Designar o líder para os formulários
- [ ] Iniciar `scanner.l` em par (P1 + P5); primeiros testes de tokens
- [ ] Fechar `scanner.l` completo (tokens, reservadas, literais, erro léxico básico), testado com `flex`/`gcc -lfl`

**Entregue:** _(preencher ao final da sprint)_

## Sprint 2 — Sintático, AST e início do semântico

**Planejado:**
- [ ] Revisar gramática, discutir estrutura dos nós da AST
- [ ] Pair programming P1+P2: conectar scanner ao `parser.y`, gramática de expressões com precedência, primeiras ações semânticas construindo AST
- [ ] Completar `parser.y` (comandos, blocos) construindo AST
- [ ] Iniciar tabela de símbolos e verificação de declaração/uso de variáveis (P3, par com P4)
- [ ] Testes de declarações, expressões, `if`/`while`; atualizar `docs/` e README

**Marco P1:** Front-end (léxico + sintático/AST + início do semântico) compilando, versionado, com `docs/` atualizado e formulário P1 enviado pelo líder.

**Entregue:** _(preencher ao final da sprint)_

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
- [ ] Fechar `docs/decisoes-tecnicas.md` e `docs/problemas-e-solucoes.md`; atualizar README com exemplos
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
