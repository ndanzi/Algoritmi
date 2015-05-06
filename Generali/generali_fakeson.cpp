#include <iostream>
#include <cassert>
#include <limits.h>
#include <stdlib.h>
#include <stack> 
#include <set>
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
#define FAKE_SON(u) G[u].fake_son
#define FAKE_SON_NUM(u) G[u].fake_son_num

#define ADJ(u, v) G[u].adj_list[v]
#define ADJ_INDEX(u,v) map[G[u].adj_list[v]]

#define FOR_EACH_VERTEX for(int i = 0; i < V; i++)
#define FOR_EACH_EDGE for(int i = 0; i < E; i++)
#define FOR_EACH_ADJ(u)                         \
        for(int v = 0; v < OUT_DEG(u); v++) 

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
    int counter;
    int lowlink;
    int on_stack;
    int fake_son_num;
    int fake_son;
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
int consiglieri = 0;
int counter = 0;
stack<int> S;

void print_tree() {
    //cout << consiglieri << endl;
    output << consiglieri << endl;
    FOR_EACH_VERTEX {
        if(P(i) == -1) {
            //cout << VERTEX(i) << " ";
            output << VERTEX(i) << " ";

        }
        
    }
    //cout << endl;
    output << endl;
    FOR_EACH_VERTEX {
        if(P(i) != -1) {
            //cout << P(i)<< " " << VERTEX(i) << endl;
            output << P(i)<< " " << VERTEX(i) << endl;
        }
    }
}

