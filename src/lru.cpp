#include "lru.h"

#define ll long long

LRU::LRU(ll cacheSize, ll blockSize, ll setAssociativity) : 
    Cache(cacheSize, blockSize, setAssociativity){
        lastUsed = (ll *)calloc(numberOfSets * (setAssociativity), sizeof(ll));
}

ll LRU::getBlockToReplace(ll address){
     int index = getIndex(address);
     int min_block = index*setAssociativity;
     for(int block = index*setAssociativity; block < (index+1)*setAssociativity; block++) {
         if(lastUsed[block] == 0) // an empty slot
             return block;
         if(lastUsed[block] < lastUsed[min_block]) {
             min_block = block;
         }
     }
     return min_block; 
}

void LRU::update(ll block, int status){
    lastUsed[block] = time;
    time++;
}

LRU::~LRU(){
    free(lastUsed);
}
