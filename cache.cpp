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
int log(long long x) {
    int power = 0;
    while(x > 0) {
        x = x>>1;
        power++;
    }
    return power;
}

//implementation of CacheLine class

CacheLine::CacheLine(int blockSize) {
    this->valid = false;
    this->tag = 0;
    this->blockSize = blockSize;
}

bool CacheLine::getValid() {
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
: cacheLines(numberOfRows, CacheLine(blockSize)) {
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
        printError("Set associativity is not divisible by number of rows.");
        error = true;
    }
    if(error == true) {
        return;
    }
}