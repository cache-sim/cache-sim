#include "nru.h"

#define ll long long

NRU::NRU(ll cacheSize, ll blockSize, ll setAssociativity, int level) :
    Cache(cacheSize, blockSize, setAssociativity, level, "NRU"){
        recentlyUsed = (bool*) calloc(setAssociativity * numberOfSets, sizeof(bool));
        allUsed = (ll*) calloc(numberOfSets, sizeof(ll));
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
    recentlyUsed[block] = true;
    allUsed[block/setAssociativity]++;
    
    if(allUsed[blockSize/setAssociativity] == setAssociativity){
        int startingBlock = (block/setAssociativity) * setAssociativity;
        for(int tempBlock = startingBlock; tempBlock < startingBlock + setAssociativity; tempBlock++){
            if(tempBlock != block){
                recentlyUsed[tempBlock] = false;
            }
        }
    }
}

NRU::~NRU(){
    free(recentlyUsed);
    free(allUsed);
}