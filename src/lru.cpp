//least recently used cache replacement implementation

#include "cache.h"
#include <chrono>
using namespace std;
using namespace std::chrono;

typedef long long ll;

//lru specific datastructures begins

ll timeElapsed = 0;
ll *lastUsed;

//lru specific datastructures ends

int main(int argc, char *argv[]) {

    vector<ll> addresses = readTrace(argv[1]); //read the trace input file

    ll numberOfSets = atoll(argv[2]); //atoll : char* to long long
    ll blockSize = atoll(argv[3]);
    ll setAssociativity = atoll(argv[4]);

    //lru specific allocation begins
    lastUsed = (ll*) malloc(numberOfSets * setAssociativity * sizeof(ll)); 
    //lru specific allocation ends 

    Cache cache(numberOfSets, blockSize, setAssociativity); //initialise a cache with relevant parameters

    //measure time
    auto start = high_resolution_clock::now();

    //go through all addresses
    for(ll address : addresses) {
        //address : the current address accessed by CPU

        ll row; //row in cache
        if((row = cache.isDataInCache(address)) != -1) { //cache hit
            //isDataInCache returns the row number where the data is present
            cache.incHits();  //increment hits

            //lru specific begins
            lastUsed[row] = timeElapsed;
            //lru specific ends
        }
        else { //cache miss
            cache.incMisses(); //increment misses
            if(!cache.isSetFull(cache.getIndexFromAddress(address))) { //if empty slot is available in cache to insert data
                row = cache.insertData(address);
                //insertDataToCache returns the row where the data is inserted
                //lru specific begins
                lastUsed[row] = timeElapsed;
                //lru specific ends
            }
            else { //if no empty slot is available in cache
                long long index = cache.getIndexFromAddress(address); //value of index of address
                long long tag = cache.getTagFromAddress(address); //value of tag of address
                long long rowBegin = index * setAssociativity; 
                long long rowEnd = (index + 1) * setAssociativity;

                //rows between rowBegin(inclusive) and rowEnd(exclusive) must be considered for eviction

                //NOTE: the row number of the block to be evicted must be stored in row!!
                //lru specific begins
                ll min = rowBegin;
                for(ll i = rowBegin; i < rowEnd; i++) {
                    if(lastUsed[i] < lastUsed[min]) {
                        min = i;
                    }
                }

                row = min; //update row to be evicted
                
                lastUsed[row] = timeElapsed;
                //lru specific ends
                
                cache.evictAndInsertCacheLine(row, address);
            }
        }
        //lru specific post action begins
        timeElapsed++;
        //lru specific post action ends

        //debugging begins
        //cache.displayCache();
        //debugging ends
    }

    //measure time
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start);

    //output
    cout << "Simulation time : " << duration.count() << " ms" << endl;
    printf("Total Number of data accesses: %lld\n", cache.getNumberOfHits() + cache.getNumberOfMisses());
    printf("Hits: %lld\n", cache.getNumberOfHits());
    printf("Misses: %lld\n", cache.getNumberOfMisses());
    printf("Hit Ratio: %f\n", cache.hitRate() * 100);

    free(lastUsed);

    return 0;
}