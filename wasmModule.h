#ifndef WASMMODULE_H
#define WASMMODULE_H
#include <stdio.h>

//include all the header files
#include "typeSection.h"
#include "functionSection.h"
#include "exportSection.h"
#include "codeSection.h"

struct WasmModule
{
    struct WasmTypeBucket typeBucket;
    struct WasmFunctionBucket functionBucket;
    struct WasmExportBucket exportBucket;
    struct WasmCodeBucket codeBucket;
};

void initWasmModule(struct WasmModule*);

struct WasmModule *newWasmModule();

//I may change the names of these
unsigned int addTypeToModule(struct WasmModule*, unsigned char, unsigned char, unsigned char*);

//addImportToModule()

unsigned int addFunctionToModule(struct WasmModule*, unsigned int);

//addWasmTable()

//addWasmMemory()

//addWasmGlobal()

void addExportToModule(struct WasmModule*, unsigned int, char*, unsigned char, unsigned int);

//addWasmStart()

//addWasmElement()

void addCodeToModule(struct WasmModule*, unsigned int, unsigned char*, unsigned int, unsigned char*);

//addWasmName()

int dumpWasmModule(struct WasmModule*, FILE*);

#endif

