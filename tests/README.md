# Testes do interpretador

Aqui ficam os programas que usamos para testar o interpretador. Os que precisam rodar sem erro estão em `validos/`, e os que precisam ser recusados com um erro estão em `invalidos/`.

## Como rodar

```bash
cd src
make test
```

O `make test` chama o `src/run_tests.sh`, que roda todos os arquivos `.c` das duas pastas e mostra um resumo no fim. A CI roda a mesma suíte em todo PR.

## Como cada caso é montado

| Arquivo | Para que serve |
|---|---|
| `caso.c` | O programa testado |
| `caso.esperado` | A saída que o programa tem que mostrar, incluindo o relatório de erros do fim |
| `caso.entrada` | Opcional: o que o usuário digitaria no `scanf` |

- Um caso de `validos/` tem que terminar com código 0, e um de `invalidos/` com código diferente de 0.
- Todos os casos têm `.esperado`, então a suíte também compara a saída. No resumo, isso aparece como `(codigo e saida)`.
- O `run_tests.sh` liga o `.entrada` e o `.esperado` ao `.c` **de mesmo nome**. Para testar o mesmo programa com entradas diferentes, fazemos uma cópia do `.c` com outro nome (como em `estudo_temperatura_fahrenheit.c`).
- Escrevemos os `.esperado` conferindo o resultado à mão, ou com o mesmo programa compilado com `gcc`. O comando `bash run_tests.sh --gerar ../tests/validos/caso.c` grava a saída atual num `.esperado`, mas só usamos depois de conferir que a saída está certa.
- O `tests/pendentes.txt` lista casos que podem falhar enquanto uma parte ainda não foi implementada. Hoje ele está vazio.

## Cobertura do núcleo da linguagem

Cada item do núcleo obrigatório de [Definição da Linguagem](../docs/documentacao/definicao-da-linguagem.md) tem pelo menos um teste executável.

| Item do núcleo | Testes |
|---|---|
| Tipos `int`, `float` e `char`; declaração, inicialização e atribuição | `funcao_parametros_retorno.c`, `interpretador_promocao.c`, `estudo_temperatura.c` |
| Expressões aritméticas, relacionais e lógicas, com precedência | `precedencia_operadores.c`, `interpretador_precedencia.c`, `expressao_multilinha.c` |
| `if`/`else`, inclusive aninhado e o *dangling else* | `if_else_aninhado.c`, `estudo_boletim.c` |
| `while` | `comando_while.c`, `organizador_compras.c` |
| `for`, inclusive com partes vazias | `comando_for.c`, `estudo_numeros.c` |
| Blocos, escopo e *shadowing* | `escopo_shadowing.c`, `estudo_temperatura.c` |
| Funções com parâmetros, retorno e chamadas | `funcao_parametros_retorno.c`, `funcao_float_retorno_int.c` |
| Recursão | `funcao_recursiva.c`, `fatorial.c`, `fibonacci.c`, `estudo_numeros.c` |
| `printf` com `%d`, `%f`, `%c`, `%s`, `%%` e escapes | `printf_formatos.c`, `printf_escape.c`, `printf_porcentagem_final.c`, `printf_scanf_formatos.c` |
| `scanf` com `%d`, `%f` e `%c` | `printf_scanf_formatos.c`, `organizador_compras.c`, `estudo_temperatura.c` |
| Palavras reservadas, identificadores, literais e operadores (léxico) | `tokens_basico.c` |

### Construções combinadas

| O que combina | Testes |
|---|---|
| Função recursiva chamada dentro de um laço, e decisão dentro de função | `combinado_recursao_laco.c` |
| Laço, decisão, `float`, `int`, `scanf` e `printf` num programa completo | `organizador_compras.c`, `total_por_quantidade.c` |
| Programas completos usados para validar o interpretador | os estudos de caso (`estudo_*.c`), descritos em [Estudos de Casos](../docs/documentacao/estudos-de-casos.md) |

### Casos-limite

Programas válidos que testam as bordas do núcleo, e não só o uso comum (S7-03). Os resultados foram conferidos contra o mesmo programa compilado com `gcc`.

