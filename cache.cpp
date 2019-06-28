#include "cache.h"

#define ll long long

ll readCounter=0, writeCounter=0;

// helper functions

bool isValidConfig(ll  cs, ll bs, ll sa){ // needs more analysis
    if(cs/(bs*sa) - (int)(cs/(bs*sa)) < 0.0001){ // ignoring precision errors
        return true;
    }
    else {
        return false;
    }
}

ll hexToDec(char hexVal[]) {
    int len = strlen(hexVal);

    // Initializing base value to 1, i.e 16^0 
    ll base = 1;
    ll decVal = 0;

    for (int i=len-1; i>=2; i--){ //2 to avoid 0x
        if (hexVal[i]>='0' && hexVal[i]<='9'){
            decVal += (hexVal[i] - '0')*base;
        }
        else if (hexVal[i] >= 'a' && hexVal[i] <= 'f'){
            decVal += (hexVal[i] - 'a' + 10) * base;
        }
        base *= 16;
    }
    return decVal;
}

int log2(long long x) {
    int power = 0;
    while(x > 1) {
        x = x>>1;
        power++;
    }
    return power;
}

// I/O

ll getNextAddress(){
    char instruction[20], accessType, address[20];
    scanf("%s %c %s", instruction, &accessType, address);
    if(strcpy(instruction, "#eof") == 0) //return address 0 if end of line
        return 0;
    if(accessType == 'R')
        readCounter++;
    else
        writeCounter++;
    return hexToDec(address);
}

void printResult(Cache* cache){
    printf("Total Number of data accesses: %lld\n", cache->getHits() + cache->getMisses());
    printf("Hits: %lld\n", cache->getHits());
    printf("Misses: %lld\n", cache->getMisses());
    printf("Hit Ratio: %f\n", cache->getHitRate() * 100);
}

// Cache class

Cache::Cache(ll cacheSize, ll blockSize, ll setAssociativity){
    
    if(!isValidConfig(cacheSize, blockSize, setAssociativity)){
        printf("Invalid Cache configuration\n");
    }

    this->cacheSize = cacheSize;
    this->blockSize = blockSize;
    this->setAssociativity = setAssociativity;

    cacheBlocks = (ll*)malloc(cacheSize/blockSize * sizeof(ll));

    numberOfSets = cacheSize/(blockSize*setAssociativity);
    offsetSize = log2(blockSize);
    indexSize = log2(numberOfSets);

}

void Cache::incHits(){
    hits++;
}

void Cache::incMisses(){
    misses++;
}

long long Cache::getTag(long long address){
    return address>>(indexSize + offsetSize);
}

long long Cache::getIndex(long long address){
    return (address>>offsetSize) & ((1<<indexSize)-1);
}

long long Cache::getBlockPosition(long long address){
    long long index = getIndex(address);
    long long tag = getTag(address);
    long long iterator;
    for(iterator=index*setAssociativity; iterator<index*(setAssociativity+1); iterator++){
        if(tag == cacheBlocks[iterator]){
            return iterator;
        }
    }
    if(iterator == index*(setAssociativity+1)){
        return -1;
    }
}

void Cache::insert(long long address, long long blockToReplace){
    cacheBlocks[blockToReplace] = address;
}

long long Cache::getHits(){
    return hits;
}

long long Cache::getMisses(){
    return misses;
}

float Cache::getHitRate(){
    return (100.0*hits)/(hits+misses);
}