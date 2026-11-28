int main()
{
    int x = 1;
    {};
    {
        ;
    }
    {
        {
        }
    }
    if (x)
    {
    }
    if (x)
    {
    }
    else
    {
        printf("nunca\n");
    }
    if (0)
    {
    }
    else
    {
        printf("else\n");
    }
    if (0)
        printf("nunca\n");
    printf("%d\n", x);
    return 0;
}