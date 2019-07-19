#include "srrip.h"

#define ll long long

//Static re-reference interval prediction(srrip) cache replacement algorithm implementation 

/*
    note: rr = re-reference; nv = no victim

    SRRIP consists of 4 states
        State 0: Immedidate
        State 1: Intermediate
        State 2: Far
        State 3: Distant

                        Entry
                         |
                         |
    (0)--nv-->(1)--nv-->(2)--nv-->(3)--nv-->Evict
     |____rr___|         |         |
     |________rr_________|         |
     |_____________rr______________|

*/

SRRIP::SRRIP(ll cacheSize, ll blockSize, ll setAssociativity, int level) : 
    Cache(cacheSize, blockSize, setAssociativity, level, "SRRIP"){
        state = (int *)calloc(numberOfSets * (setAssociativity), sizeof(int));
        if(state == NULL){
            printf("Failed to allocate memory for data members of LRU (L%d) cache\n", level);
            exit(0);
        } else {
            for(ll i=0;i<numberOfSets*(setAssociativity);i++)
                state[i] = -1;
        }
    }

ll SRRIP::getBlockToReplace(ll address){
    
    //in an event of a miss the following steps have to be carried out if no empty slots are available
    // Step 1: search for a block with state 3 in that set
    // Step 2: if 3 is found, go to step 5
    // Step 3: increment state values
    // Step 4: goto step 1
    // Step 5: replace block and set state value to 2 for the new block


    int index = getIndex(address);
    ll block_begin = index*setAssociativity;
    ll block_end = (index+1)*setAssociativity;

    //check if empty slot is available
    for(ll block = block_begin; block < block_end; block++)
        if(state[block]==-1)
            return block;

    //if none found, search for block with state 3
    while(true){
        //Step 1
        for(ll block=block_begin; block < block_end; block++){
            //if state is 3, Step 5
            if(state[block]==3)
                return block;
        }

        //state 3 not found, Step 3
        for(ll i=block_begin; i < block_end; i++){
            state[i]++;
        }
    }
}

void SRRIP::update(ll block, int status){
    state[block] = (status == 1) ? 0 : 2;
}

SRRIP::~SRRIP(){
    free(state);
}
