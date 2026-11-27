int soma(int n)
{
    if (n == 0)
        return 0;
    return n + soma(n - 1);
}
int main()
{
    printf("%d\n", soma(999));
    return 0;
}