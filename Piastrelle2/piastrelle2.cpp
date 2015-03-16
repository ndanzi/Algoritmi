#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
#include <string>

#define NMAX 20
#define UNKNOWN -1
#define ONE 49
#define TWO 50
#define THREE 51
#define FOUR 52
#define FIVE 53

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

using namespace std;

long long int memo_f[NMAX+1];
long long int memo_g[NMAX+1];
long long int memo_h[NMAX+1];

long long int f(int n);
long long int g(int n);
long long int h(int n);

void stampa_g(int n, string history1, string history2);
void stampa_i(int n, string history1, string history2);
void stampa_f(int n, string history1, string history2);
void stampa_h(int n, string history1, string history2);


int N;
ofstream output;


long long int h(int n) {
	assert(n >= 2);
	if(memo_h[n] == UNKNOWN)
		memo_h[n] = g(n-1) + f(n-2);
	return memo_h[n];
}

long long int g(int n) {
	assert(n >= 1);
	//if(n == 1) return 1;
	if(memo_g[n] == UNKNOWN)
		memo_g[n] = f(n-1) + g(n-1);
	return memo_g[n];
}

long long int f(int n) {
	assert(n >= 0);
	//if(n == 0) return 1;
	//if(n == 1) return 2;
	if(memo_f[n] == UNKNOWN)
		memo_f[n] = g(n) + h(n) + f(n-1);
	return memo_f[n];
}

long long int numeroPiastrellature(int n) {
	
	return f(n);
	
}

void stampa_g(int n, string history1, string history2) {
	assert(n >= 1);
	char color1 = 0;
	char color2 = 0;
	char my_color1 = ONE;
	string space = "";
	if(history1.compare("") != 0 && history2.compare("") != 0) {
		space = " ";
	} 
	
	//trovo un colore diverso da quelli precedenti

	color1 = history1.back(); //colore della piastrella precedente a riga 1
	color2 = history2.back(); //colore della piastrella precedente a riga 2
	//scelgo il primo colore
	for(int i = ONE; i <= FIVE; i++) {
		if(i != color1 && i != color2) {
			my_color1 = i;
			break;
		}
	}


	//caso base
	if(n == 1) {
		//coloro due piastrelle singole in verticale
		output << history1 << endl;
		output << history2 << space << my_color1 << endl << endl;
		return;
	}

	//chiamate ricorsive
	stampa_f(n-1, history1, history2 + space + my_color1);
	stampa_i(n-1, history1, history2 + space + my_color1 + " " + my_color1);
	
	return;
}

void stampa_i(int n, string history1, string history2) {
	assert(n >= 1);
	char color1 = 0;
	char color2 = 0;
	char my_color1 = ONE;
	string space = "";
	if(history1.compare("") != 0 && history2.compare("") != 0) {
		space = " ";
	} 
	//trovo un colore diverso da quelli precedenti

	color1 = history1.back(); //colore della piastrella precedente a riga 1
	color2 = history2.back(); //colore della piastrella precedente a riga 2
	//scelgo il primo colore
	for(int i = ONE; i <= FIVE; i++) {
		if(i != color1 && i != color2) {
			my_color1 = i;
			break;
		}
	}


	//caso base
	if(n == 1) {
		//coloro due piastrelle singole in verticale
		output << history1 << space << my_color1 << endl;
		output << history2 << endl << endl;
		return;
	}

	//chiamate ricorsive
	stampa_f(n-1, history1 + space + my_color1, history2);
	stampa_g(n-1, history1 + space + my_color1 + " " + my_color1, history2);
	
	return;
}

