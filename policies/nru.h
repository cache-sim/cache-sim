#pragma once

#include "../src/cache.h"

class NRU : public Cache{
    private:
        bool* recentlyUsed;
        long long* allUsed;

    public:
        NRU(long long cacheSize, long long blockSize, long long setAssociativity, int level);
        long long getBlockToReplace(long long address) override;
        void update(long long blockToReplace, int status) override;

        ~NRU();
};