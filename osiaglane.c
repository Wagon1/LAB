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

// Data structure to store a graph object
typedef struct Graph{
    // An array of pointers to Node to represent an adjacency list
    TNode* head[N];
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

 //   (graph->head) = realloc((graph->head), (size_t) wezly * (sizeof)(int));
 //   (graph->(*head)) = malloc((size_t) wezly * (sizeof)(graph->(*head)));

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

void wczytywanie_wyrazu(char *in, int inn, int *liczba, char **out, int *outn);

bool czy_to_samo(char *a, char *b);

int amount_of_Node(struct Graph* graph, int index_start, int maks_wezel);

void destroy(TNode *l);

int main(void) {
    char *pobrane_dane = NULL;
    int dlugosc_dane;
    int maks_il_wezl;

    wczytywanie_dane(&pobrane_dane, &dlugosc_dane, &maks_il_wezl);

    printf("dlugosc dane == %d\n", dlugosc_dane);

    TEdge *krawedz = malloc((size_t) maks_il_wezl * sizeof(TEdge));
    int miejsce = 0;

    char start[] = "start";
    int index_start;
    int pozycja = 0;
    int i = 0;
    bool skad = true;
    char **wezly = malloc((size_t) (maks_il_wezl + 1) * sizeof(char*));
    int ilosc_wezlow;

    printf("dlugosc dane == %d\n", dlugosc_dane);
    while (pozycja < dlugosc_dane - 1) { printf("pozycja == %d\n", pozycja);
        char *nowy_wyr;
        int dlugosc;
        printf("TESTTT\n");
        wczytywanie_wyrazu(pobrane_dane, dlugosc_dane, &pozycja, &nowy_wyr, &dlugosc);


        for(int i = 0; i < dlugosc; i++) {
        printf("%c", nowy_wyr[i]);
    }
        printf("\n");

printf("po wypisaniu\n");

        if (i == 0) { //printf("222\n");
            wezly[i] = nowy_wyr;
            krawedz[miejsce].src = i;
            if (czy_to_samo(nowy_wyr, start)) index_start = i;
            i++;
            skad = false;
        }
        else { //printf("333\n");
            int j = 0;
            while (j < i && !czy_to_samo(nowy_wyr, wezly[j])) {
                j++;
            }
            if (j == i) {//printf("nowy\n");
                wezly[i] = nowy_wyr;

                printf ("ktory wezel == %d\n", i);
                printf("maks il wezlow == %d\n", maks_il_wezl + 1);

                if (czy_to_samo(nowy_wyr, start)) index_start = i;
                i++;
            }
            if (skad) {//printf("skad\n");
                krawedz[miejsce].src = j;
                skad = false;
           //     printf("j = %d\n", j);
            }
            else {//printf("dokad\n");
                krawedz[miejsce].dest = j;
                miejsce++;
                skad = true;
             //   printf("j = %d\n", j);
            }
        }
    }
    ilosc_wezlow = i;
 /*   for(int i = 0; i<miejsce; i++) {
        printf(" %d --> %d\n", krawedz[i].src, krawedz[i].dest);
    }

    printf("start == %d\n", index_start);*/

    TGraph *graph = stworz_Graph(krawedz, maks_il_wezl, ilosc_wezlow);

    // Function to print adjacency list representation of a graph
    printf("%d\n", amount_of_Node(graph, index_start, ilosc_wezlow));

    free(pobrane_dane);
    for (int b = 0; b < ilosc_wezlow; b++) destroy(graph->head[b]);
    free(graph);
    free(krawedz);
    for (int b = 0; b <= maks_il_wezl; b++) free(wezly[b]);
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

void wczytywanie_wyrazu(char *in, int inn, int *liczba, char **out, int *outn) {
    char *wyraz = malloc(20 * (sizeof (char)));
    int i = (*liczba);
    int j = 0;
    int rozmiar = 0;
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
    *outn = j + 1;
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

void destroy(TNode *l) {
    TNode *pom;
    while (l->next) {
        pom = l->next;
        l->next = pom->next;
        free(pom);
    }
}
