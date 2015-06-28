#include <iostream>
#include <cassert>
#include <limits.h>
#include <stdlib.h>
#include <stack> 
#include <set>
#include <queue>
#include <vector> 
#include <fstream>
#include <cstring>
#include <string>

#define WHITE 0
#define GREY 1
#define BLACK 2
#define RED 3

#define OK -1

#define VERTEX(u) G[u].v
#define COLOR(u) G[u].color
#define OUT_DEG(u) G[u].out_degree
#define IN_DEG(u) G[u].in_degree
#define P(u) G[u].p
#define COUNTER(u) G[u].counter
#define LOWLINK(u) G[u].lowlink
#define START_VISIT(u) G[u].t_start
#define END_VISIT(u) G[u].t_finish
#define DIST(u) G[u].s_dist
#define ONSTACK(u) G[u].on_stack

#define ADJ(u, v) G[u].adj_list[v]
#define ADJ_INDEX(u,v) map[G[u].adj_list[v]]

#define INC(u, v) G[u].inc_list[v]

#define FOR_EACH_VERTEX for(int i = 0; i < V; i++)
#define FOR_EACH_EDGE for(int i = 0; i < E; i++)
#define FOR_EACH_ADJ(u)                         \
        for(int v = 0; v < OUT_DEG(u); v++) 
#define FOR_EACH_INC(u) for(int k = 0; k < IN_DEG(u); k++)

#define ALLOC(type, length) (type*) malloc(sizeof(type) * (length))
#define REALLOC(who, type, length) who = (type*) realloc(who, sizeof(type) * (length))

#define DEBUG 


//dimensioni del grafo
int V, E;

using namespace std;

ifstream input;
ofstream output;

//struttura di un vertice
typedef struct vertex_struct {
    int v;          //nome del vertice
    int in_degree;  //grado di archi entranti
    int out_degree; //grado di archi uscenti
    int color;      //colore del vertice durante la visita
    int p;          //padre del vertice
    int counter;    //contatore ci serve per identificare componenti connesse
    int lowlink;    //contatore ci serve per identificare componenti connesse
    int on_stack;   //identifica se il vertice è nello stack
    int s_dist;     //distanza dalla sorgente
    int t_start;    //tempo di inizio della visita del vertice
    int t_finish;   //tempo di fine della visita del vertice
    int *adj_list;  //lista di adiacenza del vertice
    int *inc_list;  //lista di incidenza del vertice
} vertex_t;

typedef struct vertex_lite_struct {
    int *adj_list;  //lista di adiacenza del vertice
    int compagno;           // mio compagno nel matching
    int dist;               // profondità
    int out_deg;
} vertex_lite_t;

typedef vertex_t* graph_t;

typedef vertex_lite_t* graph_lite_t;

typedef graph_t tree_t;

int T = 0; //contatore del tempo di visita della DFS

graph_t G;
graph_lite_t bip;
int *CCS;
int N_CCS;
int *map;
int consiglieri = 0;
int counter = 0;
stack<int> S;
int found_CCS = 0;

graph_lite_t create_bipartition();
int maximum_match();

