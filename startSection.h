#ifndef STARTSECTION_H
#define STARTSECTION_H
#include <stdio.h>

struct WasmStartBucket
{
    unsigned int isStartFunction;
    unsigned int startFunction;
};

void initWasmStartBucket(struct WasmStartBucket*);

void addWasmStart(struct WasmStartBucket*, unsigned int);

int dumpWasmStartBucket(struct WasmStartBucket*, FILE*);

#endif

