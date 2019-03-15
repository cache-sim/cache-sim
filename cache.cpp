//implementation of cache.h goes here

#include "cache.h"

//implementation of error handling
void printError(char *errorMessage) {
    printf("ERROR: %s\n", errorMessage);
}

//implementation of helper functions

bool isPowerOfTwo(long long x) {
    // First x in the below expression is for the case when x is 0 */
    return x && (!(x&(x-1))); 
}

//x is non_zero
int log2(long long x) {
    int power = 0;
    while(x > 1) {
        x = x>>1;
        power++;
    }
    return power;
}

long long hexadecimalToDecimal(char hexVal[]) {
    int len = strlen(hexVal);

    // Initializing base value to 1, i.e 16^0 
    long long base = 1;

    long long decVal = 0;

    // Extracting characters as digits from last character 
    for (int i=len-1; i>=2; i--) //2 to avoid 0x
    {
        // if character lies in '0'-'9', converting  
        // it to integral 0-9 by subtracting 48 from 
        // ASCII value. 
        if (hexVal[i]>='0' && hexVal[i]<='9')
        {
            decVal += (hexVal[i] - '0')*base;

            // incrementing base by power 
            base = base * 16;
        }

        // if character lies in 'A'-'F' , converting  
        // it to integral 10 - 15 by subtracting 55  
        // from ASCII value 
        else if (hexVal[i] >= 'a' && hexVal[i] <= 'f')
        {
            decVal += (hexVal[i] - 'a' + 10) * base;

            // incrementing base by power 
            base = base*16;
        }
    }

    return decVal;
}

//implementaion of I/O

std::vector<long long> readTrace(char filePath[]) {

    FILE *trace = fopen(filePath, "r");
    return readTrace(trace);
}

std::vector<long long> readTrace(FILE *trace) {
    /******************************************************
    * sample input line -> 0x7f110d39287e: R 0x7ffced08e7f8
    * end of line -> #eof 
    *******************************************************/

    std::vector<long long> addresses; // stores addresses of all data accesses
    char instruction[20], address[20];  
    fscanf(trace, "%s", instruction);

    while(instruction != "#eof") {
        fscanf(trace, "%*c %s", address);
        addresses.push_back(hexadecimalToDecimal(address));
        fscanf(trace, "%s", instruction);
    }

    return addresses;
}

//implementation of CacheLine class

CacheLine::CacheLine(int blockSize) {
    this->valid = false;
    this->tag = 0;
    this->blockSize = blockSize;
}

bool CacheLine::isValid() {
    return valid;
}

long long CacheLine::getTag() {
    return tag;
}

void CacheLine::setValid(bool valid) {
    this->valid = valid;
}

void CacheLine::setTag(long long tag) {
    this->tag = tag;
}


//implementation of Cache class

Cache::Cache(int numberOfRows, int blockSize, int setAssociativity = 1) 
: cacheLines(numberOfRows, CacheLine(blockSize)), nextFreeBlockInSet(numberOfRows/setAssociativity, 0)  {
    this->numberOfRows = numberOfRows;
    this->setAssociativity = setAssociativity;
    
    //error handling
    bool error = false;
    if(!isPowerOfTwo(numberOfRows)) {
        printError("Number of rows is not power of 2. (first parameter in Cache constructor)");
        error = true;
    }
    if(!isPowerOfTwo(blockSize)) {
        printError("Block size is not power of 2. (second parameter in Cache constructor)");
        error = true;
    }
    if(!isPowerOfTwo(setAssociativity)) {
        printError("Set Associativity rows is not power of 2. (third parameter in Cache constructor)");
        error = true;
    }
    if(numberOfRows%setAssociativity != 0) {
        printError("Number of rows is not divisble by Set Associativity.");
        error = true;
    }
    if(error == true) {
        return;
    }

    numberOfSets = numberOfRows/setAssociativity;
    offsetSize = log2(blockSize);
    indexSize = log2(numberOfSets);
}

void Cache::incHits() {
    hits++;
}

void Cache::incMisses() {
    misses++;
}

bool Cache::isDataInCache(long long address) {
    long long index = this->getIndexFromAddress(address);
    long long tag = this->getTagFromAddress(address);
    return this->isBlockInCache(index, tag);
}

bool Cache::isBlockInCache(long long index, long long tag) {
    
    for(int i = index*setAssociativity; i < index*setAssociativity + nextFreeBlockInSet[index]; i++) {
        if(cacheLines[i].getTag() == tag){
            return true;
        }
    }

    return false;
}

bool Cache::isSetFull(long long index) {
    return nextFreeBlockInSet[index] == setAssociativity;
}

void Cache::insertDataToCache(long long address) {
    long long index = this->getIndexFromAddress(address);
    long long tag = this->getTagFromAddress(address);
    return this->insertBlockToCache(index, tag);
}

void Cache::insertBlockToCache(long long index, long long tag) {
    int row = index * setAssociativity + nextFreeBlockInSet[index];
    cacheLines[row].setTag(tag);
    cacheLines[row].setValid(true);
    nextFreeBlockInSet[index]++;
}


void Cache::evictAndInsertData(long long evictionAddress, long long insertionAddress) {
    long long eIndex = this->getIndexFromAddress(evictionAddress);
    long long eTag = this->getTagFromAddress(evictionAddress);
    long long iIndex = this->getIndexFromAddress(insertionAddress);
    long long iTag = this->getTagFromAddress(insertionAddress);
    evictAndInsertBlock(eIndex, eTag, iIndex, iTag);
}

void Cache::evictAndInsertBlock(long long eIndex, long long eTag, long long iIndex, long long iTag) {
    
    if(eIndex != iIndex) {
        printError("evictAndInsertBlock: Evicting and Incoming block don't belong to the same set!");
        return;
    }
    else if(eTag == iTag) {
        printError("evictAndInsertBlock: Evicting and Inserting block are the same!");
        return;
    }

    long long index = iIndex;
    for(int i = index*setAssociativity; i < index*setAssociativity + nextFreeBlockInSet[index]; i++) {
        if(cacheLines[i].getTag() == eTag) {
            cacheLines[i].setTag(iTag);
        }
    }
}

double Cache::hitRate() {
    return (double)(hits)/(hits + misses);
}

double Cache::missRate() {
    return (double)(misses)/(misses + misses);
}

long long Cache::getNumberOfHits() {
    return hits;
}

long long Cache::getNumberOfMisses() {
    return misses;
}

long long Cache::getTagFromAddress(long long address) {
    return address>>(offsetSize+indexSize);
}

long long Cache::getIndexFromAddress(long long address) {
    return ((1<<indexSize - 1) & (address >> offsetSize));
}

void Cache::displayCache() {
    /****************************************
     * Display of one set with two elements
     * 2:              //index
     * |1|2345|        //valid bit and tag
     * |0|    |  
     ***************************************/

    int row = 0;
    for(int set = 0; set < numberOfSets; set++) {
        printf("%d:\n");
        for(int posInSet = 0; posInSet < setAssociativity; posInSet++) {
            printf("|%d|%40d|\n", cacheLines[row].isValid(), cacheLines[row].getTag());
            row++;
        }
    }

    printf("\n");
}