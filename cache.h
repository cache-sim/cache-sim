#pragma once

class CacheLine {
    
    private:
        bool vaild, dirty;
        int tag;
        int blockSize;

    public:
        CacheLine(bool valid, bool dirty, int tag, int blockSize);
        
        bool getValid();
        bool getDirty();
        int getTag();
        void setValid(bool valid);
        void setDirty(bool dirty);
        void setTag(int tag); 
};

class Cache {
    
    private:
        int cacheSize;
        int setAssociativity; //1 for direct mapped
        int numberOfSets;
        int indexSize, offsetSize;
        int hits, misses;
        CacheLine *cacheLines;

    public:
        Cache(int cacheSize, int setAssociativity = 1);

        void incHits();
        void incMisses();
        bool isDataInCache(int dataAddress);
        void insertDataToCache(int dataAddress); //will insert data only if free cacheLines are available
        void evictBlock(int setIndex, int posInSet = 0); //not sure if it will be useful?
        void displayCache(); //for debugging
        double hitRate();
        double missRate();
};
