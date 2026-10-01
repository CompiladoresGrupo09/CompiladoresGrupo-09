// Cobre: printf e scanf com os especificadores %d, %f, %c e %s,
// incluindo o operador & nos argumentos do scanf
#include <stdio.h>
int main() {
    int idade = 20;
    float altura = 1.75;
    char inicial = 'B';

    printf("idade: %d\n", idade);
    printf("altura: %f\n", altura);
    printf("inicial: %c\n", inicial);
    printf("%d %f %c\n", idade, altura, inicial);
    printf("nome: %s\n", "Maria");

    scanf("%d", &idade);
    scanf("%f", &altura);
    scanf("%c", &inicial);
    return 0;
}
