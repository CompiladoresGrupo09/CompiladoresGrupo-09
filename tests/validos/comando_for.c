// Cobre: comando for completo, sem chaves, com secoes vazias e for(;;)
int contar() {
    int n = 0;
    for (;;) {
        n = n + 1;
        if (n >= 3) return n;
    }
}

int main() {
    int i;
    int soma = 0;

    for (i = 0; i < 5; i = i + 1) {
        soma = soma + i;
    }

    for (i = 0; i < 5; i = i + 1)
        soma = soma - i;

    i = 0;
    for (; i < 3;) {
        i = i + 1;
    }

    for (i = 10; i > 0;)
        i = i - 2;

    soma = contar();
    return 0;
}
