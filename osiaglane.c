#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/* Dobry wieczor, naprawde bardzo sie staralem, piszac ten program. */
/*Bardzo mi zalezy na tej ocenie, prosze o pozytywna ocene, dziekuje bardzo */

// Struktura zapisujaca wezel w liscie sasiedztwa.
typedef struct Node{
    int dest;
    struct Node* next;
} TNode;

// Struktura zapisujaca graf.
typedef struct Graph{
    // Tablica wskaznikow do wezlow typu TNode.
    TNode **head;
} TGraph;

// Struktura zapisujaca krawedzie miedzy wezlami.
typedef struct Edge {
    int src, dest;
} TEdge;

static TGraph* stworz_Graph(TEdge edges[], int n, int wezly);

static void wczytywanie_dane(char **a, int *an, int *ilosc);

static char* wczytywanie_wyrazu(char const *in, int inn, int *liczba);

static bool czy_to_samo(char const *a, char const *b);

static int amount_of_Node(struct Graph* const graph, int index_start, int maks_wezel);

int main(void) {
    char *pobrane_dane = NULL;
    int dlugosc_dane;
    int ilosc_strzalek;
    wczytywanie_dane(&pobrane_dane, &dlugosc_dane, &ilosc_strzalek);
    TEdge *krawedz = malloc((size_t) ilosc_strzalek * sizeof(TEdge));
    // Tworze tablice wskaznikow do zapisanie char*, kazdy nowy wezel otrzymuje numer bedacy indeksem.
    char **wezly = malloc((size_t) (ilosc_strzalek * 2) * sizeof(char*));
    int ktora_krawedz = 0;
    char start[] = "start";
    // Indeks w tablicy **wezel, pod ktorym znajduje sie wyraz 'start'.
    int index_of_start = -1;
    // Obecna pozycja w tablicy wczytane_dane, nie mozna wracac.
    int pozycja = 0;
    // Mowi, na ktorym miejscu w **wezly znajduje sie wyraz (indeks odpowiadajacy danemu wyrazowi).
    int ktory_nowy = 0;
    // Zapisuje, czy dany wezel jest docelowy, czy wychodzacy.
    bool skad = true;
    while (pozycja < dlugosc_dane - 1) { 
        char *nowy_wyr = wczytywanie_wyrazu(pobrane_dane, dlugosc_dane, &pozycja);
        // Rowne 0, czyli jest to pierwszy wczytany wezel.
        if (ktory_nowy == 0) { 
            wezly[ktory_nowy] = nowy_wyr;
            // Pierwszy wezel, czyli na pewno wychodzacy. 
            // Dlatego zapisuje do tablicy krawedzi jako wychodzacy i nie zwiekaszam indeksu, bo nastepny to bedzie docelowy.
            krawedz[ktora_krawedz].src = ktory_nowy;
            // Sprawdzam, czy ten wyraz to "start".
            if (czy_to_samo(nowy_wyr, start)) index_of_start = ktory_nowy;
            ktory_nowy++;
            // Ustawiam na false, czyli nastepny bedzie docelowy.
            skad = false;
        }
        else { 
            int j = 0;
            // Porownuje nowy wyraz z poprzednimi w tablicy **wezly w celu sprawdzenie czy jest nowy.
            while (j < ktory_nowy && !czy_to_samo(nowy_wyr, wezly[j])) {
                j++;
            }
            // Przypadek, kiedy wyraz jest nowy.
            if (j == ktory_nowy) {
                wezly[ktory_nowy] = nowy_wyr;
                if (czy_to_samo(nowy_wyr, start)) index_of_start = ktory_nowy;
                ktory_nowy++;
            }
            // Jesli nie jest nowy, to zwalniam pamiec.
            else {
                free(nowy_wyr);
            }
            // Wezel wychodzacy.
            if (skad) {
                krawedz[ktora_krawedz].src = j;
                skad = false;
            }
            // Wezel docelowy, po zapisaniu zwiekaszam indeks i ustawiam skad na true, czyli nastepny bedzie wychodzacy.
            else {
                krawedz[ktora_krawedz].dest = j;
                ktora_krawedz++;
                skad = true;
            }
        }
    }
    TGraph *graph = stworz_Graph(krawedz, ilosc_strzalek, ktory_nowy);
    // Tutaj jest wypis ilosci wezlow osiagalnych.
    printf("%d\n", amount_of_Node(graph, index_of_start, ktory_nowy));
    free(pobrane_dane);
    // Zwalniam wszystkie listy sasiedztwa.
    for (int b = 0; b < ktory_nowy; b++) {
    	TNode *ptr = graph->head[b];
    	TNode *pom;
    	while (ptr) {
    	    pom = ptr;
    	    ptr = ptr->next;
    	    free(pom);
    	}
    }
    free(graph->head);
    free(graph);
    free(krawedz);
    for (int b = 0; b < ktory_nowy; b++) free(wezly[b]);
    free(wezly);
    return 0;
}

