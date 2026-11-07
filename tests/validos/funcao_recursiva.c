// Cobre: funcoes recursivas (fatorial e fibonacci) chamando a si mesmas
int fatorial(int n) {
    if (n <= 1) return 1;
    return n * fatorial(n - 1);
}

int fibonacci(int n) {
    if (n < 2) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main() {
    int f;
    f = fatorial(5);
    f = fibonacci(10);
    printf("Fatorial: %d\n", fatorial(5)); //Adicionando saída no terminal para teste de Fatorial -> teste silencioso
    printf("Fibonacci: %d\n", fibonacci(10)); //Adicionando saída no terminal para teste de Fibonacci -> teste silencioso
    return 0;
}
