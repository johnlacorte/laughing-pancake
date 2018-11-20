#ifndef DATASECTION_H
#define DATASECTION_H
#include <stdio.h>

struct WasmData
{
    unsigned int memoryIndex;
    //Do I need length for something like this?
    unsigned int initExprLength;
    unsigned char *initExpr;
    unsigned int dataSize;
    unsigned char *dataBytes;
};

struct WasmDataBucket
{
    struct WasmData *wasmDataListHead;
    size_t payloadSize;
    size_t numberOfEntries;
    struct WasmData *wasmDataListTail;
};

void initWasmDataBucket(struct WasmDataBucket*);

void addWasmData(struct WasmDataBucket*);

int dumpWasmDataBucket(struct WasmDataBucket*, FILE*);

#endif

