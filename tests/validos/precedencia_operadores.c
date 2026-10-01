// Cobre: precedencia de operadores aritmeticos, relacionais e logicos na mesma expressao,
// incluindo unarios (-, !) e parenteses alterando a ordem de avaliacao
int main() {
    int a = 2;
    int b = 3;
    int c = 4;
    int r;

    r = a + b * c;
    r = (a + b) * c;
    r = a * b % c - -a;
    r = a + b > c && b * c != a || !(a == b);
    r = a < b == b < c;
    r = a >= b || b <= c && c > a;
    return 0;
}
