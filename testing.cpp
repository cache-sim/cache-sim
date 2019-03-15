#include "cache.h"
#include <cstdio>
using namespace std;

typedef long long ll;
#define N 8

int main() {

    Cache cache(1<<1, 1<<0, 1<<1);

    for(int i = 0; i < N; i++) {
        printf("%d\n", i);
        if(cache.isDataInCache(i)) {
            printf("hit\n");
            cache.incHits();
        }
        else {
            cache.incMisses();
            if(!cache.isSetFull(cache.getIndexFromAddress(i))) {
                cache.insertDataToCache(i);
            }
            else {
                cache.evictAndInsertData(i-4, i);
            }
        }
        cache.displayCache();
    }

    printf("Hits: %lld\nMisses: %lld\nHit rate: %f\nMiss rate: %f\n", cache.getNumberOfHits(), cache.getNumberOfMisses(), cache.hitRate(), cache.missRate());

}