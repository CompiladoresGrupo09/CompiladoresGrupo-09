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

## Atribuição formal dos papéis técnicos P1–P4 (PENDÊNCIA PARCIAL — só P4 falta decidir)

- `scanner.l` (**P1 — Léxico**) e `parser.y` com construção de AST (**P2 — Sintático/AST**) já foram implementados e mesclados, ambos pelo mesmo autor no repositório. Ainda não está confirmado se o grupo formalizou essa pessoa como dona de P1+P2, ou se ela adiantou trabalho que estava em aberto para qualquer pessoa.
- **P3 (Semântico / tabela de símbolos) — resolvido:** issue própria (Sprint 02, prioridade alta), atribuída formalmente a dois integrantes do time. Um terceiro integrante que também estava atribuído foi removido da issue recentemente; o motivo dessa saída não está registrado e vale confirmar com o time.
- **P4 (Interpretação) — ainda pendente:** nenhuma issue ou branch de trabalho identificada até o momento. Precisa de responsável(is) definido(s).

**Pontos a decidir na próxima reunião com o time:**
1. Formalizar (ou não) a atribuição de P1 e P2 à pessoa que já entregou os dois módulos.
2. Avaliar a carga já concentrada nessa pessoa e se algum ajuste é necessário.
3. Definir quem assume P4 — considerando que P3 e P4 formam par (dependem da mesma AST e tabela de símbolos), o ideal é que quem entrar em P4 pareie com quem já está em P3.
4. Entender o motivo da saída de um integrante da issue de P3, para identificar se há algum obstáculo recorrente de alocação que precise ser resolvido.

## Acesso do professor ao repositório (RESOLVIDO)
Havia uma divergência entre duas orientações recebidas sobre quando dar acesso ao professor ao repositório. **Resolvido em conversa direta com o professor: ele não tem interesse em fazer parte do grupo/repositório neste momento.** Não é necessário adicioná-lo como colaborador por enquanto.

## Regras de branch
`main` sempre compilável. Cada pessoa/par trabalha em branch de feature (`feat/scanner`, `feat/parser`, `feat/interpreter`...) e faz merge após revisão. `main` é protegida no GitHub, exigindo Pull Request com pelo menos 1 aprovação.
