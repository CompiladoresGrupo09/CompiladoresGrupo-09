// Cobre: erro sintatico por parentese nao fechado na condicao do if
int main() {
    int x = 5;
    if (x > 0 {
        x = 1;
    }
    return 0;
}
