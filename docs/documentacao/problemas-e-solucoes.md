# Problemas e soluções

Registro contínuo de obstáculos encontrados durante o desenvolvimento e como foram resolvidos. Exigência de repositório — atualizar em tempo real, não deixar para o fim.

Cada registro segue o formato **contexto → sintoma → causa → solução → lição** e cita o arquivo, commit ou PR envolvido, para que possa ser conferido no histórico do repositório.

## Resumo

| Data | Problema | Módulo | Situação |
|---|---|---|---|
| 01/09 | Divergência sobre quando o professor acessa o repositório | Organização | Resolvido |
| 05/09 | Conflito de merge no `Makefile` entre `feat/scanner` e `feat/parser` | Build | Resolvido |
| 05/09 | Nove vazamentos de memória de identificadores e strings na AST | Sintático/AST | Resolvido |
| 05/09 | Erro 404 do docsify na publicação da documentação | Documentação | Resolvido |
| 05/09 | Conflito shift/reduce do *dangling else* | Sintático | Resolvido (documentado) |
| 09/09 | Linha errada em expressões por uso de `yylineno` | Sintático/AST | Resolvido |
| 15/09 | Lista plana do professor incompatível com escopos aninhados | Semântico | Resolvido |
| 15/09 | PR da #22 comparado contra base desatualizada | Integração | Resolvido |
| 15/09 | Push direto no `main` contornando regra de proteção | Integração | Resolvido |
| 15/09 | Git não remesclou o conteúdo revertido | Integração | Resolvido |
| 15/09 | Conflito de conteúdo em `decisoes-tecnicas.md` não detectado pelo Git | Integração | Resolvido |
| 15/09 | Critérios de aceitação da #22 sem cobertura de teste | Testes | Resolvido |
| 15/09 | Desvios do checklist da issue não sinalizados no PR | Processo | Resolvido |
| 24/09 | Mensagens diferentes para o mesmo erro de tipo `float` → `int` | Semântico | Resolvido |
| 27/09 | Armazenamento de variáveis do interpretador sem noção de escopo | Interpretação | Em aberto (planejado) |
| 30/09 | `make test` aprovava programa que imprimia resultado errado | Testes | Resolvido |
| 30/09 | Saída esperada dependente do sistema operacional | Testes | Resolvido |
| 30/09 | `Closes #N` não fechava as issues | Processo | Contornado |
| 01/10 | Tabela de símbolos vazia ao fim da análise (`--tabela`) | Semântico/Integração | Resolvido |

---

## [01/09/2026] Divergência sobre quando o professor acessa o repositório
**Contexto:** definição das regras de organização do repositório no início da Sprint 1, incluindo se o professor deveria ser adicionado como colaborador.
**Sintoma:** parte do grupo entendia que o professor acompanharia o repositório durante o semestre; outra parte, que ele só olharia na entrega final. A pendência ficou registrada em aberto em `decisoes-tecnicas.md`.
**Causa:** a política do professor (não participar dos repositórios das equipes) já existia desde o início do semestre, mas não estava escrita em nenhum documento do grupo, e cada integrante guardou uma versão diferente do que foi dito em aula.
**Solução:** a dúvida foi esclarecida diretamente com o professor em 01/09/2026, que confirmou não ter interesse em entrar no repositório. A seção foi marcada como "RESOLVIDO" em `decisoes-tecnicas.md` (commit `16c0b4e`), com a data da resposta registrada no commit `275c80f`.
**Lição:** regras combinadas oralmente precisam ir para a documentação no mesmo dia, com data e fonte; senão cada pessoa lembra de um jeito.
**Módulo/responsável:** Organização — Henrique.

