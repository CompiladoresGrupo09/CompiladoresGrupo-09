# Planejamento de Sprints

O projeto foi dividio em Sprints para garantir o acompanhamento da disciplina, além do Planejado × entregue, por sprint. 

## Sprint 1 — Ambiente, linguagem e léxico

**Planejado:**
- [x] Confirmar escopo por escrito em `docs/documentacao/definicao-da-linguagem.md`
- [ ] Desenhar gramática em alto nível, definir tokens
- [x] Criar repositório com estrutura `docs/` + `src/`
- [x] Configurar ambiente em todas as máquinas
- [x] Designar o líder para os formulários
- [ ] Iniciar `scanner.l` em par (P1 + P5); primeiros testes de tokens
- [ ] Fechar `scanner.l` completo (tokens, reservadas, literais, erro léxico básico), testado com `flex`/`gcc -lfl`

**Entregue:**

* **Escopo e Especificação da Linguagem:**
  - [x] Documento inicial de especificação e regras léxicas consolidado em `docs/documentacao/definicao-da-linguagem.md`.
  - [x] Levantamento e definição dos tokens da linguagem (palavras reservadas, operadores, delimitadores, identificadores e literais).

* **Infraestrutura e Repositório:**
  - [x] Inicialização do repositório Git com a estrutura modular de pastas (`docs/`, `src/`, `tests/`).
  - [x] Configuração padronizada dos ambientes de desenvolvimento e compilação (`gcc`, `flex`, `make`) via WSL/Linux.
  - [x] Definição de papéis da equipe e alinhamento dos fluxos de trabalho.

* **Implementação Léxica (`scanner.l`):**
  - [x] Criação do analisador léxico com Flex (`src/scanner.l`), cobrindo reconhecimento de palavras-chave, literais (`int`, `float`, `char`, `string`), operadores aritméticos/lógicos e tratamento de comentários.
  - [x] Tratamento de erros léxicos básicos (caracteres inválidos e tokens não reconhecidos com contagem de linha).
  - [x] Validação e compilação do scanner através de testes manuais e scripts de compilação com `flex` e `gcc -lfl`.


#### Tabela 1 - Issues Fechadas na Sprint 01:

| Issue | Descrição | Responsáveis / Par |
| :--- | :--- | :--- |
| [#01](link-da-issue) | Configuração do ambiente e estrutura inicial do repositório |  |
| [#02](link-da-issue) | Definição da gramática em alto nível e tokens da linguagem |  |
| [#03](link-da-issue) | Implementação e testes do analisador léxico (`scanner.l`) com Flex | |


## Sprint 2 — Sintático, AST e início do semântico

**Planejado:**
- [ ] Revisar gramática, discutir estrutura dos nós da AST
- [ ] Pair programming P1+P2: conectar scanner ao `parser.y`, gramática de expressões com precedência, primeiras ações semânticas construindo AST
- [ ] Completar `parser.y` (comandos, blocos) construindo AST
- [ ] Iniciar tabela de símbolos e verificação de declaração/uso de variáveis (P3, par com P4)
- [ ] Testes de declarações, expressões, `if`/`while`; atualizar `docs/` e README

**Entregue:** _(preencher ao final da sprint)_

#### Tabela 2 - Issues Fechadas na Sprint 02:

| Issue | Descrição | Responsáveis / Par |
| :--- | :--- | :--- |
| [#01](link-da-issue) | |  |
| [#02](link-da-issue) | |  |
| [#03](link-da-issue) | |  |


## Sprint 3 — Semântica completa e início da interpretação

**Planejado:**
- [ ] Fechar verificação semântica pendente; decidir estrutura do interpretador (`interpretNode()`)
- [ ] Pair programming P3+P4: escopo de blocos/funções, avaliação de expressões e comandos sobre a AST
- [ ] Funções e I/O: chamadas de função (inclusive recursão), retorno, `printf`/`scanf` básicos
- [ ] Checkpoint: rodar programas de teste ponta a ponta; decidir se cabe algum diferencial na sprint 4

**Entregue:** _(preencher ao final da sprint)_

#### Tabela 3 - Issues Fechadas na Sprint 03:

| Issue | Descrição | Responsáveis / Par |
| :--- | :--- | :--- |
| [#01](link-da-issue) | |  |
| [#02](link-da-issue) | |  |
| [#03](link-da-issue) | |  |

## Sprint 4 — Robustez, testes, documentação

**Planejado:**
- [ ] Revisar o que falta, priorizar robustez do núcleo > diferencial opcional
- [ ] Ajustes finais, revisão cruzada de módulos
- [ ] Suíte de testes cobrindo o núcleo inteiro (casos válidos e de erro)
- [ ] Fechar `docs/documentacao/decisoes-tecnicas.md` e `docs/documentacao/problemas-e-solucoes.md`; atualizar README com exemplos
- [ ] Ensaio da entrevista

**Marco P2:** Interpretador completo (núcleo), testado, documentado; formulário P2 enviado pelo líder.

**Entregue:** _(preencher ao final da sprint)_

#### Tabela 4 - Issues Fechadas na Sprint 04:

| Issue | Descrição | Responsáveis / Par |
| :--- | :--- | :--- |
| [#01](link-da-issue) | |  |
| [#02](link-da-issue) | |  |
| [#03](link-da-issue) | |  |

## Sprint 5 — Otimizações, recursos extras e testes integrados

**Planejado:**
- [ ] Otimizações opcionais na interpretação (ex.: simplificação de expressões constantes), só se o núcleo já estiver estável
- [ ] Recursos adicionais na linguagem, caso haja tempo (respeitando a ordem de prioridade de diferenciais definida em `definicao-da-linguagem.md`)
- [ ] Testes de integração cobrindo programas mais completos, não só casos isolados
- [ ] Preparar a versão candidata à entrega final (repositório atualizado, README completo)

**Entregue:** _(preencher ao final da sprint)_

#### Tabela 5 - Issues Fechadas na Sprint 05:

| Issue | Descrição | Responsáveis / Par |
| :--- | :--- | :--- |
| [#01](link-da-issue) | |  |
| [#02](link-da-issue) | |  |
| [#03](link-da-issue) | |  |

## Sprint 6 — Entrevistas finais e encerramento

**Planejado:**
- [ ] Participar das entrevistas finais com o professor (equipe completa)
- [ ] Corrigir pendências/bugs apontados durante as entrevistas ou testes finais
- [ ] Finalizar documentação (README, `docs/`, exemplos de uso)
- [ ] Conferir datas de entrevista no plano de ensino — falta de comparecimento pode zerar a nota da apresentação final

**Entregue:** _(preencher ao final da sprint)_

#### Tabela 6 - Issues Fechadas na Sprint 06:

| Issue | Descrição | Responsáveis / Par |
| :--- | :--- | :--- |
| [#01](link-da-issue) | |  |
| [#02](link-da-issue) | |  |
| [#03](link-da-issue) | |  |

## Histórico de Versão 

| Data | Versão | Descrição | Autor |
|------|--------|-----------|-------|
| 05/09/2026 | 1.0 | Criação da página | Brenda |
| 25/09/2026 | 1.1 | Inserção das tabelas de Issues e descrição das entregas | Mariana |