#include "fileOutput.h"
#include "wasmModule.h"
#include <stdlib.h>

void initWasmModule(struct WasmModule *module)
{
    struct WasmTypeBucket *typeBucket;
    struct WasmFunctionBucket *functionBucket;
    struct WasmExportBucket *exportBucket;
    struct WasmCodeBucket *codeBucket;

    typeBucket = &module->typeBucket;
    functionBucket = &module->functionBucket;
    exportBucket = &module->exportBucket;
    codeBucket = &module->codeBucket;

    initWasmTypeBucket(typeBucket);
    initWasmFunctionBucket(functionBucket);
    initWasmExportBucket(exportBucket);
    initWasmCodeBucket(codeBucket);
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

unsigned int addFunctionToModule(struct WasmModule *module, unsigned int typeIndex)
{
    struct WasmFunctionBucket *functionBucket;

    functionBucket = &module->functionBucket;
    return addWasmFunction(functionBucket, typeIndex);
}

void addExportToModule(struct WasmModule *module, unsigned int fieldLength, char *field, unsigned char kind, unsigned int index)
{
    struct WasmExportBucket *exportBucket;

    exportBucket = &module->exportBucket;
    addWasmExport(exportBucket, fieldLength, field, kind, index);
}

void addCodeToModule(struct WasmModule *module, unsigned int variableCount, unsigned char *variableTypes, unsigned int bytecodeSize, unsigned char *bytecode)
{
    struct WasmCodeBucket *codeBucket;

    codeBucket = &module->codeBucket;
    addWasmCode(codeBucket, variableCount, variableTypes, bytecodeSize, bytecode);
}

int dumpWasmModule(struct WasmModule *module, FILE *fp)
{
    int returnValue = 0;
    struct WasmTypeBucket *typeBucket;
    struct WasmFunctionBucket *functionBucket;
    struct WasmExportBucket *exportBucket;
    struct WasmCodeBucket *codeBucket;

    char version[4] = {1, 0, 0, 0};

    writeMagicNumber(fp);
    writeByteArray(fp, 4, version);
    typeBucket = &module->typeBucket;
    dumpWasmTypeBucket(typeBucket, fp);
    functionBucket = &module->functionBucket;
    dumpWasmFunctionBucket(functionBucket, fp);
    exportBucket = &module->exportBucket;
    dumpWasmExportBucket(exportBucket, fp);
    codeBucket = &module->codeBucket;
    dumpWasmCodeBucket(codeBucket, fp);
    return 0;
}


