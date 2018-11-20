#include "tableSection.h"
#include "fileOutput.h"
#include <stdlib.h>

void initWasmTableBucket(struct WasmTableBucket *tableBucket)
{
    tableBucket->wasmTableListHead = NULL;
    tableBucket->payloadSize = 0;
    tableBucket->numberOfEntries = 0;
    tableBucket->numberOfImports = 0;
}

unsigned int addWasmTable(struct WasmTableBucket *tableBucket, unsigned char elementType, unsigned int flags, unsigned int initialSize, unsigned int maximumSize)
{
    struct WasmTable *current;
    unsigned int tableIndex = 0;

    if(tableBucket->wasmTableListHead == NULL)
    {
        current = malloc(sizeof(struct WasmTable));
        current->elementType = elementType;
        current->flags = flags;
        current->initialSize = initialSize;
        current->maximumSize = maximumSize;
        current->next = NULL;
        //payload size
        tableBucket->payloadSize += 1 + varUIntSize(flags) + varUIntSize(initialSize);
        if(flags == 1)
        {
            tableBucket->payloadSize += varUIntSize(maximumSize);
        }
        tableBucket->numberOfEntries++;
        tableBucket->wasmTableListHead = current;
    }
    else
    {
        current = tableBucket->wasmTableListHead;
        while(current != NULL)
        {
            if(current->next != NULL)
            {
                current = current->next;
            }
            else
            {
                current->next = malloc(sizeof(struct WasmTable));
                current = current->next;
                //copy values
                current->elementType = elementType;
                current->flags = flags;
                current->initialSize = initialSize;
                current->maximumSize = maximumSize;
                current->next = NULL;
                //payload size
                tableBucket->payloadSize += 1 + varUIntSize(flags) + varUIntSize(initialSize);
                if(flags == 1)
                {
                    tableBucket->payloadSize += varUIntSize(maximumSize);
                }
                tableBucket->numberOfEntries++;
                current = current->next;
            }
            tableIndex++;
        }
    }
    return tableIndex;
}

int dumpWasmTableBucket(struct WasmTableBucket *tableBucket, FILE *fp)
{
    //section code 4
    int returnValue = 0;
    int i;
    struct WasmTable *current = NULL;

    if(tableBucket->wasmTableListHead != NULL)
    {
        //write section code (4)
        writeVarUInt(fp, 4);
        //write payload size including size of numberOfEntries
        writeVarUInt(fp, tableBucket->payloadSize + varUIntSize(tableBucket->numberOfEntries));
        //write numberOfEntries
        writeVarUInt(fp, tableBucket->numberOfEntries);
        current = tableBucket->wasmTableListHead;
        for(i = 0; i < tableBucket->numberOfEntries; i++)
        {
            if(current != NULL)
            {
                //write entry
                writeVarUInt(fp, current->elementType);
                writeVarUInt(fp, current->flags);
                writeVarUInt(fp, current->initialSize);
                if(current->flags == 1)
                {
                    writeVarUInt(fp, current->maximumSize);
                }
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
        if(tableBucket->numberOfEntries > 0)
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

