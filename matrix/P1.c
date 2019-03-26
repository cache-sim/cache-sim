#include <stdio.h>
#include <stdlib.h>

#define N 1024

int main() {

    int i, j, k;
    double **A, **B, **C;

    A = (double**) malloc(N * sizeof(double*));
    B = (double**) malloc(N * sizeof(double*));
    C = (double**) malloc(N * sizeof(double*));

    //printf("Allocating Memory\n");
    for(i = 0; i < N; i++) {
        A[i] = (double*) malloc(N * sizeof(double));
        B[i] = (double*) malloc(N * sizeof(double));
        C[i] = (double*) malloc(N * sizeof(double));    
    }
    //printf("Done Allocating memory\n");

    //printf("Initialising A and B with random values\n");
    for(i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            A[i][j] = rand()%N;
            B[i][j] = rand()%N;
        }    
    }
    //printf("Done\n");

    //printf("Performing matrix multiplication\n");
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            for(int k = 0; k < N; k++) {
                C[i][j] += (A[i][k] * B[k][j]);
            }
        }
    }
    //printf("Matrix multiplication done\n");

    return 0;
}