int inc(int x)
{
    return x + 1;
}
int soma(int a, int b)
{
    return a + b;
}
int dobro(int x)
{
    return soma(x, x);
}
int main()
{
    printf("%d\n", inc(inc(inc(inc(inc(inc(inc(inc(inc(inc(0)))))))))));
    printf("%d\n", soma(soma(1, 2), soma(3, soma(4, 5))));
    printf("%d\n", dobro(dobro(dobro(1))));
    printf("%d\n", soma(inc(1), dobro(inc(2))));
    if (inc(dobro(2)) == soma(2, 3))
        printf("igual\n");
    return 0;
}