void print_tree() {
    #ifdef DEBUG
    cout << consiglieri << endl;
    #endif
    output << consiglieri << endl;
    FOR_EACH_VERTEX {
        if(P(i) == -1) {
            #ifdef DEBUG
            cout << VERTEX(i) << " ";
            #endif
            output << VERTEX(i) << " ";

        }
        
    }
    #ifdef DEBUG
    cout << endl;
    #endif
    output << endl;
    FOR_EACH_VERTEX {
        if(P(i) != -1) {
            #ifdef DEBUG
            cout << P(i)<< " " << VERTEX(i) << endl;
            #endif
            output << P(i)<< " " << VERTEX(i) << endl;
        }
        free(G[i].adj_list);
        free(G[i].inc_list);
    }
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int DFS_visit(int u) {
    COLOR(u) = GREY; //coloro il nodo a grigio
    START_VISIT(u) = T; //setto il tempo di inizio visita
    COUNTER(u) = counter;
    LOWLINK(u) = counter;
    S.push(u);
    ONSTACK(u) = 1;
    counter++;
    #ifdef DEBUG
    cout << "T:" << T << " --\tSTART VISIT OF " << VERTEX(u) << " counter(u): " << COUNTER(u) << " lowlink(u): " << LOWLINK(u) << endl;
    #endif
    T++;
    //scorro la lista di adiacenza per visitare tutti gli archi che partono dal noodo
    FOR_EACH_ADJ(u) {
        int w = ADJ_INDEX(u, v);
        #ifdef DEBUG
        cout << "\t\tvisit edge from " << VERTEX(u) << " to " << VERTEX(w) << endl;
        cout << "\t\tBefore visit\tu = " << VERTEX(u) << "\tw = " << VERTEX(w) << endl;
        cout << "\t\tlowlink(w): " << LOWLINK(w) << " counter(w): " << COUNTER(w) << endl;
        cout << "\t\tlowlink(u): " << LOWLINK(u) << " counter(u): " << COUNTER(u) << endl << endl;
        cout << "\t\tcolor(w) is ";
        #endif
        //se trovo un nodo bianco lo setto come mio sottoposto e lo visito
        if(COLOR(w) == WHITE) {
            
            P(w) = VERTEX(u);
            #ifdef DEBUG
            cout << "WHITE" << endl;
            cout << "\t\t\tset " << VERTEX(u) << " as father of " << VERTEX(w) << endl;
            #endif
            DFS_visit(w);
            LOWLINK(u) = min(LOWLINK(u), LOWLINK(w));
            
        
        //se trovo un nodo grigio allora ho trovato un ciclo
        } else if(ONSTACK(w) == 1) {

            LOWLINK(u) = min(LOWLINK(u), COUNTER(w));

        } else if(COLOR(w) == BLACK && P(w) == -1) {
            
            consiglieri--;
            P(w) = u;
        }

        #ifdef DEBUG
        cout << "\t\tAfter visit\tu = " << VERTEX(u) << "\tw = " << VERTEX(w) << endl;
        cout << "\t\tlowlink(w): " << LOWLINK(w) << " counter(w): " << COUNTER(w) << endl;
        cout << "\t\tlowlink(u): " << LOWLINK(u) << " counter(u): " << COUNTER(u) << endl << endl;
        #endif



    }

    int padre;
    N_CCS=0;
    //controllo le componenti connesse
    if(LOWLINK(u) == COUNTER(u)) {

        int w, z;


        #ifdef DEBUG
        cout << "\t\tFound cycle starting in " << VERTEX(u) << endl;
        #endif
        do{

            w = S.top();
            S.pop();
            if(N_CCS == 0)
                CCS = ALLOC(int, 1);
            else
                REALLOC(CCS, int, N_CCS+1);


            COLOR(w) = RED;

            ONSTACK(w) = 0;
            #ifdef DEBUG
            cout << "\t\tindex of u: " << u << " index of w: " << w << endl;
            cout << "\t\t" << VERTEX(w) << " is out stack";
            #endif

            if(N_CCS == 0)
                padre = P(w);
            if(P(w) != -1)
                consiglieri++;
            P(w) = -1;
            LOWLINK(w) = LOWLINK(u);
            #ifdef DEBUG
            cout << " and consigliere" << endl;
            cout << endl;
            #endif
            if(IN_DEG(w) == 1 && w != u) {
                P(w) = -1;
            } else {
                CCS[N_CCS]=w;
                N_CCS++;
            }
            

        } while(w != u);

        if(N_CCS == 1) {
            P(u) = padre;
            COLOR(u) = BLACK;
            if(P(u) != -1)
                consiglieri--;
            #ifdef DEBUG
            cout << "\t\t" << VERTEX(u) << " was not a cycle " << endl;
            if(P(u) != -1) 
                cout << "\t\this father was " << VERTEX(padre) << " " << padre << endl;
            #endif
        } else {
            if(found_CCS == 0) {
                found_CCS = 1;
                bip = ALLOC(vertex_lite_t, V+1); //alloco il grafo bipartito
            }
            create_bipartition();
            maximum_match();
            
            free(CCS);
        }
        

    }




    //visita terminata
    if(COLOR(u) != RED)
        COLOR(u) = BLACK;


    END_VISIT(u) = T;
    #ifdef DEBUG
    cout << "\t\tset " << VERTEX(u) << " to BLACK" << endl;
    cout << "T:" << T << " --\tEND VISIT OF " << VERTEX(u) << endl;
    #endif
    T++;
    
    return -1;
}

bool BFS_max_match(){
    #ifdef DEBUG
    cout << "\t\t\tstart BFS" << endl;
    #endif
    queue<int> q;
    for(int i; i < N_CCS; i++) {
        int u = CCS[i];
        if(bip[u].compagno == V) {
            bip[u].dist = 0;
            q.push(u);
            #ifdef DEBUG
            cout << "\t\t\tenqueue " << u << "dist = 0" << endl;
            #endif
        } else {
            bip[u].dist = INT_MAX;
            #ifdef DEBUG
            cout << "\t\t\tdist of " << u << " is int max" << endl;
            #endif
        }
        
    }
    bip[V].dist = INT_MAX;
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        #ifdef DEBUG
        cout << "\t\t\tdequeue " << u << endl;
        cout << "\t\t\t\this dist is " << bip[u].dist << endl;
        cout << "\t\t\t\tdist of NIL is " << bip[V].dist << endl;
        #endif

        if(bip[u].dist < bip[V].dist) {

            for(int i = 0; i < bip[u].out_deg; i++) {
                int v = bip[u].adj_list[i];
                #ifdef DEBUG
                cout << "\t\t\t\tdist of " << bip[v].compagno << " is " << bip[bip[v].compagno].dist << endl;
                #endif
                if(bip[bip[v].compagno].dist == INT_MAX) {
                    bip[bip[v].compagno].dist = bip[u].dist + 1;
                    q.push(bip[v].compagno);
                    #ifdef DEBUG
                    cout << "\t\t\t\tdist of " << bip[v].compagno << " is now " << bip[bip[v].compagno].dist << " and enqueued" << endl;
                    #endif
                }
            }
        }
    }
    return bip[V].dist != INT_MAX;
}
bool DFS_max_match(int u){
    #ifdef DEBUG
    cout << "\t\t\tstart DFS on " << u << endl;
    #endif
    if(u != V) {
        for(int i = 0; i < bip[u].out_deg; i++) {
            int v = bip[u].adj_list[i];
            #ifdef DEBUG
            cout << "\t\t\t\tdist of " << bip[v].compagno << " is " << bip[bip[v].compagno].dist << endl;
            #endif
            if(bip[bip[v].compagno].dist == bip[u].dist + 1) {
                if(DFS_max_match(bip[v].compagno)) {
                    #ifdef DEBUG
                    cout << "\t\t\t\tfound a match between " << u << " and " << v << endl;
                    #endif
                    bip[u].compagno = v;
                    bip[v].compagno = u;
                    return true;
                }
            }

        }
        #ifdef DEBUG
        cout << "\t\t\t\tNOT found a match for " << u << endl;
        #endif
        bip[u].dist = INT_MAX;
        return false;
    }
    return true;
    
}

