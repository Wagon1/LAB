#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define ILOSC_OPERACJI 6

/* wszystkie zmienne _naf to reprezentacja w systemie BBR NAF */
/* analogicznie z innymi  */
int wiecej(int n) {
    assert(n < INT_MAX / 3 * 2);
    return n * 3 / 2 + 1;
}

void przeniesienie_bi(int *a, int i) {
    /* przenosze w liczby w systemie bin w tablicach, np. 2->10 */
    if (a[i] < -1) {
                a[i+1] -= 1;
                a[i] += 2;
            }
    if (a[i] > 1) {
                a[i+1] += 1;
                a[i] -= 2;
            }
}

void przeniesienie_bi_petla(int *a, int i) {
    while (a[i] < -1 || a[i] > 1) {
        if (a[i] < -1) {
                    a[i+1] -= 1;
                    a[i] += 2;
                }
        if (a[i] > 1) {
                    a[i+1] += 1;
                    a[i] -= 2;
                }
    }
}

void dec2naf(int x, int **tab, int *a) { 
    int rozmiar = 0;
    int *naf = NULL;
    int czy_ujemna = 0;
    if (x < 0) {
        czy_ujemna = -1;
        x = -x;
    }
    int i = 0;
    /* kotrzystam z algorytmu zamiany liczb dziesietnych na BBR NAF */
    while (x != 0) {
        if (x % 2 == 1){
            if (i >= rozmiar) {
                rozmiar = wiecej(rozmiar);
                naf = realloc(naf, (size_t) rozmiar * sizeof *naf);
            }
            naf[i] = 2 - (x % 4);
            x = x - naf[i];
        }
        else {
            if (i >= rozmiar) {
                rozmiar = wiecej(rozmiar);
                naf = realloc(naf, (size_t) rozmiar * sizeof *naf);
            }
            naf[i] = 0;
        }
        x = x/2;
        i++;
    }
    if (czy_ujemna == -1) {
        for (int j = 0; j < i; j++) {
            naf[j] = -1 * naf[j];
        }
    }
    *tab = naf;
    *a = i;
    naf = NULL;
}

void naf2napier(int *x, int xn, int **tab, int *a) {
    /* funkcja zamieniajaca sysyem BBR NAF do NApiera */
    int *n_naf = NULL;
    int rozmiar = 0;
    int b = 0;
    
    for (int i = 0; i < xn; i++) {
        if (x[i] != 0) rozmiar++;
    }
    n_naf = malloc((size_t) rozmiar * sizeof *n_naf);
    for (int i = 0; i < xn; i++) {
    int c = x[i];
        if (c == 1) {
            n_naf[b] = i;
            b++;
        }
        else if (c == -1) {
            n_naf[b] = - i - 1;
            b++;
        }
    }
    *tab = n_naf;
    *a = rozmiar;
}

void bi2naf(int *x, int xn, int **tab, int *a) {
    /* w x jest zapisana liczba w systemie binarnym zawierajace cyfry (-1, 0, 1),
    xn to dlugosc tablicy x, liczba x w systemie BBR-NAF zapisana jest w tab, a to dlugosc tabn */
    int *n_naf;
    int pamiec = xn + 1;
    /* dlugosc liczby w system BBR-NAF
    jest najwyzej o 1 dluzszy od dl. w systemie binarnym
    np. 11...1 (n jedynek) = 1000...0-1 (n-1 zer) */
    n_naf = malloc((size_t) pamiec * sizeof *n_naf);
    for (int i = 0; i < xn; i++) {
        n_naf[i] = x[i];
    }
    n_naf[xn] = 0;
    for (int j = 0; j < ILOSC_OPERACJI; j++) {
        for (int i = xn; i > 0; i--) {
            /* jesli mamy -11 to zamieniamy na 0-1, 1-1 -> 01, 11 -> 10-1, -1-1 -> -101 */
            int c = n_naf[i], d = n_naf[i-1];
            if (c == -1 && d == 1) {
                n_naf[i] = 0;
                n_naf[i-1] = -1;
            }
            if (c == 1 && d == -1) {
                n_naf[i] = 0;
                n_naf[i - 1] = 1;
            }
            if (c == 1 && d == 1) {
                n_naf[i + 1] = 1;
                n_naf[i] = 0;
                n_naf[i - 1] = -1;
            }
            if (c == -1 && d == -1) {
                n_naf[i + 1] = -1;
                n_naf[i] = 0;
                n_naf[i - 1] = 1;
            }
        }
    }
    int j = xn;
    while(n_naf[j] == 0) {
        j--;
    }
    j++;
    n_naf = realloc(n_naf, (size_t) j * sizeof(int));
    *tab = n_naf;
    *a = j;
}

