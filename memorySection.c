#include "memorySection.h"
#include "fileOutput.h"
#include <stdlib.h>

void initWasmMemoryBucket(struct WasmMemoryBucket *memoryBucket)
{
    memoryBucket->wasmMemoryListHead = NULL;
    memoryBucket->payloadSize = 0;
    memoryBucket->numberOfEntries = 0;
    memoryBucket->numberOfImports = 0;
}

unsigned int addWasmMemory(struct WasmMemoryBucket *memoryBucket, unsigned int flags, unsigned int initialSize, unsigned int maximumSize)
{
    //There should be a constant for how many memory modules are allowed
    //and there should be a check for numberOfEntries + numberOfImports
    struct WasmMemory *current;
    unsigned int memoryIndex = 0;

    if(memoryBucket->wasmMemoryListHead != NULL)
    {
        current = malloc(sizeof(struct WasmMemory));
        //copy values
        current->flags = flags;
        current->initialSize = initialSize;
        current->maximumSize = maximumSize;
        current->next = NULL;
        //add payload size
        memoryBucket->payloadSize += varUIntSize(flags) + varUIntSize(initialSize);
        if(flags == 1)
        {
            memoryBucket->payloadSize += varUIntSize(maximumSize);
        }
        //add to number of entries
        memoryBucket->numberOfEntries++;
        memoryBucket->wasmMemoryListHead = current;
    }
    else
    {
        current = memoryBucket->wasmMemoryListHead;
        while(current != NULL)
        {
            if(current != NULL)
            {
                current = current->next;
            }
            else
            {
                current->next = malloc(sizeof(struct WasmMemory));
                current = current->next;
                //copy values
                current->flags = flags;
                current->initialSize = initialSize;
                current->maximumSize = maximumSize;
                current->next = NULL;
                //add payload size
                memoryBucket->payloadSize += varUIntSize(flags) + varUIntSize(initialSize);
                if(flags == 1)
                {
                    memoryBucket->payloadSize += varUIntSize(maximumSize);
                }
                //add to number of entries
                memoryBucket->numberOfEntries++;
                current = current->next;
            }
            memoryIndex++;
        }
    }
    return memoryIndex;
}

int dumpWasmMemoryBucket(struct WasmMemoryBucket *memoryBucket, FILE *fp)
{
    int returnValue = 0;
    int i;
    struct WasmMemory *current = NULL;

    if(memoryBucket->wasmMemoryListHead != NULL)
    {
        //write section code (5)
        writeVarUInt(fp, 5);
        //writepayload size including size of number of entries
        writeVarUInt(fp, memoryBucket->payloadSize + varUIntSize(memoryBucket->numberOfEntries));
        //write number of entries
        writeVarUInt(fp, memoryBucket->numberOfEntries);
        current = memoryBucket->wasmMemoryListHead;
        for(i = 0; i < memoryBucket->numberOfEntries; i++)
        {
            if(current != NULL)
            {
                //write entry
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
        if(memoryBucket->numberOfEntries > 0)
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

