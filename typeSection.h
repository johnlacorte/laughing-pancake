#ifndef TYPESECTION_H
#define TYPESECTION_H
#include <stdio.h>

struct WasmType{
    unsigned char paramCount;
    unsigned char *paramTypes;
    unsigned char returnType;
    struct WasmType *next;
};

struct WasmTypeBucket{
    struct WasmType *wasmTypeListHead;
    size_t payloadSize;
    size_t numberOfEntries;
};

//Sets size variables to 0 and pointers to NULL
void initWasmTypeBucket(struct WasmTypeBucket*);

struct WasmTypeBucket *newWasmTypeBucket();

//Return type, number of parameters, array of bytes that are the Wasm var type constants.
//This may get moved "one level up" and newType() may no longer be static.
unsigned int addWasmType(struct WasmTypeBucket*, unsigned char, unsigned char, unsigned char*);

//Dumps a WasmTypeBucket to a file.
int dumpWasmTypeBucket(struct WasmTypeBucket*, FILE*);

//Do I need to have a function to return a WasmType from index for comparing?

#endif

