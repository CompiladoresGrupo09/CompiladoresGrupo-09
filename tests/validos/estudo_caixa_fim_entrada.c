#include <stdio.h>

float saldo = 0.0;

void depositar(float valor) {
    saldo = saldo + valor;
    printf("Deposito realizado.\n");
}

void sacar(float valor) {
    if (valor < 0 || valor > saldo) {
        printf("Saque recusado.\n");
        return;
    }

    saldo = saldo - valor;
    printf("Saque realizado.\n");
}

int main() {
    int opcao = -1;
    float valor;

    printf("1 Depositar, 2 Sacar, 3 Ver saldo, 0 Sair: ");
    while (scanf("%d", &opcao) == 1 && opcao != 0) {
        if (opcao == 1) {
            scanf("%f", &valor);
            depositar(valor);
        } else if (opcao == 2) {
            scanf("%f", &valor);
            sacar(valor);
        } else if (opcao == 3) {
            printf("Saldo: %f\n", saldo);
        } else {
            printf("Opcao invalida.\n");
        }

        printf("1 Depositar, 2 Sacar, 3 Ver saldo, 0 Sair: ");
    }

    printf("Caixa encerrado.\n");
    return 0;
}
