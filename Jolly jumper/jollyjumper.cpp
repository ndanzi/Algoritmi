#include <iostream>
#include <cassert>
#include <cstring>
#include <string>
#include <sstream>

#define MAX_N 3000

char d[MAX_N];
int a[MAX_N];

int main() {

	string line;
	int n;
	int n_1;
	int diff;
	int max;
	bool jolly = true;

	while(getline(cin, line)) {
		
		stringstream input;
		input = stringstream(line);

		int i = 0;
		max = 0;
		memset(d, 0, sizeof(int) *  MAX_N);

		while(input) {
			if(i != 0) {
				input >> n;

				diff = n_1 - n;

				n_1 = n;

				if(diff < MAX_N) {
					d[diff] = 1;
				} else {
					cout << "Not Jolly";
					jolly = false;
					break;
				}

			} else {
				input >> n_1;
			}

			if(n_1 > max)
				max = n_1;

			i++;
		}

		if(jolly && i != 1) {
			for(i = 1; i < MAX_N; i++) {
				if(d[i] == 0) {
					cout << "Not Jolly";
					jolly = false;
					break;
				}
			}
			if(jolly)
				cout << "Jolly";
		} else if( i == 1) {
			cout << "Jolly";
		}

	}

    return 0;
}