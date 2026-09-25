# Problemas e soluções

Registro contínuo de obstáculos encontrados durante o desenvolvimento e como foram resolvidos. Exigência de repositório — atualizar em tempo real, não deixar para o fim.

Formato sugerido por entrada:

## [Data] Título curto do problema
**Contexto:** o que estava sendo feito quando o problema apareceu.
**Problema:** o que deu errado.
**Solução:** como foi resolvido.
**Módulo/responsável:** quem identificou/resolveu.

## [15/09/2026] Lista plana do professor incompatível com escopos aninhados
**Contexto:** início da implementação da tabela de símbolos (issue #22), usando `semana 06/src/tabela.h` do professor como referência.
**Problema:** o material do professor usa uma lista única e plana de símbolos, adequada ao exemplo dele. O núcleo obrigatório do projeto inclui blocos `{ }` e funções com parâmetros — com lista plana, uma variável declarada dentro de um `if` continuaria visível depois do bloco, e `int a; { int a; }` (código C válido) seria rejeitado como redeclaração.
**Solução:** adaptação da estrutura para uma pilha de escopos, onde `buscar_no_escopo_atual()` verifica só o nível corrente (base da detecção de redeclaração) e `buscar_simbolo()` percorre do escopo atual para fora (resolução de nomes com shadowing). Decisão registrada em `decisoes-tecnicas.md` com justificativa técnica.
**Módulo/responsável:** Semântico (P3) — Giovana/Eduardo.

## [15/09/2026] PR da #22 comparado contra base desatualizada
**Contexto:** revisão do PR `feature/22-tabela-simbolos` antes do merge.
**Problema:** o branch foi criado a partir de `Dev` (branch de integração da equipe), mas `main` estava atrasado em relação a `Dev` — o diff contra `main` trazia embutidas 76 linhas de uma correção de outra issue (#21, PR #42) que já estava em `Dev` e ainda não tinha chegado em `main`, misturando histórico de duas issues no mesmo PR.
**Solução:** identificado que o PR deveria mesmo ser comparado contra `Dev` (fluxo correto da equipe), e resolvida separadamente a defasagem entre `Dev` e `main` via PR próprio, sem misturar com a revisão da #22.
**Módulo/responsável:** Revisão/integração — Henrique.

## [15/09/2026] Push direto no main contornando regra de proteção
**Contexto:** tentativa de sincronizar `Dev` em `main` pelo terminal, para destravar a comparação correta do PR da #22.
**Problema:** o push foi aceito, mas o GitHub reportou explicitamente `Bypassed rule violations: Changes must be made through a pull request` — permissão de admin permitiu contornar a regra de PR obrigatório em `main`, definida em `decisoes-tecnicas.md`.
**Solução:** commit revertido (`git revert -m 1`) via PR formal, revisado e aprovado por Giovana; sincronização refeita do zero através de um segundo PR devidamente revisado, restaurando o processo combinado pela equipe.
**Módulo/responsável:** Revisão/integração — Henrique, aprovação de Giovana.

## [15/09/2026] Git não remesclou o conteúdo revertido ("Already up to date")
**Contexto:** segunda tentativa de sincronizar `Dev` em `main`, logo após o revert do merge indevido.
**Problema:** `git merge origin/Dev` retornou `Already up to date`, mesmo com o conteúdo revertido — o Git verifica ancestralidade de commits, não o conteúdo da árvore, e a ponta de `Dev` já constava no histórico (revertida, mas ainda presente).
**Solução:** em vez de mesclar `Dev` novamente, foi feito `git revert` do próprio commit de revert, reaplicando o conteúdo original de forma explícita. Confirmado pelo push subsequente enviando objetos reais (não mais "Total 0").
**Módulo/responsável:** Revisão/integração — Henrique.

## [15/09/2026] Conflito de conteúdo em decisoes-tecnicas.md não detectado pelo Git
**Contexto:** atualização do branch da #22 com o `main` sincronizado, antes da revisão final.
**Problema:** o branch da #22 tinha uma versão desatualizada da seção sobre acesso do professor ao repositório ("PENDÊNCIA"), enquanto `main` já tinha essa questão resolvida ("RESOLVIDO", com decisão mais recente e informada). Como as duas versões não se sobrepunham linha a linha, o Git mesclou automaticamente sem acusar conflito — risco de contradição silenciosa no documento.
**Solução:** conferência manual do arquivo completo pós-merge, confirmando que apenas a versão correta e mais recente ("RESOLVIDO") permaneceu no resultado final.
**Módulo/responsável:** Revisão/integração — Henrique.

## [15/09/2026] Critérios de aceitação centrais da #22 sem cobertura de teste
**Contexto:** validação do PR antes da aprovação final.
**Problema:** a suíte de testes existente (`make test`) só cobria casos sem sombreamento de variáveis nem erros semânticos — nenhum teste exercitava shadowing aceito, redeclaração rejeitada, ou busca em escopo externo a partir de bloco interno, que são critérios de aceitação explícitos da issue.
**Solução:** criados três testes dedicados (`escopo_shadowing.c`, `redeclaracao_mesmo_escopo.c`, `variavel_nao_declarada.c`) e um programa de teste isolado da API da tabela, rodado sob AddressSanitizer, confirmando ausência de vazamento de memória ao sair de escopo.
**Módulo/responsável:** Revisão/integração — Henrique.

## [15/09/2026] Desvios do checklist da issue não sinalizados no PR
**Contexto:** revisão final do PR antes da aprovação.
**Problema:** dois itens do checklist original não foram cumpridos — commits fora do padrão `feat(semantic): ...` combinado pela equipe, e sem evidência de testes manuais da API antes da integração com `semantic.c` (a entrega já veio com tabela + integração semântica completas, ultrapassando o escopo original da #22).
**Solução:** registrados como observações não-bloqueantes no comentário de revisão do PR, para correção nos próximos PRs. Entrega aprovada e mesclada por estar funcionalmente correta, com os desvios documentados em vez de ignorados silenciosamente.
**Módulo/responsável:** Semântico (P3) — Giovana/Eduardo; revisão — Henrique.