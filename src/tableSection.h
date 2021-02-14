#ifndef TABLESECTION_H
#define TABLESECTION_H
#include <stdio.h>

struct WasmTable
{
    unsigned char elementType;
    unsigned int flags;
    unsigned int initialSize;
    unsigned int maximumSize;
    struct WasmTable *next;
};

struct WasmTableBucket
{
    struct WasmTable *wasmTableListHead;
    size_t payloadSize;
    size_t numberOfEntries;
    size_t numberOfImports;
};

void initWasmTableBucket(struct WasmTableBucket*);

//element type (anyfunc), flags, initialSize, maximumSize
unsigned int addWasmTable(struct WasmTableBucket*, unsigned char, unsigned int, unsigned int, unsigned int);

int dumpWasmTableBucket(struct WasmTableBucket*, FILE*);

#endif