int maximum_match() {

    #ifdef DEBUG
    cout << "\t\t------------ calculating maximum match------------" << endl;
    #endif

    int matching = 0;
    while(BFS_max_match()) {
        #ifdef DEBUG
        cout << "\t\tBFS returned true" << endl;
        #endif
        for(int i = 0; i < N_CCS; i++) {
            int u = CCS[i];
            #ifdef DEBUG
            cout << "\t\tevalueting " << u << " which is in CCS" << endl;
            #endif
            if(bip[u].compagno == V) {
                if(DFS_max_match(u)) {
                    matching++;
                    #ifdef DEBUG
                    cout << "\t\tFound a match!!" << endl;
                    #endif
                }
            }
        }
    }
    for(int i = 0; i < N_CCS; i++) {
        if(bip[CCS[i]].compagno != V) {
            P(CCS[i]) = bip[CCS[i]].compagno;
            consiglieri--;
            #ifdef DEBUG
            cout << "\t\t " << bip[CCS[i]].compagno << " is father of " << CCS[i] << endl;
            #endif
        } 
        else {
            P(CCS[i]) = -1;
        }
    }
    return matching;
}

//crea un grafo bipartito con L = CCS e R = possibili padri degli elementi della CCS
graph_lite_t create_bipartition() {

    #ifdef DEBUG
    cout << "\t\t------------ creating bipartition------------" << endl;
    #endif
    
    int w, u;  
    //inizializzo il nuovo grafo
    for(int i = 0; i < V+1; i++) {
        bip[i].dist = INT_MAX;
        bip[i].compagno = V;
    }

    //scorro tutti gli elementi della CCS
    for(int i = 0; i < N_CCS; i++) {
        u = CCS[i];
        
        //scorro tutti gli elementi della lista di incidenza di u (tutti i suoi possibili padri)
        for(int j = 0; j < IN_DEG(u); j++) {
            int found = 0;
            w = INC(u, j);
            #ifdef DEBUG
            cout << "\t\t\t--- " << w << " is in INC of " << u << endl;
            #endif

            //se non fa parte allora posso aggiungerlo all'insieme R
            if(LOWLINK(u) != LOWLINK(w)) {

                //inserisco w come vicino di u (possibile padre)
                if(bip[u].out_deg == 0) {
                    bip[u].adj_list = ALLOC(int, 1);
                } else {    // altrimenti aumento il suo spazio
                    REALLOC(bip[u].adj_list, int, bip[u].out_deg +1);
                }
                
                bip[u].adj_list[bip[u].out_deg]= w;
                bip[u].out_deg++;

                #ifdef DEBUG
                cout << "\t\t\t--- " << w << " is a possible father for " << u << endl;
                #endif
                
            }
        }
    }

    return bip;
}

void DFS() {
    FOR_EACH_VERTEX {
        if(COLOR(i) == WHITE) {
            consiglieri++;
            DFS_visit(i);
        }
    }
    return;
}



int main() {

    

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
        COUNTER(i) = -1;
        LOWLINK(i) = -1;
        DIST(i) = 0;
        ONSTACK(i) = 0;
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

        //se non ho già la lista di incidenza per v allora la creo
        if(IN_DEG(v) == 0){
            G[v].inc_list = ALLOC(int, 1);
        } else {    // altrimenti aumento il suo spazio
            REALLOC(G[v].inc_list, int, IN_DEG(v)+1);
        }
        INC(v, IN_DEG(v)) = u;     //metto il vertice di partenza nella lista di incidenza di v

        OUT_DEG(u)++;               //aumento il numero di archi uscenti da u
        IN_DEG(v)++;                //aumento l'in degree di v

    }

    //quickSort(G, 0, V-1);
    

    map = ALLOC(int, V);

    FOR_EACH_VERTEX {
        map[VERTEX(i)] = i;
    }

    DFS();

    print_tree();
    free(G);

    return 0;
}