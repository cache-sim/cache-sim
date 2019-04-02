#include <stdio.h>
#include <stdlib.h>

#define N 512

int main() {

    int i, j, k;
    double *A, *B, *C;

    A = (double*) calloc(N * N, sizeof(double));
    B = (double*) calloc(N * N, sizeof(double));
    C = (double*) calloc(N * N, sizeof(double));

    if(A != NULL && B != NULL && C != NULL){

        //printf("Initialising A and B with random values\n");
        // for(i = 0; i < N; i++) {
        //     for(int j = 0; j < N; j++) {
        //         A[i*N + j] = rand()%N;
        //         B[i*N + j] = rand()%N;
        //         // C[i][j] = 0;
        //     }    
        // }
        //printf("Done\n");
    
        //printf("Performing matrix multiplication\n");
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                for(int k = 0; k < N; k++) {
                    C[i*N + j] += (A[i*N + k] * B[k*N + j]);
                }
            }
        }
        //printf("Matrix multiplication done\n");
    
        free(A);
        free(B);
        free(C);
    }
    else{
        printf("Error in memory allocation\n");
    }

    return 0;
}