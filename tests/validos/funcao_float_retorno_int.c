int metade(float a) {
    if (a > 1.0)
        return 1;
    return 0;
}

int main() {
    int r;
    r = metade(2.5);
    printf("%d\n", r);
    return 0;
}