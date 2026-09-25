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

## Acesso do professor ao repositório (PENDÊNCIA — confirmar com o professor)
Existe uma divergência entre duas orientações recebidas:
- Uma versão do planejamento indicava que o professor só acessaria o repositório na entrega final, 15 dias antes da entrevista.
- O guia oficial de sprints instrui explicitamente adicionar o professor como colaborador do repositório já na Sprint 1 (usuário "sergioaafreitas" ou e-mail "sergiofreitas@unb.br").

**Decisão provisória:** seguir a instrução mais recente e mais específica — adicionar o professor como colaborador desde já. Perguntar diretamente ao professor qual das duas orientações vale, e atualizar esta seção com a resposta.

## Regras de branch
`main` sempre compilável. Cada pessoa/par trabalha em branch de feature (`feat/scanner`, `feat/parser`, `feat/interpreter`...) e faz merge após revisão. `main` é protegida no GitHub, exigindo Pull Request com pelo menos 1 aprovação.

# Decisões Técnicas do Projeto

## S2-07: Verificação de Compatibilidade de Tipos e Conversão Implícita (Provisório)

* **Decisão:** O compilador recusa conversões implícitas com perda de informação (como atribuir uma expressão do tipo `float` a uma variável do tipo `int`, por exemplo: `int a; a = 1.5 + 2;`).
* **Comportamento adotado:** Emissão de erro semântico na linha correspondente da atribuição.
* **Justificação:** Promover tipagem estrita e evitar truncamento silencioso de dados sem o consentimento explícito do utilizador via cast. A promoção implícita inversa (`int` para `float` em operações binárias mistas ou na atribuição para variáveis `float`) permanece suportada e válida.