## [05/09/2026] Conflito de merge no Makefile entre feat/scanner e feat/parser
**Contexto:** integração do scanner ao parser na Sprint 2, trazendo a branch `feat/scanner` para dentro de `feat/parser`.
**Sintoma:** o merge parou com conflito em `src/Makefile` (commit de merge `1ccb89d`, mensagem "Conflicts: src/Makefile").
**Causa:** cada branch tinha escrito o próprio `Makefile` para gerar um binário de teste isolado: `scanner_test` (compilado com `-DSCANNER_TEST_MAIN`) em uma, `parser_test` (com `-DPARSER_TEST_MAIN`) na outra. As duas mexiam nas mesmas linhas (variáveis, alvo `all` e alvo `test`).
**Solução:** no merge foi mantida a versão do parser, que já incluía o scanner como dependência. Depois, o `Makefile` passou a gerar um único binário `interpretador` (commit `6e0da1e`) e o `main` de teste foi retirado do `parser.y` (commit `c844e0e`), eliminando a fonte do conflito.
**Lição:** um `Makefile` com um alvo por módulo vira ponto de conflito entre branches paralelas; o build deve ter um único binário e um único alvo de teste desde cedo.
**Módulo/responsável:** Build — Brenda (merge); Giovana (binário único).

## [05/09/2026] Nove vazamentos de memória de identificadores e strings na AST
**Contexto:** revisão do PR do parser (`feat/parser`) antes do merge.
**Sintoma:** a revisão apontou memória alocada e nunca liberada em nove ações semânticas do `parser.y`.
**Causa:** o scanner devolve identificadores e strings já copiados com `strdup` (`yylval.strval`), e o parser copiava de novo: as funções da AST (`criar_no_id`, `criar_no_string`, `criar_no_addr`) fazem `strdup` internamente, e as regras de declaração de função, parâmetro e chamada faziam `strdup($n)` à mão. Em todos esses casos a cópia original do scanner nunca era liberada.
**Solução:** dois ajustes no `parser.y` (commit `d1be188`). Nas seis regras que passam o texto para uma função da AST (identificador, declarador com e sem inicialização, atribuição, string e `&ID`), foi acrescentado `free($n)` logo depois. Nas três regras que guardavam a cópia à mão (declaração de função, parâmetro e chamada), o `strdup($n)` foi trocado pela atribuição direta do ponteiro, transferindo a posse da memória para o nó.
**Lição:** quando duas camadas copiam o mesmo dado, uma delas precisa ser a dona da memória e a outra liberar a sua cópia; isso deve ficar explícito no código, perto do `strdup`.
**Módulo/responsável:** Sintático/AST — Brenda.

## [05/09/2026] Erro 404 do docsify na publicação da documentação
**Contexto:** publicação da documentação no GitHub Pages com docsify, logo depois de mover os arquivos para `docs/documentacao/`.
**Sintoma:** o site abria, mas o menu lateral e as páginas retornavam 404.
**Causa:** duas causas combinadas. O `index.html` não definia o `basePath`, então o docsify procurava as páginas no caminho errado, e o `_sidebar.md` ainda apontava para os arquivos fora de `documentacao/`. Além disso, o GitHub Pages processa o site com Jekyll, que ignora arquivos começando com `_` — como o `_sidebar.md`.
**Solução:** `basePath: 'docs/'` no `index.html` e links do menu corrigidos para `documentacao/...` (commit `963ff34`); arquivo `.nojekyll` adicionado para desligar o Jekyll (commit `2bc1e67`). Mais tarde, arquivos duplicados de configuração na raiz foram removidos (commit `025344f`).
**Lição:** ao mover arquivos de documentação, conferir o site publicado, e não só o Markdown local; o GitHub Pages tem regras próprias (Jekyll) que não aparecem ao abrir os arquivos no editor.
**Módulo/responsável:** Documentação — Brenda (correção); Mariana (configuração inicial do docsify e limpeza).

