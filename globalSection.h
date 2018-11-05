#ifndef GLOBALSECTION_H
#define EXPORTSECTION_H
#include <stdio.h>

struct WasmGlobal
{
    //global_type
    //init_expr
};

struct WasmGlobalBucket
{
    struct WasmGlobal *wasmGlobalListHead;
    size_t payloadSize;
    size_t numberOfEntries;
    //tail pointer?
};

void initWasmExportBucket(struct WasmGlobalBucket);

//I think this might want to return index
//global_type and init_expr for this
void addWasmGlobal(struct WasmGlobalBucket*, unsigned char, unsigned int);

int dumpWasmGlobalBucket(FILE*, struct WasmGlobalBucket*);

#endif

