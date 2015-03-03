#include <iostream>
#include <cassert>

using namespace std;

int count(int n) {

  assert( n >= 0);
  //casi base
  if(n == 0)
    return 0;
  else if(n == 1)
    return 1;
  else if(n == 2)
    return 2;
  else //caso induttivo
    return count(n-1) + count(n-2) % 1000000; //mod per restituire solo le ultime 6 cifre


}

int main ()
{
 
  int n;
  int out = 0;
  cout << "inserire il numero di piastrelle: ";
  cin >> n;

  out = count(n);

  cout << "il numero di combinazioni possibili con piastrelle da 1 e da 2 è: " << out << endl;

  
  return 0;
}
