# Interpretador de C (subconjunto) · Grupo 09

[![Build e testes](https://github.com/CompiladoresGrupo09/CompiladoresGrupo-09/actions/workflows/build.yml/badge.svg)](https://github.com/CompiladoresGrupo09/CompiladoresGrupo-09/actions/workflows/build.yml)
![Testes](https://img.shields.io/badge/testes-88%20passando-2ea44f)
![Feito com](https://img.shields.io/badge/feito%20com-C%20%C2%B7%20Flex%20%C2%B7%20Bison-3a5bd9)

Interpretador de um subconjunto da linguagem C, feito em **C com Flex e Bison** para a disciplina **Compiladores 1 (FGA0003)** da UnB. O programa lê um arquivo `.c`, passa pelas análises léxica, sintática e semântica e executa o código percorrendo a árvore sintática (AST), sem gerar código de máquina.

📖 **Documentação completa:** [compiladoresgrupo09.github.io/CompiladoresGrupo-09](https://compiladoresgrupo09.github.io/CompiladoresGrupo-09/)

## Sumário

- [Equipe](#equipe)
- [O que o interpretador faz](#o-que-o-interpretador-faz)
- [Requisitos e build](#requisitos-e-build)
- [Como usar](#como-usar)
- [Exemplos de entrada e saída](#exemplos-de-entrada-e-saída)
- [Arquitetura](#arquitetura)
- [Testes](#testes)
- [Estrutura do repositório](#estrutura-do-repositório)
- [Limitações conhecidas](#limitações-conhecidas)
- [Documentação](#documentação)

## Equipe

| Integrante | GitHub | Papel |
|---|---|---|
| Brenda Beatriz | [@Brwnds](https://github.com/Brwnds) | Léxico (`scanner.l`) e sintático/AST (`parser.y`); integração, testes e documentação; **líder** (envio dos formulários P1 e P2) |
| Giovana Ferreira | [@gih7915](https://github.com/gih7915) | Semântico: tabela de símbolos com escopos; integração (`main.c`) |
| Mariana Martins | [@Marianamrts](https://github.com/Marianamrts) | Semântico: tipos na AST e verificação de compatibilidade |
| Eduardo Ribeiro Xavier | [@EduardoRibeiroXavier](https://github.com/EduardoRibeiroXavier) | Interpretação |
| Henrique Fontenelle | [@HenriqueFontenelle](https://github.com/HenriqueFontenelle) | Scrum Master: revisão, integração `Dev`/`main` e decisões técnicas; **suplente** da líder |

O que cada integrante entregou, com as issues e os PRs, está em [Planejamento das Sprints](docs/documentacao/planejamento-sprints.md).

## O que o interpretador faz

**Aplicação escolhida:** um organizador de compras no mercado. O programa lê o orçamento e o preço de cada item com `scanf`, soma o total gasto e avisa se o valor passou do orçamento.

**Núcleo da linguagem:**

| Recurso | O que é aceito |
|---|---|
| Tipos | `int`, `float` e `char`; `void` como retorno de função |
| Expressões | aritméticas, relacionais e lógicas, com precedência e curto-circuito em `&&` e `\|\|` |
| Controle de fluxo | `if`/`else`, `while`, `for` e `return` |
| Escopo | blocos `{ }` com escopos aninhados e variáveis globais |
| Funções | parâmetros por valor, retorno e recursão (até 1000 chamadas aninhadas) |
| Entrada e saída | `printf` com `%d`, `%f`, `%c` e `%s`; `scanf` com `%d`, `%f` e `%c` |

**Diferenciais:** fizemos os testes automatizados mais extensos (88 testes) e o relatório de erros unificado. A coluna nas mensagens de erro e os vetores ficaram de fora, de propósito, para não arriscar o núcleo no dia da entrega.

**Fora do escopo:** otimizações, ponteiros, structs e unions, pré-processador, strings dinâmicas e arquivos. Vetores, ponteiros, structs e unions são recusados com a mensagem "construcao fora do escopo", e as linhas com `#` (como `#include`) são ignoradas.

Mais detalhes em [Definição da Linguagem](docs/documentacao/definicao-da-linguagem.md) e na [Gramática](docs/documentacao/gramatica.md).

## Requisitos e build

### O que precisa estar instalado

| Ferramenta | Para que serve | Versão que usamos |
|---|---|---|
| `flex` | gera o analisador léxico a partir de `scanner.l` | 2.6.4 |
| `bison` | gera o analisador sintático a partir de `parser.y` | 3.8.2 |
| `gcc` | compila o código C | 16.2 (qualquer versão recente serve) |
| `make` | executa os passos do build na ordem certa | qualquer |
| `bash` | roda a suíte de testes (`make test`) | qualquer |

Recomendamos o **Bison 3** ou mais novo.

<details>
<summary><b>Linux (Ubuntu/Debian)</b></summary>

```bash
sudo apt update
sudo apt install flex bison gcc make
```
</details>

<details>
<summary><b>Windows (MSYS2)</b></summary>

1. Instale o [MSYS2](https://www.msys2.org/).
2. Abra o terminal **MSYS2 MINGW64** pelo menu Iniciar. Precisa ser o MINGW64, não o MSYS.
3. Instale as ferramentas:
   ```bash
   pacman -S mingw-w64-x86_64-gcc make flex bison
   ```
4. Vá até a pasta do repositório. O disco `C:` fica em `/c`, por exemplo:
   ```bash
   cd /c/Users/seu-usuario/CompiladoresGrupo-09
   ```

No Windows, o executável gerado se chama `interpretador.exe`, mas o comando `./interpretador` funciona do mesmo jeito.
</details>

<details>
<summary><b>macOS</b></summary>

O Bison que vem com o macOS é a versão 2.3, que é antiga. Instale uma versão nova pelo [Homebrew](https://brew.sh/) e coloque-a no `PATH`:

```bash
brew install flex bison
export PATH="$(brew --prefix bison)/bin:$PATH"
```
</details>

### Build

```bash
git clone https://github.com/CompiladoresGrupo09/CompiladoresGrupo-09.git
cd CompiladoresGrupo-09/src
make
```

O `make` faz três coisas, nesta ordem:

1. `bison -d parser.y` gera o analisador sintático (`parser.tab.c` e `parser.tab.h`);
2. `flex scanner.l` gera o analisador léxico (`lex.yy.c`);
3. o `gcc` compila tudo junto e gera o executável **`interpretador`**.

| Comando | O que faz |
|---|---|
| `make` | gera o executável `interpretador` |
| `make test` | compila, se precisar, e roda os 88 testes |
| `make clean` | apaga os arquivos gerados para começar do zero |

Se tudo deu certo, `make test` termina com `Resumo: 88 passaram, 0 falharam (total 88)`.

## Como usar

Todos os comandos abaixo rodam dentro de `src/`:

```bash
./interpretador ../tests/validos/organizador_compras.c
```

Sem nenhuma opção, o interpretador faz as análises e executa o programa. As opções mostram cada fase separadamente:

| Opção | O que mostra |
|---|---|
| `--tokens` | só a análise léxica, com a lista de tokens |
| `--ast` | as análises completas e a AST, sem executar |
| `--tabela` | as análises completas e a tabela de símbolos, sem executar |
| `--no-run` | as análises completas, sem executar |
| `--limite-recursao N` | diminui o limite de chamadas aninhadas (o padrão e o máximo são 1000) |

No fim, o interpretador sempre mostra um **relatório de erros** com o total de cada fase. As mensagens seguem o formato `Erro <fase> [linha N]: <descricao>`.

**Código de saída:**

| Código | Significado |
|---|---|
| valor do `return` do `main` | execução sem erros em nenhuma fase |
| 1 | uso incorreto (arquivo ausente ou opção desconhecida) |
| 2 | erro sintático |
| 3 | erro léxico |
| 4 | erro semântico |
| 5 | erro de execução |

O passo a passo para a apresentação, com as saídas salvas de cada comando, está no [Roteiro de Demonstração](docs/documentacao/demonstracao.md).

## Exemplos de entrada e saída

### Organizador de compras (estudo de caso principal)

Programa completo em `tests/validos/organizador_compras.c`. Rodando no terminal e digitando orçamento 100 e os itens 30, 40 e 50, encerrando com -1:

```
$ ./interpretador ../tests/validos/organizador_compras.c
Digite o orcamento disponivel: 100
Digite o preco de cada item (digite -1 para finalizar):
Preco do item: 30
Preco do item: 40
Preco do item: 50
Preco do item: -1
Itens registrados: 3
Total gasto: 120.000000
Atencao: voce ultrapassou o orcamento!

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   0 erro(s)
```

### Total por quantidade

Programa mais simples, em `tests/validos/total_por_quantidade.c`: lê a quantidade e o preço unitário, mostra o total e diz se a compra é no atacado (10 itens ou mais) ou no varejo.

```
$ ./interpretador ../tests/validos/total_por_quantidade.c
Quantidade: 12
Preco unitario: 2.5
Total: 30.000000
Compra no atacado

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  0 erro(s)
Execucao   0 erro(s)
```

### Um erro sendo acusado

```
$ ./interpretador ../tests/invalidos/variavel_nao_declarada.c
Erro semantico [linha 3]: identificador nao declarado: 'y'

==== Relatorio de erros ====
Lexica     0 erro(s)
Sintatica  0 erro(s)
Semantica  1 erro(s)
Execucao   nao executada
```

Cada integrante também escreveu um programa completo para validar o interpretador: boletim escolar, caixa eletrônico, calculadora de números e controle de temperatura. Eles estão em [Estudos de Casos](docs/documentacao/estudos-de-casos.md).

## Arquitetura

```
arquivo .c → léxico (Flex) → sintático (Bison, constrói a AST) → semântico → execução → saída
```

| Fase | Arquivos | O que faz |
|---|---|---|
| Léxica | `scanner.l` | separa o código em tokens e acusa caracteres inválidos |
| Sintática | `parser.y`, `ast.c` | confere a estrutura do programa e monta a AST |
| Semântica | `semantic.c`, `tabela.c` | confere declarações, escopos, tipos e chamadas de função |
| Execução | `interpreter.c` | percorre a AST e executa o programa, com uma pilha de escopos e um frame por chamada |
| Integração | `main.c`, `erros.c` | encadeia as fases, para na primeira que falhar e mostra o relatório de erros |

As quatro fases reportam os erros pelo mesmo módulo (`erros.c`), e o pipeline **para na primeira fase que acusar erro**: não faz sentido analisar uma AST que veio com erro de sintaxe, nem executar um programa com erro de tipo.

## Testes

```bash
cd src
make test
```

São **88 testes**: 44 programas válidos em `tests/validos/` e 44 com erro em `tests/invalidos/`. Cada caso tem um `.esperado` com a saída correta, e os que leem do teclado têm um `.entrada`. A suíte confere o código de saída e compara a saída inteira.

A **CI no GitHub Actions** roda o build e a suíte em todo pull request, e a tabela que mostra qual teste cobre cada parte do núcleo está em [`tests/README.md`](tests/README.md).

## Estrutura do repositório

```
CompiladoresGrupo-09/
├── src/                 código do interpretador
│   ├── scanner.l        analisador léxico (Flex)
│   ├── parser.y         analisador sintático e construção da AST (Bison)
│   ├── ast.c            nós da AST
│   ├── tabela.c         tabela de símbolos com escopos
│   ├── semantic.c       análise semântica
│   ├── interpreter.c    execução
│   ├── erros.c          reporte e contagem de erros por fase
│   ├── main.c           pipeline, opções e relatório
│   ├── Makefile
│   └── run_tests.sh     suíte de testes
├── tests/
│   ├── validos/         programas que devem rodar sem erro
│   └── invalidos/       programas que devem ser recusados
└── docs/                documentação publicada no GitHub Pages
```

## Limitações conhecidas

- As mensagens de erro mostram a linha, mas não a coluna.
- O `printf` não aceita precisão (`%.2f`), e o `%f` sempre mostra 6 casas.
- A recursão é limitada a 1000 chamadas aninhadas.
- A divisão de `float` por zero não é erro e imprime `inf`, como em C.
- Uma string sem as aspas de fechamento aparece como "caractere invalido", sem mensagem própria.
- Vetores, ponteiros, structs e unions são recusados com a mensagem "construcao fora do escopo".

## Documentação

Toda a documentação está no [site do projeto](https://compiladoresgrupo09.github.io/CompiladoresGrupo-09/) e na pasta `docs/`:

| Documento | O que tem |
|---|---|
| [Definição da Linguagem](docs/documentacao/definicao-da-linguagem.md) | escopo, diferenciais e o que ficou de fora |
| [Gramática](docs/documentacao/gramatica.md) | a gramática aceita pelo `parser.y` |
| [Decisões Técnicas](docs/documentacao/decisoes-tecnicas.md) | cada decisão do projeto, com o porquê |
| [Planejamento das Sprints](docs/documentacao/planejamento-sprints.md) | o que foi planejado e entregue em cada sprint |
| [Atas das Reuniões](docs/documentacao/atas.md) | as reuniões de planejamento e as decisões do grupo |
| [Problemas e Soluções](docs/documentacao/problemas-e-solucoes.md) | os problemas que enfrentamos e como resolvemos |
| [Estudos de Casos](docs/documentacao/estudos-de-casos.md) | os programas usados para validar o interpretador |
| [Roteiro de Demonstração](docs/documentacao/demonstracao.md) | o passo a passo da apresentação |
