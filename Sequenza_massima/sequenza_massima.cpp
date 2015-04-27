#include <iostream>
#include <fstream>
#include <cassert>
#include <climits>

#define MAXN 1000000

using namespace std;

int P[MAXN];

int main(int argc, char** argv) {
  long int n;
  int M = INT_MIN;
  long int max, j;
 
  ifstream input;
  ofstream output;
  
  input.open("input.txt");
  assert(input.is_open());
  output.open("output.txt", ofstream::trunc);

  input >> n;

  int i = 0;
  for(i = 0; i < n; i++)
    input >> P[i];

  input.close();

  
  for(i = 0; i < n; i++) {
    max = P[i];
    if(max > M) M = max;
    for(j = i+1; j < n; j++) {
      max += P[j];
      if(max > M) M = max;
    }
  }
  
  
  output << M << endl;
  

	return 0;
}
