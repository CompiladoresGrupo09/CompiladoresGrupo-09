# Decisões técnicas

Registro de decisões e o porquê de cada uma — exigência do repositório conforme a Proposta de Trabalho.

## Metodologia: Scrum com sprints semanais
Recomendação oficial da disciplina. Cronograma organizado em Sprints (Planning na segunda, Review na quarta), encaixado no ritmo já usado pelo curso.

## AST construída diretamente no parser
Diferente de um compilador (que gera código intermediário separado), o interpretador constrói a AST diretamente nas ações semânticas do `parser.y` (Bison). É essa AST que alimenta a análise semântica e a interpretação.

## Interpretação por percurso recursivo da AST
Em vez de gerar código final, a execução acontece percorrendo a AST recursivamente (função `interpretNode()` ou equivalente). É o que diferencia o módulo de execução de um compilador tradicional.

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
