#ifndef WASMMODULE_H
#define WASMMODULE_H
#include <stdio.h>

//include all the header files
#include "typeSection.h"
#include "importSection.h"
#include "functionSection.h"
#include "tableSection.h"
#include "memorySection.h"
#include "globalSection.h"
#include "exportSection.h"
#include "startSection.h"
#include "elementSection.h"
#include "codeSection.h"
#include "dataSection.h"
//#include "nameSection.h"

#include "byteBuffer.h"

struct WasmModule
{
    struct WasmTypeBucket typeBucket;
    struct WasmImportBucket importBucket;
    struct WasmFunctionBucket functionBucket;
    struct WasmTableBucket tableBucket;
    struct WasmMemoryBucket memoryBucket;
    struct WasmGlobalBucket globalBucket;
    struct WasmExportBucket exportBucket;
    struct WasmStartBucket startBucket;
    struct WasmElementBucket elementBucket;
    struct WasmCodeBucket codeBucket;
    struct WasmDataBucket dataBucket;
    //struct WasmNameBucket nameBucket;
};

void initWasmModule(struct WasmModule*);

struct WasmModule *newWasmModule();

//I may change the names of these
unsigned int addTypeToModule(struct WasmModule*, unsigned char, unsigned char, unsigned char*);

void addFunctionImportToModule(struct WasmModule*, unsigned int, char*, unsigned int, char*, unsigned int);

void addTableImportToModule(struct WasmModule*, unsigned int, char*, unsigned int, char*, unsigned char, unsigned int, unsigned int, unsigned int);

void addMemoryImportToModule(struct WasmModule*, unsigned int, char*, unsigned int, char*, unsigned int, unsigned int, unsigned int);

void addGlobalImportToModule(struct WasmModule*, unsigned int, char*, unsigned int, char*, unsigned char, unsigned char);

unsigned int addFunctionToModule(struct WasmModule*, unsigned int);

unsigned int addTableToModule(struct WasmModule*, unsigned char, unsigned int, unsigned int, unsigned int);

void addMemoryToModule(struct WasmModule*, unsigned int, unsigned int, unsigned int);

unsigned int addGlobalToModule(struct WasmModule*, unsigned char, unsigned char, struct ByteBuffer*);

void addExportToModule(struct WasmModule*, unsigned int, char*, unsigned char, unsigned int);

void addStartToModule(struct WasmModule*, unsigned int);

void addElementToModule(struct WasmModule*, unsigned int, struct ByteBuffer*, unsigned int, unsigned int*);

void addCodeToModule(struct WasmModule*, unsigned int, unsigned char*, struct ByteBuffer*);

void addDataToModule(struct WasmModule*, unsigned int, unsigned int, unsigned char*, unsigned int, unsigned char*);

//void addNameToModule(struct WasmModule*);

int dumpWasmModule(struct WasmModule*, FILE*, unsigned int);

#endif

