int fatorial(int n) {
    if (n <= 1) return 1;
    return n * fatorial(n - 1);
}

int main() {
    printf("Fatorial: %d\n", fatorial(5));
    printf("Fatorial 10: %d\n", fatorial(10));
    return 0;
}
