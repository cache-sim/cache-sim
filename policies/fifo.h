#pragma once

#include "../src/cache.h"
#include <queue>
#include <vector>

class FIFO : public Cache{
    private:
        std::vector<std::queue<long long>> queueOfBlocks;
        bool *isBlockEmpty;

    public:
        FIFO(long long cacheSize, long long blockSize, long long setAssociativity, int level);
        long long getBlockToReplace(long long address) override;
        void update(long long blockToReplace, int status) override;

        ~FIFO();
};
