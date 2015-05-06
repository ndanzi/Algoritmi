/// flusso.cpp: implementazione algoritmo di Ford-Fulkerson
/// Autore: Davide Costanzi
/// Ultima modifica: 18/06/2014 15.34-18.14

//#define NDEBUG

#include <fstream>
#include <vector>
#include <queue>
#include <utility>
#include <cassert>
#ifndef NDEBUG
#include <sstream>
#endif

using namespace std;

const int MAXN = 100000;

const int MAXC = 1000000;

// Rappresenta un arco.
struct arco_t
{
    // Testa dell'arco.
    int dest;
    // Capacità dell'arco.
    int c;
    // Flusso assegnato.
    int f;

    arco_t(int dest, int c);
    ~arco_t();
};

arco_t::arco_t(int dest, int c)
{
    this->dest = dest;
    this->c = c;
    this->f = 0;
}

arco_t::~arco_t()
{}

// N: numero di nodi.
// M: numero di archi.
// F: valore del flusso massimo.
int N, M, F;

// La rete iniziale.
vector<arco_t> rete[MAXN];
// Grafo della rete residua trasposta.
vector<arco_t> residua[MAXN];

// Nodi visitati dall'ultima BFS.
bool visitati[MAXN];
// padri[i]: il padre del nodo i, colui che l'ha scoperto.
// NOTA: i valori sono significativi solo se i nodi in questione sono marcati
// visitati.
int padri[MAXN];
int flussi[MAXN];

void init_reti();

void bfs(int start, int end, vector<arco_t> *r);

// Opera sulla rete r e ritorna l'indice nella lista di adiacenza di a che
// corrisponde all'arco (a,b), -1 se l'arco non esite.
int cercaArco(int a, int b, vector<arco_t> *r);

void aggiornaReti(int flusso);

void stampa_rete(ofstream &ofs, vector<arco_t> *r, bool stampa_c = false);

int main()
{
    init_reti();
#ifndef NDEBUG
    ofstream outdb("debugRete.txt");
    stampa_rete(outdb, rete, true);
    outdb.close();
    outdb.open("debugResidua.txt");
    stampa_rete(outdb, residua, true);
    outdb.close();
#endif
#ifndef NDEBUG
    ostringstream oss;
    int passo = 0;
#endif
    while(true)
    {
        // BFS sulla rete residua per inizializzare il vettore dei padri.
        bfs(N-1,0,residua);
        // Non ci sono cammini aumentanti.
        if ( !visitati[0] )
            break;
        // Ho trovato un cammino aumentante di intensità flussi[N-1].
        F += flussi[0];
        // Aggiorno la rete residua & l'allocazione dei flussi della quantità
        // indicata.
        aggiornaReti(flussi[0]);
#ifndef NDEBUG
        ++passo;
        oss.str("");
        oss << "debugRetePasso" << passo << ".txt";
        ofstream outdb(oss.str().c_str());
        stampa_rete(outdb, rete, true);
        outdb.close();
        oss.str("");
        oss << "debugResiduaPasso" << passo << ".txt";
        outdb.open(oss.str().c_str());
        stampa_rete(outdb, residua, true);
        outdb.close();
#endif
    }

    // Stampa il risultato.
    ofstream out("output.txt");
    out << F << endl;
    stampa_rete(out, rete);
    out.close();
}

void init_reti()
{
    ifstream in("input.txt");
    assert(in);
    in >> N >> M;
    assert( 1 <= N && N <= MAXN );
    assert( 1 <= M && M <= N*(N-1) );
    int a, b, c;
    for ( int i = 0; i < M; ++i )
    {
        in >> a >> b >> c;
        rete[a].push_back(arco_t(b,c));
        // Inizialmente la rete residua è il trasposto della rete.
        residua[b].push_back(arco_t(a,c));
    }
    in.close();
}

// Visita il grafo della rete residua trasposta r considerando start come pozzo
// e end come sorgente.
void bfs(int start, int end, vector<arco_t> *r)
{
    assert( 0 <= start && start <= N-1 );
    // pair.first = nodo
    // pair.second = flusso max lungo il cammino 
    queue< pair<int,int> > coda;
    padri[start] = -1;
    flussi[start] = MAXC;
    coda.push(make_pair(start,MAXC));
    for ( int i = 0; i < N; ++i )
        visitati[i] = false;

    while ( !coda.empty() && !visitati[end])
    {
        pair<int,int> curr = coda.front();
        int x = curr.first;
        int max_flusso = curr.second;
        coda.pop();
        if ( visitati[x] )
            continue;
        visitati[x] = true;
        for ( unsigned int i = 0; i < r[x].size(); ++i )
            // Aggiungo il nodo alla coda solo se può essere visitato e non l'ho già visitato.
            if ( r[x][i].c > 0 && !visitati[r[x][i].dest] )
            {
                // Setto il padre per ricosturire il percorso.
                padri[r[x][i].dest] = x;
                // L'arco analizzato è il nuovo collo di bottiglia.
                if ( r[x][i].c < max_flusso )
                    max_flusso = r[x][i].c;
                flussi[r[x][i].dest] = max_flusso;
                coda.push(make_pair(r[x][i].dest,max_flusso));
            }
    }
}

int cercaArco(int a, int b, vector<arco_t> *r)
{
    for ( unsigned int i = 0; i < r[a].size(); ++i )
        if ( r[a][i].dest == b )
            return static_cast<int>(i);
    return -1;
}

void aggiornaReti(int flusso)
{
    // Parto dalla sorgente.
    int inizio = 0;
    // Finché non ho raggiunto il pozzo.
    while ( inizio != N-1 )
    {
        int coda = padri[inizio];
        int idx = cercaArco(inizio, coda, rete);
        // Richiesto aumento di flusso su arco reale.
        if ( idx != -1 )
        {
            rete[inizio][idx].f += flusso;
            // Calo la capacità residua del trasposto dell'arco reale diretto.
            // Certamente presente nella rete residua trasposta.
            idx = cercaArco(coda, inizio, residua);
            residua[coda][idx].c -= flusso;
            // Aumento la capacità residua del trasposto dell'arco inverso a quello reale.
            idx = cercaArco(inizio, coda, residua);
            // Arco già presente.
            if ( idx != -1 )
                residua[inizio][idx].c += flusso;
            // Nuovo arco.
            else
                residua[inizio].push_back(arco_t(coda,flusso));
        }
        // Richiesto aumento di flusso su arco fittizio, ossia devo calarlo
        // sull'arco reale.
        else
        {
            idx = cercaArco(coda, inizio, rete);
            rete[coda][idx].f -= flusso;
            // Aumento la capacità residua del trasposto dell'arco reale diretto.
            // Certamente presente nella rete residua trasposta.
            idx = cercaArco(inizio, coda, residua);
            residua[inizio][idx].c += flusso;
            // Calo la capacità residua del trasposto dell'arco fittizio.
            // Certamente presente nella rete residua trasposta, l'ho usato
            // nella BFS.
            idx = cercaArco(coda, inizio, residua);
        }

        inizio = coda;
    }
}

void stampa_rete(ofstream &ofs, vector<arco_t> *r, bool stampa_c)
{
    for ( int i = 0; i < N; ++i )
    {
        for ( unsigned int j = 0; j < r[i].size(); ++j )
        {
            ofs << i << " " << r[i][j].dest << " ";
            if (stampa_c)
                ofs << r[i][j].c << " ";
            ofs << r[i][j].f << endl;
        }
    }    
}
