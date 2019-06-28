#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <fstream.h>
#include "cache.h" //contains all auxillary functions
#include "plru.h"
// #include "policy.h"

using namespace std;
using namespace std::chrono;

typedef long long ll; 

Cache* createInstance(string& policy, ll cs, ll bs, ll sa){
    
    // check validity here and exit if invalid
    if(!strcmp(policy, "plru")){
        Cache* cache = new PLRU(cs, bs, sa);
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
    string line;
    getline(params, line);
    int levels = stoi(line);
    vector<Cache*> cache(levels);

    int iterator = 0;
    while(params){
        string policy;
        getline(params, policy);
        int cs, bs, sa; //cacheSize, blockSize, setAssociativity
        getline(params, line); cs = stoll(line);
        getline(params, line); bs = stoll(line);
        getline(params, line); sa = stoll(line);
        cache[iterator++] = createCacheInstance(policy, cs, bs, sa);
    }

    auto start = high_resolution_clock::now();

    while(true){
        ll address = getNextAddress();
        if(address == -1) break; //reached eof

        for(int levelItr=0; levelIt<levels; levelItr++){
            ll block = cache[levelItr]->getBlockPosition(address);
            // getBlockPosition will be implemented in cache.cpp
            if(block == -1){ //cache miss
                cache[levelItr]->incMisses();
                // incMisses will be implemented in cache.cpp
                ll blockToReplace = getBlockToReplace();
                // getBlockToEvict will be implemented in policy.cpp
                cache[levelItr]->insert(address, blockToReplace);
                // insert will be implemented in cache.cpp
                cache[levelItr]->update(blockToReplace, 0);
                // update will be implemented in policy.cpp; will include updating the tree as in plru or updating the count as in lfu; 0 denotes miss
                continue;
            }
            else{ //cache hit
                cache[levelItr]->incHits();
                // incHits will be implemented in cache.cpp
                cache[levelItr]->update(block, 1);
                // update will be implemented in policy.cpp; 1 denotes hit
            }
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop-start);

    for(int leveltr=0; levelItr<levels; levelItr++){
        cache[levelItr]->printResults();
        // will be implemented in cache.cpp
        delete cache[levelItr];
    }

    return 0;
}