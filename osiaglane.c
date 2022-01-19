#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void wczytywanie_wyrazu(char **a, int *an);

int main(void) {
    char *a = NULL;
    int an;
    wczytywanie_wyrazu(&a, &an);
    printf("YYY\n");
    for(int i = 0; i < an; i++) {
        printf("%c", a[i]);
        //putchar(a[i]);
        printf("\n");
    }
    printf("ZZZ\n");
    return 0;
}

void wczytywanie_wyrazu(char **a, int *an) {
    char znak;
    int dlugosc = 1;
    int rozmiar = 0;
    char *wyraz = NULL;
    bool dane = true;
    int j = 0;
    while ((znak = getchar()) != '{' );
    printf("XXX\n");
    while (dane && (znak = getchar()) != '}') {
        printf("111\n");
        if (j == rozmiar) {
            rozmiar *= 2;
            wyraz = realloc(wyraz, (size_t) rozmiar * sizeof *wyraz);
        }
        if ((znak >= 'a' && znak <= 'z') || (znak >= 'A' && znak <= 'Z')) {
        //if (isalpha(znak) != 0) {
            wyraz[j] = znak;
            j++;
        }
        else {
            dane = false;
        }
        ;
    }
    wyraz[j] = '\0';
    *a = wyraz;
    *an = j + 1;
}


