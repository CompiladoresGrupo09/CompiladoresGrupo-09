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

## S2-07: Verificação de compatibilidade de tipos e conversão implícita (Provisório)

* **Decisão:** o interpretador recusa conversões implícitas com perda de informação, como atribuir uma expressão do tipo `float` a uma variável do tipo `int` (por exemplo: `int a; a = 1.5 + 2;`).
* **Comportamento adotado:** emissão de erro semântico na linha da atribuição.
* **Justificativa:** promover tipagem estrita e evitar truncamento silencioso de dados. A promoção no sentido inverso (`int` para `float`, em operações binárias mistas ou na atribuição a variáveis `float`) continua válida.
* **Limitação conhecida:** a linguagem não oferece conversão de `float` para `int` (não há suporte a cast na gramática). Essa limitação é intencional no escopo atual do projeto.
* **Implementação:** PR #53 (issue #24), mesclado na `Dev`.

## Atribuição formal dos papéis técnicos P1–P4

- `scanner.l` (**P1 — Léxico**) e `parser.y` com construção de AST (**P2 — Sintático/AST**) já foram implementados e mesclados, ambos pelo mesmo autor no repositório. A formalização dessa pessoa como dona de P1+P2 será confirmada na próxima reunião.
- **Integração (`main.c`) — concluída:** implementada, revisada e mesclada (issue #18 / PR #39), com validação manual completa.
- **P3 (Semântico / tabela de símbolos) — em andamento avançado:**
  - Tabela de símbolos (issue #22) implementada e mesclada na `Dev` pelo PR #46, com testes de escopo, shadowing, redeclaração e variável não declarada (`tests/validos/escopo_shadowing.c`, `tests/invalidos/redeclaracao_mesmo_escopo.c`, `tests/invalidos/variavel_nao_declarada.c`).
  
  - Verificação de declaração e uso (S2-06, issue #23): implementada pelo PR #46 e validada pelos testes `variavel_nao_declarada.c`, `redeclaracao_mesmo_escopo.c` e `escopo_shadowing.c` (make test em 24/09/2026); issue encerrada.

  - Campo de tipo na AST e verificação de compatibilidade de tipos (S2-07, issue #24): concluída e mesclada na `Dev` pelo PR #53, trabalho em par (AST + semântico).
- **P4 (Interpretação) — iniciada:** issue #31 (esqueleto do interpretador, `interpretarNo`); avaliação de expressões em revisão no PR #52, aberto por @EduardoRibeiroXavier. A alocação formal de P4 será confirmada na próxima reunião.
- **S2-05 (tabela de símbolos com escopos aninhados):** os testes existentes indicam shadowing permitido em escopos internos e erro para redeclaração no mesmo escopo; a política será confirmada na próxima reunião e registrada numa seção própria, como a S2-07.
- **S2-07 (política de conversão implícita de tipos):** decisão registrada na seção própria acima, aguardando confirmação do grupo.

**Pontos a decidir na próxima reunião com o time:**
1. Formalizar (ou não) a atribuição de P1 e P2 à pessoa que já entregou os dois módulos.
2. Confirmar a política de escopos da S2-05 e a de conversão implícita da S2-07, retirando o "Provisório".
3. Formalizar o responsável por P4, considerando que P3 e P4 formam par.
4. Combinar o fluxo de branches: hoje há PRs de documentação indo para a `main` e de código indo para a `Dev`, e as duas estão divergindo.

## Acesso do professor ao repositório (RESOLVIDO)
Não houve uma decisão nova do professor: a política de não participar dos repositórios das equipes já era dele desde o início do semestre. Havia apenas uma dúvida remanescente no grupo sobre isso, esclarecida diretamente com ele em **01/09/2026** — ele confirmou que não tem interesse em fazer parte do repositório da equipe neste momento. Não é necessário adicioná-lo como colaborador.

## Regras de branch
`main` sempre compilável. Cada pessoa/par trabalha em branch de feature (`feat/scanner`, `feat/parser`, `feat/interpreter`...) e faz merge após revisão. `main` é protegida no GitHub, exigindo Pull Request com pelo menos 1 aprovação.