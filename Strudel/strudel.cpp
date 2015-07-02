#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>

#define MAXN 100000

#define SC(i, j) i == 0 ? S_C[j] : S_C[j] - S_C[i-1]
#define SM(i, j) i == 0 ? S_M[j] : S_M[j] - S_M[i-1]
#define SR(i, j) S_R[j] + S_R[i];

using namespace std;

int main() {
	ifstream input("input.txt");
	assert(input);

	int N;
	input >> N;
	int C[N], M[N];
	int S_C[N], S_M[N], S_R[N];

	for(int i = 0; i < N; i++) {
		input >> M[i];
		
		if(i == 0)
			S_M[0] = M[0];
		else
			S_M[i] = S_M[i-1] + M[i];

		//cout << "S_M[" << i << "] = " << S_M[i] << endl;
	}


	for(int i = 0; i < N; i++) {
		input >> C[i];

		if(i == 0)
			S_C[0] = C[0];
		else
			S_C[i] = S_C[i-1] + C[i];

		//cout << "S_C[" << i << "] = " << S_C[i] << endl;
	}

	int ok;
	for(int i = 0; i < N; i++) {

		ok = S_C[i] > S_M[i] ? 1 : 0;

		if(i == 0)
			S_R[0] = ok;
		else
			S_R[i] =  S_R[i-1] + ok;

		cout << "S_M[" << i << "] = " << S_M[i] << endl;
		cout << "S_C[" << i << "] = " << S_C[i] << endl;
		cout << "S_R[" << i << "] = " << S_R[i] << endl << endl;
	}


	int max = 0;
	for(int j = N-1; j >= 0; j--) {
		for(int i = 0; i + j < N; i++) {
			int sc = SC(i, i + j);
			int sm = SM(i, i + j);
			//cout << "SC(" << i << ", " << j << ") = " << sc << " | SM(" << i << ", " << j << ") = " << sm << endl;
			if(sc > sm) {
				max = j + 1;
				break;
			}
			//cout << " | max = " << max << endl;
		}
		if(max != 0)
			break;
	}

	ofstream output("output.txt");
	assert(output);
	output << max;
}
