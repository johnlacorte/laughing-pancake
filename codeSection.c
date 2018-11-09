#include "fileOutput.h"
#include "codeSection.h"
#include <stdlib.h>

void initWasmCodeBucket(struct WasmCodeBucket *codeBucket)
{
    codeBucket->wasmCodeListHead = NULL;
    codeBucket->payloadSize = 0;
    codeBucket->numberOfEntries = 0;
    codeBucket->wasmCodeListTail = NULL;
}

void addWasmCode(struct WasmCodeBucket *codeBucket, unsigned int variableCount, unsigned char *variableTypes, unsigned int bytecodeSize, unsigned char *bytecode)
{
    struct WasmCode *current;
    struct WasmLocalEntry *currentEntry;
    int i;
    unsigned int counter;
    unsigned char *bytecodePointer;

    if(codeBucket->wasmCodeListHead == NULL)
    {
        //Initialize structure
        current = malloc(sizeof(struct WasmCode));
        current->bodySize = bytecodeSize;
        current->localEntryCount = 0;
        current->wasmLocalEntryListHead = NULL;
        current->bytecodeSize = bytecodeSize;
        current->bytecode = NULL;
        current->next = NULL;
        if(variableCount > 0)
        {
            current->localEntryCount++;
            currentEntry = malloc(sizeof(struct WasmLocalEntry));
            current->wasmLocalEntryListHead = currentEntry;
            currentEntry->valueType = variableTypes[0];
            counter = 1;
            for(i = 1; i < variableCount; i++)
            {
                if(variableTypes[i] == currentEntry->valueType)
                {
                    counter++;
                }
                else
                {
                    currentEntry->count = counter;
                    current->localEntryCount++;
                    current->bodySize += varUIntSize(counter) + 1;
                    currentEntry->next = malloc(sizeof(struct WasmLocalEntry));
                    currentEntry = currentEntry->next;
                    currentEntry->valueType = variableTypes[i];
                    counter = 1;
                }
            }
            currentEntry->count = counter;
            currentEntry->next = NULL;
            current->bodySize += varUIntSize(counter) + 1;
            current->bodySize += varUIntSize(current->localEntryCount);
        }
        codeBucket->wasmCodeListHead = current;
        codeBucket->wasmCodeListTail = current;
        codeBucket->payloadSize += varUIntSize(current->bodySize) + current->bodySize;
    }
    else
    {
        current = codeBucket->wasmCodeListTail;
        current->next = malloc(sizeof(struct WasmCode));
        current = current->next;
        current->bodySize = bytecodeSize;
        current->localEntryCount = 0;
        current->wasmLocalEntryListHead = NULL;
        current->bytecodeSize = bytecodeSize;
        current->bytecode = NULL;
        current->next = NULL;
        if(variableCount > 0)
        {
            current->localEntryCount++;
            currentEntry = malloc(sizeof(struct WasmLocalEntry));
            current->wasmLocalEntryListHead = currentEntry;
            currentEntry->valueType = variableTypes[0];
            counter = 1;
            for(i = 1; i < variableCount; i++)
            {
                if(variableTypes[i] == currentEntry->valueType)
                {
                    counter++;
                }
                else
                {
                    currentEntry->count = counter;
                    current->localEntryCount++;
                    current->bodySize += varUIntSize(counter) + 1;
                    currentEntry->next = malloc(sizeof(struct WasmLocalEntry));
                    currentEntry = currentEntry->next;
                    currentEntry->valueType = variableTypes[i];
                    counter = 1;
                }
            }
            currentEntry->count = counter;
            currentEntry->next = NULL;
            current->bodySize += varUIntSize(counter) + 1;
            current->bodySize += varUIntSize(current->localEntryCount);
        }
        codeBucket->wasmCodeListTail = current;
        codeBucket->payloadSize += varUIntSize(current->bodySize) + current->bodySize;
    }
    codeBucket->numberOfEntries++;
    //Copy bytecode, I should probably check the size at the start of the function.
    current->bytecode = malloc(sizeof(char) * bytecodeSize);
    bytecodePointer = current->bytecode;
    for(i = 0; i < bytecodeSize; i++)
    {
        bytecodePointer[i] = bytecode[i];
    }
}

int dumpWasmCodeBucket(struct WasmCodeBucket *codeBucket, FILE *fp)
{
    int returnValue = 0;
    int i, j;
    struct WasmCode *current;
    struct WasmLocalEntry *currentEntry;

    if(codeBucket->wasmCodeListHead != NULL)
    {
        //section code
        writeVarUInt(fp, 10);
        //payload size including the numberOfEntries
        writeVarUInt(fp, varUIntSize(codeBucket->numberOfEntries) + codeBucket->payloadSize);
        //write number of entries
        writeVarUInt(fp, codeBucket->numberOfEntries);
        current = codeBucket->wasmCodeListHead;
        for(i = 0; i < codeBucket->numberOfEntries; i++)
        {
            if(current != NULL)
            {
                //write bodySize
                writeVarUInt(fp, current->bodySize);
                //write number of local entries
                writeVarUInt(fp, current->localEntryCount);
                //local entries
                currentEntry = current->wasmLocalEntryListHead;
                for(j = 0; j < current->localEntryCount; j++)
                {
                    if(currentEntry != NULL)
                    {
                        writeVarUInt(fp, currentEntry->count);
                        writeByte(fp, currentEntry->valueType);
                        currentEntry = currentEntry->next;
                    }
                    else
                    {
                        break;
                    }
                }
                if(j < current->localEntryCount)
                {
                    returnValue = -1;
                    break;
                }
                //bytecode
                writeByteArray(fp, current->bytecodeSize, current->bytecode);
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
        if(codeBucket->numberOfEntries > 0)
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