void eliminate_fake_sons(int u, int z);

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
        //cout << "\t\tBefore visit\tu = " << VERTEX(u) << "\tw = " << VERTEX(w) << endl;
        //cout << "\t\tlowlink(w): " << LOWLINK(w) << " counter(w): " << COUNTER(w) << endl;
        //cout << "\t\tlowlink(u): " << LOWLINK(u) << " counter(u): " << COUNTER(u) << endl << endl;
        //cout << "\t\tcolor(w) is ";
        #endif
        //se trovo un nodo bianco lo setto come mio sottoposto e lo visito
        if(COLOR(w) == WHITE) {
            
            P(w) = VERTEX(u);
            #ifdef DEBUG
            //cout << "WHITE" << endl;
            cout << "\t\t\tset " << VERTEX(u) << " as father of " << VERTEX(w) << endl;
            #endif
            DFS_visit(w);
            LOWLINK(u) = min(LOWLINK(u), LOWLINK(w));
            
        
        //se trovo un nodo grigio allora ho trovato un ciclo
        } else if(ONSTACK(w) == 1) {

            LOWLINK(u) = min(LOWLINK(u), COUNTER(w));

        } else if(COLOR(w) == BLACK && (P(w) == -1 || FAKE_SON(w) != -1)) {
            int found = 0;
            #ifdef DEBUG
            //cout << "BLACK" << endl;
            #endif
            //se sono il padre di w
            //scorro la lista di adj di u da w in poi
            for(int i = 0; i < OUT_DEG(u); i++) {
                int z = ADJ_INDEX(u, i);
                #ifdef DEBUG
                //cout << "\t\titerator i: " << i << " z: " << VERTEX(z) << " lowlink(z): " << LOWLINK(z) << endl;
                #endif
                //se trovo un nodo con la stessa componente connessa di w non lo inserisco come figlio di u
                if(LOWLINK(w) == LOWLINK(z) && P(z) == VERTEX(u) && VERTEX(w)!=VERTEX(z)) {
                    found = 1;
                    #ifdef DEBUG
                    cout << "\t\t vertex z:" << VERTEX(z) << " found " << "in_deg(z): " << IN_DEG(z) << endl;
                    #endif
                    //se il nodo che vorrei aggiungere ha indegree minore di un mio figlio della stassa cc
                    if(IN_DEG(w) < IN_DEG(z)) {
                        #ifdef DEBUG
                        cout << "\t\ttook w (IN_DEG(w): "<< IN_DEG(w) << " ) instead of z" << endl;
                        #endif
                        //prendo w e lascio il figlio che avevo già

                        if(FAKE_SON(z) != -1){
                            eliminate_fake_sons(u, z);
                            if(FAKE_SON(w) == -1){
                                consiglieri--;
                            }
                        }

                        if(FAKE_SON(w) != -1){
                            FAKE_SON(w) = -1;
                            FAKE_SON_NUM(P(w))--;
                        }

                        P(w) = VERTEX(u);
                        P(z) = -1;

                    //altrimenti se hanno indegree uguale
                    } else if(IN_DEG(w) == IN_DEG(z)) {
                        // li prendo ma li setto a fake son
                        if(P(w) == -1){
                            consiglieri--;
                        }else if(FAKE_SON(w) != -1){
                            FAKE_SON_NUM(P(w))--;
                        }

                        #ifdef DEBUG
                        cout << "\t\ttook "<< VERTEX(w) <<" (IN_DEG(w): "<< IN_DEG(w) << " ) and z" << endl;
                        #endif

                        P(w) = VERTEX(u);
                        FAKE_SON(w) = 1;
                        FAKE_SON(z) = 1;
                        FAKE_SON_NUM(u)+=2;

                    }
                }
            }
            if(found == 0){
                if(P(w) == -1)
                    consiglieri--;
                if(FAKE_SON(w) != -1){
                    FAKE_SON(w) = -1;
                    FAKE_SON_NUM(P(w))--;
                }
                P(w) = VERTEX(u);
                #ifdef DEBUG
                cout << "\t\t" << VERTEX(w) << " took as son" << endl;
                #endif
            }
        }

        #ifdef DEBUG
        //cout << "\t\tAfter visit\tu = " << VERTEX(u) << "\tw = " << VERTEX(w) << endl;
        //cout << "\t\tlowlink(w): " << LOWLINK(w) << " counter(w): " << COUNTER(w) << endl;
        //cout << "\t\tlowlink(u): " << LOWLINK(u) << " counter(u): " << COUNTER(u) << endl << endl;
        #endif
    }

    int w, z;

    //controllo le componenti connesse
    if(LOWLINK(u) == COUNTER(u)) {
        #ifdef DEBUG
        cout << "\t\tFound cycle starting in " << VERTEX(u) << endl;
        #endif
        do{
            w = S.top();
            S.pop();
            ONSTACK(w) = 0;
            #ifdef DEBUG
            cout << "\t\tindex of u: " << u << " index of w: " << w << endl;
            cout << "\t\t" << VERTEX(w) << " is out stack";
            #endif
            if(w != u) {
                if(P(w) != -1)
                    consiglieri++;
                P(w) = -1;
                LOWLINK(w) = LOWLINK(u);
                #ifdef DEBUG
                cout << " and consigliere" << endl;
                #endif
            } 
            #ifdef DEBUG
            else 
                cout << endl;
            #endif    
        } while(w != u);
    }
    #ifdef DEBUG
    cout << endl << "\t\tchecking third law for " << VERTEX(u) << endl;
    #endif
    //controllo la terza legge
    //scorro la lista di adiacenza di u per trovare i nodi con la stessa componente connessa
   /* FOR_EACH_ADJ(u) {
        w = ADJ_INDEX(u, v);
        #ifdef DEBUG
        cout << "\t\titerator v: " << v << " w: " << VERTEX(w) << " lowlink(w): " << LOWLINK(w) << endl;
        #endif
        //se sono il padre di w
        if(P(w) == VERTEX(u)) {
            //scorro la lista di adj di u da w in poi
            for(int i = v+1; i < OUT_DEG(u); i++) {
                z = ADJ_INDEX(u, i);
                #ifdef DEBUG
                cout << "\t\titerator i: " << i << " z: " << VERTEX(z) << " lowlink(z): " << LOWLINK(z) << endl;
                #endif
                //se trovo un nodo con la stessa componente connessa di w lo stacco da u e diventa consigliere
                if(LOWLINK(w) == LOWLINK(z) && P(z) == VERTEX(u)) {
                    P(z) = -1;
                    consiglieri++;
                    #ifdef DEBUG
                    cout << "\t\t" << VERTEX(z) << " added to consiglieri" << endl;
                    #endif
                }
            }
        }
    }*/



    //visita terminata

    COLOR(u) = BLACK;


    END_VISIT(u) = T;
    #ifdef DEBUG
    cout << "\t\tset " << VERTEX(u) << " to BLACK" << endl;
    cout << "T:" << T << " --\tEND VISIT OF " << VERTEX(u) << endl;
    #endif
    T++;


    
    
    return -1;
}