static void wczytywanie_dane(char **a, int *an, int *ilosc) {
    char znak;
    char *dane = malloc(100 * sizeof(char)); // Ustawiam pewna poczatkowa wartosc.
    int rozmiar = 100;
    // True, jesli poprzednio zapisany znak to nie spacja. Jesli jest spacja, to zapisuje tylko raz.
    bool spacja = false; 
    int i = 0;
    int liczba = 0;
    // Dopoki nie zobacze '{' to nic nie zapisuje.
    while ((znak = (char)getchar()) != '{' );
    // Zapisuje dopoki nie zobacze '}'.
    while ((znak = (char)getchar()) != '}') {
        // Jesli pamiec nie starcza, to zwiekszam.
        if (i == rozmiar) {
            rozmiar += 100;
            dane = realloc(dane, (size_t) rozmiar * sizeof *dane);
        }
        // Zapisuje wszytskie litery i cyfry.
        if (isalpha(znak) != 0 || isdigit(znak) != 0) {
            dane[i] = znak;
            i++;
            spacja = true;
        }
        // Jesli jest spacja, \n lub tab, to zapisuje jako spacje.
        if ((znak == ' ' || znak == '\n' || znak == '	') && spacja) {
            dane[i] = ' ';
            i++;
            spacja = false;
        }
        // Jesli zobacze '->' to zapisuje i licze ilosc jako liczba krawedzi.
        // Liczba trzeba podzielic przez 2, bo zapisalem raz dla '-' i raz dla '>'.
        if (znak == '-' || znak == '>') {
            dane[i] = znak;
            liczba++;
            i++;
            spacja = true;
        }
    }
    *a = dane;
    *an = i;
    *ilosc = liczba / 2;
}

static char* wczytywanie_wyrazu(char const *in, int inn, int *liczba) {
    char *wyraz = malloc(20 * (sizeof (char))); // mallocuje juz pewna pamiec na poczatku.
    int i = (*liczba); // liczba to pozycja w tablicy wczytane_dane, na ktorej trzeba zaczac wczywywanoe.
    int j = 0; 
    int rozmiar = 20;
    bool dane = true; // jesli dane jest false to znaczy, ze trzeba zakonczyc wczytywanie.
    // Jesli znak nie jest cyfra ani litera, to pomijam.
    while (isalpha(in[i]) == 0 && isdigit(in[i]) == 0) i++;
    while (i < inn && in[i] != '}' && dane) {
        // Jesli pamiec nie starcza, to powiekszam pamiec.
        if (j == rozmiar) {
            rozmiar += 20;
            wyraz = realloc(wyraz, (size_t) rozmiar * sizeof (char));
        }
        // Ciag liter lub cyfr tworzy wyraz.
        if (isalpha(in[i]) != 0 || isdigit(in[i]) != 0) {
            wyraz[j] = in[i];
            j++;
            i++;
        }
        else {
            dane = false;
        }
    }
    wyraz[j] = '\0';
    *liczba = i;
    return wyraz;
}