void napier2naf(int *a, int n, int **tab, int *tabn) {
    int *bi;
    int dlugosc_bi;
    int pozycja;
    /* jesli jest to liczba 0  */
    if (a == NULL && n == 0) { 
        bi = malloc(sizeof(int));
        bi[0] = 0;
        dlugosc_bi = 1;
    }
    else {
        if (a[n-1] >=0 ) dlugosc_bi = a[n-1] + 1;
        else dlugosc_bi = -a[n-1]; // wtedy a[n-1] < 0, oraz liczba jest ujemna
        bi = malloc((size_t) dlugosc_bi * sizeof (int));
        /* zeruje tablice bi */
        for (int i = 0; i < dlugosc_bi; i++) {
            bi[i] = 0;
        }
        for (int i = 0; i < n; i++) {
            if (a[i] >= 0) {
                pozycja = a[i];
                bi[pozycja] = 1;
            }
            else {
                pozycja = -a[i] - 1;
                bi[pozycja] = -1;
            }
        }
    }
    *tab = bi;
    *tabn = dlugosc_bi;
}

void bi_sub_1(int *a, int an, int **c, int *cn) { // a jest w bi
    int *roznica;
    int roznican;
    int *roznica_bbr;
    int roznica_bbrn;
    roznican = an + 1;
    roznica = malloc((size_t) roznican * sizeof *roznica);
    for (int i = 0; i < roznican; i++) {
        roznica[i] = 0;
    }
    roznica[0] = roznica[0] + a[0] - 1;
    przeniesienie_bi(roznica, 0);
    for (int i = 1; i < roznican - 1; i++) {
        roznica[i] += a[i];
        przeniesienie_bi(roznica, i);
    }
    bi2naf(roznica, roznican, &roznica_bbr, &roznica_bbrn);
    int i = roznica_bbrn - 1;
    while (roznica_bbr[i] == 0 && i > 1) {
        i--;
    }
    *c = roznica_bbr;
    *cn = i + 1;
    free(roznica);
}

void bi_sub(int *a, int an, int *b, int bn, int **c, int *cn) {
    int *roznica;
    int roznican;
    if (an > bn) roznican = an + 1;
    else roznican = bn + 1;
    roznica = malloc((size_t) roznican * sizeof *roznica);
    /* zeruje tablice roznica[] */
    for (int i = 0; i < roznican; i++) {
        roznica[i] = 0;
    }

    if (an > bn) {
        for (int i = 0; i < bn; i++) {
            roznica[i] = roznica[i] + a[i] - b[i];
            przeniesienie_bi(roznica, i);
        }
        for (int i = bn; i < roznican - 1; i++) {
            roznica[i] = roznica[i] + a[i];
            przeniesienie_bi(roznica, i);
        }
    }
    else {
        for (int i = 0; i < an; i++) {
            roznica[i] = roznica[i] + b[i] - a[i];
            przeniesienie_bi(roznica, i);
        }
        for (int i = an; i < roznican - 1; i++) {
            roznica[i] = roznica[i] + b[i];
            przeniesienie_bi(roznica, i);
        }
        for (int x = 0; x < roznican; x++) {
            roznica[x] *= -1;
        }
    }
    int i = roznican - 1;
    while(roznica[i] == 0) {
        i--;
    }
    *c = roznica;
    *cn = i + 1;
}

