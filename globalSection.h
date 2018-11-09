#ifndef GLOBALSECTION_H
#define EXPORTSECTION_H
#include <stdio.h>

struct WasmGlobal
{
    //global_type is value_type and immutability
    //init_expr is one of the four constant opcodes or an imported global followed by 0x0b
    //only immutable values can be imported or exported
    unsigned char variableType;
    unsigned char mutability;
    unsigned char *initExpr;
    struct WasmGlobal *next;
};

struct WasmGlobalBucket
{
    struct WasmGlobal *wasmGlobalListHead;
    size_t payloadSize;
    size_t numberOfEntries;
    size_t numberOfImports;
    //tail pointer?
};

void initWasmExportBucket(struct WasmGlobalBucket);

//I think this might want to return index
//global_type and init_expr for this
int addWasmGlobal(struct WasmGlobalBucket*, unsigned char, unsigned char, unsigned char*);

int dumpWasmGlobalBucket(struct WasmGlobalBucket*, FILE*);

#endif

