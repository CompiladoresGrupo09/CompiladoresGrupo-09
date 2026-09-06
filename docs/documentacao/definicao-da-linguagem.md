# Definição da linguagem — núcleo de C interpretado

Mesmo com cerca de um mês de prazo, C completo (ponteiros, structs, pré-processador etc.) não cabe com qualidade. Este documento fixa o escopo que a equipe decidiu manter fechado.

## Núcleo obrigatório (para o interpretador "rodar")

- Tipos: `int`, `float`, `char`
- Declaração e atribuição de variáveis
- Expressões aritméticas, relacionais e lógicas, com precedência correta
- Estruturas de controle: `if`/`else`, `while`, `for`
- Blocos `{ }` e escopo de variáveis
- Funções com parâmetros, retorno e chamadas (inclusive recursivas)
- Entrada/saída mínima: `printf` (formatos básicos `%d %f %c %s`) e `scanf`

## Diferenciais — só se o núcleo fechar adiantado, nesta ordem de prioridade

1. Mensagens de erro com linha/coluna (barato: basta propagar `yylineno` do Flex pelas fases; alto retorno em robustez percebida)
2. Testes automatizados mais extensos (cobrir mais casos-limite do núcleo — tratar como parte da qualidade do núcleo, não como "extra")
3. Vetores/arrays 1D (só se sobrar bastante tempo; exige mudanças em parser, tabela de símbolos e semântica)

## Deliberadamente fora de escopo, mesmo com um mês

- Otimizações (dobra de constantes etc.)
- Ponteiros
- Structs/unions
- Pré-processador
- Strings dinâmicas
- Arquivos

## Aplicação escolhida: organizador de compras no mercado

Para exercitar o núcleo da linguagem, a equipe escolheu como estudo de caso um **organizador de compras de mercado**: o programa lê, em loop via `scanf`, o preço de cada item comprado, soma o total gasto e avisa se o valor ultrapassou o orçamento informado pelo usuário. O tema usa apenas recursos do núcleo obrigatório (variáveis `float`/`int`, laço `while`, `if`/`else`, `scanf`/`printf`) — não depende de nenhum diferencial (arrays, strings dinâmicas etc.), o que é proposital, já que esses ficam fora de escopo.

## Exemplos de programas suportados

### Exemplo 1 — Organizador de compras no mercado

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
```

_(demais exemplos — casos dentro do orçamento, entrada inválida, etc. — serão adicionados conforme o núcleo for sendo implementado e testado)_
