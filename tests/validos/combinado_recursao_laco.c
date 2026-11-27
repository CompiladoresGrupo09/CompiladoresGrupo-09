// Cobre: construcoes combinadas, com uma funcao recursiva chamada dentro de um laco
// e uma decisao dentro de outra funcao
int fatorial(int n) {
    if (n <= 1)
        return 1;
    return n * fatorial(n - 1);
}

int eh_par(int n) {
    if (n % 2 == 0)
        return 1;
    return 0;
}

int main() {
    int i;
    int somaPares = 0;

    for (i = 1; i <= 5; i = i + 1) {
        printf("%d! = %d\n", i, fatorial(i));
        if (eh_par(i))
            somaPares = somaPares + fatorial(i);
    }

    printf("Soma dos fatoriais pares: %d\n", somaPares);
    return 0;
}
