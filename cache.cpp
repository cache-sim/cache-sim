//implementation of cache.h goes here

#include "cache.h"

//implementation of error handling
void printError(char *errorMessage) {
    printf("ERROR: %s\n", errorMessage);
}

//implementation of helper functions

bool isPowerOfTwo(long long x) {
    // First x in the below expression is for the case when x is 0 */
    return x && (!(x&(x-1))); 
}

//x is non_zero
int log(long long x) {
    int power = 0;
    while(x > 1) {
        x = x>>1;
        power++;
    }
    return power;
}

long long hexadecimalToDecimal(char hexVal[]) {
    int len = strlen(hexVal);

    // Initializing base value to 1, i.e 16^0 
    long long base = 1;

    long long decVal = 0;

    // Extracting characters as digits from last character 
    for (int i=len-1; i>=2; i--) //2 to avoid 0x
    {
        // if character lies in '0'-'9', converting  
        // it to integral 0-9 by subtracting 48 from 
        // ASCII value. 
        if (hexVal[i]>='0' && hexVal[i]<='9')
        {
            decVal += (hexVal[i] - '0')*base;

            // incrementing base by power 
            base = base * 16;
        }

        // if character lies in 'A'-'F' , converting  
        // it to integral 10 - 15 by subtracting 55  
        // from ASCII value 
        else if (hexVal[i] >= 'a' && hexVal[i] <= 'f')
        {
            decVal += (hexVal[i] - 'a' + 10) * base;

            // incrementing base by power 
            base = base*16;
        }
    }

    return decVal;
}

//implementaion of I/O

std::vector<long long> readTrace(char filePath[]) {

    FILE *trace = fopen(filePath, "r");
    return readTrace(trace);
}

std::vector<long long> readTrace(FILE *trace) {
    /******************************************************
    * sample input line -> 0x7f110d39287e: R 0x7ffced08e7f8
    * end of line -> #eof 
    *******************************************************/

    std::vector<long long> addresses; // stores addresses of all data accesses
    char instruction[20], address[20];  
    fscanf(trace, "%s", instruction);

    while(instruction != "#eof") {
        fscanf(trace, "%*c %s", address);
        addresses.push_back(hexadecimalToDecimal(address));
    }

    return addresses;
}

//implementation of CacheLine class

CacheLine::CacheLine(int blockSize) {
    this->valid = false;
    this->tag = 0;
    this->blockSize = blockSize;
}

bool CacheLine::getValid() {
    return valid;
}

long long CacheLine::getTag() {
    return tag;
}

void CacheLine::setValid(bool valid) {
    this->valid = valid;
}

void CacheLine::setTag(long long tag) {
    this->tag = tag;
}


//implementation of Cache class

Cache::Cache(int numberOfRows, int blockSize, int setAssociativity = 1) 
: cacheLines(numberOfRows, CacheLine(blockSize)) {
    this->numberOfRows = numberOfRows;
    this->setAssociativity = setAssociativity;
    
    //error handling
    bool error = false;
    if(!isPowerOfTwo(numberOfRows)) {
        printError("Number of rows is not power of 2. (first parameter in Cache constructor)");
        error = true;
    }
    if(!isPowerOfTwo(blockSize)) {
        printError("Block size is not power of 2. (second parameter in Cache constructor)");
        error = true;
    }
    if(!isPowerOfTwo(setAssociativity)) {
        printError("Set Associativity rows is not power of 2. (third parameter in Cache constructor)");
        error = true;
    }
    if(numberOfRows%setAssociativity != 0) {
        printError("Number of rows is not divisble by Set Associativity.");
        error = true;
    }
    if(error == true) {
        return;
    }

    numberOfSets = numberOfRows/setAssociativity;
    offsetSize = log(blockSize);
    indexSize = log(numberOfSets);
}

long long Cache::getTagFromAddress(long long address) {
    return address>>(offsetSize+indexSize);
}
