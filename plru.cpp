#include "cache.h"

#define ll long long

class plru : public Cache{
    
    private:
        /*
        Tree with setAssociativity-1 nodes to keep track of least recently accessed element in cache.
        A tree which would look like
        
                        1
                      /   \
                     2     3
                    / \   / \
                   4   5 6   7
        
        is represented in the array as 
            _____________________________
            | 1 | 2 | 3 | 4 | 5 | 6 | 7 |
            
        for an 8-way setAssociativity
        */
        bool *tree = (bool *)calloc(getNumberOfSets() * (getSetAssociativity() - 1), sizeof(bool));
        ll* cacheBlocks = getCacheBlocks();
        ll setAssociativity = getSetAssociativity();
        
    public:
        ll getBlockToReplace(ll address){
            
            int node = 1;
            ll index = getIndex(address);
            while(node < setAssociativity){
                if(tree[index*setAssociativity + node - 1]){
                    node = node*2 + 1;
                }
                else{
                    node = node*2;
                }
            } //node can vary from setAssociativity to 2*setAssociativity-1
            return index*setAssociativity + node - setAssociativity;
        }

        void update(ll blockToReplace, int status){
            ll node = (blockToReplace % setAssociativity) + setAssociativity;
            ll setNumber = blockToReplace / setAssociativity;
            while(node>1){
                if(node%2){
                    cacheBlocks[setNumber*(setAssociativity-1) + node/2] = false;
                }
                else{
                    cacheBlocks[setNumber*(setAssociativity-1) + node/2] = true;
                }
                node /= 2;
            }
        }
};