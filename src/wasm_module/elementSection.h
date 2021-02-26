#ifndef ELEMENTSECTION_H
#define ELEMENTSECTION_H
#include <stdio.h>

struct WasmElement
{
    unsigned int tableIndex;
    unsigned int initExprLength;
    unsigned char *initExpr;
    unsigned int numberOfElements;
    unsigned int *elements;
    struct WasmElement *next;
};

struct WasmElementBucket
{
    struct WasmElement *wasmElementListHead;
    size_t payloadSize;
    size_t numberOfEntries;
    struct WasmElement *wasmElementListTail;
};

void initWasmElementBucket(struct WasmElementBucket*);

void addWasmElement(struct WasmElementBucket*, unsigned int, unsigned int, unsigned char*, unsigned int, unsigned int*);

int dumpWasmElementBucket(struct WasmElementBucket*, FILE*);

#endif

