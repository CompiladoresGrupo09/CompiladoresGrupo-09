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

## Atribuição formal dos papéis técnicos P1–P4 (PENDÊNCIA — decidir em reunião com o time)
O trabalho de dois módulos já foi implementado, mas a atribuição dos papéis ainda não foi formalizada em equipe:

- `scanner.l` (entregável de **P1 — Léxico**) e `parser.y` com construção de AST (entregável de **P2 — Sintático/AST**) já foram implementados e mesclados, ambos pelo mesmo autor no repositório.
- Isso é um fato sobre o histórico do repositório, não uma decisão de equipe: não está confirmado se o grupo formalizou essa pessoa como dona de P1+P2, ou se ela adiantou trabalho que ainda estava em aberto para qualquer pessoa.
- **P3 (Semântico)** e **P4 (Interpretação)** ainda não têm dono definido. Os arquivos `semantic.c` e `interpreter.c` existem em `src/`, mas aparentam ser apenas esqueleto — nenhum PR próprio de implementação foi identificado para eles ainda.

**Pontos a decidir na próxima reunião com o time:**
1. Formalizar (ou não) a atribuição de P1 e P2 à pessoa que já entregou os dois módulos.
2. Avaliar a carga já concentrada nessa pessoa e se algum ajuste é necessário.
3. Definir quem assume P3 e quem assume P4 — considerando que os dois formam par (dependem da mesma AST e tabela de símbolos).
4. Confirmar se as demais pessoas do time já estão alocadas em outras frentes visíveis no repositório (ex.: templates de issue/PR, workflow de deploy) ou ainda sem papel técnico definido.

## Acesso do professor ao repositório (RESOLVIDO)
Havia uma divergência entre duas orientações recebidas sobre quando dar acesso ao professor ao repositório. **Resolvido em conversa direta com o professor: ele não tem interesse em fazer parte do grupo/repositório neste momento.** Não é necessário adicioná-lo como colaborador por enquanto.

## Regras de branch
`main` sempre compilável. Cada pessoa/par trabalha em branch de feature (`feat/scanner`, `feat/parser`, `feat/interpreter`...) e faz merge após revisão. `main` é protegida no GitHub, exigindo Pull Request com pelo menos 1 aprovação.
