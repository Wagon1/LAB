#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Data structure to store adjacency list nodes of the graph
typedef struct Node{
    int dest;
    struct Node* next;
} TNode;

// Data structure to store a graph object
typedef struct Graph{
    // An array of pointers to Node to represent an adjacency list
    TNode **head;
} TGraph;

// Data structure to store a graph edge
typedef struct Edge {
    int src, dest;
} TEdge;

static TGraph* stworz_Graph(TEdge edges[], int n, int wezly);

static void wczytywanie_dane(char **a, int *an, int *ilosc);

static char* wczytywanie_wyrazu(const char *in, int inn, int *liczba, int *dlugosc);

static bool czy_to_samo(const char *a, const char *b);

static int amount_of_Node(struct Graph* graph, int index_start, int maks_wezel);

int main(void) {
    char *pobrane_dane = NULL;
    int dlugosc_dane;
    int maks_il_wezl;
    wczytywanie_dane(&pobrane_dane, &dlugosc_dane, &maks_il_wezl);
    TEdge *krawedz = malloc((size_t) maks_il_wezl * sizeof(TEdge));
    int miejsce = 0;
    char start[] = "start";
    int index_start = -1;
    int pozycja = 0;
    int i = 0;
    bool skad = true;
    char **wezly = malloc((size_t) (maks_il_wezl * 2) * sizeof(char*));
    int ilosc_wezlow;
    while (pozycja < dlugosc_dane - 1) { 
        int dlugosc_wyr;
        char *nowy_wyr = wczytywanie_wyrazu(pobrane_dane, dlugosc_dane, &pozycja, &dlugosc_wyr);
        if (i == 0) { 
            wezly[i] = nowy_wyr;
            krawedz[miejsce].src = i;
            if (czy_to_samo(nowy_wyr, start)) index_start = i;
            i++;
            skad = false;
        }
        else { 
            int j = 0;
            while (j < i && !czy_to_samo(nowy_wyr, wezly[j])) {
                j++;
            }
            if (j == i) {
                wezly[i] = nowy_wyr;

                if (czy_to_samo(nowy_wyr, start)) index_start = i;
                i++;
            }
            else {
                free(nowy_wyr);
            }
            if (skad) {
                krawedz[miejsce].src = j;
                skad = false;
            }
            else {
                krawedz[miejsce].dest = j;
                miejsce++;
                skad = true;
            }
        }
    }
    ilosc_wezlow = i;
    TGraph *graph = stworz_Graph(krawedz, maks_il_wezl, ilosc_wezlow);
    printf("%d\n", amount_of_Node(graph, index_start, ilosc_wezlow));
    free(pobrane_dane);
    for (int b = 0; b < ilosc_wezlow; b++) {
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
    for (int b = 0; b < ilosc_wezlow; b++) free(wezly[b]);
    free(wezly);
    return 0;
}

static void wczytywanie_dane(char **a, int *an, int *ilosc) {
    char znak;
    char *dane = malloc(100 * sizeof(char));
    int rozmiar = 100;
    bool spacja = false; //jesli jest spacja, to zapisuje tylko raz
    int i = 0;
    int liczba = 0;
    while ((znak = (char)getchar()) != '{' );
    while ((znak = (char)getchar()) != '}') {
        if (i == rozmiar) {
            rozmiar += 100;
            dane = realloc(dane, (size_t) rozmiar * sizeof *dane);
        }
        if (isalpha(znak) != 0 || isdigit(znak) != 0) {
            dane[i] = znak;
            i++;
            spacja = true;
        }
        if ((znak == ' ' || znak == '\n' || znak == '	') && spacja) {
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

static char* wczytywanie_wyrazu(const char *in, int inn, int *liczba, int *dlugosc) {
    char *wyraz = malloc(20 * (sizeof (char)));
    int i = (*liczba);
    int j = 0;
    int rozmiar = 20;
    bool dane = true;
    while (isalpha(in[i]) == 0 && isdigit(in[i]) == 0) i++;
    while (i < inn && in[i] != '}' && dane) {
        if (j == rozmiar) {
            rozmiar += 20;
            wyraz = realloc(wyraz, (size_t) rozmiar * sizeof (char));
        }
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
    *dlugosc = j + 1;
    *liczba = i;
    return wyraz;
}

static bool czy_to_samo(const char *a, const char *b) {
    if (strlen(a) != strlen(b)) return false;
    else if (strcmp(a, b) != 0) return false;
    return true;
}

static int amount_of_Node(struct Graph* graph, int index_start, int maks_wezel) {
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

static TGraph* stworz_Graph(TEdge edges[], int n, int wezly) {
    // allocate storage for the graph data structure
    TGraph* graph = (TGraph*)malloc(sizeof(TGraph));
    (graph->head) = malloc((size_t) wezly * sizeof(TNode));
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