| Tipo de limite | O que é testado | Testes |
|---|---|---|
| Numérico | Maior e menor `int`, divisão e resto com negativos, precisão do `float` (2²⁴, 6 casas e `inf`), `char` usado como número | `limite_int.c`, `divisao_negativos.c`, `limite_float.c`, `char_limites.c` |
| Estrutura degenerada | Bloco e comando vazios, laço que não executa, função sem parâmetro, `return;` em função `void` | `bloco_vazio.c`, `laco_sem_execucao.c`, `funcao_sem_parametro.c`, `return_sem_valor_void.c` |
| Aninhamento profundo | Parênteses em vários níveis, 5 blocos com *shadowing* e chamadas uma dentro da outra | `parenteses_profundos.c`, `blocos_aninhados.c`, `chamadas_aninhadas.c` |
| Interação entre construções | Curto-circuito do `&&` e do `\|\|`, `return` dentro de laços aninhados, recursão com exatamente 1000 chamadas | `curto_circuito.c`, `return_dentro_de_lacos.c`, `recursao_no_limite.c` |
| Entrada do `scanf` | Número com sinal, espaços antes e dois números na mesma linha | `scanf_sinal_espacos.c` |

Os casos-limite que precisam ser recusados (programa vazio, uma chamada além do limite, entrada mal formada e laço sem fim) estão na tabela de erros abaixo.

## Erros que o interpretador precisa acusar

O interpretador para na primeira fase que encontra erro. O código de saída mostra a fase: 3 léxico, 2 sintático, 4 semântico e 5 execução.

| Fase | O que é testado | Testes |
|---|---|---|
| Léxica | Caractere inválido, string sem aspas de fechamento | `caractere_invalido.c`, `string_nao_fechada.c` |
| Sintática | Erros de sintaxe, recuperação com vários erros, chave e parêntese sem fechar, palavra reservada como nome | `erro_sintatico.c`, `multiplos_erros_sintaticos.c`, `chave_nao_fechada.c`, `parentese_nao_fechado.c`, `palavra_reservada_identificador.c` |
| Sintática | Construções fora do escopo | `ponteiro_fora_escopo.c`, `vetor_fora_escopo.c`, `struct_fora_escopo.c`, `union_fora_escopo.c`, `for_declaracao_fora_escopo.c` |
| Semântica | Variável não declarada e redeclarada | `variavel_nao_declarada.c`, `redeclaracao_mesmo_escopo.c` |
| Semântica | Tipos: `float` para `int`, `%` com `float`, retorno e argumento de tipo errado | `erro_tipo.c`, `erro_tipo_atribuicao.c`, `erro_tipo_inicializacao.c`, `erro_retorno_funcao_float_para_int.c`, `erro_tipo_argumento.c`, `estudo_boletim_erro_tipo.c` |
| Semântica | Funções: não declarada, número de argumentos, `return` errado, `void` como tipo de variável ou parâmetro, programa sem `main` ou vazio | `erro_funcao_nao_declarada.c`, `erro_aridade.c`, `return_sem_valor.c`, `return_valor_void.c`, `return_tipo_incompativel.c`, `variavel_void.c`, `parametro_void.c`, `programa_sem_main.c`, `programa_vazio.c` |
| Semântica | `&` fora do `scanf` | `endereco_fora_scanf.c` |
| Execução | Divisão por zero | `divisao_por_zero.c`, `estudo_numeros_divisao_zero.c` |
| Execução | Recursão sem fim ou funda demais (passa do limite de 1000 chamadas) | `recursao_infinita.c`, `recursao_um_alem_do_limite.c` |
| Execução | Entrada inválida (letra ou número com ponto em `%d`) e fim da entrada no `scanf` | `estudo_temperatura_entrada_invalida.c`, `scanf_letra_no_int.c`, `scanf_float_no_int.c`, `scanf_fim_entrada.c` |
| Execução | `printf` com argumento faltando ou de tipo errado | `printf_argumentos_insuficientes.c`, `printf_d_com_texto.c`, `printf_s_com_numero.c` |
| Execução | Laço que passa de 1.000.000 de voltas (o limite pode ser trocado pela variável de ambiente `INTERPRETADOR_LIMITE_ITERACOES`) | `laco_infinito.c` |
