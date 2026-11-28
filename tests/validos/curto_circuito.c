int avisa(int v)
{
    printf("avaliou %d\n", v);
    return v;
}
int main()
{
    int zero = 0;
    if (zero != 0 && 10 / zero > 1)
        printf("nunca\n");
    if (zero == 0 || 10 / zero > 1)
        printf("protegido\n");
    if (avisa(1) || avisa(2))
        printf("ok\n");
    if (avisa(0) && avisa(3))
        printf("nunca\n");
    printf("%d\n", avisa(0) || avisa(4));
    return 0;
}