//implementation of cache.h goes here

#include "cache.h"

//implementation of error handling
void printError(char *errorMessage) {
    printf("ERROR: %s\n", errorMessage);
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

Cache::Cache(int numberOfRows, int blockSize, int setAssociativity = 1) : cacheLines(numberOfRows, blockSize) {
    this->numberOfRows = numberOfRows;
    this->setAssociativity = setAssociativity;
    if(numberOfRows%setAssociativity == 0) {
        numberOfSets = numberOfRows/setAssociativity;
        // continue from here
    }
    else {
        printError("Set associativity is not divisible by number of row.");
    }
}