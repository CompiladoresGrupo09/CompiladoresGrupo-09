int main()
{
    int i = 5;
    int voltas = 0;
    while (0)
    {
        voltas = voltas + 1;
    }
    while (i < 0)
        voltas = voltas + 1;
    for (i = 0; i < 0; i = i + 1)
    {
        voltas = voltas + 1;
    }
    for (i = 10; i > 10; i = i - 1)
        voltas = voltas + 1;
    printf("%d %d\n", voltas, i);
    for (i = 0; i < 1; i = i + 1)
        voltas = voltas + 1;
    printf("%d %d\n", voltas, i);
    return 0;
}