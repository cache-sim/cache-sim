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

    ll numberOfSets = atoll(argv[1]); //atoll : char* to long long
    ll blockSize = atoll(argv[2]);
    ll setAssociativity = atoll(argv[3]);

    Cache cache(numberOfSets, blockSize, setAssociativity); //initialise a cache with relevant parameters

    //lru specific allocation begins
    lastUsed = (ll*) malloc(numberOfSets * setAssociativity * sizeof(ll)); 
    //lru specific allocation ends 

    //measure time
    auto start = high_resolution_clock::now();

    while(true) {

        ll address = getNextAddress();
        if(address == 0) break; // reached eof

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
    auto duration = duration_cast<milliseconds>(stop - start);

    //output
    printResult(duration.count(), cache);
    
    //lru specific deallocation begins
    free(lastUsed);
    //lru specific deallocation ends

    return 0;
}