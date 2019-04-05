//not recently used cache replacement implementation

#include "cache.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef long long ll;


//nru specific data structures begin

ll *recentlyUsed;

//nru specific data structures end

int main(int argc,char *argv[]){
    
        vector<ll> addresses = readTrace(argv[1]); //read the trace input file

        ll numberOfSets = atoll(argv[2]);
        ll blockSize = atoll(argv[3]);
        ll setAssociativity = atoll(argv[4]);
        

        //nru specific allocation begins        
        recentlyUsed = (ll*)calloc(setAssociativity * numberOfSets , sizeof(ll));
        ll noOfCacheLines = setAssociativity * numberOfSets ; 
        ll counter = 0;
        //nru specific allocation ends

            Cache cache(numberOfSets,blockSize,setAssociativity); //Cache initialisation with relevant parameters

        //Measure Time
        auto start = high_resolution_clock::now();

            //Go through all addresses

            while(true){

                    ll address = getNextAddress();
                    if(address == 0) break;

                ll row; // row in cache
                
                if((row = cache.isDataInCache(address)) != -1){ //cache hit
                        //isDataInCache returns the row number where the data is present
                        cache.incHits();//increment hits

                        //nru specific begins
                        recentlyUsed[row] = 1;
                        //nru specific ends
                }
                else{//cache miss
                        cache.incMisses();//increment misses

                        ll index = cache.getIndexFromAddress(address); //value of index of address
                        ll tag = cache.getTagFromAddress(address); //value of tag of address
                        ll rowBegin = index * setAssociativity; 
                        ll rowEnd = (index + 1) * setAssociativity;

                        //rows between rowBegin(inclusive) and rowEnd(exclusive) must be considered for eviction

                        //NOTE: the row number of the block to be evicted must be stored in row!!
                        //lru specific begins

                        if(!cache.isSetFull(cache.getIndexFromAddress(address))){
                                row = cache.insertData(address);
                        
                                //nru speicfic begins
                                recentlyUsed[row] = 1;
                                //nru specific ends
                        }
                        else{
                                ll temp = rowBegin;
                                //nru specific begins
                                while(recentlyUsed[temp] == 1 && temp != rowEnd){ // check for a random cacheline which is not recently used in that set
                                        temp++;
                                }

                                recentlyUsed[temp] = 1; //update the row as recently used
                                //nru specific ends
                                cache.evictAndInsertCacheLine(temp,address);        
                        }
                
                        //nru specific begins
                
                        for(ll j = rowBegin ;j < rowEnd; j++ ){
                                if(recentlyUsed[j] == 1)
                                        counter++;
                        }
                        if(counter == setAssociativity){
                                for(ll i = rowBegin;i < rowEnd;i++){
                                        if(i == row){
                                        continue;
                                        }
                                        recentlyUsed[i] = 0;
                                }
                                counter = 0;
                        }
                        //nru specific ends
                }
        
        }

    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start);

    //output
    cout << "Simulation time : " << duration.count() << " ms" << endl;
    printf("Total Number of data accesses: %lld\n", cache.getNumberOfHits() + cache.getNumberOfMisses());
    printf("Hits: %lld\n", cache.getNumberOfHits());
    printf("Misses: %lld\n", cache.getNumberOfMisses());
    printf("Hit Ratio: %f\n", cache.hitRate() * 100);

    free(recentlyUsed);

    return 0;
}