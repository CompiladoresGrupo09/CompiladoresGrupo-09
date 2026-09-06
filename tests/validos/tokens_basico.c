// Arquivo de teste: cobre palavras reservadas, identificadores, literais e operadores do nucleo
int main() {
    int quantidade = 10;
    float preco = 2.5;
    char letra = 'a';

    if (quantidade > 0 && preco <= 100) {
        quantidade = quantidade - 1;
    } else {
        quantidade = 0;
    }

    while (quantidade != 0) {
        quantidade = quantidade - 1;
    }

    for (quantidade = 0; quantidade < 5; quantidade = quantidade + 1) {
        printf("%d", quantidade);
    }

    return 0;
}
