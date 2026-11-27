int primeiro_multiplo(int limite)
{
    int i;
    int j;
    for (i = 1; i <= limite; i = i + 1)
    {
        j = 0;
        while (j < 3)
        {
            if ((i + j) % 4 == 0)
                return i * 10 + j;
            j = j + 1;
        }
    }
    return -1;
}
int main()
{
    printf("%d\n", primeiro_multiplo(5));
    printf("%d\n", primeiro_multiplo(0));
    return 0;
}