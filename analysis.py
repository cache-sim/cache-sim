#!/usr/bin/python3

import os 
import sys

file = "blockSizeAnalysis.txt" #change file name for different analysis

replacementPolicy = "plru"
numberOfSet = "64"
setAssociativity = "4"
inputTraces = ["trace1.gz", "trace2.gz", "trace3.1.gz"]
blockSize = "32"

blockSizes = ["16", "32", "64"]
numberOfSets = ["32", "64", "128"]
setAssociativities = ["2", "4", "8"]

os.system("touch " + file)
for blockSize in blockSizes: #change blockSizes for different analysis
    os.system("echo " + blockSize + " >> " + file)
    os.system("echo >> " + file)
    for inputTrace in inputTraces:
        os.system("echo " + inputTrace + " >> " + file)
        os.system("g++ src/" + replacementPolicy + ".cpp src/cache.cpp -o exe/" + replacementPolicy)    
        os.system("gzip -dc " + inputTrace + " | exe/" + replacementPolicy + " " + numberOfSet + " " + blockSize + " " + setAssociativity
        + " >> " + file)
        os.system("echo >> " + file)
    