void bi_add_1(int *a, int an, int **c, int *cn) { //a jest w bi
    int *suma;
    int suman;
    int *suma_bbr;
    int suma_bbrn;
    suman = an + 1;
    suma = malloc((size_t) suman * sizeof *suma ); 
    for (int i = 0; i < suman; i++) {
        suma[i] = 0;
    }
    suma[0] = suma[0] + a[0] + 1;
    przeniesienie_bi(suma, 0);
    for (int i = 1; i < suman - 1; i++) {
        suma[i] += a[i];
        przeniesienie_bi(suma, i);
    }
    bi2naf(suma, suman, &suma_bbr, &suma_bbrn);
    int i = suma_bbrn - 1;
    while (suma_bbr[i] == 0 && i > 1) {
        i--;
    }
    *c = suma_bbr;
    *cn = i + 1;
    free(suma);
    suma = NULL;
}

void iton(int x, int **a, int *n) {
    /* w bbr bedzie zapisana liczba w postaci BBR-NAF*/
    int *bbr;
    /* w n_naf jest zapisana liczba w postaci Napier-NAF*/
    int *n_naf;
    /* dlugosc liczby x w postaci Napier-NAF */
    int dlugosc_BBR;
    int b;
    if (x == INT_MAX) {
        n_naf = malloc(2 * sizeof(int));
        n_naf[0] = -1;
        n_naf[1] = 31;
        b = 2;
    }
    else if (x == INT_MIN) {
        n_naf = malloc(sizeof(int));
        n_naf[0] = -32;
        b = 1;
    }
    else {
        dec2naf(x, &bbr, &dlugosc_BBR);
        /* w x jest bbr*/  
        naf2napier(bbr, dlugosc_BBR, &n_naf, &b);
        free(bbr);
    }

    *a = n_naf;
    *n = b;
    /* w dec2naf() x w kazdym obrocie petli zmniejsza sie o polowe
    zlozonosc iton() bedzie nlog(n) */
    
    bbr = NULL;
}

int ntoi(int *a, int n) {
    int *bi;
    int dlugosc_bi;
    int wynik = 0;
    /* wydzielam poszczegolne przypadki: 0, intmax, intmin */
    if (n == 0) {
        wynik = 0;
    }
    else if (a[n-1] > 31 || a[n-1] < -32) {
            return 0;
    }
    else if (n == 2 && a[0] == -1 && a[1] == 31) {
            return INT_MAX;
    }
    else if (n == 1 && a[0] == -32) {
            return INT_MIN;
    }
    else {
    /* korzystam z tradycyjnego sposobu zamiany z bin na dziesietny, */
    /* czyli mnozyc przez 2 i dodac reszte */
        napier2naf(a, n, &bi, &dlugosc_bi);
        if (a[n-1] == 31) {
            for (int i = dlugosc_bi - 2; i >= 0; i--) {
                wynik = wynik * 2 + bi[i];
            }
            free(bi);
            if (wynik < -1) {
            	wynik = wynik + INT_MAX;
            } 
            else {
            	return 0;
            }
        }
        else if (a[n-1] == -32) {
            for (int i = dlugosc_bi - 2; i >= 0; i--) {
                wynik = wynik * 2 + bi[i];
            }
            free(bi);
            if (wynik > 0) {
            	wynik = wynik + INT_MAX;
            } 
            else {
            	return 0;
            }
        }
        else {
            for (int i = dlugosc_bi - 1; i >= 0; i--) {
                wynik = wynik * 2 + bi[i];
            }
            free(bi);
        }
    }
    return wynik;
}

void usunzera(int **a, int *n){
    int licznik = 0;
    for(int i = 0; i < *n; i++){
        if((*a)[i] != 0){
            licznik++;
        }
    } 
    int *new;
    new = malloc((size_t) licznik * sizeof(int));
    licznik = 0;
    for(int i = 0; i < *n; i++){
        if((*a)[i] != 0){
            new[licznik] = (*a)[i];
            licznik++;
        }
    }
    free(*a);
    *a = new;
    *n = licznik;
}

