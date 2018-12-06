#ifndef IMPORTSECTION_H
#define IMPORTSECTION_H
#include <stdio.h>

struct WasmImportTable
{
    unsigned char elementType;
    unsigned int flags;
    unsigned int initialSize;
    unsigned int maximumSize;
};

struct WasmImportMemory
{
    unsigned int flags;
    unsigned int initialSize;
    unsigned int maximumSize;
};

struct WasmImportGlobal
{
    unsigned char variableType;
    unsigned char mutability;
};

union ImportType
{
    unsigned int functionType;
    struct WasmImportTable tableType;
    struct WasmImportMemory memoryType;
    struct WasmImportGlobal globalType;
};

struct WasmImport
{
    unsigned int moduleLength;
    char *moduleName;
    unsigned int fieldLength;
    char *fieldName;
    unsigned char externalKind;
    //one of four types, not sure on the details yet
    union ImportType importType;
    struct WasmImport *next;
};

struct WasmImportBucket
{
    struct WasmImport *wasmImportListHead;
    size_t payloadSize;
    size_t numberOfEntries;
    struct WasmImport *wasmImportListTail;
};

void initWasmImportBucket(struct WasmImportBucket*);

//module_len, module_str, field_len, field_str, kind, and one of four types
//I'm either going to have to pass in a ImportType or skip that stuff and
//have a second function to set those things to the tail of the list.
void addWasmImport(struct WasmImportBucket*, unsigned int, char*, unsigned int, char*, unsigned char);

void setImportTypeToFunction(struct WasmImportBucket*, unsigned int);

void setImportTypeToTable(struct WasmImportBucket*, unsigned char, unsigned int, unsigned int, unsigned int);

void setImportTypeToMemory(struct WasmImportBucket*, unsigned int, unsigned int, unsigned int);

void setImportTypeToGlobal(struct WasmImportBucket*, unsigned char, unsigned char);

void writeImportType(FILE*, unsigned char, union ImportType);

int dumpWasmImportBucket(struct WasmImportBucket*, FILE*);

#endif

