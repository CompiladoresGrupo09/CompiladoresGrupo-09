int contador;

void processar(int contador) {
    int total;
    total = 0;
    {
        int contador;
        contador = 5;
        total = total + contador;
    }
    total = total + contador;
}

int main() {
    processar(3);
    return 0;
}
