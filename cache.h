#pragma once

#include <cstdio>

#define ADDRESS_BITS 64        //assuming 64 bits in address

//error handling
void printError(char *errorMessage);

class CacheLine {
    
    private:
        bool valid; // true when block is present in cache, false when no block is present
        int tag;
        int blockSize;

    public:
        CacheLine(int blockSize);
        
        bool getValid();
        int getTag();
        void setValid(bool valid);
        void setTag(int tag); 
};

class Cache {
    
    private:
        int numberOfRows; //Number of rows in cache
        int setAssociativity; //1 for direct mapped
        int numberOfSets;
        int indexSize, offsetSize;
        int hits, misses;
        CacheLine *cacheLines;

    public:
        Cache(int numberOfRows, int setAssociativity = 1);

        void incHits();
        void incMisses();
        bool isDataInCache(int dataAddress);
        void insertDataToCache(int dataAddress); //will insert data only if free cacheLines are available
        void evictBlock(int setIndex, int posInSet = 0); //not sure if it will be useful?
        void displayCache(); //for debugging
        double hitRate();
        double missRate();
};
