#include "functionSection.h"
#include "fileOutput.h"
#include <stdlib.h>

void initWasmFunctionBucket(struct WasmFunctionBucket *functionBucket)
{
    functionBucket->wasmFunctionListHead = NULL;
    functionBucket->payloadSize = 0;
    functionBucket->numberOfEntries = 0;
    functionBucket->numberOfImports = 0;
}

unsigned int addWasmFunction(struct WasmFunctionBucket *functionBucket, unsigned int typeIndex)
{
    struct WasmFunction *current;
    unsigned int functionIndex = 0;

    //First entry?
    if(functionBucket->wasmFunctionListHead == NULL)
    {
        current = malloc(sizeof(struct WasmFunction));
        current->typeIndex = typeIndex;
        current->next = NULL;
        functionBucket->wasmFunctionListHead = current;
        functionBucket->numberOfEntries++;
        functionBucket->payloadSize += varUIntSize(typeIndex);
    }
    else
    {
        current = functionBucket->wasmFunctionListHead;
        while(current != NULL)
        {
            if(current->next != NULL)
            {
                current = current->next;
            }
            else
            {
                current->next = malloc(sizeof(struct WasmFunction));
                current = current->next;
                current->typeIndex = typeIndex;
                current->next = NULL;
                functionBucket->numberOfEntries++;
                functionBucket->payloadSize += varUIntSize(typeIndex);
                current = current->next;
            }
            functionIndex++;
        }
    }
    return functionIndex;
}


int dumpWasmFunctionBucket(FILE *fp, struct WasmFunctionBucket *functionBucket)
{
    int returnValue = 0;
    int i;
    struct WasmFunction *current;

    if(functionBucket->wasmFunctionListHead != NULL)
    {
        //section code
        writeVarUInt(fp, 3);
        //payload size including the numberOfEntries var uint
        writeVarUInt(fp, varUIntSize(functionBucket->numberOfEntries) + functionBucket->payloadSize);
        //write numberOfEntries
        writeVarUInt(fp, functionBucket->numberOfEntries);
        //loop to write out each entry
        current = functionBucket->wasmFunctionListHead;
        for(i = 0; i < functionBucket->numberOfEntries; i++)
        {
            if(current != NULL)
            {
                writeVarUInt(fp, current->typeIndex);
                current = current->next;
                returnValue++;
            }
            else
            {
                //Something went wrong.
                returnValue = -1;
                break;
            }
        }
    }
    else
    {
        //Check if there were supposed to be entries
        if(functionBucket->numberOfEntries > 0)
        {
            returnValue = -1;
        }
    }
    //Check if the number of types in list are greater than the counter
    if(current != NULL)
    {
        returnValue = -1;
    }
    return returnValue;
}

