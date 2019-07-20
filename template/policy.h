#pragma once

#include "../src/cache.h"

class <POLICY> : public Cache{
    private:
        // declare policy specific data structures

    public:
        <POLICY>(long long cacheSize, long long blockSize, long long setAssociativity, int level);
        long long getBlockToReplace(long long address) override;
        void update(long long blockToReplace, int status) override;

        ~<POLICY>();
}