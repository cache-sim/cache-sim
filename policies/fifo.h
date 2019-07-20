#pragma once

#include "../src/cache.h"

class FIFO : public Cache{
    private:
        long long *fifoBlockInSet; // each element varies from [0, setAssociativity-1]

    public:
        FIFO(long long cacheSize, long long blockSize, long long setAssociativity, int level);
        long long getBlockToReplace(long long address) override;
        void update(long long blockToReplace, int status) override;

        ~FIFO();
};