void skroctablice(int **a, int *n){
    int licznik = 0;
    for(int i = 0; i < *n; i++){
        if((*a)[i] != 0){
            licznik++;
        }
    } 
    int *new;
    new = malloc((size_t) licznik * sizeof(int));
    licznik = 0;
    for(int i = 0; i < *n; i++){
        if((*a)[i] != 0){
            if((*a)[i] < 0){
                new[licznik] = (*a)[i];
            }else{
                new[licznik] = (*a)[i] - 1;
            }
            licznik++;
        }
    }
    free(*a);
    *a = new;
    *n = licznik;
}

void mergetablice(int *a, int an, int *b, int bn, int **c, int *cn){
    int len = an + bn;
    int *new;
    new = malloc((size_t) len * sizeof(int));
    int acounter = 0;
    int bcounter = 0;
    for(int i = 0; i < len; i++){
        if(acounter < an){
            //mamy cos w a
            if(bcounter < bn){
                //mamy cos w a i b
                if(a[acounter] >= 0){
                    if(b[bcounter] >= 0){
                        //oba z +
                        if(a[acounter] == b[bcounter]){
                            new[i] = 0;
                            new[i+1] = a[acounter] + 2;
                            acounter++;
                            bcounter++;
                            i++;
                        }else if(a[acounter] > b[bcounter]){
                            new[i] = b[bcounter] + 1;
                            bcounter++;
                        }else{
                            new[i] = a[acounter] + 1;
                            acounter++;
                        }
                    }else{
                        //a + b -
                        if(a[acounter] + 1 ==    b[bcounter] * -1){
                            new[i] = 0;
                            new[i+1] = 0;
                            acounter++;
                            bcounter++;
                            i++;
                        }else if(a[acounter] + 1 > b[bcounter] * -1){
                            new[i] = b[bcounter];
                            bcounter++;
                        }else{
                            new[i] = a[acounter] + 1;
                            acounter++;
                        }
                    }
                }else{
                    if(b[bcounter] >= 0){
                        //a- b+
                        if(b[bcounter] + 1 == a[acounter] * -1){
                            new[i] = 0;
                            new[i+1] = 0;
                            acounter++;
                            bcounter++;
                            i++;
                        }else if(b[bcounter] + 1 > a[acounter] * -1){
                            new[i] = a[acounter];
                            acounter++;
                        }else{
                            new[i] = b[bcounter] + 1;
                            bcounter++;
                        }
                    }else{
                        //a- b-
                        if(a[acounter] == b[bcounter]){
                            new[i] = 0;
                            new[i+1] = a[acounter] - 1;
                            acounter++;
                            bcounter++;
                            i++;
                        }else if(a[acounter] > b[bcounter]){
                            new[i] = a[acounter];
                            acounter++;
                        }else{
                            new[i] = b[bcounter];
                            bcounter++;
                        }
                    }
                }
            }else{
                //nie ma nic w b bierzemy z a
                if(a[acounter] >= 0){
                    new[i] = a[acounter] + 1;
                }else{
                    new[i] = a[acounter];
                }
                acounter++;
            }
        }else{
            //nie ma nic w a bierzemy z b
            if(b[bcounter] >= 0){
                new[i] = b[bcounter] + 1;
            }else{
                new[i] = b[bcounter];
            }
            bcounter++;
        }
    }
    usunzera(&new, &len);
    *c = new;
    *cn = len;
}

