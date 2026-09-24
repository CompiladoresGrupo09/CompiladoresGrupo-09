int funcao(int param) {
    int x;
    x = param;
    return x;
}

int main() {
    int a;
    a = 10;

    /* Shadowing: declarar 'a' em um bloco interno e permitido */
    {
        int a;
        a = 20;
    }

    /* 'a' do escopo externo continua acessivel aqui */
    a = 30;

    return 0;
}