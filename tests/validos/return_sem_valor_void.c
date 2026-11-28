void nada()
{
}
void sai_cedo(int x)
{
    if (x > 0)
        return;
    printf("nao saiu com %d\n", x);
}
void conta_ate(int n)
{
    int i;
    for (i = 1; i <= 10; i = i + 1)
    {
        if (i > n)
            return;
        printf("%d\n", i);
    }
    printf("nunca\n");
}
int main()
{
    nada();
    sai_cedo(1);
    sai_cedo(0);
    sai_cedo(-3);
    conta_ate(3);
    printf("fim\n");
    return 0;
}