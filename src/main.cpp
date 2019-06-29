#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <fstream>
#include <vector>
#include <sstream>
#include "cache.h" //contains all auxillary functions
#include "plru.h"
#include "lru.h"
// #include "policy.h"

using namespace std;
using namespace std::chrono;

#define ll long long

Cache* createCacheInstance(string& policy, ll cs, ll bs, ll sa){
    
    // check validity here and exit if invalid
    if(policy == "plru"){
        Cache* cache = new PLRU(cs, bs, sa);
        return cache;
    }
    else if(policy == "lru"){
        Cache* cache = new LRU(cs, bs, sa);
        return cache;
    }
    // else if(!strcmp(policy, <"policy">)){
    //     Cache* cache = new <policy>(cs, bs, sa);
    //     return cache;
    // }
     
}

int main(int argc, char *argv[]){
        
    ifstream params;
    params.open("params.cfg");
    string word;
    params >> word;
    int levels = stoi(word.c_str());

    vector<Cache*> cache(levels);

    int iterator = 0;
    while(!params.eof()){
        string policy;
        params >> policy;
        ll cs, bs, sa; //cacheSize, blockSize, setAssociativity
        params >> word; cs = stoll(word.c_str());
        params >> word; bs = stoll(word.c_str());
        params >> word; sa = stoll(word.c_str());
        cache[iterator++] = createCacheInstance(policy, cs, bs, sa);
    }

    auto start = high_resolution_clock::now();

    while(true){
        ll address = getNextAddress();
        if(address == 0) break; //reached eof

        for(int levelItr=0; levelItr<levels; levelItr++){
            ll block = cache[levelItr]->getBlockPosition(address);
            // getBlockPosition will be implemented in cache.cpp
            if(block == -1){ //cache miss
                cache[levelItr]->incMisses();
                // incMisses will be implemented in cache.cpp
                ll blockToReplace = cache[levelItr]->getBlockToReplace(address);
                // getBlockToEvict will be implemented in policy.cpp
                cache[levelItr]->insert(address, blockToReplace);
                // insert will be implemented in cache.cpp
                cache[levelItr]->update(blockToReplace, 0);
                // update will be implemented in policy.cpp; will include updating the tree as in plru or updating the count as in lfu; 0 denotes miss
            }
            else{ //cache hit
                cache[levelItr]->incHits();
                // incHits will be implemented in cache.cpp
                cache[levelItr]->update(block, 1);
                // update will be implemented in policy.cpp; 1 denotes hit
                break;
            }
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop-start);

    for(int levelItr=0; levelItr<levels; levelItr++){
        printResult(cache[levelItr]);
        // will be implemented in cache.cpp
        delete cache[levelItr];
    }

    return 0;
}
