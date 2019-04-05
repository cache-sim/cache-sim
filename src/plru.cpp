#include "cache.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef long long ll;

int main(int argc, char *argv[])
{

    ll numberOfSets = atoll(argv[1]);
    ll blockSize = atoll(argv[2]);
    ll setAssociativity = atoll(argv[3]);

    //initializing a cache with relevant parameters
    Cache cache(numberOfSets, blockSize, setAssociativity);

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
    int *tree = (int *)calloc(numberOfSets * (setAssociativity - 1), sizeof(int));

    //measure time
    auto start = high_resolution_clock::now();

    //iterate through all addresses accessed
    while(true)
    {

        ll address = getNextAddress();
        if(address == 0) break; //reached EOF

        //check if address is present in cache
        ll row = cache.isDataInCache(address); //returns row in the cache

        if (row != -1)
        { //cache hit

            //update the number of hits
            cache.incHits();

            //update the values in the tree
            int node = row % setAssociativity;      //the row in that set
            ll setNumber = row / setAssociativity;  //the set number to choose the corresponding tree
            node = node / 2 + setAssociativity / 2; //mapping the hit row to a leaf node in the tree
            if (row % 2 == 0)
            {
                tree[setNumber * (setAssociativity - 1) + node - 1] = 1; //left child was recently accessed
            }
            else
            {
                tree[setNumber * (setAssociativity - 1) + node - 1] = 0; //right child was recently accessed
            }

            while (node > 0)
            {
                if (node % 2 == 0)
                {
                    tree[setNumber * (setAssociativity - 1) + node / 2 - 1] = 1;
                }
                else
                {
                    tree[setNumber * (setAssociativity - 1) + node / 2 - 1] = 0;
                }
                node /= 2;
            }
        }
        else
        { //cache miss

            //update the number of misses
            cache.incMisses();

            int node;
            ll setNumber;
            if (!cache.isSetFull(cache.getIndexFromAddress(address)))
            {

                row = cache.insertData(address);
            }
            else
            {

                //finding the least recently used row in the set
                node = 1;
                setNumber = row / setAssociativity; //the set number to choose the corresponding tree
                while (node < setAssociativity / 2)
                {
                    if (tree[setNumber * (setAssociativity - 1) + node - 1] == 0)
                    {
                        node = node * 2;
                    }
                    else
                    {
                        node = node * 2 + 1;
                    }
                } //node can vary from setAssociativity/2 to setAssociativity-1

                if (tree[setNumber * (setAssociativity - 1) + node - 1] == 0)
                {
                    node = 2 * (node - setAssociativity / 2);
                }
                else
                {
                    node = 2 * (node - setAssociativity / 2) + 1;
                } //node can vary from 0 to setAssociativity-1

                //finding the row corresponding to calculated node in the cache
                row = cache.getIndexFromAddress(address) * setAssociativity + node;

                //replacing the row in the cache
                cache.evictAndInsertCacheLine(row, address);
            }

            //updating the tree
            node = row % setAssociativity;          //the row in that set
            setNumber = row / setAssociativity;     //the set number to choose the corresponding tree
            node = node / 2 + setAssociativity / 2; //mapping the hit row to a leaf node in the tree
            if (row % 2 == 0)
            {
                tree[setNumber * (setAssociativity - 1) + node - 1] = 1; //left child was recently accessed
            }
            else
            {
                tree[setNumber * (setAssociativity - 1) + node - 1] = 0; //right child was recently accessed
            }
            while (node > 0)
            {
                if (node % 2 == 0)
                {
                    tree[setNumber * (setAssociativity - 1) + node / 2 - 1] = 1;
                }
                else
                {
                    tree[setNumber * (setAssociativity - 1) + node / 2 - 1] = 0;
                }
                node /= 2;
            }
        }
    }

    //measure time
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    //printing the results
    printResult(duration.count(), cache);

    free(tree);
}
