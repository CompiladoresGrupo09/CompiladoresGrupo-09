# Decisões técnicas

Registro de decisões e o porquê de cada uma — exigência do repositório conforme a Proposta de Trabalho.

## Metodologia: Scrum com sprints semanais
Recomendação oficial da disciplina. Cronograma organizado em Sprints (Planning na segunda, Review na quarta), encaixado no ritmo já usado pelo curso.

## AST construída diretamente no parser
Diferente de um compilador (que gera código intermediário separado), o interpretador constrói a AST diretamente nas ações semânticas do `parser.y` (Bison). É essa AST que alimenta a análise semântica e a interpretação.

## Interpretação por percurso recursivo da AST
Em vez de gerar código final, a execução acontece percorrendo a AST recursivamente (função `interpretarNo()`, definida na issue #31). É o que diferencia o módulo de execução de um compilador tradicional.

## Aplicação de estudo de caso: organizador de compras no mercado
Escolhido um organizador de compras (soma preços lidos via `scanf` em loop e avisa se o total passou do orçamento) como programa de exemplo para exercitar o núcleo. Motivo: cobre variáveis, laço, condicional e E/S usando só recursos do núcleo obrigatório (seção 3 de `definicao-da-linguagem.md`), sem depender de nenhum diferencial (arrays, strings dinâmicas) que está fora de escopo.

## Estrutura de repositório: docs/ + src/ + tests/ na raiz
Alinhada ao padrão usado nos exemplos do professor (pastas `docs/`/`src/` por unidade), adaptada para o projeto inteiro ser a "unidade", com `tests/` separado para casos válidos/inválidos.

## Papéis fixos com pares cruzados
Pipeline sequencial (léxico → sintático/AST → semântico → interpretação), mas com pareamento cruzado (P1+P2, depois P3+P4, P5 circulando) para evitar silos de conhecimento — necessário porque a entrevista final é individual e cobra justificativa de qualquer parte do projeto.

## Tratamento do dangling else (S2-03)
O conflito clássico de `if (a) if (b) x; else y;` — a quem o `else` pertence — é resolvido pela precedência natural do Bison, que prefere deslocar (`shift`) o `else` para o `if` mais próximo sem `else`. É o comportamento padrão do C e não exigiu regra extra na gramática; o Bison emite 1 aviso de conflito shift/reduce esperado para isso, documentado em `gramatica.md`. Nenhuma ação adicional foi necessária além de reconhecer e documentar esse comportamento padrão.

* **Alternativa descartada:** desambiguar com `%prec`/`%nonassoc` para silenciar o aviso. Produziria a mesma semântica com mais regras na gramática, sem ganho funcional.
* **Evidência:** `bison -Wcounterexamples -d parser.y` mostra o único conflito da gramática, no token `ELSE`, com as derivações de *shift* e de *reduce* (saída anexada na issue #20). O teste `tests/validos/if_else_aninhado.c` (PR #56) comprova na AST que o `else` fica dentro do `if` interno.

## Recuperação de erros sintáticos (S2-02)

* **Decisão:** o parser não para no primeiro erro sintático. As regras `declaracao` e `comando` têm uma produção `error ';'` com `yyerrok`: ao encontrar um erro, o Bison descarta tokens até o próximo `;` e continua a análise a partir dali. Cada erro é contado em `erros_sintaticos` e, no fim, a AST não segue para o semântico se houve qualquer erro.
* **Justificativa:** reportar todos os erros de uma vez é mais útil para quem escreve o programa do que corrigir um erro por execução. O `;` foi escolhido como ponto de sincronização porque termina declarações e comandos no núcleo da linguagem, então é o lugar mais provável para o parser voltar a um estado conhecido.
* **Limitação conhecida:** um erro dentro de uma estrutura sem `;` próximo (por exemplo, um parêntese não fechado no `if`) pode gerar erros em cascata nas linhas seguintes, como em `tests/invalidos/parentese_nao_fechado.c`.
* **Implementação:** commit `daed43e` (PR #40), testado por `tests/invalidos/multiplos_erros_sintaticos.c`, que reporta 3 erros em linhas diferentes.

## Tabela de símbolos com escopos aninhados (S2-05)

* **Decisão:** a tabela de símbolos é uma **pilha de escopos**, e não a lista plana do exemplo do professor (`semana 06/src/tabela.h`). O semântico abre um escopo ao entrar em uma função ou bloco `{ }` e o fecha ao sair.
* **Regras adotadas:**
  * redeclarar um nome **no mesmo escopo** é erro semântico (`buscar_no_escopo_atual()`);
  * declarar o mesmo nome **num escopo interno** é permitido e esconde o externo (*shadowing*), como em C;
  * o uso de um nome procura do escopo atual para fora (`buscar_simbolo()`), e não encontrar em nenhum é erro de identificador não declarado;
  * parâmetros pertencem ao escopo da função; `printf` e `scanf` são pré-declaradas no escopo global.
* **Justificativa:** o núcleo obrigatório inclui blocos e funções com parâmetros. Com lista plana, uma variável declarada dentro de um `if` continuaria visível depois do bloco, e `int a; { int a; }`, que é C válido, seria rejeitado como redeclaração. O problema está registrado em `problemas-e-solucoes.md`.
* **Implementação:** issue #22, PR #46; testes `tests/validos/escopo_shadowing.c`, `tests/invalidos/redeclaracao_mesmo_escopo.c` e `tests/invalidos/variavel_nao_declarada.c` (PR #50).

## S2-07: Verificação de compatibilidade de tipos e conversão implícita (Provisório)

* **Decisão:** o interpretador recusa conversões implícitas com perda de informação, como atribuir uma expressão do tipo `float` a uma variável do tipo `int` (por exemplo: `int a; a = 1.5 + 2;`).
* **Comportamento adotado:** emissão de erro semântico na linha da atribuição.
* **Justificativa:** promover tipagem estrita e evitar truncamento silencioso de dados. A promoção no sentido inverso (`int` para `float`, em operações binárias mistas ou na atribuição a variáveis `float`) continua válida.
* **Limitação conhecida:** a linguagem não oferece conversão de `float` para `int` (não há suporte a cast na gramática). Essa limitação é intencional no escopo atual do projeto.
* **Implementação:** PR #53 (issue #24), mesclado na `Dev`. Na verificação manual, a atribuição e a inicialização geravam mensagens diferentes para o mesmo erro; no PR #57 (issue #55) as duas foram unificadas em `atribuicao incompativel: conversao implicita de float para int nao permitida`, com os testes `tests/invalidos/erro_tipo_atribuicao.c` e `erro_tipo_inicializacao.c`.
* **Status:** implementada e testada; segue marcada como provisória até a confirmação do grupo (ver "Pontos a decidir").

## Atribuição formal dos papéis técnicos P1–P4

- `scanner.l` (**P1 — Léxico**) e `parser.y` com construção de AST (**P2 — Sintático/AST**) já foram implementados e mesclados, ambos pelo mesmo autor no repositório. A formalização dessa pessoa como dona de P1+P2 será confirmada na próxima reunião.
- **Integração (`main.c`) — concluída:** implementada, revisada e mesclada (issue #18 / PR #39), com validação manual completa.
- **P3 (Semântico / tabela de símbolos) — em andamento avançado:**
  - Tabela de símbolos (issue #22) implementada e mesclada na `Dev` pelo PR #46, com testes de escopo, shadowing, redeclaração e variável não declarada (`tests/validos/escopo_shadowing.c`, `tests/invalidos/redeclaracao_mesmo_escopo.c`, `tests/invalidos/variavel_nao_declarada.c`).
  
  - Verificação de declaração e uso (S2-06, issue #23): implementada pelo PR #46 e validada pelos testes `variavel_nao_declarada.c`, `redeclaracao_mesmo_escopo.c` e `escopo_shadowing.c` (make test em 24/09/2026); issue encerrada.

  - Campo de tipo na AST e verificação de compatibilidade de tipos (S2-07, issue #24): concluída e mesclada na `Dev` pelo PR #53, trabalho em par (AST + semântico).
- **P4 (Interpretação) — iniciada:** issue #31 (esqueleto do interpretador, `interpretarNo`); avaliação de expressões mesclada na `Dev` pelo PR #52, de @EduardoRibeiroXavier. A alocação formal de P4 será confirmada na próxima reunião.
- **Integração ponta a ponta (S3-01, issue #32):** `main.c` encadeando as fases, com as flags `--tokens`/`--ast`/`--tabela` e o relatório de erros, mesclado pelo PR #62 (ver seção própria abaixo).
- **S2-05 (tabela de símbolos com escopos aninhados):** política registrada na seção própria acima.
- **S2-07 (política de conversão implícita de tipos):** decisão registrada na seção própria acima, aguardando confirmação do grupo.

**Pontos a decidir na próxima reunião com o time:**
1. Formalizar (ou não) a atribuição de P1 e P2 à pessoa que já entregou os dois módulos.
2. Confirmar a política de escopos da S2-05 e a de conversão implícita da S2-07, retirando o "Provisório".
3. Formalizar o responsável por P4, considerando que P3 e P4 formam par.
4. Combinar o fluxo de branches: hoje há PRs de documentação indo para a `main` e de código indo para a `Dev`, e as duas estão divergindo. *Atualização (01/10/2026):* desde o PR #56, todos os PRs (código e documentação) vão para a `Dev`; falta o grupo confirmar esse fluxo e combinar quando a `Dev` é mesclada na `main` (ver "Regras de branch").

## Líder dos formulários e suplente (S2-12)

* **Líder:** **Brenda Beatriz** é a responsável por enviar os formulários de apresentação. Decisão tomada na issue #6, fechada em 07/09/2026, e registrada por escrito em 01/10/2026 (issue #30).
* **Suplente:** **Henrique Fontenelle**. Se a líder não puder enviar, o suplente assume, **avisando o professor antes do envio**.
* **Justificativa:** a Proposta de Trabalho determina que somente o líder da equipe envia o formulário, e o não envio no prazo zera a nota da apresentação para a equipe inteira. Com uma única pessoa responsável, qualquer imprevisto vira um ponto único de falha; o suplente é o plano B.
* **Formulário P1:** `https://forms.office.com/r/MyKh4HiAAu`, prazo 23/09/2026 às 23h59 — **enviado pela líder em 22/09/2026**, dentro do prazo.

## Acesso do professor ao repositório (RESOLVIDO)
Não houve uma decisão nova do professor: a política de não participar dos repositórios das equipes já era dele desde o início do semestre. Havia apenas uma dúvida remanescente no grupo sobre isso, esclarecida diretamente com ele em **01/09/2026** — ele confirmou que não tem interesse em fazer parte do repositório da equipe neste momento. Não é necessário adicioná-lo como colaborador.

## Regras de branch
`main` sempre compilável. Cada pessoa/par trabalha em branch de feature (`feat/scanner`, `feat/parser`, `feat/interpreter`...) e faz merge após revisão. `main` é protegida no GitHub, exigindo Pull Request com pelo menos 1 aprovação.

Como o fluxo funciona na prática (atualizado em 01/10/2026):
* **Fluxo:** branch de trabalho → PR para `Dev` (integração) → `Dev` mesclada na `main` nos marcos de entrega, como o P1 (S3-06).
* **Nomes:** prefixo pelo tipo da mudança, como nos commits: `feat/`, `fix/`, `test/`, `ci/`. Branches de documentação usam **hífen** (`docs-...`), porque já existe uma branch chamada `docs` e o Git não aceita criar `docs/algo` ao lado dela.
* **Verificação automática:** o workflow `build.yml` (S2-10) roda `make` e `make test` em todo PR. O motivo é transformar a regra "`main` sempre compilável" em verificação automática, em vez de depender só da revisão manual.
* **Fechamento de issues:** o `Closes #N` só fecha a issue quando o PR é mesclado na branch padrão (`main`). Como os PRs vão para a `Dev`, a issue é fechada manualmente depois do merge, com um comentário citando o PR.

## Armazenamento de Variáveis no Interpretador

**Decisão:**
o interpretador de expressões usa uma lista própria e linear (`Variavel *variaveis` em `interpreter.c`), separada da `TabelaSimbolos` com escopos, que será decidido futuramente como iremos prosseguir.

**Justificativa**
o escopo é só avaliação de expressões e literais sobre um único `main()`, sem blocos, funções ou recursão. Reaproveitar `tabela.c` teria exigido decidir o mapeamento símbolo→valor sem os comandos (`if`/`while`/chamadas) que ainda não existem.

**Limitação conhecida:**
essa lista não tem noção de escopo. A Sprint 4 (blocos, funções) vai exigir migrar o armazenamento de valores para cima da `TabelaSimbolos` existente, para não duplicar a lógica de entrar/sair de escopo.

## Pipeline com parada na primeira fase que falhar (S3-01)

* **Decisão:** o `main.c` executa as fases na ordem léxico → sintático → semântico → execução e **para na primeira que acusar erro**. No fim, sempre imprime (no `stderr`) um relatório com o total de erros por fase, marcando como "nao executada" as fases que não chegaram a rodar.
* **Justificativa:** não faz sentido analisar semanticamente uma AST que veio de um parse com erro, nem executar um programa com erro de tipo; os erros das fases seguintes seriam consequência dos primeiros e confundiriam a leitura. O relatório vai para o `stderr` para não se misturar com a saída do programa interpretado.
* **Flags de inspeção:** `--tokens` (só o léxico), `--ast` (análises e AST, sem executar) e `--tabela` (análises e tabela de símbolos, sem executar). Existem para a apresentação: permitem mostrar cada fase isoladamente enquanto se explica o pipeline. Sem flag, a AST não é impressa, para a execução mostrar só a saída do programa.
* **Códigos de saída:** 0 sucesso, 1 uso incorreto, 2 sintático, 3 léxico, 4 semântico, 5 execução. Como o léxico e o sintático rodam juntos (o parser pede os tokens ao scanner), quando há erro léxico o código é 3 mesmo que também haja erro sintático, porque o léxico é a primeira fase.
* **Tabela de símbolos para o `--tabela`:** como o semântico libera cada escopo ao sair dele, a tabela guarda um histórico dos símbolos declarados só para a impressão.
* **Implementação:** PR #62; roteiro de uso em `demonstracao.md`.

## Testes com comparação de saída esperada (S2-09)

* **Decisão:** a suíte é executada por `src/run_tests.sh`, chamado pelo `make test`. Além do código de saída (0 para `tests/validos/`, diferente de 0 para `tests/invalidos/`), o script compara a saída com o arquivo `<caso>.esperado` quando ele existe.
* **Justificativa:** conferir só o código de saída deixava passar um programa que termina com sucesso mas imprime o resultado errado. Com um erro proposital no parser (`*` gerando `+`), o `make test` antigo passava e o novo acusa a falha.
* **Detalhes:** `stdout` e `stderr` são capturados separadamente e juntados sempre na mesma ordem, e o `\r` é removido antes da comparação, para o resultado não depender do sistema operacional; o `.gitattributes` força quebra de linha LF em `.sh` e `.esperado`. `bash run_tests.sh --gerar <caso.c>` cria ou atualiza um `.esperado` quando a mudança de saída é intencional.
* **Implementação:** PR #58.

## Formato padrão das mensagens de erro

* **Decisão:** toda mensagem de erro segue o formato `Erro <fase> [linha N]: <descricao>` — por exemplo, `Erro semantico [linha 4]: ...` — e é escrita **sem acentos**.
* **Justificativa:** o formato único deixa claro em que fase e em que linha o problema está, e permite que os testes comparem as mensagens nos arquivos `.esperado`. Sem acentos, a saída aparece igual em qualquer terminal, independentemente da codificação configurada.
* **Implementação:** mensagens em `scanner.l`, `parser.y`, `semantic.c`, `tabela.c` e `interpreter.c`; a última mensagem com acento foi corrigida no PR #57.
