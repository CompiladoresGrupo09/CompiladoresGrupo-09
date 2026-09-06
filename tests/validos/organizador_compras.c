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