void nadd(int *a, int an, int *b, int bn, int **c, int *cn){
    int *new;
    int len;
    mergetablice(a, an, b, bn, &new, &len);
    int licznik = 0;
    while(licznik < len - 1){
        if(abs(new[licznik]) == abs(new[licznik+1])){
            if(new[licznik] < 0 && new[licznik+1] < 0){
                new[licznik] = 0;
                new[licznik+1] = new[licznik+1] - 1;
            }else if(new[licznik] > 0 && new[licznik + 1] > 0){
                new[licznik] = 0;
                new[licznik+1] = new[licznik+1] + 1;
            }else{
                new[licznik] = 0;
                new[licznik+1] = 0;
            }
        }else if(abs(new[licznik]) == abs(new[licznik+1]) - 1){
            if(new[licznik] < 0){
                if(new[licznik+1] < 0){
                    new[licznik] = new[licznik] * -1;
                    new[licznik + 1] = new[licznik + 1] - 1;
                }else{
                    new[licznik] = new[licznik] * -1;
                    new[licznik + 1] = 0;
                }
            }else{
                if(new[licznik+1] < 0){
                    new[licznik] = new[licznik] * -1;
                    new[licznik + 1] = 0;
                }else{
                    new[licznik] = new[licznik] * -1;
                    new[licznik + 1] = new[licznik + 1] + 1;
                }
            }
        }       
        licznik++;

    }
    skroctablice(&new, &len);
    *c = new;
    *cn = len;
}

void nsub(int *a, int an, int *b, int bn, int **c, int *cn){
    for(int i = 0; i < bn; i++){
        b[i] = (b[i] + 1) * -1;
    }
    nadd(a, an, b, bn, c, cn);
}

void napier_div_2(int *a, int an, int **b, int *bn) {

    int *wynikd;
    if(a[0] == 0){
        wynikd = malloc((size_t) (an - 1) * sizeof(int));
        for (int i = 0; i < an - 1; i++){
            if (a[i+1] < 0) {
                wynikd[i] = a[i+1] + 1;
            }
            else {
                wynikd[i] = a[i+1] - 1;
            }
        }
        *b = wynikd;
        *bn = an - 1;
    }
    else if(a[0] == -1){
        wynikd = malloc((size_t) (an - 1) * sizeof(int));
        for(int i = 0; i < an - 1; i++){
            if(a[i+1] < 0){
                wynikd[i] = a[i+1] + 1;
            }
            else{
                wynikd[i] = a[i+1] - 1;
            }
        }
        int *pomoc;
        int pomn;
        int jeden[] = {0};
        nsub(wynikd, an-1, jeden, 1, &pomoc, &pomn);
        free(wynikd);
        *b = pomoc;
        *bn = pomn;
    }
    else{
        wynikd = malloc((size_t) an * sizeof(int));
        for(int i = 0; i < an; i++){
            if(a[i] < 0) {
                wynikd[i] = a[i] + 1;
            }
            else{
                wynikd[i] = a[i] - 1;
            }
        }
        *b = wynikd;
        *bn = an;
    }

}

void napier_mul_2(int *a, int an, int **b, int *bn) {
    int *wynikd;
    wynikd = malloc((size_t) an * sizeof(int));
    for (int i = 0; i < an; i++) {
        if(a[i] < 0){
            wynikd[i] = a[i] - 1;
        }
        else {
            wynikd[i] = a[i] + 1;
        }
    }
    *b = wynikd;
    *bn = an;
}

void nmul(int *a, int an, int *b, int bn, int **c, int *cn) {
    int *x;
    int *y;
    int xn;
    int yn;
    int *wynik = NULL;
    int w = 0;
    int *pom;
    int pomn;
    int ujemna;
    if(a[an-1] < 0){
        if(b[bn-1] < 0){
            ujemna = 1;
            for(int i = 0; i < bn; i++){
                b[i] = (b[i] + 1) * -1;
            }
            for(int i = 0; i < an; i++){
                a[i] = (a[i] + 1) * -1;
            }
        }else{
            ujemna = -1;
            for(int i = 0; i < an; i++){
                a[i] = (a[i] + 1) * -1;
            }
        }
    }
    else {
        if (b[bn-1] < 0){
            ujemna = -1;
            for (int i = 0; i < bn; i++){
                b[i] = (b[i] + 1) * -1;
            }
        }
        else {
            ujemna = 1;
        }
    }
    x = a;
    y = b;
    xn = an;
    yn = bn;
    while(yn != 0){
        if(y[0] == 0 || y[0] == -1){ // nieparzysta

            nadd(x, xn, wynik, w, &pom, &pomn);

            if (wynik != NULL) free(wynik);
            wynik = pom;
            w = pomn;


        }
       // free(pom);
        napier_mul_2(x, xn, &pom, &pomn);

        if (x != a) free(x);
        x = pom;
        xn = pomn;
      // free(pom);
        napier_div_2(y, yn, &pom, &pomn);


        if (y != b) free(y);
        y = pom;
        yn = pomn;



    }
    if (ujemna == -1){
        for (int i = 0; i < w; i++){
                wynik[i] = (wynik[i] + 1) * -1;
            }
    }
    free(pom);
    *c = wynik;
    *cn = w;
}

