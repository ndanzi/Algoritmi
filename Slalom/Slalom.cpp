#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

#define MAXN 400000
#define NERO 1
#define BIANCO 0
#define GRIGIO 2

int N;
int P[MAXN+1];
int colori[MAXN+1];
int P_ottimi[MAXN+1];
int P_preso[MAXN+1];

using namespace std;

vector<vector<int>*> G;
vector<int> telecamere;

void scegli_nodo(int u, bool taken)
{

	if(colori[u] == BIANCO)
	{
		colori[u] = GRIGIO;
		if( taken || P_preso[u]==P_ottimi[u])
		{
			telecamere.push_back(u);
			for(int i=0; i<G[u]->size(); i++)
			{
				scegli_nodo(G[u]->at(i), false);
			}
		}
		else
		{
			for(int i=0; i<G[u]->size(); i++)
			{
				scegli_nodo(G[u]->at(i), true);
			}
		}
		colori[u] = NERO;
	}
}

void DFS(int u)
{
	int p_ottimo_foglia = 0;
	int p_preso_foglia = 0;
	int foglia = 1;
	colori[u] = GRIGIO;

	for(int i=0; i<G[u]->size(); i++ )
	{
		int v = G[u]->at(i);
		//cout << "v = " << v << endl;
		if(colori[v]==BIANCO) 
		{
			foglia = 0;
			DFS(v);
			p_ottimo_foglia += P_ottimi[v];
			p_preso_foglia += P_preso[v];
		}
	}

	colori[u] = NERO;

	if(foglia == 1)
	{
		P_preso[u] = P[u];
		P_ottimi[u] = 0;
	}
	else
	{
		P_preso[u] = P[u] + p_ottimo_foglia;
		P_ottimi[u] = min(P_preso[u], p_preso_foglia);
	}
}


void risolvi()
{	
	int i;
	for(i=1; i<N+1; i++)
		colori[i] = BIANCO;
	DFS(1);
	for(i=1; i<N+1; i++)
		colori[i] = BIANCO;
	scegli_nodo(1, P_preso[1]==P_ottimi[1]);

}


int main()
{
	ifstream input("input.txt");
	assert(input);
	input >> N;

	for(int i=0; i<N+1; i++)
	{
		if(i > 0)
			input >> P[i];
		G.push_back(new vector<int>);
	}

	for(int j=0; j<N-1; j++)
	{
		int a,b;
		input >> a >> b;
		G[a]->push_back(b);
		G[b]->push_back(a);
	}

	input.close();

	risolvi();

	ofstream output("output.txt");
	output << telecamere.size() << endl;
	for(int i=0; i<telecamere.size(); i++)
		output << telecamere.at(i) << " ";
	output << endl;
	output.close();

}