#include "nru.h"

#define ll long long

NRU::NRU(ll cacheSize, ll blockSize, ll setAssociativity, int level) :
    Cache(cacheSize, blockSize, setAssociativity, level, "NRU"){
        recentlyUsed = (bool*) calloc(setAssociativity * numberOfSets, sizeof(bool));
        nRecentlyUsed = (ll*) calloc(numberOfSets, sizeof(ll));
    }

ll NRU::getBlockToReplace(ll address){
    int index = getIndex(address);
    for(int block = index * setAssociativity; block < (index+1) * setAssociativity; block++){
        if(recentlyUsed[block] == false){
            return block;
        }
    }
}

void NRU::update(ll block, int status){
    if(recentlyUsed[block] != true){
        recentlyUsed[block] = true;
        nRecentlyUsed[block/setAssociativity]++;
        
        if(nRecentlyUsed[block/setAssociativity] == setAssociativity){
            int startingBlock = (block/setAssociativity) * setAssociativity;
            for(int tempBlock = startingBlock; tempBlock < startingBlock + setAssociativity; tempBlock++){
                if(tempBlock != block){
                    recentlyUsed[tempBlock] = false;
                }
            }
            nRecentlyUsed[block/setAssociativity] = 1;
        }
    }
}

NRU::~NRU(){
    free(recentlyUsed);
    free(nRecentlyUsed);
}