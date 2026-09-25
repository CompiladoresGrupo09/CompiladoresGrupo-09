# Glossário de termos

Glossário pode ser definido como uma lista de termos técnicos, presentes em um texto, acompanhados de seus respectivos significados. Utilizado como um dicionário específico para o trabalho desenvolvido.

## Termos gerais em compiladores

1. **Ambiguidade** — Situação em que uma mesma sentença pode ser interpretada de múltiplas formas pela gramática.
2. **Analisador léxico (scanner)** — Primeira fase do compilador, que transforma o código-fonte em uma sequência de tokens.
3. **Analisador semântico** — Verifica se a estrutura sintática tem significado válido na linguagem de programação, incluindo tipos e escopo das variáveis.
4. **Analisador sintático (parser)** — Fase que verifica se a sequência de tokens segue as regras gramaticais da linguagem, produzindo uma árvore sintática.
5. **Árvore sintática** — Estrutura hierárquica resultante da análise sintática, que representa as construções gramaticais do código-fonte.
6. **Back-end** — Parte do compilador que realiza otimização e gera o código-alvo.
7. **Compilador** — Programa que traduz código-fonte em uma linguagem de alto nível para código de máquina ou linguagem intermediária.
8. **Erro semântico** — Erro relacionado ao significado das construções sintáticas válidas, como incompatibilidade de tipos.
9. **Erro sintático** — Violação das regras gramaticais identificada pelo parser.
10. **Front-end** — Parte do compilador que realiza análise léxica, sintática e semântica, produzindo uma representação intermediária.
11. **Gerador de código final** — Transforma a representação intermediária otimizada em código executável específico para uma arquitetura computacional.
12. **Gerador de código intermediário** — Produz uma representação intermediária da estrutura sintática validada, simplificando a geração de código final.
13. **Gramática** — Conjunto de regras que definem a estrutura válida de sentenças em uma linguagem.
14. **Interpretador** — Executa diretamente as instruções escritas em uma linguagem de programação, sem gerar código intermediário ou executável.
15. **Linguagem-alvo** — Linguagem resultante após o processo de compilação, geralmente código de máquina ou uma linguagem intermediária.
16. **Linguagem-fonte** — Linguagem em que o programador escreve seu código (ex.: C, Java, Python).
17. **Linguagem intermediária** — Representação interna do código-fonte, gerada durante o processo de compilação, que facilita otimizações e traduções posteriores.
18. **Otimizador** — Melhora a eficiência do código intermediário ou final, reduzindo tempo de execução ou consumo de memória.
19. **Tabela de símbolos** — Estrutura de dados que armazena informações sobre identificadores usados no programa, como nomes, tipos e escopos.
20. **Token** — Unidade básica da análise léxica, representando palavras-chave, identificadores, operadores, constantes e símbolos.

## Termos específicos — Flex (analisador léxico)

1. **Ação (action)** — Código executado ao reconhecer um padrão definido na expressão regular.
2. **Arquivo `.l`** — Arquivo de entrada usado pelo Flex contendo especificações léxicas em expressões regulares e ações.
3. **Estado inicial (INITIAL state)** — Estado padrão em que o analisador léxico inicia seu processamento.
4. **Expressão regular** — Padrão usado para descrever tokens na definição de regras léxicas.
5. **Lexema** — Sequência específica de caracteres que forma um token reconhecido pelo analisador léxico.
6. **Padrão (pattern)** — Descrição formal em expressões regulares para reconhecimento de tokens.
7. **Tokenização** — Processo de dividir o fluxo de entrada em tokens válidos.
8. **yytext** — Variável global que armazena o lexema correspondente ao padrão reconhecido pelo Flex.
9. **yyleng** — Variável que indica o comprimento do lexema armazenado em `yytext`.
10. **yylex()** — Função gerada pelo Flex responsável pela análise léxica, retornando tokens ao parser.

## Termos específicos — Bison (analisador sintático)

1. **$$, $1, $2, etc.** — Variáveis especiais usadas nas ações semânticas no Bison: `$$` representa o valor da produção atual, enquanto `$1`, `$2`, etc., representam os valores dos símbolos da regra.
2. **Ação semântica** — Código associado às produções gramaticais executado durante a análise sintática para construção de árvores, avaliações ou geração intermediária.
3. **Arquivo `.y`** — Arquivo de entrada usado pelo Bison contendo especificações gramaticais e ações associadas.
4. **Associatividade** — Especifica a ordem de avaliação de operadores em situações de precedência igual (esquerda, direita ou nenhuma).
5. **Conflito reduce/reduce** — Situação onde existem múltiplas regras válidas para reduzir a mesma sequência de tokens, indicando ambiguidade na gramática.
6. **Conflito shift/reduce** — Situação onde o parser não sabe se deve deslocar um novo símbolo ou reduzir pela regra existente, indicando ambiguidade.
7. **Precedência** — Indica a ordem relativa de operadores, determinando quais operações são executadas primeiro.
8. **Produção** — Regra específica na gramática do Bison, composta por um símbolo não terminal seguido de símbolos terminais e/ou não terminais.
9. **Regra gramatical** — Descrição formal que define como sentenças são formadas a partir de símbolos terminais e não terminais.
10. **Símbolo não terminal** — Símbolo abstrato utilizado para definir regras gramaticais, representando construções complexas da linguagem.
11. **Símbolo terminal** — Símbolo básico da gramática, equivalente a um token reconhecido pelo analisador léxico.
12. **yyparse()** — Função gerada pelo Bison que realiza a análise sintática com base na gramática definida.

## Histórico de Versão

| Data | Versão | Descrição | Autor |
|------|--------|-----------|-------|
| 05/09/2026 | 1.0 | Criação da página | Brenda |
| 25/09/2026 | 1.1 | Modificação da Introdução e Revisão | Brenda |