#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cassert>

#define MAXN 10000
#define MAXP 10000

using namespace std;

int N;
vector<int> P;

int main() {
	ifstream input("input.txt");
	assert(input);

	input >> N;
	int p;

	P.push_back(MAXP+1);

	for(int i = 0; i < N; i++) {
		input >> p;
		P.push_back(p);
	}
	input.close();

	int E[N+1];

	for(int i = N; i >= 0; i--) {

		int max = 0;
		for(int j = i+1; j < N+1; j++) {
			if(P.at(j) < P.at(i)) {
				if(E[j] > max)
					max = E[j];
			}
		}
		E[i] = max+1;
	}

	ofstream output("output.txt");
	assert(output);
	output << E[0]-1;

}