void nexp(int *a, int an, int *b, int bn, int **c, int *cn) { //a to podstawa, b to wykladnik
    /* potegowanie to b krotne mnozenie a */
    int *naf_b;
    int naf_bn;
    int *naf_pom;
    int naf_pomn;
    int *potega_pom;
    int potega_pomn;
    int *potega_wynik;
    int potega_wynikn;
    int *pom;
    int pomn;
    /* przypadek b = 0, czyli calosc jest rowne 1 */
    if (bn == 0) {
        pom = malloc(sizeof(int));
        pom[0] = 0;
        pomn = 1;
    }
    /* b = 1, czyli wynik rowne a*/
    else if (bn == 1 && b[0] == 0) {
        pom = a;
        pomn = an;
    }
    /* a = 0, czyli wynik rowne 0 */
    else if (an == 0) {
        pom = NULL;
        pomn = 0;
    }
    else {
        napier2naf(b, bn, &naf_b, &naf_bn); //wykladnik przerzut na bi
        potega_wynik = a;
        potega_wynikn = an;
        while (naf_bn > 1 || naf_b[0] != 1) {
            nmul(potega_wynik, potega_wynikn, a, an, &potega_pom, &potega_pomn);
            if (potega_wynik != a) {
            	free(potega_wynik);
            }
            potega_wynik = potega_pom;
            potega_wynikn = potega_pomn;
            potega_pom = NULL;
        
            bi_sub_1(naf_b, naf_bn, &naf_pom, &naf_pomn);
            free(naf_b);
            naf_b = naf_pom;
            naf_bn = naf_pomn;
            naf_pom = NULL;
        }
        pom = potega_wynik;
        pomn = potega_wynikn;
        free(naf_b);
        free(naf_pom);
        free(potega_pom);
    }
    *c = pom;
    *cn = pomn;
}

