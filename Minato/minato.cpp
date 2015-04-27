#include <iostream>
#include <cassert>
#include <limits.h>
#include <stdlib.h>
#include <queue> 
#include <fstream>
#include <cstring>
#include <string>

#define MAXM 100
#define SI '*'
#define NO '+'

int N, M;

using namespace std;


int BFS(int s_r, int s_c, char **labirinto) {

    queue<int> r_queue; //coda coordinata verticale
    queue<int> c_queue; //coda coordinata orizzontale
    queue<int> dist_queue; //coda delle distanze
    bool raggiunto = false;
    int count = 0;
    int dist;
    char colored[N][M]; //matrice delle lastre visitate
    for(int i = 0; i < N; i++) {
        memset(colored[i], 0, sizeof(char) * M);
    }

    //inserisco la sorgente
    if(labirinto[s_r][s_c] == SI) {
        r_queue.push(s_r);
        c_queue.push(s_c);
        dist_queue.push(0);
        colored[s_r][s_c] = 1;
        //cout << "aggiunta sorgente" << endl;
    }

    int r, c;

    while(!r_queue.empty()) {
        r = r_queue.front();
        r_queue.pop();
        c = c_queue.front();
        c_queue.pop();
        dist = dist_queue.front();
        dist_queue.pop();

        cout << "\nestratto\t" << r << "\t" << c << "\tdist = " << dist << endl;


        //vicino a in basso
        if(r+1 == N-1 && c == M-1) {

            //ho raggiunto la destinazione
            //raggiunto = true;
            count++;
            cout << "destinazione raggiunta con\t" << r+1 << "\t" << c << endl;
            //break;

        } else if((r+1 != N) && labirinto[r+1][c] == SI /*&& colored[r+1][c] == 0*/) {

            //se non è una trappola e non l'ho già visitato lo inserisco in coda
            r_queue.push(r+1);
            c_queue.push(c);
            dist_queue.push(dist+1);
            colored[r+1][c] = 1;
            cout << "\taggiunto alla coda\t" << r+1 << "\t" << c << endl;
        }
        //vicino a destra
        if(r == N-1 && c+1 == M-1) {
            //ho raggiunto la destinazione
            //raggiunto = true;
            count++;
            cout << "destinazione raggiunta con\t" << r << "\t" << c+1 << endl;
            //break;
        } else if((c+1 != M) && labirinto[r][c+1] == SI /*&& colored[r][c+1] == 0*/) {

            //se non è una trappola e non l'ho già visitato lo inserisco in coda
            r_queue.push(r);
            c_queue.push(c+1);
            dist_queue.push(dist+1);
            colored[r][c+1] = 1;
            cout << "\taggiunto alla coda\t" << r << "\t" << c+1 << endl;
        }

        if(r == N-1 && c == M-1) {
            count++;
            cout << "destinazione raggiunta con\t" << r+1 << "\t" << c << endl;
        }
    }

    //return raggiunto;
    return count;

}



int main() {

    ifstream input;
    ofstream output;

    input.open("input.txt");
    assert(input.is_open());
    output.open("output.txt", ofstream::trunc);

    input >> N;
    cout << N << endl;
    input >> M;
    cout << M << endl;


    char **labirinto = (char**) malloc(sizeof(char*) * N);
    for(int i = 0; i < N; i++) {
        labirinto[i] = (char*) malloc(sizeof(char) * M);
    }

    for(int i = 0; i < N; i++) {
        cout << i << "\t";
        for(int j = 0; j < M; j++) {
            input >> labirinto[i][j];
            cout << labirinto[i][j];
        }
        
        cout << endl;
    }

    //cout << endl << "count = " << BFS(0, 0, labirinto) << endl;
    output << BFS(0, 0, labirinto);

    return 0;
}

