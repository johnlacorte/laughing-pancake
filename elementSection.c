#include "elementSection.h"
#include "fileOutput.h"
#include <stdlib.h>

void initWasmElementBucket(struct WasmElementBucket *elementBucket)
{
    elementBucket->wasmElementListHead = NULL;
    elementBucket->payloadSize = 0;
    elementBucket->numberOfEntries = 0;
    elementBucket->wasmElementListTail = NULL;
}

void addWasmElement(struct WasmElementBucket *elementBucket, unsigned int tableIndex, unsigned int initExprLength, unsigned char *initExpr, unsigned int numberOfElements, unsigned int *elements)
{
    struct WasmElement *current;
    unsigned char *byteArray;
    unsigned int *intArray;
    int i;

    if(elementBucket->wasmElementListHead == NULL)
    {
        current = malloc(sizeof(struct WasmElement));
        //copy function arguments to new structure
        current->tableIndex = tableIndex;
        current->initExprLength = initExprLength;
        current->initExpr = malloc(sizeof(unsigned char) * initExprLength);
        byteArray = current->initExpr;
        for(i = 0; i < initExprLength; i++)
        {
            byteArray[i] = initExpr[i];
        }
        current->numberOfElements = numberOfElements;
        current->elements = malloc(sizeof(unsigned int) * numberOfElements);
        intArray = current->elements;
        for(i = 0; i < numberOfElements; i++)
        {
            intArray[i] = elements[i];
            elementBucket->payloadSize += varUIntSize(intArray[i]);
        }
        current->next = NULL;
        //add payload size
        elementBucket->payloadSize += varUIntSize(tableIndex) + initExprLength + varUIntSize(numberOfElements);
        //add number of entries
        elementBucket->numberOfEntries++;
        elementBucket->wasmElementListHead = current;
        elementBucket->wasmElementListTail = current;
    }
    else
    {
        current = elementBucket->wasmElementListTail;
        current->next = malloc(sizeof(struct WasmElement));
        current = current->next;
        //copy function arguments to new structure
        current->tableIndex = tableIndex;
        current->initExprLength = initExprLength;
        current->initExpr = malloc(sizeof(unsigned char) * initExprLength);
        byteArray = current->initExpr;
        for(i = 0; i < initExprLength; i++)
        {
            byteArray[i] = initExpr[i];
        }
        current->numberOfElements = numberOfElements;
        current->elements = malloc(sizeof(unsigned int) * numberOfElements);
        intArray = current->elements;
        for(i = 0; i < numberOfElements; i++)
        {
            intArray[i] = elements[i];
            elementBucket->payloadSize += varUIntSize(intArray[i]);
        }
        current->next = NULL;
        //add payload size
        elementBucket->payloadSize += varUIntSize(tableIndex) + initExprLength + varUIntSize(numberOfElements);
        //add numberOfEntries
        elementBucket->numberOfEntries++;
        elementBucket->wasmElementListTail = current;
    }
}

int dumpWasmElementBucket(struct WasmElementBucket *elementBucket, FILE *fp)
{
    int returnValue = 0;
    int i, j;
    unsigned int *intArray;
    struct WasmElement *current = NULL;

    if(elementBucket->wasmElementListHead != NULL)
    {
        //write section code (9)
        writeVarUInt(fp, 9);
        //write payload size including size of number of entries in bytes
        writeVarUInt(fp, elementBucket->payloadSize + varUIntSize(elementBucket->numberOfEntries));
        //numberOfEntries
        writeVarUInt(fp, elementBucket->numberOfEntries);
        current = elementBucket->wasmElementListHead;
        for(i = 0; i < elementBucket->numberOfEntries; i++)
        {
            if(current != NULL)
            {
                //write entry
                writeVarUInt(fp, current->tableIndex);
                writeByteArray(fp, current->initExprLength, current->initExpr);
                writeVarUInt(fp, current->numberOfElements);
                intArray = current->elements;
                for(j = 0; j < current->numberOfElements; i++)
                {
                    writeVarUInt(fp, intArray[j]);
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
        if(elementBucket->numberOfEntries > 0)
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

