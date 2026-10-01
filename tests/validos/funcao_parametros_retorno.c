// Cobre: declaracao e chamada de funcoes com parametros (int, float, char),
// sem parametros, com retorno de valor e funcao void com return vazio
int soma(int a, int b) {
    return a + b;
}

float media(float x, float y) {
    return (x + y) / 2.0;
}

char proxima_letra(char c) {
    return c;
}

int constante() {
    return 42;
}

void nada(int x) {
    if (x > 0) return;
    x = 0;
}

int main() {
    int r;
    float m;
    char letra;

    r = soma(1, 2);
    r = soma(soma(1, 2), constante());
    m = media(1.5, 2.5);
    letra = proxima_letra('a');
    nada(r);
    return 0;
}
