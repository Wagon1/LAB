#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int wiecej(int n) {
    assert(n < INT_MAX / 3 * 2);
    return n * 3 / 2 + 1;
}

void przeniesienie_bi(int *a, int i) {
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
    while(a[i] < -1 || a[i] > 1) {
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

void dec2naf(int x, int **tab, int *a) { // ok
    int rozmiar = 0;
    int *naf = NULL;
    int czy_ujemna = 0;
    if (x < 0) {
        czy_ujemna = -1;
        x = -x;
    }
    int i = 0;
    while (x != 0) {
        if(x % 2 == 1){
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
    int *n_naf = NULL;
    int rozmiar = 0;
    int b = 0;
    
    for (int i = 0; i < xn; i++) {
        if (x[i] != 0) rozmiar++;
    }
    n_naf = malloc((size_t) rozmiar * sizeof *n_naf);
    for(int i = 0; i < xn; i++) {
    int c = x[i];
        if(c == 1) {
            n_naf[b] = i;
            b++;
        }
        else if(c == -1) {
            n_naf[b] = - i - 1;
            b++;
        }
    }
    *tab = n_naf;
    *a = rozmiar;
}

void bi2naf(int *x, int xn, int **tab, int *a) { // ok
    /* w x jest zapisana liczba w systemie binarnym zawierajace cyfry (-1, 0, 1),
    xn to dlugosc tablicy x, liczba x w systemie BBR-NAF zapisana jest w tab, a to dlugosc tabn */
    int *n_naf;
    int pamiec = xn + 1;
    /* dlugosc liczby w system BBR-NAF
    jest najwyzej o 1 dluzszy od dl. w systemie binarnym
    np. 11...1 (n jedynek) = 1000...0-1 (n-1 zer) */
    n_naf = malloc((size_t) pamiec * sizeof *n_naf);
    for(int i = 0; i < xn; i++) {
        n_naf[i] = x[i];
    }
    n_naf[xn] = 0;

    for(int j = 0; j < 5; j++) {
        for(int i = xn; i > 0; i--) {
            /* jesli mamy -11 to zamieniamy na 0-1 */
            int c = n_naf[i], d = n_naf[i-1];
            if(c == -1 && d == 1) {
                n_naf[i] = 0;
                n_naf[i-1] = -1;
            }
            if(c == 1 && d == -1) {
                n_naf[i] = 0;
                n_naf[i-1] = 1;
            }
            if(c == 1 && d == 1) {
                n_naf[i+1] = 1;
                n_naf[i] = 0;
                n_naf[i-1] = -1;
            }
            if(c == -1 && d == -1) {
                n_naf[i+1] = -1;
                n_naf[i] = 0;
                n_naf[i-1] = 1;
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

void bi_sub_1(int *a, int an, int **c, int *cn) { //a jest w bi
    int *roznica;
    int roznican;
    int *roznica_bbr;
    int roznica_bbrn;
    roznican = an + 1;
    roznica = malloc((size_t) roznican * sizeof *roznica);
    for(int i = 0; i < roznican; i++) {
        roznica[i] = 0;
    }
    roznica[0] = roznica[0] + a[0] - 1;
    przeniesienie_bi(roznica, 0);
    for(int i = 1; i < roznican - 1; i++) {
        roznica[i] += a[i];
        przeniesienie_bi(roznica, i);
    }
    bi2naf(roznica, roznican, &roznica_bbr, &roznica_bbrn);
    int i = roznica_bbrn - 1;
    while(roznica_bbr[i] == 0 && i > 1) {
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
    for(int i = 0; i < roznican; i++) {
        roznica[i] = 0;
    }

    if (an > bn) {
        for(int i = 0; i < bn; i++) {
            roznica[i] = roznica[i] + a[i] - b[i];
            przeniesienie_bi(roznica, i);
        }
        for(int i = bn; i < roznican - 1; i++) {
            roznica[i] = roznica[i] + a[i];
            przeniesienie_bi(roznica, i);
        }
    }
    else {
        for(int i = 0; i < an; i++) {
            roznica[i] = roznica[i] + b[i] - a[i];
            przeniesienie_bi(roznica, i);
        }
        for(int i = an; i < roznican - 1; i++) {
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
 //   napier2naf(a, an, &a, &an);
    suman = an + 1;
    suma = malloc((size_t) suman * sizeof *suma ); 
    for(int i = 0; i < suman; i++) {
        suma[i] = 0;
    }
    suma[0] = suma[0] + a[0] + 1;
    przeniesienie_bi(suma, 0);
    for(int i = 1; i < suman - 1; i++) {
        suma[i] += a[i];
        przeniesienie_bi(suma, i);
    }
    bi2naf(suma, suman, &suma_bbr, &suma_bbrn);
    int i = suma_bbrn - 1;
    while(suma_bbr[i] == 0 && i > 1) {
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
    if (n == 0) {
            wynik = 0;
    }
    else {
        napier2naf(a, n, &bi, &dlugosc_bi);
        if (dlugosc_bi == 2 && bi[0] == -1 && bi[1] == 31) {
            wynik = INT_MAX;
        }
        else if (dlugosc_bi == 1 && bi[0] == -32) {
            wynik = INT_MIN;
        }
        else {
            for (int i = dlugosc_bi - 1; i >= 0; i--) {
                if (wynik < INT_MIN / 2 - bi[i] / 2) return 0;
                else if (wynik > INT_MAX / 2 - bi[i] / 2) return 0; 
                else wynik = wynik * 2 + bi[i];
            }
        }
    }
    free(bi);
    return wynik;
}

void nadd(int *a, int an, int *b, int bn, int **c, int *cn) {
    int *suma;
    int suman;
    int *naf_a;
    int naf_an;
    int *naf_b;
    int naf_bn;
    int *suma_bbr;
    int suma_bbrn;
    int *suma_napier;
    int suma_napiern;
    napier2naf(a, an, &naf_a, &naf_an);
    napier2naf(b, bn, &naf_b, &naf_bn);
    if (naf_an > naf_bn) suman = naf_an + 1;
    else suman = naf_bn + 1;
    suma = malloc((size_t) suman * sizeof (int)); 
    for(int i = 0; i < suman; i++) {
        suma[i] = 0;
    }
    if (naf_an > naf_bn) {
        for(int i = 0; i < naf_bn; i++) {
            suma[i] = suma[i] + naf_a[i] + naf_b[i];
            przeniesienie_bi(suma, i);
        }
        for(int i = naf_bn; i < suman - 1; i++) {
            suma[i] += naf_a[i];
            przeniesienie_bi(suma, i);
        }
    }
    else {
        for(int i = 0; i < naf_an; i++) {
            suma[i] = suma[i] + naf_a[i] + naf_b[i];
            przeniesienie_bi(suma, i);
        }
        for(int i = naf_an; i < suman - 1; i++) {
            suma[i] += naf_b[i];
            przeniesienie_bi(suma, i);
        }
    }
    int i = suman - 1;
    while(suma[i] == 0) {
        i--;
    }
    int dlugosc = i + 1;
    bi2naf (suma, dlugosc, &suma_bbr, &suma_bbrn);
    naf2napier (suma_bbr, suma_bbrn, &suma_napier, &suma_napiern);
    *c = suma_napier;
    *cn = suma_napiern;
    free(naf_a);
    free(naf_b);
    free(suma);
    free(suma_bbr);
}

void nsub(int *a, int an, int *b, int bn, int **c, int *cn) {
    int *roznica;
    int roznican;
    int *naf_a;
    int naf_an;
    int *naf_b;
    int naf_bn;
    int *roznica_bbr;
    int roznica_bbrn;
    int *roznica_napier;
    int roznica_napiern;
    napier2naf(a, an, &naf_a, &naf_an);
    napier2naf(b, bn, &naf_b, &naf_bn);
    bi_sub(naf_a, naf_an, naf_b, naf_bn, &roznica, &roznican);
    bi2naf (roznica, roznican, &roznica_bbr, &roznica_bbrn);
    naf2napier (roznica_bbr, roznica_bbrn, &roznica_napier, &roznica_napiern);
    *c = roznica_napier;
    *cn = roznica_napiern;
    free(naf_a);
    free(naf_b);
    free(roznica);
    free(roznica_bbr);
}

void nmul(int *a, int an, int *b, int bn, int **c, int *cn) {
    int *iloczyn;
    int iloczynn;
    int *naf_a;
    int naf_an;
    int *naf_b;
    int naf_bn;
    int *iloczyn_bbr;
    int iloczyn_bbrn;
    int *iloczyn_napier;
    int iloczyn_napiern;
    if ( !a || !b) {
        iloczyn_napier = NULL;
        iloczyn_napiern = 0;
    }
    else {
	    napier2naf(a, an, &naf_a, &naf_an);
	    napier2naf(b, bn, &naf_b, &naf_bn);
	    iloczynn = naf_an + naf_bn;
	    iloczyn = malloc((size_t) iloczynn * sizeof *iloczyn);
	    /* zeruje tablice iloczyn */
	    for(int i = 0; i < iloczynn; i++) {
		iloczyn[i] = 0;
	    }
	    if(naf_an > naf_bn) {
		for(int i = 0; i < naf_bn; i++) {
		        int pamiec = naf_b[i];
		        int l = i;
		    for(int j = 0; j < naf_an; j++) {
		        iloczyn[l] += pamiec * naf_a[j];
		        przeniesienie_bi_petla(iloczyn, l);
		        l++;
		    }
		}
	    }
	    else {
		for(int i = 0; i < naf_an; i++) {
		        int pamiec = naf_a[i];
		        int l = i;
		    for(int j = 0; j < naf_bn; j++) {
		        iloczyn[l] += pamiec * naf_b[j];
		        przeniesienie_bi_petla(iloczyn, l);
		        l++;
		    }
		}
	    }
	    int i = iloczynn - 1;
	    while(iloczyn[i] == 0) {
		i--;
	    }
	    int dlugosc = i + 1;
	    bi2naf (iloczyn, dlugosc, &iloczyn_bbr, &iloczyn_bbrn);
	    naf2napier (iloczyn_bbr, iloczyn_bbrn, &iloczyn_napier, &iloczyn_napiern);
	    free(naf_a);
	    free(naf_b);
	    free(iloczyn);
	    free(iloczyn_bbr);
    }
    *c = iloczyn_napier;
    *cn = iloczyn_napiern;
    
    
}

void nexp(int *a, int an, int *b, int bn, int **c, int *cn) { //a to podstawa, b to wykladnik
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
    napier2naf(b, bn, &naf_b, &naf_bn); //wykladnik przerzut na bi
    potega_wynik = a;
    potega_wynikn = an;
    while(naf_bn > 1 || naf_b[0] != 1) {
        nmul(potega_wynik, potega_wynikn, a, an, &potega_pom, &potega_pomn);
        if(potega_wynik != a) {
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
    *c = pom;
    *cn = pomn;
    free(naf_b);
    free(naf_pom);
    free(potega_pom);
}

void ndivmod(int *a, int an, int *b, int bn, int **q, int *qn, int **r, int *rn) {
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

    if (naf_a[naf_an - 1] > 0) {
        if (naf_b[naf_bn - 1] > 0) {
            rodzaj = 1; // a>0, b>0
        }
        else {
            rodzaj = 2; // a>0, b<0
            for(int i = 0; i < naf_bn; i++) {
                naf_b[i] = -1 * naf_b[i];
            }
        }

    }
    else {
        for(int i = 0; i < naf_an; i++) {
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
        else if(naf_b[i] > naf_a[i]) {
            reszta = naf_a;
            resztan = naf_an;
        }
    }
    else {
        reszta = naf_a;
        resztan = naf_an;
    }
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

//int main(void) {

 /*   int a[2] = {0,-3};
    int b[2] = {-1,2};
    int *tab11;
    int tabn11;
    nexp(a,2,b,2,&tab11,&tabn11);
    for (int i = tabn11 - 1; i >= 0; i--) {
        printf("%d ", tab11[i]);
    }*/

  /*  int a[3] = {1,0,-1}; chyba ook
    int *tab10;
    int tabn10;
    bi_sub_1(a,3,&tab10,&tabn10);
        for (int i = tabn10 - 1; i >= 0; i--) {
            printf("%d ", tab10[i]);
        }
        printf("koniec\n");
    free(tab10);*/


 /*   int a[1] = {1};
    int an = 1;
    int *tab10;
    int tabn10;
    printf("CCC\n");
    while (an != 1 || a[0] != 0) {
            printf("AA\n");

            for (int i = an - 1; i >= 0; i--) {
            printf("%d ", a[i]);
        }
        printf("przed --1\n");

        bi_sub_1(a,1,&tab10,&tabn10);
    printf("BB\n");
        *a = *tab10;
        an = tabn10;
      //  free(tab10);
        for (int i = an - 1; i >= 0; i--) {
            printf("%d ", a[i]);
        }
        printf("koniec --1\n");
    }*/

  /*  int a[2] = {0,-3};
    int b[1] = {1};
    int *tab1;
    int tabn1;
    nmul(a,2,b,1,&tab1,&tabn1);
    for (int i = tabn1 - 1; i >= 0; i--) {
        printf("%d ", tab1[i]);
    }
    free(tab1);*/

 /*   int a[2] = {0,3};
    int b[2] = {-1, 3};
    int *tab2;
    int tabn2;
    nsub(a,2,b,2,&tab2,&tabn2);
    for (int i = tabn2 - 1; i >= 0; i--) {
        printf("%d ", tab2[i]);
    }
    free(tab2);*/

 /*   int a[4] = {1,0,0,1};  ook
    int *tab3;
    int tabn3;
    naf2napier(a,4,&tab3,&tabn3);
    for (int i = tabn3 - 1; i >= 0; i--) {
        printf("%d ", tab3[i]);
    }
    free(tab3);
    tab3 = NULL;*/

 /*   int a[1] = {0};
    int b[2] = {-1, 3};
    int *tab4;
    int tabn4;
    nadd(a,1,b,2,&tab4,&tabn4);
    for (int i = tabn4 - 1; i >= 0; i--) {
        printf("%d ", tab4[i]);
    }
    free(tab4);*/

  //  int a[2] = {1,3};   ook
  /*  int *a = NULL;
    int *tab5;
    int tabn5;
    napier2naf(a,0,&tab5,&tabn5);
    for (int i = tabn5 - 1; i >= 0; i--) {
        printf("%d ", tab5[i]);
    }
    free(tab5);*/

  /*  int a[4] = {0,1,0,1};  ook
    int *tab6;
    int tabn6;
    bi2naf(a,4,&tab6,&tabn6);
    for (int i = tabn6 - 1; i >= 0; i--) {
        printf("%d ", tab6[i]);
    }
    free(tab6);*/

 /*   int licz = 10;
    int *tab7;
    int tabn7;
    iton(licz, &tab7, &tabn7);
    printf("dlugosc = %d\n", tabn7);
    printf("i wylicznona = %d\n",ntoi(tab7, tabn7));
    for(int i = 0; i < tabn7; i++) {
        printf("%d ", tab7[i]);
    }
    free(tab7);*/


 /*   int wynik = 0;   ook
    int tab8[2] = {1, 3};
    int n = 2;
    wynik = (ntoi(tab8, n));
    printf("wynik to = %d", wynik); */


 /*   int *tab9;   ook
    int a;
    int x;
    scanf("%d", &x);
    dec2naf(x, &tab9, &a);
    for (int i = a - 1; i >= 0; i--) {
        printf("%d", tab9[i]);
    }
    printf("\n");
    free(tab9);*/

/*    int a[2] = {-1,3};
    int b[1] = {2};
    int *tab2;
    int tabn2;
    int *reszta;
    int resztan;
    ndivmod(a,2,b,1,&tab2,&tabn2, &reszta, &resztan);
    for (int i = tabn2 - 1; i >= 0; i--) {
        printf("%d ", tab2[i]);
    }
    printf("\n");
    for (int i = resztan - 1; i >= 0; i--) {
        printf("%d ", reszta[i]);
    }
    free(tab2);
    free(reszta);*/


 //   return 0;
//}
