# Atas das reuniões

Aqui ficam as atas das reuniões de planejamento de cada sprint e das reuniões em que o grupo tomou decisões importantes. Todas seguem o mesmo modelo: data, local, participantes, objetivo, o que foi planejado, as tarefas definidas, os riscos, a meta da sprint (*Sprint Goal*) e, no fim, o resultado do que foi entregue.

O detalhe de cada entrega, com todas as issues e PRs, está em [Planejamento das Sprints](/documentacao/planejamento-sprints.md). As decisões citadas aqui estão explicadas em [Decisões Técnicas](/documentacao/decisoes-tecnicas.md).

| Ata | Data da reunião | Período da sprint |
|---|---|---|
| Sprint 0: Planning | 19/08/2026 | 19/08 a 31/08 |
| Sprint 1: Planning | 02/09/2026 | 02/09 a 09/09 |
| Sprint 2: Planning | 10/09/2026 | 10/09 a 16/09 |
| Sprint 2: Reunião de Alinhamento 01 | 12/09/2026 | |
| Sprint 3: Planning | 17/09/2026 | 17/09 a 01/10 (P1 em 05/10) |
| Sprint 4: Planning e replanejamento | 26/10/2026 | 06/10 a 08/11 |
| Sprint 5: Planning | 09/11/2026 | 09/11 a 18/11 |
| Sprint 6: Planning | 19/11/2026 | 19/11 a 27/11 |
| Sprint 7 e 8: Planning e decisão dos diferenciais | 27/11/2026 | 27/11 a 02/12 |

## Sprint 0: Planning

**Projeto:** Interpretador de C (subconjunto)<br>
**Data da reunião:** 19/08/2026<br>
**Local:** presencial (sala de aula)<br>
**Participantes:** equipe completa (Brenda, Eduardo, Giovana, Henrique e Mariana)<br>
**Período da sprint:** 19/08/2026 a 31/08/2026

**Objetivo da reunião:** entender a proposta da disciplina e o que seria cobrado, alinhar a disponibilidade de todos para as dailys do semestre, decidir entre fazer um compilador ou um interpretador e preparar a criação do repositório no GitHub.

**Planejado:**
- Reunir o grupo completo em sala de aula.
- Discutir a dinâmica da disciplina de Compiladores, usando o cronograma da disciplina como referência.
- Decidir entre compilador e interpretador, e em que linguagem desenvolver o projeto.
- Criar e configurar o repositório.
- Levar as dúvidas ao professor, em sala ou de forma remota.

**Discussão e entendimento:**
- Revisamos cada fase do projeto no repositório modelo do professor.
- Alinhamos como o grupo ia trabalhar nas fases seguintes.

**Tarefas técnicas definidas:**
- Duração das sprints e trabalho em pares (*pair programming*).
- Desenvolver um **interpretador**, escrito em **C** com Flex e Bison.
- Fazer um primeiro recorte do escopo da linguagem, para entender o tamanho do trabalho.
- Montar as sprints a partir das datas do cronograma da disciplina.

**Capacidade da equipe:** avaliamos a disponibilidade de cada um e a familiaridade de cada integrante com C, Flex e Bison.

**Riscos e impedimentos:**
- Parte do grupo precisava estudar mais a linguagem e as ferramentas antes de começar.
- Várias regras do interpretador ainda dependiam de decisões do grupo.

**Acordos:** escolher um tema para os programas de exemplo, para o projeto fazer mais sentido para o grupo.

**Sprint Goal:** *"Entender a dinâmica da disciplina e os critérios de avaliação, alinhar a disponibilidade da equipe e decidir o formato do projeto (compilador ou interpretador)."*

