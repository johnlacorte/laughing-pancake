//Adding a type currently does not check if memory was allocated but everything is checked
//to make sure it matches up when dumping to file and will return a negative return value.
#include "typeSection.h"
#include "fileOutput.h"
#include <stdlib.h>

void initWasmTypeBucket(struct WasmTypeBucket *typeBucket)
{
    typeBucket->wasmTypeListHead = NULL;
    typeBucket->payloadSize = 0;
    typeBucket->numberOfEntries = 0;
}

struct WasmTypeBucket *newWasmTypeBucket()
{
    struct WasmTypeBucket *typeBucket;
    typeBucket = malloc(sizeof(struct WasmTypeBucket));
    typeBucket->wasmTypeListHead = NULL;
    typeBucket->payloadSize = 0;
    typeBucket->numberOfEntries = 0;
    return typeBucket;
}

static int compareTypes(unsigned int numberOfParams, unsigned char *type1, unsigned char *type2)
{
    //Might want to take WasmType pointers instead to check function calls with as well.
    int i;
    int returnValue = 1;
    for(i = 0; i < numberOfParams; ++i)
    {
        if(type1[i] != type2[i])
        {
            returnValue = 0;
            break;
        }
    }
    return returnValue;
}

static struct WasmType *newType(unsigned char returnValueType, unsigned char numberOfParams, unsigned char *paramTypes)
{
    struct WasmType *new;
    unsigned char *paramArray;
    int i;

    //I should allocate memory for params and copy
    new = malloc(sizeof(struct WasmType));
    if(new != NULL){
        new->paramCount = numberOfParams;
        paramArray = malloc(sizeof(unsigned char) * numberOfParams);
        //copy to a new chunk of memory
        for(i = 0; i < numberOfParams; i++)
        {
            paramArray[i] = paramTypes[i];
        }
        new->paramTypes = paramArray;
        new->returnType = returnValueType;
        new->next = NULL;
    }
    return new;
}

static void addSizeToPayloadCounter(struct WasmTypeBucket *typeBucket, unsigned char returnValueType, unsigned char numberOfParams)
{
    size_t sizeOfType = 2;
    //I have 127 as the maximum number of parameters but if that ever changes this will work.
    sizeOfType += varUIntSize(numberOfParams);
    sizeOfType += numberOfParams;
    if(returnValueType != 0x40)
    {
        sizeOfType++;
    }
    typeBucket->payloadSize += sizeOfType;
}

//This may get moved "one level up".
//This may also take a WasmType pointer instead. Not sure yet.
unsigned int addWasmType(struct WasmTypeBucket *typeBucket, unsigned char returnValueType, unsigned char numberOfParams, unsigned char *paramTypes)
{
    struct WasmType *current;
    unsigned int typeIndex = 0;
    int checkReturnValue;

    //Add to the head of the list
    if(typeBucket->wasmTypeListHead == NULL)
    {
        typeBucket->wasmTypeListHead = newType(returnValueType, numberOfParams, paramTypes);
        addSizeToPayloadCounter(typeBucket, returnValueType, numberOfParams);
        typeBucket->numberOfEntries++;
    }
    //Find a matching type or add a new one if not found.
    else
    {
        current = typeBucket->wasmTypeListHead;
        while(current != NULL)
        {
            if(current->returnType == returnValueType)
            {
                if(current->paramCount == numberOfParams)
                {
                    checkReturnValue = compareTypes(numberOfParams, paramTypes, current->paramTypes);
                    if(checkReturnValue == 1)
                    {
                        break;
                    }
                }
            }
            if(current->next != NULL)
            {
                current = current->next;
            }
            else
            {
                current->next = newType(returnValueType, numberOfParams, paramTypes);
                addSizeToPayloadCounter(typeBucket, returnValueType, numberOfParams);
                typeBucket->numberOfEntries++;
            }
            typeIndex++;
        }
    }
    return typeIndex;
}


//This may get moved "one level up".
//struct WasmType *getWasmTypePointer(struct WasmTypeBucket *typeBucket, unsigned int index)
//{
    
//}

int dumpWasmTypeBucket(struct WasmTypeBucket *typeBucket, FILE *fp)
{
    int returnValue = 0;
    int i;
    struct WasmType *current = NULL;

    if(typeBucket->wasmTypeListHead != NULL)
    {
        //section code
        writeVarUInt(fp, 1);
        //payload size including the numberOfEntries var uint
        writeVarUInt(fp, varUIntSize(typeBucket->numberOfEntries) + typeBucket->payloadSize);
        //write numberOfEntries
        writeVarUInt(fp, typeBucket->numberOfEntries);
        //loop to write out each entry
        current = typeBucket->wasmTypeListHead;
        for(i = 0; i < typeBucket->numberOfEntries; i++)
        {
            //check if current is NULL
            if(current != NULL)
            {
                //write function type constructor
                writeByte(fp, 0x60);
                //write number of parameters
                writeVarUInt(fp, current->paramCount);
                //write parameter types
                writeByteArray(fp, current->paramCount, current->paramTypes);
                //write return type count (0 or 1)
                if(current->returnType == 0x40)
                {
                    writeByte(fp, 0);
                }
                else
                {
                    writeByte(fp, 1);
                    writeByte(fp, current->returnType);
                }
                //write current->returnType
                current = current->next;
                returnValue++;
            }
            else
            {
                returnValue = -1;
                break;
            }
        }
            //check to make sure current is NULL
    }
    else
    {
        //list head is NULL but number of entries > 0
        if(typeBucket->numberOfEntries > 0)
        {
            returnValue = -1;
        }
    }
    //check if the number of types in list is greater than the counter.
    if(current != NULL)
    {
        returnValue = -1;
    }
    //Possibly return the number of types written or -1 if it fails
    return returnValue;
}

