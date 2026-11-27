# Decisões técnicas

Aqui registramos as decisões que tomamos e o porquê de cada uma, como pede a Proposta de Trabalho.

## Metodologia: Scrum com sprints semanais
Seguimos a recomendação da disciplina e organizamos o cronograma em sprints (Planning na segunda, Review na quarta), no mesmo ritmo das aulas.

## AST construída diretamente no parser
Diferente de um compilador, que gera um código intermediário separado, o nosso interpretador monta a AST direto nas ações semânticas do `parser.y` (Bison). É essa AST que alimenta a análise semântica e a interpretação.

## Uso de `&` restrito ao `scanf`
**Decisão:** o operador `&` só é aceito como argumento de `scanf`; usos em argumentos de outras funções são erro semântico.
**Justificativa:** a gramática representa `& ID` apenas na regra `argumento: '&' ID`, para permitir leitura por referência no `scanf`. Como ponteiros estão fora do escopo da linguagem, essa sintaxe não deve ser interpretada como passagem geral de endereço para funções.

## Interpretação por percurso recursivo da AST
Em vez de gerar código final, executamos o programa percorrendo a AST recursivamente (função `interpretarNo()`, criada na issue #31). É isso que diferencia o nosso módulo de execução de um compilador tradicional.

## Aplicação de estudo de caso: organizador de compras no mercado
Escolhemos um organizador de compras (soma os preços lidos com `scanf` num laço e avisa se o total passou do orçamento) como programa de exemplo para exercitar o núcleo. Ele cobre variáveis, laço, condicional e entrada e saída usando só recursos do núcleo obrigatório (seção 3 de [Definição da Linguagem](/documentacao/definicao-da-linguagem.md)), sem depender de nenhum diferencial que está fora de escopo (arrays, strings dinâmicas).

## Estrutura de repositório: docs/ + src/ + tests/ na raiz
Seguimos o padrão dos exemplos do professor (pastas `docs/` e `src/` por unidade), mas tratando o projeto inteiro como a "unidade", com `tests/` separado para os casos válidos e inválidos.

## Papéis fixos com pares cruzados
O pipeline é sequencial (léxico → sintático/AST → semântico → interpretação), mas trabalhamos em pares cruzados (P1+P2, depois P3+P4, com o P5 circulando) para ninguém ficar isolado num módulo só. Isso é importante porque, na entrevista final, qualquer um de nós pode ser perguntado sobre qualquer parte do projeto.

## Recuperação de erros sintáticos (S2-02)

* **Decisão:** o parser não para no primeiro erro sintático. As regras `declaracao` e `comando` têm uma produção `error ';'` com `yyerrok`: ao encontrar um erro, o Bison descarta tokens até o próximo `;` e continua a análise a partir dali. Cada erro é contado em `erros_sintaticos` e, no fim, a AST não segue para o semântico se houve qualquer erro.
* **Justificativa:** reportar todos os erros de uma vez é mais útil para quem escreve o programa do que corrigir um erro por execução. O `;` foi escolhido como ponto de sincronização porque termina declarações e comandos no núcleo da linguagem, então é o lugar mais provável para o parser voltar a um estado conhecido.
* **Limitação conhecida:** um erro dentro de uma estrutura sem `;` próximo (por exemplo, um parêntese não fechado no `if`) pode gerar erros em cascata nas linhas seguintes, como em `tests/invalidos/parentese_nao_fechado.c`.
* **Implementação:** commit `daed43e` (PR #40), testado por `tests/invalidos/multiplos_erros_sintaticos.c`, que reporta 3 erros em linhas diferentes.

## Tratamento do dangling else (S2-03)
O conflito clássico de `if (a) if (b) x; else y;` é saber a qual `if` o `else` pertence. Resolvemos pela preferência natural do Bison, que desloca (`shift`) o `else` para o `if` mais próximo sem `else`. É o comportamento padrão do C e não exigiu regra extra na gramática; o Bison emite 1 aviso de conflito shift/reduce, que é esperado e está documentado em `gramatica.md`. Não precisamos fazer nada além de reconhecer e documentar esse comportamento.

* **Alternativa descartada:** desambiguar com `%prec`/`%nonassoc` para silenciar o aviso. Produziria a mesma semântica com mais regras na gramática, sem ganho funcional.
* **Evidência:** `bison -Wcounterexamples -d parser.y` mostra o único conflito da gramática, no token `ELSE`, com as derivações de *shift* e de *reduce* (saída anexada na issue #20). O teste `tests/validos/if_else_aninhado.c` (PR #56) comprova na AST que o `else` fica dentro do `if` interno.

## Tabela de símbolos com escopos aninhados (S2-05)

* **Decisão:** a tabela de símbolos é uma **pilha de escopos**, e não a lista plana do exemplo do professor (`semana 06/src/tabela.h`). O semântico abre um escopo ao entrar em uma função ou bloco `{ }` e o fecha ao sair.
* **Regras adotadas:**
  * redeclarar um nome **no mesmo escopo** é erro semântico (`buscar_no_escopo_atual()`);
  * declarar o mesmo nome **num escopo interno** é permitido e esconde o externo (*shadowing*), como em C;
  * o uso de um nome procura do escopo atual para fora (`buscar_simbolo()`), e não encontrar em nenhum é erro de identificador não declarado;
  * parâmetros pertencem ao escopo da função; `printf` e `scanf` são pré-declaradas no escopo global.
* **Justificativa:** o núcleo obrigatório inclui blocos e funções com parâmetros. Com lista plana, uma variável declarada dentro de um `if` continuaria visível depois do bloco, e `int a; { int a; }`, que é C válido, seria rejeitado como redeclaração. O problema está registrado em `problemas-e-solucoes.md`.
* **Implementação:** issue #22, PR #46; testes `tests/validos/escopo_shadowing.c`, `tests/invalidos/redeclaracao_mesmo_escopo.c` e `tests/invalidos/variavel_nao_declarada.c` (PR #50).

## Análise semântica inicial: declarações e escopo (S2-06)

* **Objetivo:** iniciar a verificação semântica recursiva sobre a AST gerada pelo `parser.y` (apoiando-se na pilha de escopos da tabela de símbolos de S2-05), cobrindo o marco do **P1** com detecção de variáveis não declaradas, redeclaração no mesmo escopo e controle de escopo léxico.
* **Regras adotadas:**
  * percurso recursivo sobre a AST controlando entrada e saída de escopo (`entrar_escopo()` e `sair_escopo()`) em `NODE_BLOCK` e `NODE_FUNC_DECL`, inserindo os parâmetros da função no escopo do corpo;
  * ao visitar `NODE_ID`, verifica a existência do identificador na tabela de símbolos e reporta erro semântico caso não esteja declarado;
  * ao declarar um símbolo, verifica duplicidade no escopo atual (`buscar_no_escopo_atual()`) para acusar redeclaração, permitindo *shadowing* em escopos internos e mantendo visibilidade restrita de parâmetros e variáveis de bloco;
  * contabiliza ocorrências em `erros_semanticos` e retorna código de saída diferente de `0` quando houver falha, com mensagens no padrão `Erro semantico [linha N]: ...`.
* **Justificativa:** atende aos requisitos do guia oficial, do núcleo obrigatório (declaração/atribuição e blocos/escopo) e ao item *"o que foi implementado"* do **P1**, garantindo que estruturas válidas (como `int a; a = 10;`, *shadowing* em `int a; { int a; }` e toda a suíte em `tests/validos/`) continuem passando sem falsos positivos.
* **Implementação:** módulo `src/semantic.h` e `src/semantic.c` com `analisar_semantica(ASTNode *raiz)` na branch `feat/semantic-declaracoes` (commits `feat(semantic): ...` e revisão via PR), acompanhado dos casos de teste em `tests/invalidos/erro_nao_declarada.c`, `tests/invalidos/erro_redeclaracao.c` e verificação explícita de *shadowing*.

## Verificação de compatibilidade de tipos e conversão implícita (S2-07)

* **Decisão:** o interpretador recusa conversões implícitas com perda de informação, como atribuir uma expressão do tipo `float` a uma variável do tipo `int` (por exemplo: `int a; a = 1.5 + 2;`).
* **Comportamento adotado:** erro semântico na linha da atribuição.
* **Justificativa:** quisemos uma tipagem estrita, para os dados não serem truncados sem aviso. A promoção no sentido inverso (`int` para `float`, em operações binárias mistas ou na atribuição a variáveis `float`) continua válida.
* **Limitação conhecida:** a linguagem não oferece conversão de `float` para `int` (não há suporte a cast na gramática). Essa limitação é intencional no escopo atual do projeto.
* **Implementação:** PR #53 (issue #24), mesclado na `Dev`. Na verificação manual, a atribuição e a inicialização geravam mensagens diferentes para o mesmo erro; no PR #57 (issue #55) as duas foram unificadas em `atribuicao incompativel: conversao implicita de float para int nao permitida`, com os testes `tests/invalidos/erro_tipo_atribuicao.c` e `erro_tipo_inicializacao.c`.
* **Status:** implementada e testada; segue marcada como provisória até a confirmação do grupo (ver "Pontos a decidir").

## Atribuição formal dos papéis técnicos P1 a P4

**Papéis que confirmamos em grupo em 01/10/2026 (S2-11, issue #28),** com base no que cada integrante de fato entregou:

| Papel | Responsável |
|---|---|
| P1 (Léxico) e P2 (Sintático/AST) | Brenda Beatriz |
| P3 (Semântico) | Giovana Ferreira (tabela de símbolos) e Mariana Martins (tipos e compatibilidade) |
| P4 (Interpretação) | Eduardo Ribeiro Xavier |
| Integração (`main.c`) | Giovana Ferreira; pipeline integrado por Brenda Beatriz |
| P5 (Integração, testes e documentação); líder dos formulários | Brenda Beatriz |
| Scrum Master; suplente do líder | Henrique Fontenelle |

**Justificativa:** a divisão original previa uma pessoa por papel, com pares cruzados (P1+P2, P3+P4). Na prática, P1 e P2 foram entregues pela mesma pessoa e P3 foi dividido entre duas, por tipo de verificação. Preferimos registrar a divisão real, e não a planejada, para ficar claro o que cada um defende na entrevista.

Histórico do andamento de cada papel até a formalização:

- `scanner.l` (**P1, Léxico**) e `parser.y` com construção de AST (**P2, Sintático/AST**) já foram implementados e mesclados, ambos pelo mesmo autor no repositório (Brenda Beatriz).
- **Integração (`main.c`), concluída:** implementada, revisada e mesclada (issue #18 / PR #39), com validação manual completa.
- **P3 (Semântico / tabela de símbolos), em andamento avançado:**
  - Tabela de símbolos (issue #22) implementada e mesclada na `Dev` pelo PR #46, com testes de escopo, shadowing, redeclaração e variável não declarada (`tests/validos/escopo_shadowing.c`, `tests/invalidos/redeclaracao_mesmo_escopo.c`, `tests/invalidos/variavel_nao_declarada.c`).
  
  - Verificação de declaração e uso (S2-06, issue #23): implementada pelo PR #46 e validada pelos testes `variavel_nao_declarada.c`, `redeclaracao_mesmo_escopo.c` e `escopo_shadowing.c` (make test em 24/09/2026); issue encerrada.

  - Campo de tipo na AST e verificação de compatibilidade de tipos (S2-07, issue #24): concluída e mesclada na `Dev` pelo PR #53, trabalho em par (AST + semântico).
- **P4 (Interpretação), iniciada:** issue #31 (esqueleto do interpretador, `interpretarNo`); avaliação de expressões mesclada na `Dev` pelo PR #52, de @EduardoRibeiroXavier.
- **Integração ponta a ponta (S3-01, issue #32):** `main.c` encadeando as fases, com as flags `--tokens`/`--ast`/`--tabela` e o relatório de erros, mesclado pelo PR #62 (ver seção própria abaixo).
- **S2-05 (tabela de símbolos com escopos aninhados):** política registrada na seção própria acima.
- **S2-07 (política de conversão implícita de tipos):** decisão registrada na seção própria acima, aguardando confirmação do grupo.

**Pontos a decidir na próxima reunião com o time:**
1. ~~Formalizar (ou não) a atribuição de P1 e P2 à pessoa que já entregou os dois módulos.~~ Resolvido em 01/10/2026: Brenda Beatriz (tabela acima).
2. Confirmar a política de escopos da S2-05 e a de conversão implícita da S2-07, retirando o "Provisório".
3. ~~Formalizar o responsável por P4, considerando que P3 e P4 formam par.~~ Resolvido em 01/10/2026: Eduardo Ribeiro Xavier (tabela acima).
4. Combinar o fluxo de branches: hoje há PRs de documentação indo para a `main` e de código indo para a `Dev`, e as duas estão divergindo. *Atualização (01/10/2026):* desde o PR #56, todos os PRs (código e documentação) vão para a `Dev`; falta o grupo confirmar esse fluxo e combinar quando a `Dev` é mesclada na `main` (ver "Regras de branch").

## Testes com comparação de saída esperada (S2-09)

* **Decisão:** a suíte é executada por `src/run_tests.sh`, chamado pelo `make test`. Além do código de saída (0 para `tests/validos/`, diferente de 0 para `tests/invalidos/`), o script compara a saída com o arquivo `<caso>.esperado` quando ele existe.
* **Justificativa:** conferir só o código de saída deixava passar um programa que termina com sucesso mas imprime o resultado errado. Com um erro proposital no parser (`*` gerando `+`), o `make test` antigo passava e o novo acusa a falha.
* **Detalhes:** `stdout` e `stderr` são capturados separadamente e juntados sempre na mesma ordem, e o `\r` é removido antes da comparação, para o resultado não depender do sistema operacional; o `.gitattributes` força quebra de linha LF em `.sh` e `.esperado`. `bash run_tests.sh --gerar <caso.c>` cria ou atualiza um `.esperado` quando a mudança de saída é intencional.
* **Implementação:** PR #58.

## Líder dos formulários e suplente (S2-12)

* **Líder:** **Brenda Beatriz** é a responsável por enviar os formulários de apresentação. Decisão tomada na issue #6, fechada em 07/09/2026, e registrada por escrito em 01/10/2026 (issue #30).
* **Suplente:** **Henrique Fontenelle**. Se a líder não puder enviar, o suplente assume, **avisando o professor antes do envio**.
* **Justificativa:** a Proposta de Trabalho determina que somente o líder da equipe envia o formulário, e o não envio no prazo zera a nota da apresentação para a equipe inteira. Com uma única pessoa responsável, qualquer imprevisto vira um ponto único de falha; o suplente é o plano B.
* **Formulário P1:** `https://forms.office.com/r/MyKh4HiAAu`, prazo 23/09/2026 às 23h59. **Enviado pela líder em 22/09/2026**, dentro do prazo.

## Acesso do professor ao repositório (RESOLVIDO)
Não houve uma decisão nova do professor: desde o início do semestre a política dele é não participar dos repositórios das equipes. Ainda tínhamos uma dúvida sobre isso, e perguntamos direto a ele em **01/09/2026**. Ele confirmou que não tem interesse em fazer parte do repositório da equipe neste momento, então não precisamos adicioná-lo como colaborador.

## Regras de branch
`main` sempre compilável. Cada pessoa/par trabalha em branch de feature (`feat/scanner`, `feat/parser`, `feat/interpreter`...) e faz merge após revisão. `main` é protegida no GitHub, exigindo Pull Request com pelo menos 1 aprovação.

Como o fluxo funciona na prática (atualizado em 01/10/2026):
* **Fluxo:** branch de trabalho → PR para `Dev` (integração) → `Dev` mesclada na `main` nos marcos de entrega, como o P1 (S3-06).
* **Nomes:** prefixo pelo tipo da mudança, como nos commits: `feat/`, `fix/`, `test/`, `ci/`. Branches de documentação usam **hífen** (`docs-...`), porque já existe uma branch chamada `docs` e o Git não aceita criar `docs/algo` ao lado dela.
* **Verificação automática:** o workflow `build.yml` (S2-10) roda `make` e `make test` em todo PR. O motivo é transformar a regra "`main` sempre compilável" em verificação automática, em vez de depender só da revisão manual.
* **Fechamento de issues:** o `Closes #N` só fecha a issue quando o PR é mesclado na branch padrão (`main`). Como os PRs vão para a `Dev`, a issue é fechada manualmente depois do merge, com um comentário citando o PR.

## Pipeline com parada na primeira fase que falhar (S3-01)

* **Decisão:** o `main.c` executa as fases na ordem léxico → sintático → semântico → execução e **para na primeira que acusar erro**. No fim, sempre imprime (no `stderr`) um relatório com o total de erros por fase, marcando como "nao executada" as fases que não chegaram a rodar.
* **Justificativa:** não faz sentido analisar semanticamente uma AST que veio de um parse com erro, nem executar um programa com erro de tipo; os erros das fases seguintes seriam consequência dos primeiros e confundiriam a leitura. O relatório vai para o `stderr` para não se misturar com a saída do programa interpretado.
* **Flags de inspeção:** `--tokens` (só o léxico), `--ast` (análises e AST, sem executar) e `--tabela` (análises e tabela de símbolos, sem executar). Existem para a apresentação: permitem mostrar cada fase isoladamente enquanto se explica o pipeline. Sem flag, a AST não é impressa, para a execução mostrar só a saída do programa.
* **Códigos de saída:** 0 sucesso, 1 uso incorreto, 2 sintático, 3 léxico, 4 semântico, 5 execução. Como o léxico e o sintático rodam juntos (o parser pede os tokens ao scanner), quando há erro léxico o código é 3 mesmo que também haja erro sintático, porque o léxico é a primeira fase.
* **Tabela de símbolos para o `--tabela`:** como o semântico libera cada escopo ao sair dele, a tabela guarda um histórico dos símbolos declarados só para a impressão.
* **Implementação:** PR #62; roteiro de uso em `demonstracao.md`.
## Testes com saida esperada e entrada redirecionada para todos os casos validos (S4-01)

* **Decisão:** a suíte passou a conferir também o que o programa imprime, e não só o código de saída.
* **Contexto:** na revisão da suíte (`run_tests.sh`), percebemos que a maioria dos casos válidos só tinha o código de saída conferido. Com isso, atribuições, condicionais (`if`/`else`) e laços podiam ser ignorados pelo interpretador sem nenhum teste falhar.
* **O que mudamos:**
  * **Saída esperada:** escrevemos à mão o `.esperado` de todos os casos válidos, com a saída que um programa C de verdade daria.
  * **Entrada redirecionada:** um arquivo `.entrada` faz o papel do que o usuário digitaria no `scanf`. Antes, a entrada era sempre `/dev/null`.
  * **Casos pendentes:** o arquivo `tests/pendentes.txt` marcava os casos que dependiam de partes ainda não implementadas, para a CI continuar mostrando com clareza o que passava e o que faltava.
  * **Windows e Linux:** o script remove o `\r` do fim das linhas (`tr -d '\r'`) antes de comparar, porque desenvolvemos no Windows e a CI roda no Linux.
* **Consequência:** a suíte virou uma rede de segurança de verdade para o interpretador, e mostrou com clareza o que ainda faltava nas Sprints 4 e 5.

## Correção de robustez e escapes no printf (S4-02)

* **Decisão:** corrigimos três defeitos da função `executar_printf`, em `src/interpreter.c`:
  1. **Argumentos faltando:** antes de ler um argumento, conferimos se ele existe (`indice_arg < num_children`). Assim o `printf` não lê fora da AST e mostra o erro `Erro de execucao [linha N]: printf com menos argumentos do que especificadores`, com código de saída 5.
  2. **`%` no fim do texto:** ajustamos o avanço no texto do formato para aceitar um `%` sozinho no final (como em `printf("100%");`), sem passar do fim da string (`\0`).
  3. **Sequências de escape:** `\n`, `\t`, `\\`, `\"` e `\0` passaram a ser trocadas pelos caracteres de verdade na hora da impressão.
* **Justificativa:** os dois primeiros defeitos liam memória inválida e podiam derrubar o programa; o terceiro fazia todo programa com `\n` imprimir errado.
* **Testes:** `tests/invalidos/printf_argumentos_insuficientes.c` confere o erro de execução, e `tests/validos/printf_escape.c` e `printf_porcentagem_final.c` conferem a saída certa.

## Ambiente de execução do interpretador (S4-03)

* **Decisão:** o interpretador tem um **ambiente de execução próprio**, separado da `TabelaSimbolos`: uma pilha de escopos (`EscopoExec`) em `interpreter.c`, em que cada variável guarda nome e valor (o `Valor` já carrega o tipo). Substitui a lista linear `Variavel *variaveis` usada até a Sprint 3.
* **Regras adotadas:**
  * `interpretar()` abre o escopo global; cada `NODE_BLOCK` abre um escopo ao entrar e o fecha ao sair, liberando as variáveis dele;
  * `definir_variavel()` cria sempre no escopo atual; `ler_variavel()` e `atribuir_variavel()` procuram do escopo mais interno para fora (*shadowing*, como no semântico);
  * o `=` é executado como expressão: guarda o valor e devolve o valor guardado, o que permite `a = b = 3`;
  * o valor é sempre convertido para o tipo declarado da variável, na declaração e na atribuição (`float x = 5;` guarda `5.0`).
* **Justificativa:** a tabela de símbolos guarda informação de análise (tipo, linha, aridade) e é liberada ao final da semântica (`analisar_semantica`). O ambiente guarda informação de execução (valores), que muda a cada comando e precisa de escopos novos toda vez que um bloco é executado, por exemplo a cada volta de um laço e, na S5-01, a cada chamada de função. Por isso a previsão anterior de migrar os valores para a `TabelaSimbolos` foi revista: separar as duas estruturas mantém uma responsabilidade por fase, e a lógica de pilha segue o mesmo padrão da `tabela.c`.
* **Implementação:** issue #72; verificado com os critérios de aceitação da issue e com `make test`.

## Código de saída vindo do `return` do `main` (S4-08)

* **Decisão:** quando nenhuma fase tem erro, o processo sai com o valor do `return` do `main` interpretado, como um programa C compilado. A flag `--no-run` roda todas as análises e não executa.
* **Regras adotadas:**
  * o interpretador guarda o valor do `return` (convertido para `int`) e `interpretar()` o devolve ao `main.c`;
  * `return;` sem valor, ou um `main` que termina sem `return`, resulta em 0;
  * havendo erro em qualquer fase, vale o código da fase (2 sintático, 3 léxico, 4 semântico, 5 execução), e não o `return`;
  * `--no-run` usa o mesmo desvio do `--ast` e do `--tabela`: qualquer modo diferente do completo encerra antes da execução.
* **Limitação conhecida:** se o `main` retornar 1, 2, 3, 4 ou 5, o código de saída é igual ao de uma categoria de erro. Para diferenciar, basta olhar o relatório de erros no fim da saída, que mostra 0 erro em todas as fases. É a mesma limitação de qualquer interpretador que repassa o código de saída do programa.
* **Implementação:** issue #77.

## Chamada de função, parâmetros e retorno (S5-01)

* **Decisão:** cada chamada abre um **frame**, um escopo novo no ambiente de execução, e guarda em `base_frame` o índice onde ele começa. `ler_variavel()` procura do escopo mais interno até `base_frame` e, depois, só no escopo global (índice 0). Assim, a função enxerga os próprios escopos e as variáveis globais, mas não as variáveis locais de quem a chamou.
* **Regras adotadas:**
  * as funções são localizadas pelo nome na raiz da AST, guardada em `programa` no início de `interpretar()`;
  * os argumentos são avaliados no escopo de quem chama e só depois o frame novo é aberto; cada parâmetro recebe o valor convertido para o tipo declarado (passagem por valor);
  * o valor do `return` é convertido para o tipo de retorno da função; uma função `void` devolve um valor do tipo `TIPO_VOID`;
  * as declarações globais são executadas no escopo 0 antes de o `main` ser chamado; o `main` é executado pelo mesmo mecanismo de qualquer função.
* **Propagação do `return` por sinalizador:** a execução usa dois sinalizadores, `retornou` (um `return` foi executado) e `abortou` (houve erro que interrompe o programa). Os dois fazem blocos e laços pararem; ao fim de uma chamada, `retornou` volta a 0, porque o `return` encerra só aquela função, mas `abortou` continua valendo até o fim do programa. Até a Sprint 4 havia um único sinalizador para as duas situações, o que faria um erro dentro de uma função ser "consumido" pela chamada e o programa continuar.
* **Justificativa:** a alternativa seria `interpretarNo()` devolver um valor de controle junto com o resultado, o que mudaria a assinatura de todas as funções de execução. O sinalizador aproveita a estrutura que a S4-04 já tinha criado para o `return` do `main`. A recursão funciona sem código extra, porque cada chamada tem o próprio frame; o limite de profundidade fica para a S5-02.
* **Testes:** os casos de `tests/validos/` que dependiam de funções saíram de `pendentes.txt`. Três `.esperado` foram corrigidos: `escopo_shadowing` e `comando_for` não batiam com o `printf` do próprio programa, e `funcao_parametros_retorno` usava `%d` com uma variável `float`, comportamento indefinido em C, e passou a usar `%f`.
* **Implementação:** issue #80.

## Formato padrão das mensagens de erro

* **Decisão:** toda mensagem de erro segue o formato `Erro <fase> [linha N]: <descricao>` (por exemplo, `Erro semantico [linha 4]: ...`) e é escrita **sem acentos**.
* **Justificativa:** o formato único deixa claro em que fase e em que linha o problema está, e permite que os testes comparem as mensagens nos arquivos `.esperado`. Sem acentos, a saída aparece igual em qualquer terminal, independentemente da codificação configurada.
* **Implementação:** mensagens em `scanner.l`, `parser.y`, `semantic.c`, `tabela.c` e `interpreter.c`; a última mensagem com acento foi corrigida no PR #57.

## `printf`: formatos, conversões e erros (S5-03)

* **Decisão:** o `printf` aceita `%d`, `%f`, `%c`, `%s` e `%%`. Qualquer outro especificador (por exemplo `%x`) é impresso literalmente.
* **Regras adotadas:**
  * `%f` usa 6 casas decimais, igual ao `printf` do C (`3.5` imprime `3.500000`);
  * as escapes `\n`, `\t`, `\\`, `\"` e `\0` são decodificadas na hora da impressão, tanto no formato quanto no texto passado para `%s`;
  * entre números, o valor é convertido para o especificador: `%d` com `float` trunca (`2.9` imprime `2`), `%f` com `int` converte (`2` imprime `2.000000`) e `%c` imprime o caractere daquele código;
  * `%s` só aceita texto entre aspas, porque a linguagem não tem variável do tipo texto;
  * argumentos sobrando são ignorados, como no C.
* **Erros de execução (código 5, com o relatório):** argumento faltando para um especificador, `%s` com número e `%d`/`%f`/`%c` com texto. Antes, o argumento faltando encerrava o programa com `exit(5)` sem mostrar o relatório; agora segue o mesmo caminho dos outros erros de execução.
* **Implementação:** issue #89.

## Diferenciais: só os testes mais extensos (S7-01)

* **Decisão:** dos três diferenciais da [Definição da Linguagem](/documentacao/definicao-da-linguagem.md), fizemos só o **diferencial 2, testes automatizados mais extensos**. Decidimos em grupo, em 27/11/2026, não fazer a coluna nas mensagens de erro (diferencial 1, issue #99), os vetores 1D (diferencial 3) nem o relatório de erros unificado (issue #97).
* **O que foi feito no diferencial 2:** a suíte do núcleo, com saída esperada em todos os casos e a tabela de cobertura em `tests/README.md` (PR #126), os casos-limite (PR #129) e a lista completa de erros de execução (PR #130). São 88 testes, todos comparando a saída.
* **Por que os outros ficaram de fora:** o núcleo está completo e testado, e as entrevistas são em 30/11 e 02/12. A coluna nas mensagens e o relatório unificado mudariam todas as mensagens de erro e os 88 arquivos `.esperado` às vésperas da entrega, com risco de quebrar o que já funciona. Os vetores mexeriam no parser, na tabela de símbolos, no semântico e no interpretador ao mesmo tempo. Preferimos entregar um núcleo sólido a um diferencial pela metade.
* **Limitações que ficam:** as mensagens de erro mostram a linha, mas não a coluna; vetores continuam recusados com a mensagem "construcao fora do escopo"; cada fase continua escrevendo as próprias mensagens, todas no mesmo formato `Erro <fase> [linha N]: ...`.
* **Implementação:** issue #98.