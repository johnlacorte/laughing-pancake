#ifndef MEMORYSECTION_H
#define MEMORYSECTION_H
#include <stdio.h>

struct WasmMemory
{
    unsigned int flags;
    unsigned int initialSize;
    unsigned int maximumSize;
    struct WasmMemory *next;
};

struct WasmMemoryBucket
{
    struct WasmMemory *wasmMemoryListHead;
    size_t payloadSize;
    size_t numberOfEntries;
    size_t numberOfImports;
};

void initWasmMemoryBucket(struct WasmMemoryBucket*);

unsigned int addWasmMemory(struct WasmMemoryBucket*, unsigned int, unsigned int, unsigned int);

int dumpWasmMemoryBucket(struct WasmMemoryBucket*, FILE*);

#endif

