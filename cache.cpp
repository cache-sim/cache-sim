//implementation of cache.h goes here

#include "cache.h"

//implementation of CacheLine class

CacheLine::CacheLine(bool valid, int tag, int blockSize) {
    this->valid = valid;
    this->tag = tag;
    this->blockSize = blockSize;
}

bool CacheLine::getValid() {
    return valid;
}

int CacheLine::getTag() {
    return tag;
}

void CacheLine::setValid(bool valid) {
    this->valid = valid;
}

void CacheLine::setTag(int tag) {
    this->tag = tag;
}
