#include <iostream>
#include <cstdlib>
#include <cassert>

#define NEXT_IN_RANGE(h1, h2) h_nani[p_nani[h1]+1] <= h2 && h_nani[p_nani[h1]+1] > h1
#define BEFORE_IN_RANGE(h1, h2) h_nani[p_nani[h1]-1] <= h2 && h_nani[p_nani[h1]-1] > h1

using namespace std;

int main() {

	int N, M;

	cin >> N >> M;

	int h_nani[N+1];
	int p_nani[N+1];

	for(int i = 1; i < N+1; i++) {
		cin >> h_nani[i];
		p_nani[h_nani[i]] = i;
		cout << i << " - h_nani[i] = " << h_nani[i] << " p_nani[h_nani[i]] = " << p_nani[h_nani[i]] << endl;
	}

	int c, p1, p2, h1, h2, tmp, count, h, p;
	bool response;
	for(int i = 0; i < M; i++) {
		cin >> c;
		if(c == 1) {
			cin >> p1 >> p2;

			p_nani[h_nani[p2]] = p1;
			p_nani[h_nani[p1]] = p2;

			tmp = h_nani[p1];
			h_nani[p1] = h_nani[p2];
			h_nani[p2] = tmp;

		} else {
			cin >> h1 >> h2;
			response = true;
			count = 0;
			h = h1;
			p = p_nani[h1];

			cout << "p_nani[h1] = " << p_nani[h1] << endl;
			cout << "h_nani[p_nani[h1]+1] = "  << h_nani[p_nani[h1]+1] << endl;
			cout << "h_nani[p_nani[h1]-1] = "  << h_nani[p_nani[h1]-1] << endl;

			if(p != 1 && p != N && NEXT_IN_RANGE(h1, h2) && BEFORE_IN_RANGE(h1, h2)) {
				while(h_nani[p+1] < h1 && h_nani[p+1] > h2 && p+1 <= N) {
					++p;
					cout << "p = " << p << " h_nani[p] = " << h_nani[p] << " count = " << count << endl;
					if(h_nani[p] < h1 && h_nani[p] > h2) {
						response = false;
						break;
					}
					count++;
				}
				if(response == true) {
					while(count != h2 - h1) {
						--p;
						cout << "p = " << p << " h_nani[p] = " << h_nani[p] << " count = " << count << endl;
						if(h_nani[p] < h1 && h_nani[p] > h2) {
							response = false;
							break;
						}
						count++;
					}
				}
			} else if(p != N && NEXT_IN_RANGE(h1, h2)) {
				while(count != h2 - h1) {
					++p;
					cout << "p = " << p << " h_nani[p] = " << h_nani[p] << " count = " << count << endl;
					if(h_nani[p] < h1 && h_nani[p] > h2) {
						response = false;
						break;
					}
					count++;
				}
			} else if(p != 0 && BEFORE_IN_RANGE(h1, h2)) {
				while(count != h2 - h1) {
					--p;
					cout << "p = " << p << " h_nani[p] = " << h_nani[p] << " count = " << count << endl;
					if(h_nani[p] < h1 && h_nani[p] > h2) {
						response = false;
						break;
					}
					count++;
				}
			}

			if(response && count == h1 - h2 - 1)
				cout << "YES" << endl;
			else
				cout << "NO" << endl;
		}
	}
}