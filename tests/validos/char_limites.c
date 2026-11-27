int main()
{
    char c = 'a';
    char n = '\n';
    char digito = '0' + 5;
    printf("%c%c\n", c + 1, 'Z');
    printf("%d %d\n", c, n);
    printf("%d\n", 'z' - 'a');
    printf("%c\n", digito);
    printf("%d\n", 'a' < 'b');
    printf("[%c][%c]\n", '\\', '\'');
    return 0;
}