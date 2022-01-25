#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define N 10

// Data structure to store adjacency list nodes of the graph
typedef struct Node{
    int dest;
    struct Node* next;
} TNode;


typedef struct Graph{
    TNode *head[N];
} TGraph;

// Data structure to store a graph edge
typedef struct Edge {
    int src, dest;
} TEdge;

// Function to create an adjacency list from specified edges
TGraph* stworz_Graph(TEdge edges[], int n, int wezly) {
    // allocate storage for the graph data structure
    TGraph* graph = (TGraph*)malloc(sizeof(TGraph));
 //   TNode* graph = malloc((size_t) wezly * sizeof(*TNode));

 //   (graph->head) = realloc((graph->head), (size_t) wezly * sizeof(int));
 //   (graph->(*head)) = malloc((size_t) wezly * sizeof(graph->(*head)));

    // initialize head pointer for all vertices
    for (int i = 0; i < wezly; i++) {
        graph->head[i] = NULL;
    }

    // add edges to the directed graph one by one
    for (int i = 0; i < n; i++) {
        // get the source and destination vertex
        int src = edges[i].src;
        int dest = edges[i].dest;

        // allocate a new node of adjacency list from src to dest
        TNode* newNode = (TNode*)malloc(sizeof(TNode));
        newNode->dest = dest;

        // point new node to the current head
        newNode->next = graph->head[src];

        // point head pointer to the new node
        graph->head[src] = newNode;
    }
    return graph;
}

void wczytywanie_dane(char **a, int *an, int *ilosc);

void wczytywanie_wyrazu(char *in, int inn, int *liczba, char **out);

bool czy_to_samo(char *a, char *b);

int amount_of_Node(struct Graph* graph, int index_start, int maks_wezel);

int main(void) {
    char *pobrane_dane = NULL;
    int dlugosc_dane;
    int ilosc_stralek;
    int ktory_nowy;
    int ktora_krawedz = 0;
    /* Tutaj dane sa wczytane bez '{' '}' ora zbednych spacji. */
    wczytywanie_dane(&pobrane_dane, &dlugosc_dane, &ilosc_stralek);

    for(int i = 0; i < dlugosc_dane; i++) {
        printf("%c", pobrane_dane[i]);
    }
        printf("\n");
printf("po wypisaniu\n");

    TEdge *krawedz = malloc((size_t) ilosc_stralek * sizeof(TEdge));

    char start[] = "start";
    int index_of_start;
    bool skad = true;
    char **wezly = malloc((size_t) (2 * ilosc_stralek) * sizeof(char*));
    for (int i = 0; i < ilosc_stralek; i++) wezly[i] = ".";
    int pozycja_w_danych = 0;
    while (pozycja_w_danych < dlugosc_dane - 1) {
        char *nowy_wyr;
        wczytywanie_wyrazu(pobrane_dane, dlugosc_dane, &pozycja_w_danych, &nowy_wyr);

        if (ktory_nowy == 0) { // pierwszy wyraz, to nie trzeba porownywac z poprzednimi
printf("pierwszy nowy\n");
            wezly[ktory_nowy] = nowy_wyr;
            krawedz[ktora_krawedz].src = ktory_nowy;
            skad = false;
            if (czy_to_samo(nowy_wyr, start)) index_of_start = ktory_nowy;
            ktory_nowy++;
        }
        else {
            int j = 0;
            while (j < ktory_nowy && !czy_to_samo(nowy_wyr, wezly[j])) j++;

            if (j == ktory_nowy) { // czyli wczytany wyraz jest rzeczywiscie nowy i trzeba wstawic do tablicy z wezlami
                wezly[ktory_nowy] = nowy_wyr;
   printf ("ktory wezel == %d\n", ktory_nowy);
  printf("maks il wezlow == %d\n", ilosc_stralek);
                if (czy_to_samo(nowy_wyr, start)) index_of_start = ktory_nowy;
                ktory_nowy++;
            }
            if (skad) {
                krawedz[ktora_krawedz].src = j;
  printf("ktora krawedz == %d, skad == %d\n",ktora_krawedz, j);
                skad = false;
            }
            else {
                krawedz[ktora_krawedz].dest = j;
  printf("ktroa krawedz == %d, dokad == %d\n",ktora_krawedz, j);
                ktora_krawedz++;
                skad = true;
            }
        }
    }

printf("start == %d\n", index_of_start);
for(int i =0; i< ilosc_stralek; i++) printf("%d ---> %d\n", krawedz[i].src, krawedz[i].dest);

    TGraph *graph = stworz_Graph(krawedz, ilosc_stralek, ktory_nowy);
    printf("%d\n", amount_of_Node(graph, index_of_start, ktory_nowy));

    free(pobrane_dane);
    free(krawedz);
    for (int i = 0; i < ktory_nowy; i++) {
        TNode *ptr = graph->head[i];
        TNode *pom;
        while (ptr) {
            pom = ptr;
            ptr = ptr->next;
            free(pom);
        }
    }
    free(graph);
    for (int i = 0; i < ilosc_stralek; i++) free(wezly[i]);
    free(wezly);

    return 0;
}

