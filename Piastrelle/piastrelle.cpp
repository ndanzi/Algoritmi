#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

#define MAXN 1000000
#define UNKNOWN -1
#define SING "[O]"
#define DOUB "[OOOO]"
#define END "\n"

using namespace std;

//MEMOIZZAZIONE
int memo[MAXN + 1];
string lines[MAXN + 1];

ofstream output;

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
  
  return memo[n];


}

void stampa(int n, string history) {

  //CONTRATTO: listare una riga per le possibili piastrellature di un corridoio di lunghezza n,
  // ciascuna prefissata con history


  //CASI BASE (quelli in cui ho una scelta sola)
  if(n == 0) {
    output << history << END;
    return;
  } 
  if(n == 1) {
    output << history << SING << END;
    return;
  }

  //CASI INDUTTIVI (quelli in cui posso scegliere una piastrella da due o due da uno)
  stampa(n-1, history + SING);
  stampa(n-2, history + DOUB);
  return;

}

int main ()
{
 
  int n;
  int out = 0;

  ifstream input;
  string history = "";
  
  input.open("input.txt");
  assert(input.is_open());
  output.open("output.txt", ofstream::trunc);


  input >> n;

  for(int i = 0; i <= MAXN; i++) 
    memo[i] = UNKNOWN;

  memo[0] = 0;
  memo[1] = 1;


  out = count(n);
  stampa(n, history);




  
  return 0;
}