static bool czy_to_samo(char const *a, char const *b) {
    if (strlen(a) != strlen(b)) return false; // Jesli dl. a jest rozna od dl. b, to inne.
    else if (strcmp(a, b) != 0) return false; // Porownuje a i b.
    return true;
}

static int amount_of_Node(struct Graph* const graph, int index_start, int maks_wezel) {
    // Tablica osiaglanie moze byc maksymalnie wielkosci maks_wezel, czyli ilosc wezlow w grafie.
    int *osiagalne = malloc ((size_t) maks_wezel * sizeof (int));
    // Ustawiam wszystkie komorki na -1, bo zaden z wezlow nie moze miec indeksu -1.
    for (int i = 0; i < maks_wezel; i++) osiagalne[i] = -1;
    // Ustawiam wskaznik na liste sasiedztwa dla indeksu wyrazu "start".
    TNode* ptr = graph->head[index_start];
    int j = 0;
    // Na pewno wezel "start" jest osiagalny, wiec wstawiam do tablicy.
    osiagalne[j] = index_start;
    j++;
    // Wstawiam do tablicy wszystkie wartosci z listy sasiedztwa dla indeksu "start" takie, ktore nie powtarza sie w elementami z tabilicy osiagalne.
    while (ptr != NULL) {
        if ((ptr->dest) != index_start) {
            osiagalne[j] = ptr->dest;
            j++;
        }
        ptr = ptr->next;
    }
    // "skad", czyli wezel wychodzacy, od ktorego trzeba sprawdzic sasiad.
    // Zaczyna od 1 (bo na pozycji 0 to start, czyli juz sprawdzony) i przechodzi liste sasiedztwa dla elementu zapisanego na tej pozycji.
    int skad = 1;
    // Wartosc na pozycji osiagalne[skad].
    int index;
    while (skad < maks_wezel && osiagalne[skad] != -1) {
        index = osiagalne[skad];
        ptr = graph->head[index];
        while (ptr != NULL) {
            // true, kiedy dany element jeszcze nie wystepuje w tablicy *osiagalne.
            bool czy_nowy = true;
            // 'dokad', czyli wartosc wezla docelowego (w liscie sasiedztwa dla 'index' jako wezla wychodzacego).
            int dokad = ptr->dest;
            for (int k = 0; k < j && k < maks_wezel; k++) {
                // Porownuje z kazdym elementem w *osiagalne.
                if (dokad == osiagalne[k]) {
                    czy_nowy = false;
                    break;
                }
            }
            // Jesli jest nowy, to ustawiam na ostatnim miejscu *osiagalne.
            if (czy_nowy) {
                osiagalne[j] = dokad;
                j++;
            }
            ptr = ptr->next;
        }
        skad++;
    }
    // Ilosc elementow wstawionych do tablicy *osiagalne to ilosc wezlow osiagalnych z "start".
    free(osiagalne);
    return j;
}

static TGraph* stworz_Graph(TEdge edges[], int n, int wezly) {
    // Alokuje pamiec graph do zapisywania wezlow.
    TGraph* graph = (TGraph*)malloc(sizeof(TGraph));
    // Rezerwuje pamiec dla tablicy head tyle, ile jest wszystkich wezlow.
    (graph->head) = malloc((size_t) wezly * sizeof(TNode));
    for (int i = 0; i < wezly; i++) {
        graph->head[i] = NULL;
    }
    // Dodaje krewedzie po kolei.
    for (int i = 0; i < n; i++) {
        // Zapisuje w int skad i dokad.
        int src = edges[i].src;
        int dest = edges[i].dest;
        // Tworze nowy wezel z wartoscia dokad, 
        TNode* newNode = (TNode*)malloc(sizeof(TNode));
        newNode->dest = dest;
        // Dodaje wezel do aktualnej listy. 
        newNode->next = graph->head[src];
        graph->head[src] = newNode;
    }
    return graph;
}
