//functions for setting the import to function, table, memory or global
//maybe functions for dumping these to file
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
        characterArray = current->moduleName;
        for(i = 0; i < moduleLength; i++)
        {
            characterArray[i] = moduleName[i];
        }
        current->fieldLength = fieldLength;
        current->fieldName = malloc(sizeof(char) * fieldLength);
        //copy fieldName
        characterArray = current->fieldName;
        for(i = 0; i < fieldLength; i++)
        {
            characterArray[i] = fieldName[i];
        }
        current->externalKind = externalKind;
        current->next = NULL;
        //add payload size, the 1 is for the externalKind.
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
        current->moduleLength = moduleLength;
        current->moduleName = malloc(sizeof(char) * moduleLength);
        //copy module name
        characterArray = current->moduleName;
        for(i = 0; i < moduleLength; i++)
        {
            characterArray[i] = moduleName[i];
        }
        current->fieldLength = fieldLength;
        current->fieldName = malloc(sizeof(char) * fieldLength);
        //copy fieldName
        characterArray = current->fieldName;
        for(i = 0; i < fieldLength; i++)
        {
            characterArray[i] = fieldName[i];
        }
        current->externalKind = externalKind;
        current->next = NULL;
        //add payload size
        importBucket->payloadSize += varUIntSize(moduleLength) + moduleLength + varUIntSize(fieldLength) + fieldLength + 1;
        //add number of entries
        importBucket->numberOfEntries++;
        importBucket->wasmImportListTail = current;
    }
}

void setImportTypeToFunction(struct WasmImportBucket *importBucket, unsigned int typeIndex)
{
    struct WasmImport *tail = importBucket->wasmImportListTail;
    if(tail->externalKind == 0)
    {
        tail->importType.functionType = typeIndex;
        importBucket->payloadSize += varUIntSize(typeIndex);
    }
}

void setImportTypeToTable(struct WasmImportBucket *importBucket, unsigned char elementType, unsigned int flags, unsigned int initialSize, unsigned int maximumSize)
{
    struct WasmImport *tail = importBucket->wasmImportListTail;
    if(tail->externalKind == 1)
    {
        tail->importType.tableType.elementType = elementType;
        tail->importType.tableType.flags = flags;
        tail->importType.tableType.initialSize = initialSize;
        tail->importType.tableType.maximumSize = maximumSize;
        //elementType is one byte
        importBucket->payloadSize += 1 + varUIntSize(flags) + varUIntSize(initialSize) + varUIntSize(maximumSize);
    }
}

void setImportTypeToMemory(struct WasmImportBucket *importBucket, unsigned int flags, unsigned int initialSize, unsigned int maximumSize)
{
    struct WasmImport *tail = importBucket->wasmImportListTail;
    if(tail->externalKind == 2)
    {
        tail->importType.memoryType.flags = flags;
        tail->importType.memoryType.initialSize = initialSize;
        tail->importType.memoryType.maximumSize = maximumSize;
        importBucket->payloadSize += varUIntSize(flags) + varUIntSize(initialSize) + varUIntSize(maximumSize);
    }
}

void setImportTypeToGlobal(struct WasmImportBucket *importBucket, unsigned char variableType, unsigned char mutability)
{
    struct WasmImport *tail = importBucket->wasmImportListTail;
    if(tail->externalKind == 3)
    {
        tail->importType.globalType.variableType = variableType;
        tail->importType.globalType.mutability = mutability;
        //variableType and mutability are both bytes
        importBucket->payloadSize += 2;
    }
}

void writeImportType(FILE *fp, unsigned char externalKind, union ImportType importType)
{
    //I need to add to the payload size for the above functions
    if(externalKind == 0)
    {
        //function
        writeVarUInt(fp, importType.functionType);
    }
    if(externalKind == 1)
    {
        //table
        writeByte(fp, importType.tableType.elementType);
        writeVarUInt(fp, importType.tableType.flags);
        writeVarUInt(fp, importType.tableType.initialSize);
        writeVarUInt(fp, importType.tableType.maximumSize);
    }
    if(externalKind == 2)
    {
        //memory
        writeVarUInt(fp, importType.memoryType.flags);
        writeVarUInt(fp, importType.memoryType.initialSize);
        writeVarUInt(fp, importType.memoryType.maximumSize);
    }
    if(externalKind ==3)
    {
        //global
        writeByte(fp, importType.globalType.variableType);
        writeByte(fp, importType.globalType.mutability);
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
        for(i = 0; i < importBucket->numberOfEntries; i++)
        {
            if(current != NULL)
            {
                //write entry
                writeVarUInt(fp, current->moduleLength);
                writeByteArray(fp, current->moduleLength, current->moduleName);
                writeVarUInt(fp, current->fieldLength);
                writeByteArray(fp, current->fieldLength, current->fieldName);
                writeVarUInt(fp, current->externalKind);
                //write one of the four types
                writeImportType(fp, current->externalKind, current->importType);
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

