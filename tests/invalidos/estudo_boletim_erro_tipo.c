#include <stdio.h>

float media(float a, float b, float c) { //cálculo da média da nota do aluno
    float Soma;
    Soma = a + b + c;
    return Soma / 3; //Int -> float 

}

int main() { 
    float n1; //Declaração das variáveis que armazenam as notas do aluno
    float n2;
    float n3;
    int m; // declaro a variável m como int, mas a função media retorna float, isso gera um erro de tipo

    scanf("%f", &n1);
    scanf("%f", &n2);
    scanf("%f", &n3);

    m = media(n1, n2, n3);

    printf("Nota 1: %f\n", n1);
    printf("Nota 2: %f\n", n2);
    printf("Nota 3: %f\n", n3);
    printf("Média: %f\n", m);

    if (m >= 7) { //Verifica se a média presente é maior ou igual a 7
        printf("Situacao: %s\n", "APROVADO");
    } else {
        if (m >= 5) { //OPeradores relacionais com int e float 
            printf("Situacao: %s\n", "RECUPERACAO");
        } else {
            printf("Situacao: %s\n", "REPROVADO");
        }
    }
    return 0;
}