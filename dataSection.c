#include "dataSection.h"
#include "fileOutput.h"
#include <stdlib.h>

void initWasmDataBucket(struct WasmDataBucket *dataBucket)
{
    dataBucket->wasmDataListHead = NULL;
    dataBucket->payloadSize = 0;
    dataBucket->numberOfEntries = 0;
    dataBucket->wasmDataListTail = NULL;
}

void addWasmData(struct WasmDataBucket *dataBucket, unsigned int memoryIndex, unsigned int initLength, unsigned char *initExpr, unsigned int dataSize, unsigned char *dataBytes)
{

}

int dumpWasmDataBucket(struct WasmDataBucket *dataBucket, FILE *fp)
{

}

