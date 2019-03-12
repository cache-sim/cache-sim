#pragma once

#include <cstdio>
#include <cstring>
#include <vector>

#define ADDRESS_BITS 64        //assuming 64 bits in address

//error handling
void printError(char *errorMessage);

//helper functions

bool isPowerOfTwo(long long x);
int log(long long x);
long long hexadecimalToDecimal(char hexVal[]);

//I/O
std::vector<long long> readTrace(FILE *trace);

//cache classes

class CacheLine {
    
    private:
        bool valid; // true when block is present in cache, false when no block is present
        long long tag;
        int blockSize;

    public:
        CacheLine(int blockSize);
        
        bool getValid();
        long long getTag();
        void setValid(bool valid);
        void setTag(long long tag); 
};

class Cache {
    
    private:
        int numberOfRows; //Number of rows in cache
        int setAssociativity; //1 for direct mapped
        int numberOfSets;
        int indexSize, offsetSize;
        int hits, misses;
        std::vector<CacheLine> cacheLines;
        long long getTagFromAddress(long long address);

    public:
        Cache(int numberOfRows, int blockSize, int setAssociativity = 1);

        void incHits();
        void incMisses();
        bool isDataInCache(int dataAddress);
        void insertDataToCache(int dataAddress); //will insert data only if free cacheLines are available
        void evictBlock(int setIndex, int posInSet = 0); //not sure if it will be useful?
        void displayCache(); //for debugging
        double hitRate();
        double missRate();
};
