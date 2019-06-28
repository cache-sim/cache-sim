#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// classes defined
class Cache;

// helper functions
bool isValidConfig(long long cs, long long bs, long long sa);
long long hexToDec(char hexVal[]);
int log2(long long x);

// I/O
long long getNextAddress();
void printResult(Cache* cache);

// cache class
class Cache{

    private:
        long long cacheSize;
        long long blockSize;
        long long setAssociativity;
        long long numberOfSets;
        int offsetSize;
        int indexSize;
        long long hits, misses;
        long long* cacheBlocks;

    public:
        Cache(long long cacheSize, long long blockSize, long long setASsociativity);

        void incHits();
        void incMisses();
        long long getTag(long long address);
        long long getIndex(long long address);
        long long getBlockPosition(long long address);
        void insert(long long address, long long blockToReplace);

        long long getHits();
        long long getMisses();
        float getHitRate();

        ~Cache();

};