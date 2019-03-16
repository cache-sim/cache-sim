#include <stdlib.h>

int main() {
    double a[2048], b[2048];

    for(int i = 0; i < 2048; i++) {
        a[i] = rand();
        b[i] = rand();
    }

    for(int i = 0; i < 2048; i++) {
        a[i] = a[i] * b[i];
    }
}