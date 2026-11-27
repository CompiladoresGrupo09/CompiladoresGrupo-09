# Definição da linguagem: núcleo de C interpretado

Mesmo com cerca de um mês de prazo, C completo (ponteiros, structs, pré-processador etc.) não caberia com qualidade. Aqui registramos o escopo que decidimos manter fechado.

## Núcleo obrigatório (para o interpretador "rodar")

- Tipos: `int`, `float`, `char`
- Declaração e atribuição de variáveis
- Expressões aritméticas, relacionais e lógicas, com precedência correta
- Estruturas de controle: `if`/`else`, `while`, `for`
- Blocos `{ }` e escopo de variáveis
- Funções com parâmetros, retorno e chamadas (inclusive recursivas)
- Entrada/saída mínima: `printf` (formatos básicos `%d %f %c %s`) e `scanf`

O `for` aceita expressões nas três posições (que podem ficar vazias), mas não
declaração de variável no inicializador. Com essa escolha, mantivemos o subconjunto no
estilo C89: a variável é declarada antes do laço. `for (int i = 0; ...)` é recusado
com uma mensagem explícita.

## Diferenciais: só se o núcleo fechasse adiantado, nesta ordem de prioridade

1. Mensagens de erro com linha/coluna (barato: basta propagar `yylineno` do Flex pelas fases; alto retorno em robustez percebida)
2. Testes automatizados mais extensos (cobrir mais casos-limite do núcleo, tratando isso como parte da qualidade do núcleo, não como "extra")
3. Vetores/arrays 1D (só se sobrar bastante tempo; exige mudanças em parser, tabela de símbolos e semântica)

## Deliberadamente fora de escopo, mesmo com um mês

- Otimizações (dobra de constantes etc.)
- Ponteiros
- Structs/unions
- Pré-processador
- Strings dinâmicas
- Arquivos

## Restrições semânticas do núcleo

- Todo programa deve declarar uma função `main`.
- `void` é permitido como tipo de retorno de função, mas não como tipo de variável ou parâmetro.
- `return` deve respeitar o tipo de retorno da função; funções `void` não retornam valor e funções não `void` exigem valor.
- O operador `&` não é um operador geral de endereço: só é aceito em argumentos de `scanf`.

## Aplicação escolhida: organizador de compras no mercado

Para exercitar o núcleo da linguagem, escolhemos como estudo de caso um **organizador de compras de mercado**: o programa lê, num laço com `scanf`, o preço de cada item comprado, soma o total gasto e avisa se o valor passou do orçamento informado pelo usuário. O tema usa só recursos do núcleo obrigatório (variáveis `float`/`int`, laço `while`, `if`/`else`, `scanf`/`printf`) e não depende de nenhum diferencial (arrays, strings dinâmicas etc.). Fizemos isso de propósito, já que esses recursos ficam fora de escopo.

## Exemplos de programas suportados

### Exemplo 1: Organizador de compras no mercado

Entrada (`exemplo.c`):

```c
#include <stdio.h>

int main() {
    float orcamento;
    float preco;
    float total = 0;
    int quantidadeItens = 0;

    printf("Digite o orcamento disponivel: ");
    scanf("%f", &orcamento);

    printf("Digite o preco de cada item (digite -1 para finalizar):\n");

    preco = 0;
    while (preco >= 0) {
        printf("Preco do item: ");
        scanf("%f", &preco);

        if (preco >= 0) {
            total = total + preco;
            quantidadeItens = quantidadeItens + 1;
        }
    }

    printf("Itens registrados: %d\n", quantidadeItens);
    printf("Total gasto: %f\n", total);

    if (total > orcamento) {
        printf("Atencao: voce ultrapassou o orcamento!\n");
    } else {
        printf("Dentro do orcamento.\n");
    }

    return 0;
}
```

Execução de exemplo (orçamento de 100, itens 30, 40 e 50, encerrando com -1):

```
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
Os números aparecem na saída porque foram digitados no terminal. Com a entrada vinda de um arquivo (`./interpretador organizador_compras.c < entrada.txt`), como faz a suíte de testes, eles não aparecem e cada mensagem fica colada na seguinte. Esse caso está automatizado em `tests/validos/organizador_compras.c`, com `.entrada` e `.esperado`.

Outros exemplos com entrada e saída reais estão no `README.md` (seção "Exemplos de entrada e saída") e em `demonstracao.md` (seção "Estudos de caso"), que reúne os programas usados por cada integrante para validar o interpretador.

| Data | Versão | Descrição | Autor |
|------|--------|-----------|-------|
| 05/09/2026 | 1.0 | Criação da página Definição da Linguagem | Brenda |
| 23/11/2026 | 1.1 | Revisão da página Definição da Linguagem | Mariana |