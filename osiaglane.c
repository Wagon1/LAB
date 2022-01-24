#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define N 6

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
//    graph->head = malloc(wezly * (sizeof)(graph->head));

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

void wczytywanie_wyrazu(char *in, int *liczba, char **out, int *outn);

bool czy_to_samo(char *a, char *b);

void list_of_edges(TEdge ***edge, int *dlugosc, int *index, char *dane, int dlugosc_dane, int maks_il_wezl);

int printGraph(struct Graph* graph, int i, int il_wezl) {
    int *osiagalne = malloc((size_t) il_wezl * sizeof(int));
    for (int i = 0; i < il_wezl; i++) osiagalne[i] = -1;
 /*   for(int b = 0; b < 6; b++) {
    printf("%d\n", osiagalne[b]);
    }

    printf("\n");*/
    int j = 0;
  //  print current vertex and all its neighbors
    TNode* ptr = graph->head[i];
    while (ptr != NULL)
    {
  //      printf("(%d —> %d)\t", i, ptr->dest);
        printf("\n");
        osiagalne[j] = ptr->dest;
        j++;
        ptr = ptr->next;
    }
    int k = 0;
    while (osiagalne[k] != -1 && k < 6) {
        i = osiagalne[k];
        ptr = graph->head[i];
        while (ptr != NULL) {
  //          printf("(%d —> %d)\t", i, ptr->dest);
  //          printf("\n");
            int dokad = ptr->dest;
            int l = 0, m = j;
            while (l < m && dokad != osiagalne[l]) {
                    //printf ("weszlo do while\n");
      //          printf ("l == %d, m == %d, dokad == %d, w tablicy == %d\n",l, m, dokad, osiagalne[l]);
       //         printf("\n");
                l++;

        /*        for(int b = 0; b < 6; b++) {
    printf("%d\n", osiagalne[b]);
    }

    printf("\n");*/

            }
            if (l == m) {//printf ("jest inny\n");
                osiagalne[j] = ptr->dest;
        //        printf ("wstawiam, a[j] == %d, j == %d\n",osiagalne[j], j);
      //          printf ("\n");
                j++;
            }
            ptr = ptr->next;
        }
        k++;
    }
   // printf("\n");

    printf("osiagalne = %d \n", j + 1);
    return (j + 1);

}

int main(void) {
    char *pobrane_dane = NULL;
    int dlugosc_dane;
    int maks_il_wezl;

    wczytywanie_dane(&pobrane_dane, &dlugosc_dane, &maks_il_wezl);

    TEdge *krawedz = malloc((size_t) maks_il_wezl * sizeof(TEdge));
    int miejsce = 0;

    char start[] = "start";
    int index_start;
    int pozycja = 0;
    int i = 0;
    bool skad = true;
    char **wezly = malloc((size_t) maks_il_wezl * sizeof(char*));
    int ilosc_wezlow;
    while (pozycja < dlugosc_dane) { //printf("while\n");
        char *nowy_wyr;
        int dlugosc;
        wczytywanie_wyrazu(pobrane_dane, &pozycja, &nowy_wyr, &dlugosc);


 /*       for(int i = 0; i < dlugosc; i++) {
        printf("%c", nowy_wyr[i]);
    }
        printf("\n");

printf("111\n");*/

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
    printf("%d\n", printGraph(graph, index_start, ilosc_wezlow));

    free(pobrane_dane);
    free(krawedz);
    for (int b = 0; b < ilosc_wezlow; b++) free(wezly[i]);
    free(wezly);

    return 0;
}

void wczytywanie_dane(char **a, int *an, int *ilosc) {
    char znak;
    char *dane = malloc(100 * sizeof(char));
    int dlugosc = 1;
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

void wczytywanie_wyrazu(char *in, int *liczba, char **out, int *outn) {
    char *wyraz = malloc(20 * (sizeof (char)));
    int i = (*liczba);
    int j = 0;
    int rozmiar = 0;
    bool dane = true;
    while (isalpha(in[i]) == 0) i++;
    while (in[i] != '}' && dane) {
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

void list_of_edges(TEdge ***edge, int *dlugosc, int *index, char *dane, int dlugosc_dane, int maks_il_wezl) {
    TEdge *krawedz = malloc((size_t) maks_il_wezl * sizeof(TEdge));
    int miejsce = 0;


    char start[] = "start";
    int index_start;
    int pozycja = 0;
    int i = 0;
    bool skad = true;
    char **wezly = malloc((size_t) maks_il_wezl * sizeof(char*));
    while (pozycja < dlugosc_dane) { //printf("while\n");
        char *nowy_wyr;
        int dlugosc;
        wczytywanie_wyrazu(dane, &pozycja, &nowy_wyr, &dlugosc);


 /*       for(int i = 0; i < dlugosc; i++) {
        printf("%c", nowy_wyr[i]);
    }
        printf("\n");

printf("111\n");*/
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


    **edge = krawedz;
    *dlugosc = miejsce;
    *index = index_start;
}


