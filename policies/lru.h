#pragma once

#include "cache.h"

class LRU : public Cache{
    private:
        long long time;
        long long *lastUsed;
    public:
        LRU(long long cacheSize, long long blockSize, long long setAssociativity, int level);
        long long getBlockToReplace(long long address) override;
        void update(long long blockToReplace, int status) override;

        ~LRU();
};
