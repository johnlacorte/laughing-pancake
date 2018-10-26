#ifndef TYPESECTION_H
#define TYPESECTION_H
#include <stdio.h>

struct WasmType{
    unsigned char paramCount;
    unsigned char *paramCount;
    unsigned char returnType;
    struct WasmType *next;
};

struct WasmTypeBucket{
    struct WasmType wasmTypeListHead;
    size_t payloadSize;
    size_t numberOfEntries;
    //Do I want a tail pointer? I think I want a tail pointer.
};

//Sets size variables to 0 and pointers to NULL
void initWasmTypeBucket(struct WasmTypeBucket*);

//Return type, number of parameters, array of bytes that are the Wasm var type constants.
//This may get moved "one level up" and newType() may no longer be static.
int addType(struct WasmTypeBucket*, unsigned char, unsigned char, unsigned char*);

//Dumps a WasmTypeBucket to a file.
int dumpWasmTypeBucket(FILE*, struct WasmTypeBucket*);

//Do I need to have a function to return a WasmType from index for comparing?

#endif

