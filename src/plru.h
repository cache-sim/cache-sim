#pragma once

#include "cache.h"

class PLRU : public Cache{
    private:
        bool* tree;

    public:
        PLRU(long long cacheSize, long long blockSize, long long setAssociativity);
        long long getBlockToReplace(long long address) override;
        void update(long long blockToReplace, int status) override;

        ~PLRU() override;
};