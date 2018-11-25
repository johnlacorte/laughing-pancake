#include "fileOutput.h"
#include "wasmModule.h"
#include <stdlib.h>

void initWasmModule(struct WasmModule *module)
{
    struct WasmTypeBucket *typeBucket;
    //struct WasmImportBucket *importBucket;
    struct WasmFunctionBucket *functionBucket;
    struct WasmTableBucket *tableBucket;
    struct WasmMemoryBucket *memoryBucket;
    struct WasmGlobalBucket *globalBucket;
    struct WasmExportBucket *exportBucket;
    struct WasmStartBucket *startBucket;
    struct WasmElementBucket *elementBucket;
    struct WasmCodeBucket *codeBucket;
    struct WasmDataBucket *dataBucket;
    //struct WasmNameBucket *nameBucket;

    typeBucket = &module->typeBucket;
    //importBucket = &module->importBucket;
    functionBucket = &module->functionBucket;
    tableBucket = &module->tableBucket;
    memoryBucket = &module->memoryBucket;
    globalBucket = &module->globalBucket;
    exportBucket = &module->exportBucket;
    startBucket = &module->startBucket;
    elementBucket = &module->elementBucket;
    codeBucket = &module->codeBucket;
    dataBucket = &module->dataBucket;
    //nameBucket = &module->nameBucket;

    initWasmTypeBucket(typeBucket);
    //initWasmImportBucket(importBucket);
    initWasmFunctionBucket(functionBucket);
    initWasmTableBucket(tableBucket);
    initWasmMemoryBucket(memoryBucket);
    initWasmGlobalBucket(globalBucket);
    initWasmExportBucket(exportBucket);
    initWasmStartBucket(startBucket);
    initWasmElementBucket(elementBucket);
    initWasmCodeBucket(codeBucket);
    initWasmDataBucket(dataBucket);
    //initWasmNameBucket(nameBucket);
}

struct WasmModule *newWasmModule()
{
    struct WasmModule *newModule;

    newModule = malloc(sizeof(struct WasmModule));
    initWasmModule(newModule);
    return newModule;
}

unsigned int addTypeToModule(struct WasmModule *module, unsigned char returnValueType, unsigned char numberOfParams, unsigned char *paramTypes)
{
    struct WasmTypeBucket *typeBucket;

    typeBucket = &module->typeBucket;
    return addWasmType(typeBucket, returnValueType, numberOfParams, paramTypes);
}

/*
void addImportToModule(struct WasmModule *module)
{
    struct WasmImportBucket *importBucket;

    importBucket = &module->importBucket;
    addWasmImport(importBucket);
}
*/

unsigned int addFunctionToModule(struct WasmModule *module, unsigned int typeIndex)
{
    struct WasmFunctionBucket *functionBucket;

    functionBucket = &module->functionBucket;
    return addWasmFunction(functionBucket, typeIndex);
}

unsigned int addTableToModule(struct WasmModule *module, unsigned char elementType, unsigned int flags, unsigned int initialSize, unsigned int maximumSize)
{
    struct WasmTableBucket *tableBucket;

    tableBucket = &module->tableBucket;
    addWasmTable(tableBucket, elementType, flags, initialSize, maximumSize);
}

void addMemoryToModule(struct WasmModule *module, unsigned int flags, unsigned int initialSize, unsigned int maximumSize)
{
    struct WasmMemoryBucket *memoryBucket;

    memoryBucket = &module->memoryBucket;
    addWasmMemory(memoryBucket, flags, initialSize, maximumSize);
}


unsigned int addGlobalToModule(struct WasmModule *module, unsigned char variableType, unsigned char mutability, unsigned int initSize, unsigned char *initExpr)
{
    struct WasmGlobalBucket *globalBucket;

    globalBucket = &module->globalBucket;
    return addWasmGlobal(globalBucket, variableType, mutability, initSize, initExpr);
}

