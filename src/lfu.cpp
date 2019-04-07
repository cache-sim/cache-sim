//least frequently used cache replacement implementation

#include "cache.h"
#include <chrono>
using namespace std;
using namespace std::chrono;

typedef long long ll;

//lfu specific datastructures begins

ll *timesUsed;

//lfu specific datastructures ends

int main(int argc, char *argv[]) {

    ll numberOfSets = atoll(argv[1]); //atoll : char* to long long
    ll blockSize = atoll(argv[2]);
    ll setAssociativity = atoll(argv[3]);

    Cache cache(numberOfSets, blockSize, setAssociativity); //initialise a cache with relevant parameters

    //lfu specific allocation begins
    timesUsed = (ll*) malloc(numberOfSets * setAssociativity * sizeof(ll)); 
    //lfu specific allocation ends 

    //measure time
    auto start = high_resolution_clock::now();

    while(true) {
        
        ll address = getNextAddress();
        if(address == 0) break; // reached eof

        ll row; //row in cache
        if((row = cache.isDataInCache(address)) != -1) { //cache hit
            //isDataInCache returns the row number where the data is present
            cache.incHits();  //increment hits

            //lfu specific begins
            timesUsed[row]++;
            //lfu specific ends
        }
        else { //cache miss
            cache.incMisses(); //increment misses
            if(!cache.isSetFull(cache.getIndexFromAddress(address))) { //if empty slot is available in cache to insert data
                row = cache.insertData(address);
                //insertDataToCache returns the row where the data is inserted
                //lru specific begins
                timesUsed[row] = 0;
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
                    if(timesUsed[i] < timesUsed[min]) {
                        min = i;
                    }
                }

                row = min; //update row to be evicted
                
                timesUsed[row] = 0;
                //lru specific ends
                
                cache.evictAndInsertCacheLine(row, address);
            }
        }

        //debugging begins
        //cache.displayCache();
        //debugging ends
    }

    //measure time
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<milliseconds>(stop - start);

    //output
    printResult(duration.count(), cache);

    //lfu specific deallocation begins
    free(timesUsed);
    //lfu specific deallocation ends

    return 0;
}
