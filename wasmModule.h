#ifndef WASMMODULE_H
#define WASMMODULE_H
#include <stdio.h>

//include all the header files
#include "typeSection.h"
//#include "importSection.h"
#include "functionSection.h"
//#include "tableSection.h"
//#include "memorySection.h"
#include "globalSection.h"
#include "exportSection.h"
//#include "startSection.h"
//#include "elementSection.h"
#include "codeSection.h"
//#include "dataSection.h"
//#include "nameSection.h"

struct WasmModule
{
    struct WasmTypeBucket typeBucket;
    //struct WasmImportBucket importBucket;
    struct WasmFunctionBucket functionBucket;
    //struct WasmTableBucket tableBucket;
    //struct WasmMemoryBucket memoryBucket;
    struct WasmGlobalBucket globalBucket;
    struct WasmExportBucket exportBucket;
    //struct WasmStartBucket startBucket;
    //struct WasmElementBucket elementBucket;
    struct WasmCodeBucket codeBucket;
    //struct WasmDataBucket dataBucket;
    //struct WasmNameBucket nameBucket;
};

void initWasmModule(struct WasmModule*);

struct WasmModule *newWasmModule();

//I may change the names of these
unsigned int addTypeToModule(struct WasmModule*, unsigned char, unsigned char, unsigned char*);

//void addImportToModule(struct WasmModule*);

unsigned int addFunctionToModule(struct WasmModule*, unsigned int);

//void addTableToModule(struct WasmModule*);

//void addMemoryToModule(struct WasmModule*);

unsigned int addGlobalToModule(struct WasmModule*, unsigned char, unsigned char, unsigned int, unsigned char*);

void addExportToModule(struct WasmModule*, unsigned int, char*, unsigned char, unsigned int);

//void addStartToModule(struct WasmModule*);

//void addElementToModule(struct WasmModule*);

void addCodeToModule(struct WasmModule*, unsigned int, unsigned char*, unsigned int, unsigned char*);

//void addDataToModule(struct WasmModule*);

//void addNameToModule(struct WasmModule*);

int dumpWasmModule(struct WasmModule*, FILE*);

#endif

