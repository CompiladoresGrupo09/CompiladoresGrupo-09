#include <stdio.h>

int mdc(int a, int b) {
    if (b == 0)
        return a;
    return mdc(b, a % b);
}

int fatorial(int n) {
    if (n <= 1)
        return 1;
    return n * fatorial(n - 1);
}

int main() {
    int a;
    int b;
    int i;
    int j;
    int divisores;
    int quantidadePrimos = 0;

    printf("Digite dois numeros inteiros: ");
    scanf("%d %d", &a, &b);

    printf("MDC(%d, %d) = %d\n", a, b, mdc(a, b));
    printf("MMC(%d, %d) = %d\n", a, b, a * b / mdc(a, b));
    printf("%d! = %d\n", a, fatorial(a));

    printf("Primos ate %d:", b);
    for (i = 2; i <= b; i = i + 1) {
        divisores = 0;
        for (j = 2; j * j <= i; j = j + 1)
            if (i % j == 0)
                divisores = divisores + 1;
        if (divisores == 0) {
            printf(" %d", i);
            quantidadePrimos = quantidadePrimos + 1;
        }
    }
    printf("\nTotal de primos: %d\n", quantidadePrimos);

    printf("Tabuada do %d:\n", a);
    for (i = 1; i <= 10; i = i + 1)
        printf("%d x %d = %d\n", a, i, a * i);

    return 0;
}