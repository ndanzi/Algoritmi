#include <iostream>
#include <fstream>
#include <cassert>

#define MAXN 1000000
#define UNKNOWN -1

using namespace std;

//MEMOIZZAZIONE
int memo[MAXN + 1];

//int closed[MAXN + 1], ntime = 0;

int count(int n) {

  assert( n >= 0); //AASSERT: PER SCONGIURARE BAD INPUT

  if(memo[n] != UNKNOWN) //MEMOIZZAZIONE
    return memo[n];

  //casi base
  /*if(n == 0)
    return 0;
  else if(n == 1)
    return 1;
  else if(n == 2)
    return 2;*/
  //caso induttivo 
  memo[n] = count(n-1) + count(n-2); //mod per restituire solo le ultime 6 cifre
  
  //closed[n] = ntime++; 
  return memo[n];


}

int main ()
{
 
  int n;
  int out = 0;
  ofstream output;
  ifstream input;
  
  input.open("input.txt");
  assert(input.is_open());
  output.open("output.txt", ofstream::trunc);
  //cout << "inserire il numero di piastrelle: ";
  //cin >> n;


  input >> n;

  for(int i = 0; i <= MAXN; i++) 
    memo[i] = UNKNOWN;

  memo[0] = 0;
  memo[1] = 1;
  //closed[0] = closed[1] = ntime++;

  out = count(n);


  //cout << "il numero di combinazioni possibili con piastrelle da 1 e da 2 è: " << out << endl;

  //cout << "i\tmemo[i]\tclosed[i]" << endl;

  /*for(int i = 0; i <= n; i++) {
    cout << i << "\t" << memo[i] << "\t" << closed[i] << endl;
  }*/


  
  return 0;
}
