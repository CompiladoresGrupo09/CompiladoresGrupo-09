// Cobre: recuperacao de erros, com varios erros sintaticos reportados no mesmo arquivo
int 123 invalido;

int soma(int a, int b) {
    return a + b;
}

int main() {
    int x = 5
    int y = 10;
    int z = soma(x, y)
    return 0;
}