void stampa_h(int n, string history1, string history2) {
	assert(n >= 1);
	char color1 = 0;
	char color2 = 0;
	char color3 = 0;
	char my_color1 = ONE;
	char my_color2 = ONE;
	string space = "";
	if(history1.compare("") != 0 && history2.compare("") != 0) {
		space = " ";
	} 
	//trovo un colore diverso da quelli precedenti

	color1 = history1.back(); //colore della piastrella precedente a riga 1
	color2 = history2.back(); //colore della piastrella precedente a riga 2
	color3 = history1.at(history1.length()-2); //colore della penultima piastrella a riga 1
		//scelgo il primo colore
	for(int i = ONE; i <= FIVE; i++) {
		if(i != color1 && i != color2) {
			my_color1 = i;
			break;
		}
	}

	//scelgo il secondo colore tenendo conto anche di color3
	for(int i = ONE; i <= FIVE; i++) {
		if(i != color1 && i != color2 && i != color3) {
			my_color2 = i;
			break;
		}
	}

	//chiamate ricorsive
	stampa_g(n-1, history1, history2 + space + my_color1);
	stampa_f(n-2, history1 , history2 + space + my_color2 + " " + my_color2);
	
	return;
}

void stampa_f(int n, string history1, string history2) {
	assert(n >= 0);
	char color1 = 0;
	char color2 = 0;
	char my_color1 = ONE;
	char my_color2 = ONE;
	char my_color3 = TWO;
	string space;
	
	//trovo due colori diversi da quelli precedenti
	if(n != 0) {
		if(history1.compare("") != 0 && history2.compare("") != 0) {
			color1 = history1.back(); //colore della piastrella precedente a riga 1
			color2 = history2.back(); //colore della piastrella precedente a riga 2
			//scelgo il primo colore per piastrella doppia verticale
			for(int i = ONE; i <= FIVE; i++) {
				if(i != color1 && i != color2) {
					my_color1 = i;
					break;
				}
			}
			//scelgo il primo colore per piastrella singola sopra
			for(int i = ONE; i <= FIVE; i++) {
				if(i != color1) {
					my_color2 = i;
					break;
				}
			}
			//scelgo il secondo colore per piastrella singola sotto
			for(int i = ONE; i <= FIVE; i++) {
				if(i != color2 && i != my_color1) {
					my_color3 = i;
					break;
				}
			}
			space = " ";
		} else {
			space = "";
		}
		
	} 
	
	
	if(n == 0) {
		output << history1 << endl << history2 << endl << endl;
		return;
	}
	if(n == 1) {
		
		//coloro due piastrelle singole in verticale
		output << history1 << space << my_color2 << endl;
		output << history2 << space << my_color3 << endl << endl;
		//coloro una piastrella verticale 2*1
		output << history1 << space << my_color1 << endl;
		output << history2 << space << my_color1 << endl << endl;
		return;
	}

	//chiamate ricorsive
	stampa_g(n, history1 + space + my_color2, history2);
	stampa_h(n, history1 + space + my_color2 + " " + my_color2, history2);
	stampa_f(n-1, history1 + space + my_color1 , history2 + space + my_color1);
	
	return;
}



void listaPiastrellature(int n, string history1, string history2) {
	
	stampa_f(n, history1, history2);
	
	/*history1[0] = 1;
	stampa_g(n, history);
	
	memset(history1, UNKNOWN, sizeof(int) * n);
	memset(history2, UNKNOWN, sizeof(int) * n);
	history1[0] = 1;
	history1[1] = 1;
	stampa_h(n, history);
	
	memset(history1, UNKNOWN, sizeof(int) * n);
	memset(history2, UNKNOWN, sizeof(int) * n);
	history1[0] = 1;
	history2[0] = 1;
	stampa_f(n-1, history);*/
	
}




int main(int argc, char** argv) {
	int n;
	long long int K = 0;
	string history1;
	string history2;

  ifstream input;
  
  input.open("input.txt");
  assert(input.is_open());
  output.open("output.txt", ofstream::trunc);

  input >> n;
  N = n;
  
  memset(memo_f, UNKNOWN, (NMAX+1) * sizeof(long long int));
  memset(memo_g, UNKNOWN,  (NMAX+1) * sizeof(long long int));
  memset(memo_h, UNKNOWN,  (NMAX+1) * sizeof(long long int));

  
  memo_f[0] = 1;
  memo_f[1] = 2;
  memo_g[1] = 1;
  
 	history1 = "";
 	history2 = "";
    
  K = numeroPiastrellature(n);
  
  output << K << endl;
  
  listaPiastrellature(N, history1, history2);

	return 0;
}