void addExportToModule(struct WasmModule *module, unsigned int fieldLength, char *field, unsigned char kind, unsigned int index)
{
    struct WasmExportBucket *exportBucket;

    exportBucket = &module->exportBucket;
    addWasmExport(exportBucket, fieldLength, field, kind, index);
}

void addStartToModule(struct WasmModule *module, unsigned int functionIndex)
{
    struct WasmStartBucket *startBucket;

    startBucket = &module->startBucket;
    addWasmStart(startBucket, functionIndex);
}

void addElementToModule(struct WasmModule *module, unsigned int tableIndex, unsigned int initExprLength, unsigned char *initExpr, unsigned int numberOfElements, unsigned int *elements)
{
    struct WasmElementBucket *elementBucket;

    elementBucket = &module->elementBucket;
    addWasmElement(elementBucket, tableIndex, initExprLength, initExpr, numberOfElements, elements);
}

void addCodeToModule(struct WasmModule *module, unsigned int variableCount, unsigned char *variableTypes, unsigned int bytecodeSize, unsigned char *bytecode)
{
    struct WasmCodeBucket *codeBucket;

    codeBucket = &module->codeBucket;
    addWasmCode(codeBucket, variableCount, variableTypes, bytecodeSize, bytecode);
}

void addDataToModule(struct WasmModule *module, unsigned int memoryIndex, unsigned int initExprLength, unsigned char *initExpr, unsigned int dataLength, unsigned char *dataBytes)
{
    struct WasmDataBucket *dataBucket;

    dataBucket = &module->dataBucket;
    addWasmData(dataBucket, memoryIndex, initExprLength, initExpr, dataLength, dataBytes);
}

/*
void addNameToModule(struct WasmModule *module)
{
    struct WasmNameBucket *nameBucket;

    nameBucket = &module->nameBucket;
    addWasmName(nameBucket);
}
*/

int dumpWasmModule(struct WasmModule *module, FILE *fp)
{
    int returnValue = 0;
    struct WasmTypeBucket *typeBucket;
    //struct WasmImportBucket *importBucket;
    struct WasmFunctionBucket *functionBucket;
    struct WasmTableBucket *tableBucket;
    struct WasmMemoryBucket *memoryBucket;
    struct WasmGlobalBucket *globalBucket;
    struct WasmExportBucket *exportBucket;
    struct WasmStartBucket *startBucket;
    struct WasmElementBucket *elementBucket;
    struct WasmCodeBucket *codeBucket;
    struct WasmDataBucket *dataBucket;
    //struct WasmNameBucket *nameBucket;

    char version[4] = {1, 0, 0, 0};

    writeMagicNumber(fp);
    writeByteArray(fp, 4, version);
    typeBucket = &module->typeBucket;
    dumpWasmTypeBucket(typeBucket, fp);
    //importBucket = &module->importBucket;
    //dumpWasmImportBucket(importBucket, fp);
    functionBucket = &module->functionBucket;
    dumpWasmFunctionBucket(functionBucket, fp);
    tableBucket = &module->tableBucket;
    dumpWasmTableBucket(tableBucket, fp);
    memoryBucket = &module->memoryBucket;
    dumpWasmMemoryBucket(memoryBucket, fp);
    globalBucket = &module->globalBucket;
    dumpWasmGlobalBucket(globalBucket, fp);
    exportBucket = &module->exportBucket;
    dumpWasmExportBucket(exportBucket, fp);
    startBucket = &module->startBucket;
    dumpWasmStartBucket(startBucket, fp);
    elementBucket = &module->elementBucket;
    dumpWasmElementBucket(elementBucket, fp);
    codeBucket = &module->codeBucket;
    dumpWasmCodeBucket(codeBucket, fp);
    dataBucket = &module->dataBucket;
    dumpWasmDataBucket(dataBucket, fp);
    //nameBucket = &module->nameBucket;
    //dumpWasmNameBucket;
    return 0;
}

