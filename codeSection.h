#ifndef CODESECTION_H
#define CODESECTION_H
#include <stdio.h>

struct WasmLocalEntry
{
    unsigned int count;
    unsigned char valueType;
    struct WasmLocalEntry *next;
};

struct WasmCode
{
    size_t bodySize;
    size_t localEntryCount;
    struct WasmLocalEntry *wasmLocalEntryListHead;
    size_t bytecodeSize;
    unsigned char *bytecode;
    struct WasmCode *next;
};

struct WasmCodeBucket
{
    struct WasmCode *wasmCodeListHead;
    size_t payloadSize;
    size_t numberOfEntries;
    struct WasmCode *wasmCodeListTail;
};

void initWasmCodeBucket(struct WasmCodeBucket*);

//code bucket, number of local variables, value types, size of bytecode, bytecode followed
//by 0x0b 
void addWasmCode(struct WasmCodeBucket*, unsigned int, unsigned char*, unsigned int, unsigned char*);

int dumpWasmCodeBucket(struct WasmCodeBucket*, FILE*);

#endif

