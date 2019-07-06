#include "policy.h"

#define ll long long

<POLICY>::<POLICY>(ll cacheSize, ll blockSize, ll setAssociativity) :
    Cache(cacheSize, blockSize, setAssociativity){
        // initialize policy specific data structure 
    }

ll <POLICY>::getBlockToReplace(ll address){
    // write policy logic
}

void <POLICY>::update(ll block, int status){
    // write policy logic
}

<POLICY>::~<POLICY>(){
    // free declared data structures
}