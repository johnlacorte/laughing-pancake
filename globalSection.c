#include "globalSection.h"
#include "fileOutput.h"
#include <stdlib.h>

void initWasmGlobalBucket(struct WasmGlobalBucket *globalBucket)
{
    globalBucket->wasmGlobalListHead = NULL;
    globalBucket->payloadSize = 0;
    globalBucket->numberOfEntries = 0;
    globalBucket->numberOfImports = 0;
}

unsigned int addWasmGlobal(struct WasmGlobalBucket *globalBucket, unsigned char variableType, unsigned char mutability, unsigned int initSize, unsigned char *initExpr)
{
    struct WasmGlobal *current;
    unsigned int globalIndex = 0;
    unsigned char *byteArray;
    int i;

    if(globalBucket->wasmGlobalListHead == NULL)
    {
        current = malloc(sizeof(struct WasmGlobal));
        //copy values to new structure
        current->variableType = variableType;
        current->mutability = mutability;
        current->initSize = initSize;
        current->initExpr = malloc(sizeof(char) * initSize);
        byteArray = current->initExpr;
        for(i = 0; i < initSize; i++)
        {
            byteArray[i] = initExpr[i];
        }
        current->next = NULL;
        //add payload size
        globalBucket->payloadSize += 2 + initSize;
        //add to number of entries
        globalBucket->numberOfEntries++;
        //set head pointer to new global structure
        globalBucket->wasmGlobalListHead = current;
    }
    else
    {
        current = globalBucket->wasmGlobalListHead;
        //loop until current->next == NULL counting index
        while(current != NULL)
        {
            if(current->next != NULL)
            {
                current = current->next;
            }
            else
            {
                current->next = malloc(sizeof(struct WasmGlobal));
                current = current->next;
                current->variableType = variableType;
                current->mutability = mutability;
                current->initSize = initSize;
                current->initExpr = malloc(sizeof(char) * initSize);
                byteArray = current->initExpr;
                for(i = 0; i < initSize; i++)
                {
                    byteArray[i] = initExpr[i];
                }
                current->next = NULL;
                globalBucket->payloadSize += 2 + initSize;
                globalBucket->numberOfEntries++;
                current = current->next;
            }
            globalIndex++;
        }
    }
    return globalIndex;
}

int dumpWasmGlobalBucket(struct WasmGlobalBucket *globalBucket, FILE *fp)
{
    int returnValue = 0;
    int i;
    struct WasmGlobal *current;

    if(globalBucket->wasmGlobalListHead != NULL)
    {
        //write section code
        writeVarUInt(fp, 6);
        //write payload size including size of number of entries
        writeVarUInt(fp, varUIntSize(globalBucket->numberOfEntries) + globalBucket->payloadSize);
        //write number of entries
        writeVarUInt(fp, globalBucket->numberOfEntries);
        current = globalBucket->wasmGlobalListHead;
        for(i = 0; i < globalBucket->numberOfEntries; i++)
        {
            if(current != NULL)
            {
                //write out entry
                writeByte(fp, current->variableType);
                writeByte(fp, current->mutability);
                writeByteArray(fp, current->initSize, current->initExpr);
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
        if(globalBucket->numberOfEntries > 0)
        {
            returnValue = -1;
        }
    }
    if(current !=NULL)
    {
        returnValue = -1;
    }
    return returnValue;
}

