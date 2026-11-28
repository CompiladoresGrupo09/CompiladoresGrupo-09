int resposta()
{
    return 42;
}
float meio()
{
    return 0.5;
}
char letra()
{
    return 'x';
}
int seguinte()
{
    return resposta() + 1;
}
int main()
{
    printf("%d\n", resposta());
    printf("%f\n", meio());
    printf("%c\n", letra());
    printf("%d\n", seguinte());
    printf("%d\n", resposta() - resposta());
    return 0;
}