void DFS() {
    FOR_EACH_VERTEX {
        if(COLOR(i) == WHITE) {
            consiglieri++;
            DFS_visit(i);
        }
    }
    /*int w, z;
    FOR_EACH_VERTEX {
        FOR_EACH_ADJ(i) {
            w = ADJ_INDEX(i, v);
            #ifdef DEBUG
            cout << "\t\titerator v: " << v << " w: " << VERTEX(w) << " lowlink(w): " << LOWLINK(w) << endl;
            #endif
            //se sono il padre di w
            if(P(w) == VERTEX(i)) {
                //scorro la lista di adj di u da w in poi
                for(int j = v+1; j< OUT_DEG(i); j++) {
                    z = ADJ_INDEX(i, j);
                    #ifdef DEBUG
                    cout << "\t\titerator i: " << j<< " z: " << VERTEX(z) << " lowlink(z): " << LOWLINK(z) << endl;
                    #endif
                    //se trovo un nodo con la stessa componente connessa di w lo stacco da u e diventa consigliere
                    if(LOWLINK(w) == LOWLINK(z) && P(z) == VERTEX(i)) {
                        P(z) = -1;
                        consiglieri++;
                        #ifdef DEBUG
                        cout << "\t\t" << VERTEX(z) << " added to consiglieri" << endl;
                        #endif
                    }
                }
            }
        }
    }*/
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

void eliminate_fake_son(){
    int w, c, k;
    FOR_EACH_VERTEX{
        if(FAKE_SON_NUM(i) > 0){
            #ifdef DEBUG
            cout << "ELIMINATE SONS OF " << VERTEX(i) << endl;
            #endif
            FOR_EACH_ADJ(i){
                w = ADJ_INDEX(i, v);
                for(int j=v+1; j < OUT_DEG(i); j++){
                    k = ADJ_INDEX(i, j);
                    if(P(w) == VERTEX(i) && P(k)== P(w) && FAKE_SON(k)!=-1 && LOWLINK(w)==LOWLINK(k)){
                        #ifdef DEBUG
                        cout << "---" << VERTEX(k) << " ELIMINATE AS SON" << endl;
                        #endif
                        P(k) = -1;
                        consiglieri++;
                    }
                }
            }
        }
    }
}

void eliminate_fake_sons(int u, int z){
    int w;
    if(FAKE_SON_NUM(u) > 0){
        #ifdef DEBUG
        cout << "ELIMINATE SONS OF " << VERTEX(u) << endl;
        #endif
        FOR_EACH_ADJ(u){
            w = ADJ_INDEX(u, v);
            if(P(w) == VERTEX(u) && FAKE_SON(w)!=-1 && LOWLINK(w)==LOWLINK(z)){
                #ifdef DEBUG
                cout << "---" << VERTEX(w) << " ELIMINATE AS SON" << endl;
                #endif
                P(w) = -1;
                FAKE_SON(w)=-1;
                FAKE_SON_NUM(u)--;
                consiglieri++;
            }
        }
    }
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
        FAKE_SON(i) = -1;
        FAKE_SON_NUM(i) = 0;

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

    //quickSort(G, 0, V-1);

    map = ALLOC(int, V);

    FOR_EACH_VERTEX {
        map[VERTEX(i)] = i;
    }

    DFS();


    /*FOR_EACH_VERTEX  {
        cout << "vertice " << VERTEX(i) << " in deg = " << IN_DEG(i) << endl << "\t";
        if(P(i) == -1) cout << "consigliere!! ";
        FOR_EACH_ADJ(i) {
            cout << ADJ(i,v) << " ";
        }
        cout << endl;
    }*/

    eliminate_fake_son();

    print_tree();

    return 0;
}