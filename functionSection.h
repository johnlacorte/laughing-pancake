#ifndef FUNCTIONSECTION_H
#define FUNCTIONSECTION_H
#include <stdio.h>

struct WasmFunction
{
    unsigned int typeIndex;
    struct WasmFunction *next;
};

struct WasmFunctionBucket
{
    struct WasmFunction *wasmFunctionListHead;
    size_t payloadSize;
    size_t numberOfEntries;
    size_t numberOfImports;
};

void initWasmFunctionBucket(struct WasmFunctionBucket*);

//function bucket, type index for function
unsigned int addWasmFunction(struct WasmFunctionBucket*, unsigned int);

int dumpWasmFunctionBucket(FILE*, struct WasmFunctionBucket*);

#endif

