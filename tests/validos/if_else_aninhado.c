// Cobre: if/else aninhados, com e sem chaves, incluindo o caso de dangling else
// (o else pertence ao if mais proximo)
int main() {
    int x = 5;
    int y = 10;
    int r = 0;

    if (x > 0)
        if (y > 0)
            r = 1;
        else
            r = 2;

    if (x > y) {
        r = 3;
    } else {
        if (x == y) {
            r = 4;
        } else if (x < y) {
            r = 5;
        } else {
            r = 6;
        }
    }

    if (r != 0) r = 0; else r = 1;
    return 0;
}