void ndivmod(int *a, int an, int *b, int bn, int **q, int *qn, int **r, int *rn) {
    /* algorytm euklidesa */
    int *iloraz;
    int ilorazn;
    int *naf_a;
    int naf_an;
    int *naf_b = NULL;
    int naf_bn;
    int *iloraz_pom = NULL;
    int ilorazn_pom;
    int *reszta = NULL;
    int resztan = 0;
    int *reszta_bbr = NULL;
    int resztan_bbr;
    int *iloraz_bbr = NULL;
    int ilorazn_bbr;
    int *reszta_napier = NULL;
    int resztan_napier;
    int *iloraz_napier = NULL;
    int ilorazn_napier;
    int rodzaj = 0;
    napier2naf(a, an, &naf_a, &naf_an); 
    napier2naf(b, bn, &naf_b, &naf_bn); 
    iloraz = malloc(sizeof(int));
    ilorazn = 1;
    iloraz[0] = 0;
    /* w tym miejscu zamienia a i b na dodatnie, i zapamietam rodzaj dzielenia */
    if (naf_a[naf_an - 1] > 0) {
        if (naf_b[naf_bn - 1] > 0) {
            rodzaj = 1; // a>0, b>0
        }
        else {
            rodzaj = 2; // a>0, b<0
            for (int i = 0; i < naf_bn; i++) {
                naf_b[i] = -1 * naf_b[i];
            }
        }
    }
    else {
        for (int i = 0; i < naf_an; i++) {
                naf_a[i] = -1 * naf_a[i];
        }
        if (naf_b[naf_bn - 1] > 0) {
            rodzaj = 3; // a<0, b>0
        }
        else {
            rodzaj = 4; // a<0, b<0
            for(int i = 0; i < naf_bn; i++) {
                naf_b[i] = -1 * naf_b[i];
            }
        }
    }
    while (naf_an > naf_bn) { //a>b
        // naf_a, czyli dzialna stale zmniejszam o naf_b
        bi_sub (naf_a, naf_an, naf_b, naf_bn, &iloraz_pom, &ilorazn_pom);
        naf_a = iloraz_pom;
        naf_an = ilorazn_pom;
        iloraz_pom = NULL;
        bi_add_1(iloraz, ilorazn, &iloraz_pom, &ilorazn_pom);
        iloraz = iloraz_pom;
        ilorazn = ilorazn_pom;
        iloraz_pom = NULL;
    }
    if (naf_an == naf_bn) {
        int i = naf_an -1;
        while (i >= 0 && naf_a[i] == naf_b[i]) {
            i--;
        }
        printf("i= %d\n", i);
        if (i == -1) {
            bi_add_1(iloraz, ilorazn, &iloraz_pom, &ilorazn_pom);
            iloraz = iloraz_pom;
            ilorazn = ilorazn_pom;
            iloraz_pom = NULL;
            reszta = malloc (sizeof(int));
            resztan = 1;
            reszta[0] = 0;
        }
        else if (naf_a[i] > naf_b[i]) {
            bi_add_1(iloraz, ilorazn, &iloraz_pom, &ilorazn_pom);
            iloraz = iloraz_pom;
            ilorazn = ilorazn_pom;
            iloraz_pom = NULL;
            bi_sub (naf_a, naf_an, naf_b, naf_bn, &iloraz_pom, &ilorazn_pom);
            reszta = iloraz_pom;
            resztan = ilorazn_pom;
            iloraz_pom = NULL;
        }
        else if (naf_b[i] > naf_a[i]) {
            reszta = naf_a;
            resztan = naf_an;
        }
    }
    else {
        reszta = naf_a;
        resztan = naf_an;
    }
    /* jesli a i b dodatnie, to nie zmieniam nic */
    /* jesli a dod, b uj, to iloraz jest przzeciwny, reszta bez zmian */ 
    if (rodzaj == 2) {
        for (int i = 0; i < ilorazn; i++) {
            iloraz[i] = -1 * iloraz[i];
        }
    }
    else if (rodzaj == 3 || rodzaj == 4) {
        bi_add_1(iloraz, ilorazn, &iloraz_pom, &ilorazn_pom);
        iloraz = iloraz_pom;
        ilorazn = ilorazn_pom;
        iloraz_pom = NULL;
        bi_sub(naf_b, naf_bn, reszta, resztan, &iloraz_pom, &ilorazn_pom);
        reszta = iloraz_pom;
        resztan = ilorazn_pom;
        /* a uj, b dod, iloraz przeciwny, reszta to dpelnienie do dzilnika*/
        if (rodzaj == 3) {
            for (int i = 0; i < ilorazn; i++) {
            iloraz[i] = -1 * iloraz[i];
            }
        }
    }
    bi2naf(iloraz, ilorazn, &iloraz_bbr, &ilorazn_bbr);
    bi2naf(reszta, resztan, &reszta_bbr, &resztan_bbr);
    naf2napier(iloraz_bbr, ilorazn_bbr, &iloraz_napier, &ilorazn_napier);
    naf2napier(reszta_bbr, resztan_bbr, &reszta_napier, &resztan_napier);
    *q = iloraz_napier;
    *qn = ilorazn_napier;
    *r = reszta_napier;
    *rn = resztan_napier;
    free(iloraz);
    free(reszta);
    free(naf_a);
    free(naf_b);
    free(iloraz_pom);
    free(reszta_bbr);
    free(iloraz_bbr); 
}


