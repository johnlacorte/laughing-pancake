#ifndef GLOBALSECTION_H
#define GLOBALSECTION_H
#include <stdio.h>

struct WasmGlobal
{
    //global_type is value_type and immutability
    //init_expr is one of the four constant opcodes or an imported global followed by 0x0b
    //only immutable values can be imported or exported
    unsigned char variableType;
    unsigned char mutability;
    unsigned int initSize;
    unsigned char *initExpr;
    struct WasmGlobal *next;
};

struct WasmGlobalBucket
{
    struct WasmGlobal *wasmGlobalListHead;
    size_t payloadSize;
    size_t numberOfEntries;
    size_t numberOfImports;
};

void initWasmGlobalBucket(struct WasmGlobalBucket*);

//I think this might want to return index
//global_type and init_expr for this
unsigned int addWasmGlobal(struct WasmGlobalBucket*, unsigned char, unsigned char, unsigned int, unsigned char*);

int dumpWasmGlobalBucket(struct WasmGlobalBucket*, FILE*);

#endif

