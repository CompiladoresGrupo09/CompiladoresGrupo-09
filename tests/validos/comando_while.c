// Cobre: comando while com bloco, sem chaves, com corpo vazio e aninhado
int main() {
    int i = 0;
    int j;
    int soma = 0;

    while (i < 10) {
        soma = soma + i;
        i = i + 1;
    }

    while (i > 0)
        i = i - 1;

    while (i > 0);

    while (i < 3) {
        j = 0;
        while (j < 3) {
            soma = soma + i * j;
            j = j + 1;
        }
        i = i + 1;
    }
    return 0;
}
