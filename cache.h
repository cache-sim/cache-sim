#pragma once

#include <cstring>
#include <vector>
#include <string>
#include <iostream>

//error handling
void printError(std::string errorMessage);

//helper functions

bool isPowerOfTwo(long long x);
int log2(long long x);
long long hexadecimalToDecimal(char hexVal[]);

//I/O

std::vector<long long> readTrace(char filePath[]); // takes in file path as parameter (use this)
std::vector<long long> readTrace(FILE *trace); // takes in file pointer as parameter 

//cache classes

class CacheLine {
    
    private:
        bool valid; // true when block is present in cache, false when no block is present
        long long tag;
        int blockSize;

    public:
        CacheLine();
        CacheLine(int blockSize);
        
        bool isValid();
        long long getTag();
        void setValid(bool valid);
        void setTag(long long tag); 
        void setBlockSize(long long blockSize);
};

class Cache {
    
    private:
        int numberOfRows; //Number of rows in cache
        int setAssociativity; //1 for direct mapped
        int numberOfSets;
        int indexSize, offsetSize;
        long long hits, misses;
        CacheLine* cacheLines;
        int* nextFreeBlockInSet;
        //set is full if nextFreeBlockInSet == setAssociativity

    public:
        Cache(int numberOfSets, int blockSize, int setAssociativity);

        void incHits(); //Must be called when there is a cache hit
        void incMisses(); //Must be called when there is a cache miss
        
        long long isDataInCache(long long address); //returns the row in cache data is present at
        long long isBlockInCache(long long index, long long tag);
        bool isSetFull(long long index);
        long long insertDataToCache(long long address); //will insert data only if free cacheLines are available and returns row where data inserted
        long long insertBlockToCache(long long index, long long tag);
        void evictAndInsertData(long long evictionAddress, long long insertionAddress); //A block is evicted when some other block is inserted
        void evictAndInsertBlock(long long eIndex, long long eTag, long long iIndex, long long iTag);
        void evictAndInsertBlock(long long row, long long insertionAddress); //row to be evicted, insertionAddress: address of incoming data
        
        double hitRate();
        double missRate();
        long long getNumberOfHits(); //for debugging
        long long getNumberOfMisses(); //for debugging
        long long getTagFromAddress(long long address);
        long long getIndexFromAddress(long long address);

        void displayCache(); //for debugging

        ~Cache(); //destructor
};
