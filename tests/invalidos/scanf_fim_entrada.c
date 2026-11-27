// Cobre: erro de execucao quando a entrada acaba antes de o scanf terminar de ler
int main() {
    int a;
    int b;

    scanf("%d", &a);
    scanf("%d", &b);
    printf("%d\n", a + b);
    return 0;
}
