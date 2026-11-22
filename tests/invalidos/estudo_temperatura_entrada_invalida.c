// Cobre: estudo de caso de controle de temperatura com entrada invalida (texto no lugar da temperatura).
// le temperaturas em Celsius ate a sentinela -1, converte numa funcao com parametro char
// e mostra quantidade, maior, menor e media. Inclui um bloco com shadowing de variavel.
float converter(float celsius, char escala) {
    if (escala == 'F' || escala == 'f')
        return celsius * 9.0 / 5.0 + 32.0;
    return celsius;
}

int main() {
    char escala;
    float temperatura;
    float convertida;
    float soma;
    float maior;
    float menor;
    int leituras;

    printf("Escala de saida (C ou F): ");
    scanf("%c", &escala);
    printf("Digite as temperaturas em Celsius (-1 para terminar):\n");

    leituras = 0;
    soma = 0.0;
    scanf("%f", &temperatura);
    while (temperatura != -1) {
        convertida = converter(temperatura, escala);

        if (leituras == 0) {
            maior = convertida;
            menor = convertida;
        } else {
            if (convertida > maior)
                maior = convertida;
            if (convertida < menor)
                menor = convertida;
        }

        soma = soma + convertida;
        leituras = leituras + 1;
        scanf("%f", &temperatura);
    }

    printf("Escala: %c\n", escala);
    printf("Leituras: %d\n", leituras);
    if (leituras > 0) {
        printf("Maior: %f\n", maior);
        printf("Menor: %f\n", menor);
        printf("Media: %f\n", soma / leituras);
    } else {
        printf("Nenhuma leitura registrada\n");
    }

    {
        int leituras = 0;
        printf("Leituras dentro do bloco: %d\n", leituras);
    }
    printf("Leituras fora do bloco: %d\n", leituras);

    return 0;
}
