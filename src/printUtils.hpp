#include "cache.h"
#include <stdio.h>
#include <typeinfo>

#define ll long long

ll getNextAddress(){
    char instruction[20], accessType, address[20];
    scanf("%s %c %s", instruction, &accessType, address);
    if(strcpy(instruction, "#eof") == 0) //return address 0 if end of line
        return 0;
    if(accessType == 'R')
        incReads();
    else
        incWrites();
    return hexToDec(address);
}

void printTraceInfo(){
    printf("\n");
    printf("Total Number Of Instructions: %lld\t\t\t Number of reads: %lld\n",
        getReads() + getWrites(), getReads());
    printf("\t\t\t\t\t\t\tNumber of writes: %lld\n",
        getWrites());
    printf("\n");
}

void printCacheStatus(Cache* cache){
    printf("L%d: %s\t\t\t\t\tTotal number of accesses: %lld\n",
        cache->getLevel(), cache->getPolicy().c_str(), cache->getHits() + cache->getMisses());
    printf("Hits: %lld\t\t\tMisses: %lld\t\t\tHit Rate: %f\n",
        cache->getHits(), cache->getMisses(), cache->getHitRate());
    printf("\n");
}