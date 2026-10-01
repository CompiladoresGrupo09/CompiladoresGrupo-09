// Cobre: interpretador respeitando a precedencia de * sobre + ao imprimir com printf
#include <stdio.h>
int main()
{
    int a = 2;
    printf("%d", 1 + a * 3);
    return 0;
}