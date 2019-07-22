#include "lfu.h"

#define ll long long

LFU::LFU(ll cacheSize, ll blockSize, ll setAssociativity, int level) :
    Cache(cacheSize, blockSize, setAssociativity, level, "LFU"){
        timesUsed = (ll *)calloc(numberOfSets * (setAssociativity), sizeof(ll));
        if(timesUsed == NULL){
            printf("Failed to allocate memory for data members of LFU (L%d) cache\n", level);
            exit(0);
        }
    }

ll LFU::getBlockToReplace(ll address){
    int index = getIndex(address);
    int min_block = index*setAssociativity;
    for(int block = index*setAssociativity; block < (index+1)*setAssociativity; block++) {
        if(timesUsed[block] == 0) // an empty slot
            return block;
        if(timesUsed[block] < timesUsed[min_block]) {
            min_block = block;
        }
    }
    return min_block;
}

void LFU::update(ll block, int status){
    if(status == 1){
        timesUsed[block]++;
    }
    else{
        timesUsed[block] = 0;
    }
}

LFU::~LFU(){
    free(timesUsed);
}