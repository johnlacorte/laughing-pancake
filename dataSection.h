#ifndef DATASECTION_H
#define DATASECTION_H
#include <stdio.h>

struct WasmData
{
    unsigned int memoryIndex;
    //Do I need length for something like this?
    unsigned int initExprLength;
    unsigned char *initExpr;
    unsigned int dataLength;
    unsigned char *dataBytes;
    struct WasmData *next;
};

struct WasmDataBucket
{
    struct WasmData *wasmDataListHead;
    size_t payloadSize;
    size_t numberOfEntries;
    struct WasmData *wasmDataListTail;
};

void initWasmDataBucket(struct WasmDataBucket*);

//data bucket, memory index, init expr length, init expr, datasize, data
void addWasmData(struct WasmDataBucket*, unsigned int, unsigned int, unsigned char*, unsigned int, unsigned char*);

int dumpWasmDataBucket(struct WasmDataBucket*, FILE*);

#endif

