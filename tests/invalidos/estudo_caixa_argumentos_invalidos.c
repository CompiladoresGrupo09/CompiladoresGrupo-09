#include <stdio.h>

float saldo = 0.0;

void sacar(float valor) {
    if (valor < 0 || valor > saldo) {
        printf("Saque recusado.\n");
        return;
    }
    saldo = saldo - valor;
}

int main() {
    sacar(10.0, 20.0);
    return 0;
}
