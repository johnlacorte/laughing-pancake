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

void addWasmData(struct WasmDataBucket *dataBucket, unsigned int memoryIndex, unsigned int initLength, unsigned char *initExpr, unsigned int dataLength, unsigned char *dataBytes)
{
    struct WasmData *current;
    unsigned char *byteArray;
    int i;

    if(dataBucket->wasmDataListHead == NULL)
    {
        current = malloc(sizeof(struct WasmData));
        //copy function arguments
        current->memoryIndex = memoryIndex;
        current->initExprLength = initLength;
        current->initExpr = malloc(sizeof(char) * initLength);
        byteArray = current->initExpr;
        for(i = 0; i < initLength; i++)
        {
            byteArray[i] = initExpr[i];
        }
        current->dataLength = dataLength;
        current->dataBytes = malloc(sizeof(char) * dataLength);
        byteArray = current->dataBytes;
        for(i = 0; i < dataLength; i++)
        {
            byteArray[i] = dataBytes[i];
        }
        current->next = NULL;
        //add payload size
        dataBucket->payloadSize += varUIntSize(memoryIndex) + initLength + varUIntSize(dataLength) + dataLength;
        //add number of entries
        dataBucket->numberOfEntries++;
        dataBucket->wasmDataListHead = current;
        dataBucket->wasmDataListTail = current;
    }
    else
    {
        current = dataBucket->wasmDataListTail;
        current->next = malloc(sizeof(struct WasmData));
        current = current->next;
        //copy function arguments
        current->memoryIndex = memoryIndex;
        current->initExprLength = initLength;
        current->initExpr = malloc(sizeof(char) * initLength);
        byteArray = current->initExpr;
        for(i = 0; i < initLength; i++)
        {
            byteArray[i] = initExpr[i];
        }
        current->dataLength = dataLength;
        current->dataBytes = malloc(sizeof(char) * dataLength);
        byteArray = current->dataBytes;
        for(i = 0; i < current->dataLength; i++)
        {
            byteArray[i] = dataBytes[i];
        }
        current->next = NULL;
        //add payload size
        dataBucket->payloadSize += varUIntSize(memoryIndex) + initLength + varUIntSize(dataLength) + dataLength;
        //add number of entries
        dataBucket->wasmDataListTail = current;
    }
}

int dumpWasmDataBucket(struct WasmDataBucket *dataBucket, FILE *fp)
{
    int returnValue = 0;
    int i;
    struct WasmData *current = NULL;

    if(dataBucket->wasmDataListHead != NULL)
    {
        //write section code (11)
        writeVarUInt(fp, 11);
        //write payload size including size of numberOfEntries
        writeVarUInt(fp, dataBucket->payloadSize + varUIntSize(dataBucket->numberOfEntries));
        //number of entries
        writeVarUInt(fp, dataBucket->numberOfEntries);
        current = dataBucket->wasmDataListHead;
        for(i = 0; i < dataBucket->numberOfEntries; i++)
        {
            if(current != NULL)
            {
                //write entry
                writeVarUInt(fp, current->memoryIndex);
                writeByteArray(fp, current->initExprLength, current->initExpr);
                writeVarUInt(fp, current->dataLength);
                writeByteArray(fp, current->dataLength, current->dataBytes);
                current = current->next;
                returnValue++;
            }
            else
            {
                returnValue = -1;
                break;
            }
        }
    }
    else
    {
        if(dataBucket->numberOfEntries > 0)
        {
            returnValue = -1;
        }
    }
    if(current != NULL)
    {
        returnValue = -1;
    }
    return returnValue;
}

