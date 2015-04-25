#include <iostream>
#include <cassert>
#include <limits.h>
#include <stdlib.h>
#include <queue> 
#include <fstream>
#include <cstring>
#include <string>

#define WHITE 0
#define GREY 1
#define BLACK 2

#define VERTEX(u) G.vertici[u].v
#define COLOR(u) G.vertici[u].color
#define OFFSET(u) G.vertici[u].offset
#define IN_DEG(u) G.vertici[u].in_degree
#define P(u) G.vertici[u].p
#define START_VISIT(u) G.vertici[u].t_start
#define END_VISIT(u) G.vertici[u].t_finish
#define DIST(u) G.vertici[u].s_dist

#define EDGE(v) G.archi[v]

#define FOR_EACH_VERTEX for(int i = 0; i < V; i++)
#define FOR_EACH_EDGE for(int i = 0; i < E; i++)
#define FOR_EACH_ADJ(u)                         \
        for(int v = OFFSET(u); v < ( u == V-1 ? E -  OFFSET(u) : OFFSET(u+1) - OFFSET(u))  + OFFSET(u); v++) 

#define ALLOC(type, length) (type*) malloc(sizeof(type) * (length))
#define REALLOC(who, type, length) who = (type*) realloc(who, sizeof(type) * (length))


//dimensioni del grafo
int V, E;

using namespace std;

//struttura di un vertice
typedef struct vertex_struct {
    int v; //nome del vertice
    int in_degree; //grado di archi entranti
    int offset; //offset nell'array delle liste di adj
    int color; //colore del vertice durante la visita
    int p; //padre del vertice
    int s_dist; //distanza dalla sorgente
    int t_start; //tempo di inizio della visita del vertice
    int t_finish; //tempo di fine della visita del vertice
} vertex_t;

typedef struct graph_struct {
    vertex_t *vertici;
    int *archi;
} graph_t;

typedef graph_t tree_t;

int T = 0; //contatore del tempo di visita della DFS

graph_t G;

void DFS_visit(int u) {
    COLOR(u) = GREY;
    START_VISIT(u) = T;
    T++;
    
    FOR_EACH_ADJ(u) {
        if(COLOR(u) == WHITE) {
            P(u) = u;
            DFS_visit(v);
        }
    }
    COLOR(u) = BLACK;
    END_VISIT(u) = T;
    T++;
    return;
}

void DFS() {
    FOR_EACH_VERTEX {
        if(COLOR(i) == WHITE) {
            DFS_visit(i);
        }
    }
    return;
}





int main() {

    ifstream input;
    ofstream output;

    //apro il file di input e di output
    input.open("input.txt");
    assert(input.is_open());
    output.open("output.txt", ofstream::trunc);

    //leggo numero nodi e archi
    input >> V;
    input >> E;

    //alloco spazio per i vertici
    vertex_t *vertici = ALLOC(vertex_t, V);
    //alloco spazio per gli archi
    int *archi = ALLOC(int, E);

    G.vertici = vertici;
    G.archi = archi;

    //inizializzo i vertici
    FOR_EACH_VERTEX {
        VERTEX(i) = i;
        IN_DEG(i) = 0;
        OFFSET(i) = 0;
        COLOR(i) = WHITE;
        P(i) = -1;
        DIST(i) = 0;
        START_VISIT(i) = -1;
        END_VISIT(i) = -1;
    }
    //alloco spazio per la lista di liste di adj temporanea
    int **liste_adj = ALLOC(int*, V);

    int u, v;

    /*####################  leggo il grafo  #######################*/
    FOR_EACH_EDGE { //per ogni arco

        //leggo il nome dei nodi dell'arco
        input >> u;
        input >> v;

        //se non ho già la lista di adiacenza per u allora la creo
        if(OFFSET(u) == 0) {
            liste_adj[u] = ALLOC(int, 1);
        } else {    // altrimenti aumento il suo spazio
            REALLOC(liste_adj[u], int, OFFSET(u)+1);
        }

        liste_adj[u][OFFSET(u)] = v;   //metto il vertice di arrivo nella lista di adj di u
        OFFSET(u)++;                   //aumento il numero di archi uscenti da u
        IN_DEG(u)++;                //aumento l'in degree di v

    }

    int k = 0; //contatore degli archi
    int out_deg = 0;
    FOR_EACH_VERTEX { 
        out_deg = OFFSET(i); //salvo il numero di archi uscenti da i
        OFFSET(i) = k; //il nuovo offset di i è l'indice della sua lista di adj

        for(int j = 0; j < out_deg; j++) { //inserisco la lista di adj di i nella lista degli archi
            EDGE(k) = liste_adj[i][j];
            k++;
        }

    }


    DFS();


    FOR_EACH_VERTEX  {
        FOR_EACH_ADJ(i) {
            cout << VERTEX(i) << " " << EDGE(v) << "\n";
        }
    }

    return 0;
}