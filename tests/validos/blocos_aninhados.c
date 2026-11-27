int main()
{
    int a = 1;
    int total = 0;
    {
        int a = 2;
        total = total + a;
        {
            int a = 3;
            total = total + a;
            {
                int a = 4;
                total = total + a;
                {
                    int a = 5;
                    total = total + a;
                    printf("%d\n", a);
                }
                printf("%d\n", a);
            }
            printf("%d\n", a);
        }
        printf("%d\n", a);
    }
    printf("%d %d\n", a, total);
    return 0;
}