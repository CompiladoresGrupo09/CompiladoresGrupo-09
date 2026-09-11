# Decisões técnicas

Registro de decisões e o porquê de cada uma — exigência do repositório conforme a Proposta de Trabalho.

## Metodologia: Scrum com sprints semanais
Recomendação oficial da disciplina. Cronograma organizado em Sprints (Planning na segunda, Review na quarta), encaixado no ritmo já usado pelo curso.

## AST construída diretamente no parser
Diferente de um compilador (que gera código intermediário separado), o interpretador constrói a AST diretamente nas ações semânticas do `parser.y` (Bison). É essa AST que alimenta a análise semântica e a interpretação.

## Interpretação por percurso recursivo da AST
Em vez de gerar código final, a execução acontece percorrendo a AST recursivamente (função `interpretNode()` ou equivalente). É o que diferencia o módulo de execução de um compilador tradicional.

## Aplicação de estudo de caso: organizador de compras no mercado
Escolhido um organizador de compras (soma preços lidos via `scanf` em loop e avisa se o total passou do orçamento) como programa de exemplo para exercitar o núcleo. Motivo: cobre variáveis, laço, condicional e E/S usando só recursos do núcleo obrigatório (seção 3 de `definicao-da-linguagem.md`), sem depender de nenhum diferencial (arrays, strings dinâmicas) que está fora de escopo.

## Estrutura de repositório: docs/ + src/ + tests/ na raiz
Alinhada ao padrão usado nos exemplos do professor (pastas `docs/`/`src/` por unidade), adaptada para o projeto inteiro ser a "unidade", com `tests/` separado para casos válidos/inválidos.

## Papéis fixos com pares cruzados
Pipeline sequencial (léxico → sintático/AST → semântico → interpretação), mas com pareamento cruzado (P1+P2, depois P3+P4, P5 circulando) para evitar silos de conhecimento — necessário porque a entrevista final é individual e cobra justificativa de qualquer parte do projeto.

## Atribuição formal dos papéis técnicos P1–P4 (PENDÊNCIA PARCIAL — P3 e P4 seguem em aberto)

- `scanner.l` (**P1 — Léxico**) e `parser.y` com construção de AST (**P2 — Sintático/AST**) já foram implementados e mesclados, ambos pelo mesmo autor no repositório. Ainda não está confirmado se o grupo formalizou essa pessoa como dona de P1+P2, ou se ela adiantou trabalho que estava em aberto para qualquer pessoa.
- **Integração (`main.c`) — concluída:** implementada, revisada e mesclada (issue #18 / PR #39), com validação manual completa (build limpo, execução nos casos de teste válidos e inválidos, `make test` passando). O binário `interpretador` já funciona conforme documentado no README.
- **P3 (Semântico / tabela de símbolos) — parcialmente resolvido, com uma dúvida em aberto:** issue própria (Sprint 02, prioridade alta), inicialmente atribuída a três integrantes. Um deles se removeu da issue pouco antes de assumir e entregar sozinho(a) a implementação de `main.c` (issue #18), que estava marcada como a tarefa mais urgente do backlog e bloqueava outras três issues. **Hipótese a confirmar com o time:** a saída de P3 pode ter sido uma repriorização legítima (essa pessoa migrou para desbloquear `main.c` primeiro), não necessariamente um problema de alocação ou sobrecarga. Falta confirmar isso diretamente com ela antes de tratar como pendência resolvida ou como obstáculo a corrigir.
- **P4 (Interpretação) — ainda pendente:** nenhuma issue ou branch de trabalho identificada até o momento. Precisa de responsável(is) definido(s).

**Pontos a decidir na próxima reunião com o time:**
1. Formalizar (ou não) a atribuição de P1 e P2 à pessoa que já entregou os dois módulos.
2. Avaliar a carga já concentrada nessa pessoa e se algum ajuste é necessário.
3. Confirmar com quem saiu da issue de P3 se foi repriorização (para destravar `main.c`) ou outro motivo, e se ela retorna a P3 ou permanece em outra frente.
4. Definir quem assume P3 (considerando a resposta do item 3) e quem assume P4 — os dois formam par (dependem da mesma AST e tabela de símbolos).
5. Confirmar se as demais pessoas do time já estão alocadas em outras frentes visíveis no repositório (ex.: templates de issue/PR, workflow de deploy) ou ainda sem papel técnico definido.

## Acesso do professor ao repositório (RESOLVIDO)
Havia uma divergência entre duas orientações recebidas sobre quando dar acesso ao professor ao repositório. **Resolvido em conversa direta com o professor: ele não tem interesse em fazer parte do grupo/repositório neste momento.** Não é necessário adicioná-lo como colaborador por enquanto.

## Regras de branch
`main` sempre compilável. Cada pessoa/par trabalha em branch de feature (`feat/scanner`, `feat/parser`, `feat/interpreter`...) e faz merge após revisão. `main` é protegida no GitHub, exigindo Pull Request com pelo menos 1 aprovação.