## [05/09/2026] Conflito shift/reduce do dangling else
**Contexto:** implementação da gramática no `parser.y` (commit `f3d822d`, issue #7).
**Sintoma:** o Bison emite `1 shift/reduce conflict` a cada compilação.
**Causa:** a gramática de C é ambígua em `if (a) if (b) x; else y;`: o `else` pode pertencer ao `if` interno ou ao externo. A saída de `bison -Wcounterexamples` mostra as duas derivações possíveis ao ler o `ELSE`.
**Solução:** mantido o comportamento padrão do Bison, que escolhe *shift* e liga o `else` ao `if` mais próximo — a mesma semântica do C. A decisão foi registrada em `gramatica.md` e `decisoes-tecnicas.md` (commit `275c80f`) e comprovada pelo teste `tests/validos/if_else_aninhado.c` (PR #56), cuja AST mostra o `else` dentro do `if` interno. Issue #20 (S2-03).
**Lição:** nem todo aviso do Bison é erro; mas todo aviso precisa ter uma explicação escrita, porque vira pergunta de entrevista.
**Módulo/responsável:** Sintático — Brenda (gramática); Henrique (documentação).

## [09/09/2026] Linha errada em expressões por uso de yylineno
**Contexto:** conferência das linhas impressas pela AST e pelas mensagens de erro, issue #21 (S2-04).
**Sintoma:** numa expressão quebrada em várias linhas, os nós apontavam para a linha do último token lido. Em `int x = 1 +` / `2 +` / `3;`, a atribuição aparecia na linha 4 em vez da 2.
**Causa:** as ações semânticas de expressão usavam `yylineno`, que acompanha o token de *lookahead* já lido pelo parser, e não o início da construção reduzida. As construções compostas (`if`, `while`, funções) já usavam `@$.first_line` corretamente.
**Solução:** todas as ações semânticas passaram a usar `@$.first_line` (commit `330d0f6`, PR #42, aprovado por Mariana). `yylineno` ficou só no `yyerror`, onde a linha do token atual é a informação certa. O teste `tests/validos/expressao_multilinha.c` comprova a correção.
**Lição:** `yylineno` diz onde o scanner está, não onde a construção começa; para posição de nós da AST, usar as *locations* do Bison.
**Módulo/responsável:** Sintático/AST — Brenda.

## [15/09/2026] Lista plana do professor incompatível com escopos aninhados
**Contexto:** início da implementação da tabela de símbolos (issue #22), usando `semana 06/src/tabela.h` do professor como referência.
**Sintoma:** o material do professor usa uma lista única e plana de símbolos, adequada ao exemplo dele. O núcleo obrigatório do projeto inclui blocos `{ }` e funções com parâmetros — com lista plana, uma variável declarada dentro de um `if` continuaria visível depois do bloco, e `int a; { int a; }` (código C válido) seria rejeitado como redeclaração.
**Causa:** o exemplo do professor modela um único escopo, suficiente para o programa dele, mas não para o núcleo da nossa linguagem.
**Solução:** adaptação da estrutura para uma pilha de escopos, onde `buscar_no_escopo_atual()` verifica só o nível corrente (base da detecção de redeclaração) e `buscar_simbolo()` percorre do escopo atual para fora (resolução de nomes com shadowing). Decisão registrada em `decisoes-tecnicas.md` com justificativa técnica.
**Lição:** material de referência é ponto de partida; antes de copiar uma estrutura, conferir se ela atende aos requisitos do nosso núcleo.
**Módulo/responsável:** Semântico (P3) — Giovana/Eduardo.

## [15/09/2026] PR da #22 comparado contra base desatualizada
**Contexto:** revisão do PR `feature/22-tabela-simbolos` antes do merge.
**Sintoma:** o branch foi criado a partir de `Dev` (branch de integração da equipe), mas `main` estava atrasado em relação a `Dev` — o diff contra `main` trazia embutidas 76 linhas de uma correção de outra issue (#21, PR #42) que já estava em `Dev` e ainda não tinha chegado em `main`, misturando histórico de duas issues no mesmo PR.
**Causa:** o GitHub mostra o diff contra a base escolhida no PR; com `main` atrasada, tudo o que só existia em `Dev` aparecia como mudança do PR.
**Solução:** identificado que o PR deveria mesmo ser comparado contra `Dev` (fluxo correto da equipe), e resolvida separadamente a defasagem entre `Dev` e `main` via PR próprio, sem misturar com a revisão da #22.
**Lição:** antes de revisar um diff, conferir a base do PR; PRs de código vão para `Dev`.
**Módulo/responsável:** Revisão/integração — Henrique.

## [15/09/2026] Push direto no main contornando regra de proteção
**Contexto:** tentativa de sincronizar `Dev` em `main` pelo terminal, para destravar a comparação correta do PR da #22.
**Sintoma:** o push foi aceito, mas o GitHub reportou explicitamente `Bypassed rule violations: Changes must be made through a pull request`.
**Causa:** a permissão de administrador permite contornar a regra de PR obrigatório em `main`, definida em `decisoes-tecnicas.md`.
**Solução:** commit revertido (`git revert -m 1`) via PR formal, revisado e aprovado por Giovana; sincronização refeita do zero através de um segundo PR devidamente revisado, restaurando o processo combinado pela equipe (PRs #47 e #48).
**Lição:** a proteção de branch não substitui a disciplina do time; administradores também abrem PR.
**Módulo/responsável:** Revisão/integração — Henrique, aprovação de Giovana.

## [15/09/2026] Git não remesclou o conteúdo revertido ("Already up to date")
**Contexto:** segunda tentativa de sincronizar `Dev` em `main`, logo após o revert do merge indevido.
**Sintoma:** `git merge origin/Dev` retornou `Already up to date`, mesmo com o conteúdo revertido.
**Causa:** o Git decide o merge pela ancestralidade dos commits, não pelo conteúdo da árvore; a ponta de `Dev` já constava no histórico (revertida, mas ainda presente).
**Solução:** em vez de mesclar `Dev` novamente, foi feito `git revert` do próprio commit de revert, reaplicando o conteúdo original de forma explícita. Confirmado pelo push subsequente enviando objetos reais (não mais "Total 0").
**Lição:** para desfazer o revert de um merge, reverte-se o revert; mesclar de novo não funciona.
**Módulo/responsável:** Revisão/integração — Henrique.

## [15/09/2026] Conflito de conteúdo em decisoes-tecnicas.md não detectado pelo Git
**Contexto:** atualização do branch da #22 com o `main` sincronizado, antes da revisão final.
**Sintoma:** o branch da #22 tinha uma versão desatualizada da seção sobre acesso do professor ao repositório ("PENDÊNCIA"), enquanto `main` já tinha essa questão resolvida ("RESOLVIDO"). O Git mesclou automaticamente sem acusar conflito.
**Causa:** o merge do Git é textual e linha a linha; como as duas versões não se sobrepunham nas mesmas linhas, não houve conflito, embora o conteúdo fosse contraditório.
**Solução:** conferência manual do arquivo completo pós-merge, confirmando que apenas a versão correta e mais recente ("RESOLVIDO") permaneceu no resultado final.
**Lição:** "merge sem conflito" não significa "documento coerente"; depois de mesclar documentação, reler as seções afetadas.
**Módulo/responsável:** Revisão/integração — Henrique.

## [15/09/2026] Critérios de aceitação centrais da #22 sem cobertura de teste
**Contexto:** validação do PR antes da aprovação final.
**Sintoma:** a suíte de testes existente (`make test`) só cobria casos sem sombreamento de variáveis nem erros semânticos — nenhum teste exercitava shadowing aceito, redeclaração rejeitada, ou busca em escopo externo a partir de bloco interno, que são critérios de aceitação explícitos da issue.
**Causa:** a suíte tinha sido criada antes da tabela de símbolos, e a entrega da #22 não trouxe testes novos para os próprios critérios.
**Solução:** criados três testes dedicados (`escopo_shadowing.c`, `redeclaracao_mesmo_escopo.c`, `variavel_nao_declarada.c`, PR #50) e um programa de teste isolado da API da tabela, rodado sob AddressSanitizer, confirmando ausência de vazamento de memória ao sair de escopo.
**Lição:** cada critério de aceitação vira um caso de teste no mesmo PR que o implementa.
**Módulo/responsável:** Revisão/integração — Henrique.

## [15/09/2026] Desvios do checklist da issue não sinalizados no PR
**Contexto:** revisão final do PR antes da aprovação.
**Sintoma:** dois itens do checklist original não foram cumpridos — commits fora do padrão `feat(semantic): ...` combinado pela equipe, e sem evidência de testes manuais da API antes da integração com `semantic.c`.
**Causa:** a entrega veio com tabela + integração semântica completas, ultrapassando o escopo original da #22, e os itens do checklist ficaram para trás.
**Solução:** registrados como observações não-bloqueantes no comentário de revisão do PR, para correção nos próximos PRs. Entrega aprovada e mesclada por estar funcionalmente correta, com os desvios documentados em vez de ignorados silenciosamente.
**Lição:** quando uma entrega foge do checklist, o desvio deve ser dito no PR, não descoberto na revisão.
**Módulo/responsável:** Semântico (P3) — Giovana/Eduardo; revisão — Henrique.

## [24/09/2026] Mensagens diferentes para o mesmo erro de tipo float → int
**Contexto:** verificação manual da regra da S2-07 depois do merge do PR #53, que introduziu a checagem de compatibilidade de tipos.
**Sintoma:** `int a; a = 1.5 + 2;` e `int a = 1.5 + 2;` geravam o mesmo erro com textos diferentes — `conversao de float para int recusada` e `conversao implicita de float para int não permitida`, esta a única mensagem do projeto com acento. Também não havia testes versionados para os dois casos.
**Causa:** a atribuição e a inicialização são tratadas em pontos diferentes de `semantic.c` (expressão `=` e declaração com inicializador), e cada ponto ganhou a própria mensagem.
**Solução:** as duas mensagens foram unificadas, sem acento, e criados `tests/invalidos/erro_tipo_atribuicao.c` e `erro_tipo_inicializacao.c` (commits `5747f43` e `22de46f`, PR #57, issue #55).
**Lição:** a mesma regra verificada em dois lugares precisa da mesma mensagem; um teste por caminho do código evita que um dos dois mude sozinho.
**Módulo/responsável:** Semântico — Mariana (regra, PR #53); Henrique (verificação); Brenda (padronização e testes).

## [27/09/2026] Armazenamento de variáveis do interpretador sem noção de escopo
**Contexto:** implementação do esqueleto do interpretador avaliando expressões (issue #31, PR #52).
**Sintoma:** o interpretador guarda os valores numa lista linear própria (`Variavel *variaveis` em `interpreter.c`), separada da tabela de símbolos com escopos.
**Causa:** nesta etapa o interpretador avalia apenas expressões dentro de um único `main()`, sem blocos, funções ou recursão; reaproveitar `tabela.c` exigiria decidir o mapeamento símbolo → valor antes de existirem os comandos que criam escopos.
**Solução:** decisão provisória registrada em `decisoes-tecnicas.md` (commit `926bfe9`), com a limitação explícita. A migração para cima da `TabelaSimbolos` está planejada para quando o interpretador passar a executar blocos e funções.
**Lição:** uma simplificação consciente é aceitável se a limitação e o momento de revê-la ficarem escritos.
**Módulo/responsável:** Interpretação (P4) — Eduardo.

## [30/09/2026] make test aprovava programa que imprimia resultado errado
**Contexto:** S2-09 (issue #26), transformar o `make test` num verificador de verdade.
**Sintoma:** com um erro proposital no parser (`*` gerando `+`), `interpretador_precedencia.c` imprimia `6` em vez de `7`, mas o `make test` continuava passando.
**Causa:** o alvo `test` do `Makefile` era um laço que conferia apenas o código de saída do programa. Um resultado errado com código 0 passava despercebido.
**Solução:** criado `src/run_tests.sh`, que compara a saída de cada caso com um arquivo `.esperado` quando ele existe, mostra um resumo e termina com código 1 se algo falhar. O `make test` passou a chamar o script (commits `e247319`, `f03cbf5` e `4b00d3b`, PR #58). Com o mesmo erro proposital, a suíte nova acusa a falha.
**Lição:** um teste só vale se for capaz de falhar; a verificação foi feita quebrando o código de propósito.
**Módulo/responsável:** Testes — Brenda.

## [30/09/2026] Saída esperada dependente do sistema operacional
**Contexto:** geração dos arquivos `.esperado` no Windows, para serem conferidos também no Linux da CI (S2-10).
**Sintoma:** havia risco de os mesmos testes passarem numa máquina e falharem em outra.
**Causa:** duas diferenças entre sistemas. A ordem entre `stdout` e `stderr` capturados juntos (`2>&1`) depende do buffer de cada sistema, e o Windows usa quebra de linha `\r\n` enquanto o Linux usa `\n`.
**Solução:** o `run_tests.sh` captura `stdout` e `stderr` separadamente e os junta sempre na mesma ordem, removendo `\r` antes de comparar; o `.gitattributes` força quebra de linha LF em `.sh` e `.esperado` (PR #58). A CI do PR #59 confirmou a suíte passando no Linux.
**Lição:** saída de teste gerada em um sistema e conferida em outro precisa ser normalizada; testar na CI desde o primeiro PR revela isso cedo.
**Módulo/responsável:** Testes — Brenda.

## [30/09/2026] `Closes #N` não fechava as issues
**Contexto:** merge dos PRs #56 e #57, que tinham `Closes #25` e `Closes #55` na descrição.
**Sintoma:** os PRs foram mesclados, mas as issues continuaram abertas no quadro.
**Causa:** o GitHub só aplica as palavras de fechamento (`Closes`, `Fixes`, `Resolves`) quando o PR é mesclado na branch padrão do repositório, que é a `main`. Os PRs da equipe vão para a `Dev`.
**Solução:** as issues passaram a ser fechadas manualmente após o merge, com um comentário citando o PR. O `Closes` foi mantido nas descrições, porque passa a valer quando a `Dev` for mesclada na `main`.
**Lição:** a automação do GitHub assume um fluxo com uma única branch; com `Dev` + `main`, o fechamento das issues é um passo manual do processo.
**Módulo/responsável:** Processo — Brenda.

## [01/10/2026] Tabela de símbolos vazia ao fim da análise (--tabela)
**Contexto:** S3-01 (issue #32), flag `--tabela` para mostrar a tabela de símbolos na demonstração.
**Sintoma:** a função `imprimir_tabela()`, chamada depois da análise semântica, não teria nenhum símbolo para mostrar.
**Causa:** o semântico chama `sair_escopo()` ao terminar cada bloco e `liberar_tabela()` ao fim da análise; quando a análise termina, todos os escopos já foram liberados.
**Solução:** a tabela passou a guardar um histórico dos símbolos inseridos (com o nível de escopo), que sobrevive à liberação dos escopos e é impresso pelo `--tabela` (commit `1823b2b`, PR #62).
**Lição:** a estrutura de dados certa para a análise (pilha que encolhe) não é a mesma que serve para inspeção depois dela; quando precisamos das duas, guardamos um registro à parte.
**Módulo/responsável:** Semântico/Integração — Brenda.

---

## Problemas identificados e ainda em aberto

Encontrados durante os testes da Sprint 3; entram no planejamento da próxima sprint.

- **`printf` imprime `\n` literalmente e não implementa `%s`:** a função `executar_printf` em `interpreter.c` não decodifica as sequências de escape, embora o comentário em `scanner.l` diga que isso seria feito na interpretação. Visível em `tests/validos/printf_scanf_formatos.c`.
- **Interpretador não executa comandos:** `if`, `while`, `for`, `return` e chamadas de função passam pelas análises, mas não são executados; só as declarações e expressões do bloco da `main` rodam.
- **String não fechada reportada como "caractere invalido":** o `scanner.l` não tem regra para string sem aspas de fechamento, e o padrão `LIT_STRING` aceita quebra de linha. Visível em `tests/invalidos/string_nao_fechada.c`.
