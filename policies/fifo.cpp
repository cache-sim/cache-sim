#include "fifo.h"

#define ll long long

FIFO::FIFO(ll cacheSize, ll blockSize, ll setAssociativity, int level) :
    Cache(cacheSize, blockSize, setAssociativity, level, "FIFO"){
    fifoBlockInSet = (ll*) malloc(numberOfSets*sizeof(ll));
    memset(fifoBlockInSet, 0, numberOfSets*sizeof(ll));
}

ll FIFO::getBlockToReplace(ll address){
    ll index = getIndex(address);
    return fifoBlockInSet[index]+index*setAssociativity;
}

void FIFO::update(ll block, int status){
    if(status == 0) {
        ll index = block/setAssociativity;
        fifoBlockInSet[index] = (fifoBlockInSet[index]+1)%setAssociativity;
    }
}

FIFO::~FIFO(){
    free(fifoBlockInSet);
}
