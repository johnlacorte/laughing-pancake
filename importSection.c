#include "importSection.h"
#include "fileOutput.h"
#include <stdlib.h>

void initWasmImportBucket(struct WasmImportBucket *importBucket)
{
    importBucket->wasmImportListHead = NULL;
    importBucket->payloadSize = 0;
    importBucket->numberOfEntries = 0;
    importBucket->wasmImportListTail = NULL;
}

void addWasmImport(struct WasmImportBucket *importBucket, unsigned int moduleLength, char *moduleName, unsigned int fieldLength, char *fieldName, unsigned char externalKind)
{
    //Should I set ImportType union to be empty?
    struct WasmImport *current;
    char *characterArray;
    int i;

    if(importBucket->wasmImportListHead == NULL)
    {
        current = malloc(sizeof(struct WasmImport));
        //copy function arguments to new structure
        current->moduleLength = moduleLength;
        current->moduleName = malloc(sizeof(char) * moduleLength);
        //copy module name

        current->fieldLength = fieldLength;
        current->fieldName = malloc(sizeof(char) * fieldLength);
        //copy fieldName
        
        current->externalKind = externalKind;
        current->next = NULL;
        //add payload size
        importBucket->payloadSize += varUIntSize(moduleLength) + moduleLength + varUIntSize(fieldLength) + fieldLength + 1;
        //add number of entries
        importBucket->numberOfEntries++;
        importBucket->wasmImportListHead = current;
        importBucket->wasmImportListTail = current;
    }
    else
    {
        current = importBucket->wasmImportListTail;
        current->next = malloc(sizeof(struct WasmImport));
        current = current->next;
        //copy fuction arguments to new structure
        current->next = NULL;
        //add payload size
        //add number of entries
        importBucket->wasmImportListTail = current;
    }
}

int dumpWasmImportBucket(struct WasmImportBucket *importBucket, FILE *fp)
{
    int returnValue = 0;
    int i, j;
    char *characterArray;
    struct WasmImport *current = NULL;

    if(importBucket->wasmImportListHead != NULL)
    {
        //write section code (2)
        writeVarUInt(fp, 2);
        //write payloadSize including the number of entries in bytes
        writeVarUInt(fp, importBucket->payloadSize + varUIntSize(importBucket->numberOfEntries));
        //number of entries
        writeVarUInt(fp, importBucket->numberOfEntries);
        current = importBucket->wasmImportListHead;
        for(i = 0; i < importBucket->numberOFEntries; i++)
        {
            if(current != NULL)
            {
                //write entry
                writeVarUInt(fp, current->moduleLength);
                writeByteArray(fp, current->moduleLength, current->moduleName);
                writeVarUInt(fp, current->fieldLength);
                writeByteArray(fp, current->fieldName);
                writeVarUInt(fp, current->kind);
                //write one of the four types
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
        if(importBucket->numberOfEntries > 0)
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

