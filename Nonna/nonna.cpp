#include <stdio.h>
#include <assert.h>
#include <limits.h>

#define MAXN 5000
#define MAXK 5000
#define MAXP 1000000

int memo[MAXN][MAXK];

int min(int n1, int n2) {
    if(n1 < n2) return n1;
    if(n1 > n2) return n2;
    return n1;
}

int mangia(int N, int K, int P[]) {
    // Mettete qui il codice della soluzione

    if(N == 0) {
        if(K > 0) return INT_MAX;
        return 0;
    }
    if(K < 0) {
        return 0;
    }
    int si = 0, no = 0;
    si = mangia(N-1, K-P[N-1], P);
    
    no = mangia(N-1, K, P);

    if(si != INT_MAX) si += P[N-1];

    //printf("N = %d, K = %d, P[N-1] = %d, si = %d, no = %d, min(si,no) = %d\n", N, K, P[N-1], si, no, min(si, no));

    return min(si, no);
}


int P[MAXN];

int main() {
    FILE *fr, *fw;
    int N, K, i;

    fr = fopen("input.txt", "r");
    fw = fopen("output.txt", "w");
    assert(2 == fscanf(fr, "%d %d", &N, &K));
    for(i=0; i<N; i++)
        assert(1 == fscanf(fr, "%d", &P[i]));

    fprintf(fw, "%d\n", mangia(N, K, P));
    fclose(fr);
    fclose(fw);
    return 0;
}

