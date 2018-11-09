#include "exportSection.h"
#include "fileOutput.h"
#include <stdlib.h>

void initWasmExportBucket(struct WasmExportBucket *exportBucket)
{
    exportBucket->wasmExportListHead = NULL;
    exportBucket->payloadSize = 0;
    exportBucket->numberOfEntries = 0;
    exportBucket->wasmExportListTail = NULL;
}

void addWasmExport(struct WasmExportBucket *exportBucket, unsigned int fieldLength, char *field, unsigned char kind, unsigned int index)
{
    //The function section and every other section may make more sense to have a tail pointer.
    //adding a function should add an item to the code section and a  pointer to an empty 
    //function body.
    struct WasmExport *current;
    char *stringPointer;
    int i;

    //I know I could have the repeating code in both blocks placed after but I will
    //probably have to change this later and this makes it easier to rewrite. I may,
    //for example, check for matching field names.
    if(exportBucket->wasmExportListHead == NULL)
    {
        current = malloc(sizeof(struct WasmExport));
        current->fieldLength = fieldLength;
        exportBucket->payloadSize += varUIntSize(fieldLength);
        current->fieldString = malloc(sizeof(char) * fieldLength);
        //Copy field name to a new character array
        stringPointer = current->fieldString;
        for(i = 0; i < fieldLength; i++)
        {
            stringPointer[i] = field[i];
        }
        exportBucket->payloadSize += fieldLength;
        current->kind = kind;
        exportBucket->payloadSize++;
        current->index = index;
        exportBucket->payloadSize += varUIntSize(index);
        current->next = NULL;
        exportBucket->wasmExportListHead = current;
        exportBucket->wasmExportListTail = current;
        exportBucket->numberOfEntries++;
    }
    else
    {
        current = exportBucket->wasmExportListTail;
        current->next = malloc(sizeof(struct WasmExport));
        current = current->next;
        current->fieldLength = fieldLength;
        exportBucket->payloadSize += varUIntSize(fieldLength);
        current->fieldString = malloc(sizeof(char) * fieldLength);
        stringPointer = current->fieldString;
        for(i = 0; i < fieldLength; i++)
        {
            stringPointer[i] = field[i];
        }
        exportBucket->payloadSize += fieldLength;
        current->kind = kind;
        exportBucket->payloadSize++;
        current->index = index;
        exportBucket->payloadSize += varUIntSize(index);
        current->next = NULL;
        exportBucket->wasmExportListTail = current;
        exportBucket->numberOfEntries++;
    }
}

int dumpWasmExportBucket(struct WasmExportBucket *exportBucket, FILE *fp)
{
    int returnValue = 0;
    int i;
    struct WasmExport *current;

    if(exportBucket->wasmExportListHead != NULL)
    {
        //section code
        writeVarUInt(fp, 7);
        //payload size including the numberOfEntries var uint
        writeVarUInt(fp, varUIntSize(exportBucket->numberOfEntries) + exportBucket->payloadSize);
        //write number of entries
        writeVarUInt(fp, exportBucket->numberOfEntries);
        //loop to write out each entry
        current = exportBucket->wasmExportListHead;
        for(i = 0; i < exportBucket->numberOfEntries; i++)
        {
            if(current != NULL)
            {
                writeVarUInt(fp, current->fieldLength);
                writeByteArray(fp, current->fieldLength, current->fieldString);
                writeByte(fp, current->kind);
                writeVarUInt(fp, current->index);
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
        if(exportBucket->numberOfEntries > 0)
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

