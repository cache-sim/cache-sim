#include "fifo.h"
#include <iostream>

#define ll long long

FIFO::FIFO(ll cacheSize, ll blockSize, ll setAssociativity, int level) :
    Cache(cacheSize, blockSize, setAssociativity, level, "FIFO"), queueOfBlocks(numberOfSets){
    nextEmptyBlockInSet = new ll(numberOfSets);
    for(ll set = 0; set < numberOfSets; set++) {
        nextEmptyBlockInSet[set] = set*setAssociativity;
    }
}

ll FIFO::getBlockToReplace(ll address){
    ll index = getIndex(address);
    ll blockToReplace;
    if(queueOfBlocks[index].size() < setAssociativity) { // If set is not full
        blockToReplace = nextEmptyBlockInSet[index]++;
    }
    else {
        blockToReplace = queueOfBlocks[index].front(); // get First In block
        queueOfBlocks[index].pop(); // remove First In block
    }
    return blockToReplace;
}

void FIFO::update(ll block, int status){
    if(status == 0) { // push to queue if miss
        ll index = block/setAssociativity;
        queueOfBlocks[index].push(block);
    }
}

FIFO::~FIFO(){
    
}
