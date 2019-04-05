//Static re-reference interval prediction(srrip) cache replacement algorithm implementation 

/*
	note: rr = re-reference; nv = no victim
	SRRIP consists of 4 states
		State 0: Immedidate
		State 1: Intermediate
		State 2: far
		State 3: distant

	 					entry
						 |
						 |
	(0)--nv-->(1)--nv-->(2)--nv-->(3)--nv-->evict
	 |____rr___|		 |		   |
	 |________rr_________|		   |
	 |_____________rr______________|

*/

#include "cache.h"
#include <chrono>
using namespace std;
using namespace std::chrono;

typedef long long ll;

//srrip specific data structure
ll *state;

int main(int argc, char *argv[]){

    ll numberOfSets = atoll(argv[1]); //atoll : char* to long long
    ll blockSize = atoll(argv[2]);
    ll setAssociativity = atoll(argv[3]);

    //allocating memory to state equal to that of cache
    state = (ll *)malloc(numberOfSets * setAssociativity * sizeof(ll));
    //allocation ends

    Cache cache(numberOfSets, blockSize, setAssociativity); //initialise a cache with relevant parameters

    //measure time
    auto start = high_resolution_clock::now();

	//go through all addresses
    while(true){
        //address : the current address accessed by CPU
        ll address = getNextAddress();
        if(address == 0) break; //reached EOF

        //check if address is present in cache
        ll row;//row in cache
        if((row= cache.isDataInCache(address)) != -1) { //cache hit
            //isDataInCache returns the row number where the data is present
            cache.incHits();  //increment hits

            //change state of hit row to 0
            //srrip specific implementation
            state[row] = 0;

        } else {//cache miss 
            cache.incMisses(); //increment misses

            if(!cache.isSetFull(cache.getIndexFromAddress(address))){ //if empty slot is available to insert data
                row = cache.insertData(address);
                //insertDataToCache returns the row where the data is inserted

                //srrip specific implementation begins
                //entry state of every new cache block
                state[row] = 2;
                //srrip specific implementation ends
            } else { //no empty slot available to insert data

                ll index = cache.getIndexFromAddress(address); //value of index of address
                ll rowBegin = index * setAssociativity; 
                ll rowEnd = (index + 1) * setAssociativity;
                
                /*
                    following steps need to be carried out
                    Step 1: search for a block with state 3 in that set
                    Step 2: if 3 is found, go to step 5
                    Step 3: increment state values
                    Step 4: goto step 1
                    Step 5: replace block and set state value to 2 for the new block
                */

                ll rowToBeReplaced = 0;
                while(true){
                    bool state_3_exists_flag = false; //checks if there exists a block in state 3 to evict
                    //step 1
                    for(ll i=rowBegin;i<rowEnd;i++){
                        if(state[i]==3){
                            rowToBeReplaced = i; //this is the row to be replaced
                            state_3_exists_flag = true;
                            break;
                        }
                    }
                    //step 2
                    if(state_3_exists_flag) //if exists break and replace
                        break;
                    //step 3 and 4
                    //else increment each state by 1 and recheck 
                    else 
                        for(ll i=rowBegin;i<rowEnd;i++)
                            state[i]++;
                }

                //row to be replaced will have state 3
                //step 5
                cache.evictAndInsertCacheLine(rowToBeReplaced, address);
                state[rowToBeReplaced] = 2;
            }
        }
    }
    
    //measure time
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<milliseconds>(stop - start);

    //output
    cout << "Simulation time : " << duration.count() << " ms" << endl;
    printf("Total Number of data accesses: %lld\n", cache.getNumberOfHits() + cache.getNumberOfMisses());
    printf("Hits: %lld\n", cache.getNumberOfHits());
    printf("Misses: %lld\n", cache.getNumberOfMisses());
    printf("Hit Ratio: %f\n", cache.hitRate() * 100);

    free(state);

    return 0;

}