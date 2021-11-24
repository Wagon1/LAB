#include <stdio.h>

#define MAX_WSP 11
#define MAX_M_WSP 1000
int wsp[MAX_M_WSP] = {'a'};
int licznik = 0;
int pozycja = 0;
int maks_potega = 0;

void wczytywanie (int znak) {
    int i = 0;
   // int j = 0;
    while((znak = getchar()) != '.') {// w char wsp nie ma spacji
        if(znak != ' ') {
            wsp[i] = znak;
            i++;
        }
    }
    wsp[i] = '.';
}

void plus_minus (int *A, int *pami, int *a) { //pami to wspolczynnik
    *pami = 0;
    while (A[*a] >= '0' && A[*a] <= '9') {
        *pami = (*pami) * 10 + A[*a] - '0';
        (*a)++;
    }
}

void rzut_wielom (int *A) {
    int i = pozycja;
    int wspol = 0; //wspolczynnik
    int potega_j = 0; //potega jednomianu
    licznik = 0;
    if(wsp[i] == 'x') wspol = 1;
    while(wsp[i] != '\n') { //przerzucic po 1 wielomianie do tablicy int
        if(wsp[i] != '.'){
        }
        if(wsp[i] != 'x'){
            if(wsp[i] >= '0' && wsp[i] <= '9') {
                plus_minus(wsp, &wspol, &i);
            }
            if(wsp[i] == '+'){
                i++;
                if(wsp[i] >= '0' && wsp[i] <= '9'){
                    plus_minus(wsp, &wspol, &i);
                }
                else if(wsp[i] == 'x') {
                    wspol = 1;
                    //i++;
                }
            }
            else if (wsp[i] == '-'){
                    i++;
                    if(wsp[i] >= '0' && wsp[i] <= '9'){
                        plus_minus(wsp, &wspol, &i);
                    }
                    else if(wsp[i] == 'x'){
                        wspol = 1;
                       // i++;
                    }
                    wspol = -1 * wspol;
            }
        }
        if(wsp[i] == 'x') {  //szukanie stopnia wielomianu
            i++;
            if (wsp[i] == '^') {
                i++;
                while(wsp[i] >= '0' && wsp[i] <= '9') {
                    potega_j = potega_j * 10 + wsp[i] - '0';
                    i++;
                }
            }
            else {
                potega_j = 1;
            }
        }
        if (potega_j > licznik) licznik = potega_j;
        A[potega_j] = wspol;
        potega_j = 0;
        wspol = 0;
    }
    i++;
    pozycja = i;
}

void dodawanie (int *A, int *B) {
    for (int i = 0; i < MAX_WSP; i++) {
            B[i] += A[i];
    }
}

void mnozenie  (int *A, int *B, int *C) {
    int pam = 0;
    int potega = maks_potega + 1;
    for(int i = 0; i < potega; i++) {
        pam = A[i];
        for(int j = 0; j <= potega; j++) {
            C[i+j] += pam * B[j];
        }
    }
}

void wypis(int *A) { //wynik
	int potega = maks_potega;
	printf("DD\n");
    while (A[potega] == 0) potega--;
    for (int i = potega; i > 0; i--) {printf("FF\n");
        if (A[i] < 0 && i == potega && i == 1) {printf("FF\n");
            if(A[i] == -1)
            printf ("-x");
            else
            printf ("%dx", A[i]);
            printf("FF\n");
        }
        else if (A[i] > 0 && i == potega && i == 1) {printf("FF\n");
                if(A[i] == 1)
                    printf ("x");
                else
                    printf ("%dx", A[i]);
              printf("FF\n");
        }
            else if (A[i] < 0 && i == potega) {printf("FF\n");
                    if(A[i] == -1)
                        printf ("-x^%d", i);
                    else
                        printf ("%dx^%d", A[i], i);
                    printf("FF\n");
            }
                else if (A[i] > 0 && i == potega) {printf("FF\n");
                        if(A[i] == 1)
                            printf ("x^%d", i);
                        else
                            printf ("%dx^%d", A[i], i);
                        printf("FF\n");
                }
                    else if (A[i] < 0 && i == 1) {printf("FF\n");
                            if(A[i] == -1)
                                printf (" - x");
                            else
                                printf (" - %dx", -A[i]);
                            printf("FF\n");
                    }
                        else if (A[i] > 0 && i == 1) {printf("FF\n");
                                if(A[i] == 1)
                                    printf (" + x");
                                else
                                    printf (" + %dx", A[i]);
                                printf("FF\n");
                        }
                            else if (A[i] < 0 ) {printf("FF\n");
                                    if(A[i] == -1)
                                        printf (" - x^%d", i);
                                    else
                                        printf (" - %dx^%d", -A[i], i);
                                   printf("FF\n");
                            }
                                else if (A[i] > 0) {printf("FF\n");
                                        if(A[i] == 1)
                                            printf (" + x^%d", i);
                                        else
                                            printf (" + %dx^%d", A[i], i);
                                        printf("FF\n");
                                }
    }
        if (A[0] > 0) {printf("FF\n");
            if(potega == 0) printf ("%d", A[0]);
            else printf (" + %d", A[0]);
        }
        else if (A[0] < 0) {printf("FF\n");
        	if(potega == 0) printf ("-%d", -A[0]);
        	else printf (" - %d", -A[0]);
        }
        int miar = 0;
        for(int i = 0; i < MAX_WSP; i++) {printf("FF\n");
            if(A[i] == 0) miar++;
        }
        if(miar == MAX_WSP) printf("0");
        printf("\n");
}

int main(void) {
    int wyraz = 'a';
    int dane[MAX_WSP] = {0};
    int robocza[MAX_M_WSP] = {0};
    int akumulator[MAX_M_WSP] = {0};

    wczytywanie (wyraz);
    if(wsp[pozycja] == '*'){
   	pozycja++;
       	rzut_wielom(dane);
       	printf("BB\n");
       	wypis(akumulator);
       	printf("CC\n");
       	for(int i = 0; i < MAX_WSP; i++) dane[i] = 0; //zerowanie
    }
    rzut_wielom(dane);
    if(licznik > maks_potega) maks_potega = licznik;
    dodawanie(dane, robocza);
    printf("BB\n");
    wypis(robocza);
    printf("CC\n");

    while(wsp[pozycja] != '\n') {
       if(wsp[pozycja] == '.') {
        return 0;
       }
       if(licznik > maks_potega) maks_potega = licznik;
        if(wsp[pozycja] != '*') {
            if(wsp[pozycja] == '+') {
                for(int i = 0; i < MAX_WSP; i++) dane[i] = 0; //zerowanie
                rzut_wielom(dane);
                if(licznik > maks_potega) maks_potega = licznik;
                dodawanie(dane, robocza);
                printf("BB\n");
                wypis(robocza);
                printf("CC\n");
            }
        }
        else if(wsp[pozycja] == '*') {
                pozycja++;
                for(int i = 0; i < MAX_WSP; i++) dane[i] = 0; //zerowanie

                rzut_wielom(dane);
                maks_potega = licznik +  maks_potega;
                mnozenie(dane, robocza, akumulator);
                printf("BB\n");
                wypis(akumulator);
                printf("CC\n");
                for(int i = 0; i <= maks_potega; i++) {
                    robocza[i] = 0;
                    robocza[i] = akumulator[i];
                    akumulator[i] = 0;
                }
            }
    }
}
