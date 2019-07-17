# cache-sim  

*A multi-level cache simulator built using C++.*  

## Installation  

No installations needed here, unless you don't have the ncurses library, in which case just run  
```sudo apt-get install libncurses5-dev```

## Input Format  

The configuration for the cache you want to simulate has to be given in the form of a text file. A sample format can be found in ```paras.cfg```  

The input format is as so:  
```
<levels>  
<policy> <cacheSize> <blockSize> <setAssociativity>
.  
.  
.  
```  

The trace for the simulator is expected to be a gzip file generated the [Pin](https://software.intel.com/en-us/articles/pin-a-binary-instrumentation-tool-downloads) tool from Intel.  

## Usage  

```./run.sh -t|--trace <TRACE> -c|--config <CONFIG_FILE> [-i|--interactive] [-d|--debug]```  
The ```-i``` option outputs the hits and misses in all the levels of the cache in live time (obviously making it slower but hopefully increases your patience :).  
The ```-d``` option checks if the the cacheBlock chosen by the policy indeed matches the index of the address being accessed, just a safety measure.  

## Contribution  

To add support for more eviction policies, have a look at the template folder and declare the required data structures and complete the functions (don't forget to put them in the policies folder). If the template seems to be lacking something, raise an issue and we can have a look at it.  