void wczytywanie_dane(char **a, int *an, int *ilosc) {
    char znak;
    char *dane = malloc(100 * sizeof(char));
   // int dlugosc = 1;
    int rozmiar = 0;
    bool spacja = false; //jesli jest spacja, to zapisuje tylko raz
    int i = 0;
    int liczba = 0;
//    printf("YYX\n");
    while ((znak = getchar()) != '{' );
    while ((znak = getchar()) != '}') {
   //     printf("111\n");
  //      printf("i = %d\n",i);
  //      printf("znak = %c\n", znak);
        if (i == rozmiar) {
            rozmiar += 100;
            dane = realloc(dane, (size_t) rozmiar * sizeof *dane);
        }
        if (isalpha(znak) != 0) {
        //if ((znak >= 'a' && znak <= 'z') || (znak >= 'A' && znak <= 'Z')) {
            dane[i] = znak;
            i++;
            spacja = true;
        }
        if ((znak == ' ' || znak == '\n') && spacja) {
            dane[i] = ' ';
            i++;
            spacja = false;
        }
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

void wczytywanie_wyrazu(char *in, int inn, int *liczba, char **out) {
    char *wyraz = malloc(20 * (sizeof(char)));
    int i = (*liczba);
    int j = 0;
    int rozmiar = 20;
    bool dane = true;
    while (isalpha(in[i]) == 0) i++;
    while (i < inn && in[i] != '}' && dane) {

            printf("pozycja i dane == %d\n", i);

        if (j == rozmiar) {
            rozmiar += 20;
            wyraz = realloc(wyraz, (size_t) rozmiar * sizeof (char));
        }
        //if ((znak >= 'a' && znak <= 'z') || (znak >= 'A' && znak <= 'Z')) {
        if (isalpha(in[i]) != 0) {
            wyraz[j] = in[i];
            j++;
            i++;
        }
        else {
            dane = false;
        }
    }
    wyraz[j] = '\0';
    *out = wyraz;
    *liczba = i;
}

bool czy_to_samo(char *a, char *b) {
    if (strlen(a) != strlen(b)) return false;
    else if (strcmp(a, b) != 0) return false;
    return true;
}

int amount_of_Node(struct Graph* graph, int index_start, int maks_wezel) {
    int *osiagalne = malloc ((size_t) maks_wezel * sizeof (int));
    for (int i = 0; i < maks_wezel; i++) osiagalne[i] = -1;
    TNode* ptr = graph->head[index_start];
    int j = 0;
    osiagalne[j] = index_start;
    j++;
    while (ptr != NULL) {
        if ((ptr->dest) != index_start) {
            osiagalne[j] = ptr->dest;
            j++;
        }
        ptr = ptr->next;
    }
    int skad = 1;
    int index;
    while (skad < maks_wezel && osiagalne[skad] != -1) {
        index = osiagalne[skad];
        ptr = graph->head[index];
        while (ptr != NULL) {
            bool czy_nowy = true;
            int dokad = ptr->dest;
            for (int k = 0; k < j && k < maks_wezel; k++) {
                if (dokad == osiagalne[k]) {
                    czy_nowy = false;
                    break;
                }
            }
            if (czy_nowy) {
                osiagalne[j] = dokad;
                j++;
            }
            ptr = ptr->next;
        }
        skad++;
    }
    free(osiagalne);
    return j;
}
