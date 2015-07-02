#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>

#define MAXN 100

using namespace std;

int main() {
	ifstream input("input.txt");
	assert(input);

	int N;
	input >> N;

	int numeri = ((N+1)*N)/2;
	int T[numeri];
	int S[numeri];

	for(int i = 0; i < numeri; i++) {
		input >> T[i];
		S[i] = 0;
	}

	for(int i = numeri-1; i > numeri - N - 1; i--)
		S[i] = T[i];

	int livello = N-1;
	int figlioDX, figlioSX;
	int indice = numeri - N;

	for(int i = livello; i >= 1; i--) {
		indice -= i;
		for(int j = 0; j < i; j++) {
			figlioSX = indice + j + i;
			figlioDX = figlioSX + 1;

			S[indice + j] = max(S[figlioSX], S[figlioDX]) + T[indice + j];
		}
	}

	ofstream output("output.txt");
	assert(output);
	output << S[0];

}