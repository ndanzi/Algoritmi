#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>

#define MAXN 2000000
#define MAXH 1000000
#define DX 1
#define SX 0
//#define DEBUG

using namespace std;

ofstream output;

int **E;
int n;
int A_DX[MAXN];
int A_SX[MAXN];

/*void Abbatti(int posizione, int direzione) {
	output << posizione << " " << direzione << endl;
	#ifdef DEBUG
		cout << posizione << " " << direzione << endl;
	#endif
}*/

void Abbatti(int, int);
//void Abbatti(int indice, int direzione);

void Soluzione(int i, int j) {

	#ifdef DEBUG
		cout << "Soluzione(" << i << ", " << j << ")" << endl;
		cout << "A_DX[i]+1 = " << A_DX[i]+1 << endl; 
		cout << "A_SX[j]-1 = " << A_SX[j]-1 << endl;
	#endif

	if(i > n-1 || j < 0 ) {
		return;
	}

	if(A_DX[i]+1 > n-1) {
		Abbatti(i, DX);
		return;
	}

	if(A_SX[j]-1 < 0) {
		Abbatti(j, SX);
		return;
	}

	if(E[A_DX[i]+1][j] <= E[i][A_SX[j]-1]){
		Abbatti(i, DX);
		Soluzione(A_DX[i]+1, j);
	} else {
		Abbatti(j, SX);
		Soluzione(i, A_SX[j]-1);
	}

}

void Pianifica(int N, int altezza[]) {
    
	n = N;

    A_DX[N-1] = N-1;
    int max;
    for(int i = N-2; i >= 0; i--) {
    	max = i;
    	for(int j = i+1; j <= i + altezza[i] - 1 && j <= N-1; j++) {
    		if(A_DX[j] > max)
    			max = A_DX[j];
    	}
    	A_DX[i] = max;
    	#ifdef DEBUG
    		cout << "i = " << i << " A_DX[i] = " << A_DX[i] << endl;
    	#endif
    }

    A_SX[0] = 0;
    int minimum;
    for(int i = 1; i <= N-1; i++) {
    	minimum = i;
    	for(int j = i-1; j >= i - altezza[i] + 1 && j >= 0; j-- ) {
    		if (A_SX[j] < minimum) 
    			minimum = A_SX[j];
    	}
    	A_SX[i] = minimum;
    	#ifdef DEBUG
    		cout << "i = " << i << " A_SX[i] = " << A_SX[i] << endl;
    	#endif
    }

    
    E = (int**)malloc(sizeof(int*)*N);
    for(int i = 0; i < N; i++)
    	E[i] = (int*)malloc(sizeof(int)*N);

    for(int i = N-1; i >= 0; i--) {
    	for(int j = 0; j <= N-1; j++) {
    		if(j < i)
    			E[i][j] = 0;

    		if(i == j)
    			E[i][j] = 1;

    		if(i < j) {
    			if(A_DX[i]+1 > N-1 || A_SX[j]-1 < 0)
    				E[i][j] = 1;
    			else
    				E[i][j] = min(E[A_DX[i]+1][j], E[i][A_SX[j]-1]) + 1;
    		}
    	}
    }

    #ifdef DEBUG
    	for(int i = 0; i <= N-1; i++) {
    		for (int j = 0; j <= N-1; j++) {
    			cout << "E[" << i << "][" << j << "] = " << E[i][j] << " | ";
    		}
    		cout << endl;
    	}
    #endif

    Soluzione(0, N-1);

}

/*int main() {

	ifstream input("input.txt");
	assert(input);

	input >> N;

	int H[N];
	for(int i = 0; i < N; i++)
		input >> H[i];

	output.open("output.txt", ofstream::trunc);

	Pianifica(N, H);

}*/
