#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>

#define MAXN 5000
#define MAXK 5000
#define MAXP 1000000
#define UNKNOWN -1

//int memo[2][MAXN+1];

int min(int n1, int n2) {
    if(n1 < n2) return n1;
    if(n1 > n2) return n2;
    return n1;
}
int P[MAXN];

int mangia(int N, int K) {
    // Mettete qui il codice della soluzione

    /*if(N == 0) {
        if(K > 0) return INT_MAX;
        return 0;
    }*/

    if(K == 0) {
      return memo[0][N];
    } else {  
      if(memo[1][N] != UNKNOWN)
        return memo[1][N];
    }
    
      

    int si = 0, no = 0, m = 0;
    
    no = mangia(N-1, K);

    if(K - P[N-1] < 0) 
      K = 0;
    else
      K -= P[N-1];

    si = mangia(N-1, K);

    if(P[N-1] < 0) {
      m = INT_MAX;
    } else {
      m = P[N-1];
    }
    if(si < INT_MAX) si += m;

    printf("N = %d, K = %d, P[N-1] = %d, si = %d, no = %d, min(si,no) = %d\n", N, K, P[N-1], si, no, min(si, no));
    memo[1][N] = min(si, no);

    return memo[1][N];
}


int main() {
    FILE *fr, *fw;
    int N, K, i;

    fr = fopen("input.txt", "r");
    fw = fopen("output.txt", "w");
    assert(2 == fscanf(fr, "%d %d", &N, &K));
    for(i=0; i<N; i++)
        assert(1 == fscanf(fr, "%d", &P[i]));

    memo = (int**) malloc(sizeof(int*)*(N+1));

    for(i = 0; i < N+1; i++) {
      memo[i] = (int*) malloc(sizeof(int)*(K+1));
    }
    

    for(i = 0; i <= N; i++) {
      memo[1][i] = UNKNOWN;
    }


    for(i = 1; i <= MAXN; i++)
      memo[0][i] = INT_MAX;

    memo[0][0] = 0;
    memo[1][0] = INT_MAX;

    fprintf(fw, "%d\n", mangia(N, K));
    fclose(fr);
    fclose(fw);
    return 0;
}


