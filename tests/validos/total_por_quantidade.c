#include <stdio.h>

int main() {
    int quantidade;
    float precoUnitario;
    float total;

    printf("Quantidade: ");
    scanf("%d", &quantidade);
    printf("Preco unitario: ");
    scanf("%f", &precoUnitario);

    total = quantidade * precoUnitario;
    printf("Total: %f\n", total);

    if (quantidade >= 10) {
        printf("Compra no atacado\n");
    } else {
        printf("Compra no varejo\n");
    }

    return 0;
}