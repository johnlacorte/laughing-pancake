#include "startSection.h"
#include "fileOutput.h"
#include <stdlib.h>

void initWasmStartBucket(struct WasmStartBucket *startBucket)
{
    startBucket->isStartFunction = 0;
    startBucket->startFunction = 0;   
}

void addWasmStart(struct WasmStartBucket *startBucket, unsigned int functionIndex)
{
    startBucket->isStartFunction = 1;
    startBucket->startFunction = functionIndex;
}

int dumpWasmStartBucket(struct WasmStartBucket *startBucket, FILE *fp)
{
    int returnValue = 0;

    if(startBucket->isStartFunction == 1)
    {
        //section code is 8
        writeVarUInt(fp, 8);
        writeVarUInt(fp, varUIntSize(startBucket->startFunction));
        writeVarUInt(fp, startBucket->startFunction);
        returnValue = 1;
    }
    //section code is 8
    //check if there is a start funtion, write the size of it for payload and start
    //function index
    return returnValue;
}

