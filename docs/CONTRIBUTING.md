# Contribuindo

Obrigado por contribuir com o interpretador de C do Grupo 09.

## Padrão de commits

Este projeto utiliza o padrão [Conventional Commits](https://www.conventionalcommits.org/pt-br/v1.0.0/). Cada commit deve descrever uma mudança pequena e relacionada a um único objetivo.

### Formato

```text
<tipo>(<escopo opcional>): <descrição curta no imperativo>
```

Exemplo:

```text
feat(parser): adicionar suporte a declaração de variáveis
```

### Tipos permitidos

- `feat`: adiciona uma funcionalidade.
- `fix`: corrige um erro.
- `docs`: altera documentação.
- `test`: adiciona ou altera testes.
- `refactor`: reorganiza o código sem alterar seu comportamento.
- `perf`: melhora o desempenho.
- `build`: altera compilação, dependências ou o `Makefile`.
- `ci`: altera integração ou automação contínua.
- `style`: altera formatação sem mudar o comportamento.
- `chore`: realiza manutenção que não se encaixa nos tipos anteriores.
- `revert`: desfaz um commit anterior.

### Escopos sugeridos

Use um escopo quando ele ajudar a identificar a parte alterada:

- `scanner`: análise léxica e `scanner.l`.
- `parser`: análise sintática e `parser.y`.
- `ast`: construção e manipulação da AST.
- `semantic`: análise semântica.
- `interpreter`: execução dos programas.
- `tests`: casos válidos e inválidos.
- `docs`: documentação do projeto.

### Regras

- Escreva a descrição em português, com verbo no infinitivo ou no imperativo.
- Use letras minúsculas no tipo, no escopo e no início da descrição.
- Não termine a descrição com ponto.
- Mantenha a primeira linha curta e objetiva, preferencialmente com até 72 caracteres.
- Não misture mudanças sem relação no mesmo commit.
- Antes de commitar, compile o projeto e execute os testes aplicáveis.

### Exemplos

```text
feat(scanner): reconhecer literais de cadeia
fix(semantic): rejeitar variável usada antes da declaração
test(interpreter): adicionar caso de divisão inteira
docs: atualizar instruções de compilação
refactor(ast): simplificar criação de nós
build: ajustar regras do Makefile
```

### Alterações incompatíveis

Para indicar uma alteração que quebra compatibilidade, acrescente `!` após o tipo ou o escopo e explique a mudança no corpo do commit:

```text
feat(parser)!: alterar a representação de chamadas de função
```

Também é possível usar um rodapé `BREAKING CHANGE:` quando a explicação for mais detalhada.
