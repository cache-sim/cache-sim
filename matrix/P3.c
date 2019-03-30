#include <stdio.h>
#include <stdlib.h>

#define N 512
#define BLOCK 4

int min(int a, int b){
    if(a < b)
        return a;
    else
        return b;
}

int main() {

    int i, j, k, J, K, r;
    double *A, *B, *C;

    A = (double*) calloc(N * N, sizeof(double));
    B = (double*) calloc(N * N, sizeof(double));
    C = (double*) calloc(N * N, sizeof(double));

    if(A != NULL && B != NULL && C != NULL){

        //printf("Initialising A and B with random values\n");
        for(i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                A[i*N + j] = rand()%N;
                B[i*N + j] = rand()%N;
                // C[i][j] = 0;
            }    
        }
        //printf("Done\n");
    
        //printf("Performing matrix multiplication\n");
        
        for(J = 0; J < N; J+= BLOCK){
            for(K = 0; K < N; K+= BLOCK){

                for(int i = 0; i < N; i++) {
                    //if we check if N is divisble by B, there is no need for min function
                    for(int j = J; j < min(J+BLOCK, N); j++) {
                        r=0;
                        for(int k = K; k < min(K+BLOCK, N); k++) {
                            //r variable seems useless
                            r += (A[i*N + k] * B[k*N + j]);
                        }
                        C[i*N + j] += r;
                    }
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