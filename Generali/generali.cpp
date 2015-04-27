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

#define VERTEX(u) G[u].v
#define COLOR(u) G[u].color
#define OUT_DEG(u) G[u].out_degree
#define IN_DEG(u) G[u].in_degree
#define P(u) G[u].p
#define START_VISIT(u) G[u].t_start
#define END_VISIT(u) G[u].t_finish
#define DIST(u) G[u].s_dist

#define ADJ(u, v) G[u].adj_list[v]
#define ADJ_INDEX(u,v) map[G[u].adj_list[v]]

#define FOR_EACH_VERTEX for(int i = 0; i < V; i++)
#define FOR_EACH_EDGE for(int i = 0; i < E; i++)
#define FOR_EACH_ADJ(u)                         \
        for(int v = 0; v < OUT_DEG(u); v++) 

#define ALLOC(type, length) (type*) malloc(sizeof(type) * (length))
#define REALLOC(who, type, length) who = (type*) realloc(who, sizeof(type) * (length))


//dimensioni del grafo
int V, E;

using namespace std;

//struttura di un vertice
typedef struct vertex_struct {
    int v;          //nome del vertice
    int in_degree;  //grado di archi entranti
    int out_degree; //grado di archi uscenti
    int color;      //colore del vertice durante la visita
    int p;          //padre del vertice
    int s_dist;     //distanza dalla sorgente
    int t_start;    //tempo di inizio della visita del vertice
    int t_finish;   //tempo di fine della visita del vertice
    int *adj_list;  //lista di adiacenza del vertice
} vertex_t;

typedef vertex_t* graph_t;

typedef graph_t tree_t;

int T = 0; //contatore del tempo di visita della DFS

graph_t G;
int *map;
int congiglieri = 0;

void print_tree() {
    cout << congiglieri << endl;
    FOR_EACH_VERTEX {
        if(P(i) == -1) {
            cout << VERTEX(i) << " ";

        }
        
    }
    cout << endl;
    FOR_EACH_VERTEX {
        if(P(i) != -1) {
            cout << VERTEX(P(i)) << " " << VERTEX(i) << endl;
        }
    }
}

int DFS_visit(int u) {
    COLOR(u) = GREY;
    START_VISIT(u) = T;
    T++;
    
    FOR_EACH_ADJ(u) {
        if(COLOR(ADJ_INDEX(u,v)) == WHITE) {
            P(ADJ_INDEX(u,v)) = u;
            int code = DFS_visit(ADJ_INDEX(u,v));
            if (code != -1 && code != u) {
                COLOR(u) = WHITE;
                //ADJ(u,v) = -1;
                P(ADJ_INDEX(u,v)) = -1;
                END_VISIT(u) = T;
                T++;
                return code;
            }
        } else if (COLOR(ADJ_INDEX(u,v)) == GREY) {
            COLOR(u) = WHITE;
            //ADJ(u,v) = -1;
            P(ADJ_INDEX(u,v)) = -1;
            END_VISIT(u) = T;
            T++;
            return ADJ_INDEX(u,v);
        }
    }
    COLOR(u) = BLACK;
    END_VISIT(u) = T;
    T++;
    return -1;
}

void DFS() {
    FOR_EACH_VERTEX {
        if(COLOR(i) == WHITE) {
            if(DFS_visit(i) == -1)
                congiglieri++;
        }
    }
    return;
}

int partition( vertex_t* v, int l, int r) {
   int pivot, i, j;
   vertex_t t;
   pivot = v[l].in_degree;
   i = l; j = r+1;
        
   while( 1)
   {
    do ++i; while( v[i].in_degree <= pivot && i <= r );
    do --j; while( v[j].in_degree > pivot );
    if( i >= j ) break;
    t = v[i]; v[i] = v[j]; v[j] = t;
   }
   t = v[l]; v[l] = v[j]; v[j] = t;
   return j;
}

void quickSort( vertex_t* v, int l, int r)
{
   int j;

   if( l < r ) 
   {
       j = partition( v, l, r);
       quickSort( v, l, j-1);
       quickSort( v, j+1, r);
   }
    
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
    graph_t g = ALLOC(vertex_t, V);

    G = g;

    //inizializzo i vertici
    FOR_EACH_VERTEX {
        VERTEX(i) = i;
        IN_DEG(i) = 0;
        OUT_DEG(i) = 0;
        COLOR(i) = WHITE;
        P(i) = -1;
        DIST(i) = 0;
        START_VISIT(i) = -1;
        END_VISIT(i) = -1;
    }
    

    int u, v;

    /*####################  leggo il grafo  #######################*/
    FOR_EACH_EDGE { //per ogni arco

        //leggo il nome dei nodi dell'arco
        input >> u;
        input >> v;

        //se non ho già la lista di adiacenza per u allora la creo
        if(OUT_DEG(u) == 0) {
            G[u].adj_list = ALLOC(int, 1);
        } else {    // altrimenti aumento il suo spazio
            REALLOC(G[u].adj_list, int, OUT_DEG(u)+1);
        }
        ADJ(u, OUT_DEG(u)) = v;     //metto il vertice di arrivo nella lista di adj di u
        OUT_DEG(u)++;               //aumento il numero di archi uscenti da u
        IN_DEG(v)++;                //aumento l'in degree di v

    }

    quickSort(G, 0, V-1);

    map = ALLOC(int, V);

    FOR_EACH_VERTEX {
        map[VERTEX(i)] = i;
    }

    DFS();


    FOR_EACH_VERTEX  {
        cout << "vertice " << VERTEX(i) << " in deg = " << IN_DEG(i) << endl << "\t";
        if(P(i) == -1) cout << "consigliere!! ";
        FOR_EACH_ADJ(i) {
            cout << ADJ(i,v) << " ";
        }
        cout << endl;
    }

    print_tree();

    return 0;
}