**Resultado:** decidimos fazer um interpretador de um subconjunto de C, com Flex e Bison. O tema escolhido foi o organizador de compras no mercado, e o repositório recebeu a primeira estrutura em 02/09 (PR #2).

## Sprint 1: Planning

**Projeto:** Interpretador de C (subconjunto)<br>
**Data da reunião:** 02/09/2026<br>
**Local:** online (Teams)<br>
**Participantes:** equipe completa (Brenda, Eduardo, Giovana, Henrique e Mariana)<br>
**Período da sprint:** 02/09/2026 a 09/09/2026

**Objetivo da reunião:** colocar o ambiente para funcionar em todas as máquinas, fechar o escopo da linguagem por escrito e fazer o analisador léxico.

**Planejado:**
- Criar o repositório com a estrutura `docs/` e `src/`.
- Configurar o ambiente (Flex, Bison e GCC) em todas as máquinas.
- Escrever o escopo da linguagem em `definicao-da-linguagem.md`.
- Desenhar a gramática em alto nível e definir os tokens.
- Escolher a líder responsável pelos formulários.

**Discussão e entendimento:**
- Revisamos de novo as fases no repositório modelo do professor e mantivemos o planejamento inicial.
- Combinamos o padrão de commits e a publicação da documentação no GitHub Pages.

**Tarefas técnicas definidas:**
- Começar o `scanner.l` em par e escrever os primeiros testes de tokens (issue #8).
- Fechar o `scanner.l` com tokens, palavras reservadas, literais e erro léxico (issue #9).
- Desenhar a gramática em alto nível (issue #7).
- Padronizar os commits (issue #4) e criar o GitHub Pages (issue #5).

**Capacidade da equipe:** avaliamos a disponibilidade de cada um e a familiaridade com o GitHub, já que nem todos tinham trabalhado com branches e pull requests.

**Riscos e impedimentos:** parte do grupo ainda estava aprendendo Flex, Bison e o fluxo de branches do GitHub.

**Acordos:** Brenda ficou como líder dos formulários (issue #6).

**Sprint Goal:** *"Ter o ambiente funcionando em todas as máquinas, o escopo da linguagem escrito e o analisador léxico pronto e testado."*

**Resultado:** `scanner.l` completo e testado (PR #15, em 06/09), gramática em alto nível e `parser.y` inicial (PR #16), escopo da linguagem documentado, padrão de commits definido e site publicado no GitHub Pages.

## Sprint 2: Planning

**Projeto:** Interpretador de C (subconjunto)<br>
**Data da reunião:** 10/09/2026<br>
**Local:** online (Teams)<br>
**Participantes:** Brenda, Eduardo, Giovana e Mariana<br>
**Período da sprint:** 10/09/2026 a 16/09/2026

**Objetivo da reunião:** planejar o analisador sintático, a construção da AST e o começo do semântico.

**Planejado:**
- Revisar a gramática e discutir a estrutura dos nós da AST.
- Ligar o scanner ao `parser.y`, com a precedência das expressões.
- Completar o `parser.y` com comandos e blocos, construindo a AST.
- Começar a tabela de símbolos e a verificação de declaração e uso de variáveis.
- Escrever testes de declarações, expressões, `if` e `while`.

**Tarefas técnicas definidas:**
- `main.c` gerando o binário `interpretador` (S2-01, issue #18).
- Recuperação de erros sintáticos com o token `error` (S2-02, issue #19).
- Linha correta nas expressões (S2-04, issue #21).
- Tabela de símbolos com escopos aninhados (S2-05, issue #22).
- Variável não declarada e redeclaração no mesmo escopo (S2-06, issue #23).
- Tipo nos nós da AST e verificação de compatibilidade (S2-07, issue #24).

**Riscos e impedimentos:** a tabela de símbolos precisava ficar pronta antes do conteúdo de semântica nas aulas, e o semântico dependia dela.

**Sprint Goal:** *"Ter o sintático e a AST prontos e começar o semântico com a tabela de símbolos."*

**Resultado:** `main.c` e binário `interpretador` (PR #39), recuperação de erros sintáticos (PR #40), linha correta nas expressões (PR #42), tabela de símbolos com verificação de declaração e uso (PR #46) e testes de escopo e redeclaração (PR #50). O tipo nos nós da AST (issue #24) passou para a Sprint 3.

## Sprint 2: Reunião de Alinhamento 01

**Data da reunião:** 12/09/2026<br>
**Local:** online (Teams), com gravação<br>
**Participantes:** Brenda, Eduardo, Giovana e Henrique<br>
**Registro:** issue #45

**Objetivo da reunião:** ver em que ponto a Sprint 2 estava e combinar os próximos passos até o P1.

**Situação no dia:**

| Situação | Item |
|---|---|
| Concluído | Léxico, sintático e AST, `main.c` com `make` e recuperação de erros sintáticos |
| Em andamento | Linha correta nas expressões e tabela de símbolos (issue #22) |
| Pendente | Declaração e redeclaração no semântico, tipo em cada nó da AST, testes, documentação e formulário do P1 |

**Decisões e encaminhamentos:**
- A tabela de símbolos (issue #22) deveria ficar pronta até 16/09, quando o professor começaria esse conteúdo. Se houvesse atraso, todos ajudariam.
- Depois dela, seguiríamos para as issues #23 (variável não declarada) e #24 (tipo na AST e compatibilidade).
- As issues de testes, documentação e versão ficaram com Brenda e Henrique.
- Todo bug ou dificuldade passaria a ser registrado nos comentários da issue, para usarmos em [Problemas e Soluções](/documentacao/problemas-e-solucoes.md) e na apresentação final.

**Datas combinadas:** formulário do P1 até 23/09 (com meta interna em 22/09) e entrevista final em 30/11 ou 02/12. A apresentação do P1 depois foi marcada para 05/10.

**Resultado:** a tabela de símbolos entrou em 16/09 (PR #46), dentro do prazo combinado.

## Sprint 3: Planning

**Projeto:** Interpretador de C (subconjunto)<br>
**Data da reunião:** 17/09/2026<br>
**Local:** online (Teams)<br>
**Participantes:** Brenda, Eduardo, Giovana e Mariana<br>
**Período da sprint:** 17/09/2026 a 01/10/2026, com o formulário do P1 em 23/09 e a apresentação em 05/10

**Objetivo da reunião:** fechar o que o P1 cobra: o semântico de tipos, o começo da interpretação, os testes automatizados, o pipeline integrado e a documentação.

**Planejado:**
- Tipo nos nós da AST e verificação de compatibilidade (S2-07, issue #24).
- Avaliar expressões sobre a AST, como primeiro passo do interpretador (issue #31).
- Ampliar a suíte de testes léxicos e sintáticos (S2-08, issue #25).
- `run_tests.sh` comparando a saída esperada, chamado pelo `make test` (S2-09, issue #26).
- CI que compila e roda os testes em cada PR (S2-10, issue #27).
- Pipeline integrado de ponta a ponta, com as flags de inspeção e o relatório de erros (S3-01, issue #32).
- Registrar problemas e decisões técnicas (S3-03 e S3-04, issues #34 e #35).
- Enviar o formulário do P1 (S3-05, issue #17).

**Riscos e impedimentos:** a Semana Universitária caiu no meio do período e reduziu o tempo útil. Por isso dividimos a Sprint 2 original em duas e renumeramos as seguintes (issue #33).

**Acordos:**
- Brenda como líder dos formulários e Henrique como suplente, registrado por escrito (S2-12, issue #30).
- Papéis técnicos P1 a P4 confirmados pelo grupo em 01/10 (issue #28).

**Sprint Goal:** *"Chegar ao P1 com o front-end compilando, testado e documentado, e o interpretador já avaliando expressões."*

**Resultado:** marco P1 atingido. Tipos na AST e recusa de `float` para `int` (PRs #53 e #57), interpretador avaliando expressões (PR #52), suíte de testes, `run_tests.sh` e CI (PRs #56, #58 e #59), pipeline com flags e relatório de erros (PR #62) e documentação de problemas e decisões (PRs #63 e #64). O formulário do P1 foi enviado em 22/09, dentro do prazo.

## Sprint 4: Planning e replanejamento

**Projeto:** Interpretador de C (subconjunto)<br>
**Data da reunião:** 26/10/2026 (revisão de código e replanejamento)<br>
**Local:** online (Teams)<br>
**Participantes:** Brenda, Eduardo, Giovana e Mariana<br>
**Período da sprint:** 06/10/2026 a 08/11/2026

**Objetivo da reunião:** revisar o código da `main` depois do P1 e refazer o backlog até a entrega final.

**Discussão e entendimento:**
- Depois do P1, ficamos de 01/10 até a revisão sem mesclar nenhum PR.
- Na revisão, descobrimos que a suíte passava sem o interpretador executar a maior parte dos programas: atribuição, `if`, `while`, `for` e `return` eram pulados sem aviso, e os testes válidos só conferiam o código de saída.

**Tarefas técnicas definidas:**
- Testes com saída esperada e entrada redirecionada (S4-01, issue #70).
- `printf` com argumentos, `%` no fim e escapes (S4-02, issue #71).
- Ambiente de execução com escopos, atribuição e conversão (S4-03, issue #72).
- `if`/`else`, `while`, `for`, `return` e curto-circuito (S4-04, issue #73).
- Semântica das chamadas: existência, aridade e tipos (S4-05, issue #74).
- Programa sem `main`, `void`, `&` e `return` (S4-06, issue #75).
- Construções fora do escopo recusadas com mensagem clara (S4-07, issue #76).
- `--no-run` e código de saída vindo do `return` do `main` (S4-08, issue #77).
- Exigir aprovação antes do merge (S4-09, issue #78).

**Riscos e impedimentos:** chamadas de função, `scanf` e o resto do `printf` não cabiam nesta sprint e foram para a Sprint 5.

**Acordos:** o backlog das Sprints 4 a 8 foi montado nesta reunião, com as issues nos milestones do GitHub. Cada PR passaria a ser testado por outro integrante antes da aprovação.

**Sprint Goal:** *"Fazer o interpretador executar comandos de verdade e testar a saída de cada programa, não só o código de saída."*

**Resultado:** ambiente de execução (PR #81), testes com saída esperada (PRs #82 e #84) e `if`, `while`, `for` e `return` executando (PR #83), mesclados em 08/11. O restante da sprint entrou em 17/11 e 18/11: `printf` com escapes (PR #87), recusas do semântico (PR #85), construções fora do escopo (PR #86), `--no-run` (PR #107) e semântica das chamadas (PR #112).

## Sprint 5: Planning

**Projeto:** Interpretador de C (subconjunto)<br>
**Data da reunião:** 09/11/2026<br>
**Local:** online (Teams)<br>
**Participantes:** Brenda, Eduardo, Giovana e Mariana<br>
**Período da sprint:** 09/11/2026 a 18/11/2026

**Objetivo da reunião:** completar o núcleo da linguagem e chegar ao marco P2.

**Tarefas técnicas definidas:**
- Chamada de função com parâmetros, retorno e recursão (S5-01, issue #80).
- Limite de profundidade da recursão (S5-02, issue #88).
- `printf` com `%s` e erros de tipo (S5-03, issue #89).
- `scanf` com `%d`, `%f` e `%c` (S5-04, issue #90).
- Organizador de compras rodando de ponta a ponta (S5-05, issue #91).
- Tabela de cobertura dos testes (S5-06, issue #92).

**Riscos e impedimentos:** o organizador de compras dependia ao mesmo tempo do `scanf` e das chamadas de função.

**Sprint Goal:** *"Ter o núcleo completo, com funções, entrada e saída, e o organizador de compras rodando de ponta a ponta."*

**Resultado:** marco P2 atingido em 18/11, com chamadas de função (PR #115), `printf` com `%s` (PR #114), `scanf` (PR #111) e o organizador de compras rodando de ponta a ponta (PR #113). O limite de recursão entrou em 26/11 (PR #120), e a tabela de cobertura em 27/11 (PR #126).

## Sprint 6: Planning

**Projeto:** Interpretador de C (subconjunto)<br>
**Data da reunião:** 19/11/2026<br>
**Local:** online (Teams)<br>
**Participantes:** Brenda, Eduardo, Giovana e Mariana<br>
**Período da sprint:** 19/11/2026 a 27/11/2026

**Objetivo da reunião:** validar o núcleo com programas completos e reforçar os testes.

**Tarefas técnicas definidas:**
- Um estudo de caso por integrante: boletim escolar (Mariana), caixa eletrônico (Giovana), calculadora de números (Eduardo) e controle de temperatura (Brenda).
- Lista completa de erros de execução (S6-02, issue #95).
- Revisão cruzada dos módulos (S6-01, issue #94).
- Relatório de erros unificado (S6-08, issue #97).

**Riscos e impedimentos:** pouco tempo até a entrega, por isso otimizações só entrariam com o núcleo estável.

**Acordos:** a revisão cruzada seria feita durante o trabalho: cada integrante testa o PR de outro antes de aprovar e revisa a documentação conforme ela é escrita.

**Sprint Goal:** *"Provar que o interpretador roda programas completos e cobrir os casos-limite com testes."*

**Resultado:** os quatro estudos de caso entraram (PRs #121, #123, #125 e #127). A calculadora mostrou que a divisão por zero não interrompia a execução, e corrigimos isso (issue #124). Também entraram a lista de erros de execução (PR #130), os casos-limite (PR #129) e o relatório de erros unificado (PR #131), chegando a 88 testes.

## Sprint 7 e 8: Planning e decisão dos diferenciais

**Projeto:** Interpretador de C (subconjunto)<br>
**Data da reunião:** 27/11/2026<br>
**Local:** online (Teams)<br>
**Participantes:** Brenda, Eduardo, Giovana e Mariana<br>
**Período das sprints:** 27/11/2026 a 02/12/2026

**Objetivo da reunião:** decidir quais diferenciais entram, organizar a entrega final e preparar as entrevistas.

**Decisões:**
- Entram o diferencial 2 (testes mais extensos) e o relatório de erros unificado (S7-01, issue #98).
- Ficam de fora a coluna nas mensagens de erro (issue #99) e os vetores, para não arriscar o núcleo no dia da entrega.
- A entrega no repositório é em 27/11, e as entrevistas são em 30/11 e 02/12.
- A preparação para a entrevista é individual. Fica um material de apoio para o estudo de cada um (S8-01, issue #104).

**Tarefas técnicas definidas:**
- Casos-limite na suíte (S7-03, issue #100).
- Fechar os documentos de `docs/` (S7-05, issue #102).
- Entrega final e tag `v1.0` (S7-06, issue #103).
- Conferir o checklist final de entrega num clone limpo (S8-02, issue #105).

**Sprint Goal:** *"Entregar o repositório final com o núcleo testado e a documentação fechada, e chegar às entrevistas com cada integrante preparado para explicar o projeto."*

**Resultado:** diferenciais decididos e registrados, casos-limite na suíte (PR #129), relatório unificado na `Dev` (PR #131) e documentação fechada. A tag `v1.0` e o checklist num clone limpo são os últimos passos da entrega.

| Data | Versão | Descrição | Autor |
|------|--------|-----------|-------|
| 27/11/2026 | 1.0 | Criação da página, com as atas das Sprints 0 a 2 revisadas e as das Sprints 3 a 8 | Brenda |
