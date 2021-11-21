#include <stdio.h>

#define MAX_WSP 10
#define MAX_M_WSP 1000000
char wsp[MAX_M_WSP];

void wczytywanie (char znak) {
    int i = 0;
    while((znak = getchar()) != '.') {// w char wsp nie ma spacji
        if(znak != ' ') {
            wsp[i] = znak;
            i++;
        }
    }
    wsp[i] = '.';
}

void plus_minus (char *A, int pami, int a) { //pami to wspolczynnik
    pami = 0;
   while (A[a] >= '0' && A[a] <= '9') {
        pami = pami * 10 + A[a] - '0';
        a++;
   }
}

void rzut_wielom (char *wsp, int *A) {
    int i = 0;
    int wspol = 0; //wspolczynnik
    int potega = 0; //potega jednomianu

    while(wsp[i] != '\n') { //przerzucic po 1 wielomianie do tablicy int
        if(wsp[i] != 'x'){
            if(wsp[i] >= '0' && wsp[i] <= '9') {
                plus_minus(wsp, wspol, i);
            }
            if(wsp[i] == '+'){
                i++;
                if(wsp[i] >= '0' && wsp[i] <= '9'){
                    plus_minus(wsp, wspol, i);
                }
                else {
                    wspol = 1;
                    i++;
                }
            }
            else if (wsp[i] == '-'){
                    i++;
                    if(wsp[i] >= '0' && wsp[i] <= '9'){
                        plus_minus(wsp, wspol, i);
                    }
                    else {
                        wspol = 1;
                        i++;
                    }
                    wspol = -1 * wspol;
            }
        }

        else if(wsp[i] == 'x') {  //szukanie stopnia wielomianu
            i++;
            if (wsp[i] == '^') {
                while(wsp[i] >= '0' && wsp[i] <= '9') {
                    potega = potega * 10 + wsp[i] - '0';
                    i++;
                }
            }
            else {
                potega = 1;
                i++;
            }
        }
        A[potega] = wspol;
        potega = 0;
        wspol = 0;
    }
}

int main(void) {
    char wyraz;
    int wynik[10] = {0};
    int i = 0;
    wczytywanie (wyraz);
    rzut_wielom(wsp, wynik);

    while(i < 10) {
        printf("%d\n", wynik[i]); //chce wyswietlic wspolczynnik przekazane z char wsp do int wynik
        i++;
    }

    return 0;
}
