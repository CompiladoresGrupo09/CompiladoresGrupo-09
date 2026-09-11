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

## Tratamento do dangling else (S2-03)
O conflito clássico de `if (a) if (b) x; else y;` — a quem o `else` pertence — é resolvido pela precedência natural do Bison, que prefere deslocar (`shift`) o `else` para o `if` mais próximo sem `else`. É o comportamento padrão do C e não exigiu regra extra na gramática; o Bison emite 1 aviso de conflito shift/reduce esperado para isso, documentado em `gramatica.md`. Nenhuma ação adicional foi necessária além de reconhecer e documentar esse comportamento padrão.

## Atribuição formal dos papéis técnicos P1–P4

- `scanner.l` (**P1 — Léxico**) e `parser.y` com construção de AST (**P2 — Sintático/AST**) já foram implementados e mesclados, ambos pelo mesmo autor no repositório. Ainda não está confirmado se o grupo formalizou essa pessoa como dona de P1+P2, ou se ela adiantou trabalho que estava em aberto para qualquer pessoa.
- **Integração (`main.c`) — concluída:** implementada, revisada e mesclada (issue #18 / PR #39), com validação manual completa.
- **P3 (Semântico / tabela de símbolos) — em discussão:** issue #22 aberta desde a Sprint 02, inicialmente atribuída a três integrantes; um deles se removeu da issue pouco antes de assumir e entregar sozinho a integração (`main.c`), então prioritária e bloqueante. **Proposta em avaliação:** Henrique assumir P3, considerando o acúmulo com o papel de Scrum Master. Ainda não confirmado com o restante do time.
- **P4 (Interpretação) — pendente:** issue #31 já existe (esqueleto do interpretador, `interpretarNo`) e sugere pareamento com quem entregou a integração, mas ninguém está formalmente alocado.
- **S2-05 (tabela de símbolos com escopos aninhados) e S2-07 (política de conversão implícita de tipos):** decisões técnicas ainda não tomadas, porque dependem de quem for implementar P3. Não é possível registrar o "porquê" de uma decisão que ainda não foi feita.

**Pontos a decidir na próxima reunião com o time:**
1. Formalizar (ou não) a atribuição de P1 e P2 à pessoa que já entregou os dois módulos.
2. Confirmar a proposta de Henrique assumir P3, e se isso exige algum ajuste na carga de quem já acumula múltiplos papéis.
3. Definir quem assume P4 — considerando que P3 e P4 formam par (dependem da mesma AST e tabela de símbolos).
4. Confirmar com quem saiu da issue de P3 se foi repriorização (para destravar o `main.c`) ou outro motivo.
5. Só depois de P3 ter responsável, decidir as políticas de S2-05 e S2-07 e registrá-las aqui.

## Acesso do professor ao repositório (RESOLVIDO)
Não houve uma decisão nova do professor: a política de não participar dos repositórios das equipes já era dele desde o início do semestre. Havia apenas uma dúvida remanescente no grupo sobre isso, esclarecida diretamente com ele em **01/09/2026** — ele confirmou que não tem interesse em fazer parte do repositório da equipe neste momento. Não é necessário adicioná-lo como colaborador.

## Regras de branch
`main` sempre compilável. Cada pessoa/par trabalha em branch de feature (`feat/scanner`, `feat/parser`, `feat/interpreter`...) e faz merge após revisão. `main` é protegida no GitHub, exigindo Pull Request com pelo menos 1 aprovação.
