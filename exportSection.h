#ifndef EXPORTSECTION_H
#define EXPORTSECTION_H
#include <stdio.h>

struct WasmExport
{
    unsigned int fieldLength;
    char *fieldString;
    unsigned char kind;
    unsigned int index;
    struct WasmExport *next;
};

struct WasmExportBucket
{
    struct WasmExport *wasmExportListHead;
    size_t payloadSize;
    size_t numberOfEntries;
    struct WasmExport *wasmExportListTail;
};

void initWasmExportBucket(struct WasmExportBucket*);

//export bucket, length of field string, field string, kind of definition, index of definition
void addWasmExport(struct WasmExportBucket*, unsigned int, char*, unsigned char, unsigned int);

int dumpWasmExportBucket(FILE*, struct WasmExportBucket*);

#endif

