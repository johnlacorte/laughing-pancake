//I should add an option to pad varuints for indexes that may change during linking
#include <stdlib.h>
#include "byteBuffer.h"
#include "opCodes.h"
#include "wasmConstants.h"

union Float
{
    float f;
    char b[4];
};

union Double
{
    double d;
    char b[8];
};

void initByteBuffer(struct ByteBuffer *byteBuffer, unsigned int size)
{
    byteBuffer->bytes = malloc(sizeof(unsigned char) * size);
    byteBuffer->index = 0;
    byteBuffer->size = size;
    byteBuffer->head = NULL;
    byteBuffer->tail = NULL;
}

void growByteBuffer(struct ByteBuffer *byteBuffer)
{
    //probably want to return some value if allocate fails
    unsigned char *sourceBuffer;
    unsigned char *newByteBuffer;
    int i;

    sourceBuffer = byteBuffer->bytes;
    newByteBuffer = malloc(sizeof(unsigned char) * byteBuffer->size * 2);
    for(i = 0; i < byteBuffer->index; i++)
    {
        newByteBuffer[i] = sourceBuffer[i];
    }
    free(byteBuffer->bytes);
    byteBuffer->bytes = newByteBuffer;
    byteBuffer->size *= 2;
}

size_t addByteToBuffer(struct ByteBuffer *byteBuffer, unsigned char byte)
{
    size_t bytesWritten = 0;

    if(byteBuffer->index >= byteBuffer->size)
    {
        growByteBuffer(byteBuffer);
    }
    byteBuffer->bytes[byteBuffer->index] = byte;
    byteBuffer->index++;
    bytesWritten = 1;
    return bytesWritten;
}

void addVarUIntToBuffer(struct ByteBuffer *byteBuffer, unsigned int number)
{
    unsigned char byte;

    byte = number & 127;
    number = number >> 7;
    while(number > 0)
    {
        byte += 128;
        addByteToBuffer(byteBuffer, byte);
        byte = number & 127;
        number = number >> 7;
    }
    addByteToBuffer(byteBuffer, byte);
}

void addVarIntToBuffer(struct ByteBuffer *byteBuffer, long number)
{
    unsigned char byte;

    byte = number & 127;
    number = number >> 7;
    while(number != 0 && number != -1)
    {
        byte += 128;
        addByteToBuffer(byteBuffer, byte);
        byte = number & 127;
        number = number >> 7;
    }
    if(number == 0 && (byte & 64) == 64)
    {
        byte += 128;
        addByteToBuffer(byteBuffer, byte);
        byte = number & 127;
    }
    else if(number == -1 && (byte & 64) == 0)
    {
        byte += 128;
        addByteToBuffer(byteBuffer, byte);
        byte = number & 127;
    }
    addByteToBuffer(byteBuffer, byte);
}

size_t addFloatToBuffer(struct ByteBuffer *byteBuffer, float number)
{
    size_t bytesWritten = 4;
    union Float fl;
    int i;

    fl.f = number;
    for(i = 0; i < 4; i++)
    {
        if(addByteToBuffer(byteBuffer, fl.b[i]) != 1)
        {
            bytesWritten = 0;
            break;
        }
    }
    return bytesWritten;
}

size_t addDoubleToBuffer(struct ByteBuffer *byteBuffer, double number)
{
    size_t bytesWritten = 8;
    union Double db;
    int i;

    db.d = number;
    for(i = 0; i < 8; i++)
    {
        if(addByteToBuffer(byteBuffer, db.b[i]) != 1)
        {
            bytesWritten = 0;
            break;
        }
    }
    return bytesWritten;
}

size_t copyArrayToBuffer(struct ByteBuffer *byteBuffer, unsigned int length, char *byteArray)
{
    size_t bytesWritten = 0;
    unsigned int i;

    for(i = 0; i < length; i++)
    {
        if(addByteToBuffer(byteBuffer, byteArray[i]) == 1)
        {
            bytesWritten++;
        }
        else
        {
            bytesWritten = 0;
            break;
        }
    }
    return bytesWritten;
}

void addIndexOffsetToBuffer(struct ByteBuffer *byteBuffer, unsigned char indexType)
{
    struct IndexOffset *newIndexOffset;

    newIndexOffset = malloc(sizeof(struct IndexOffset));
    newIndexOffset->indexType = indexType;
    newIndexOffset->indexOffset = byteBuffer->index;
    newIndexOffset->next = NULL;

    if(byteBuffer->head == NULL)
    {
        byteBuffer->head = newIndexOffset;
        byteBuffer->tail = newIndexOffset;
    }
    else
    {
        byteBuffer->tail->next = newIndexOffset;
        byteBuffer->tail = newIndexOffset;
    }
}

void appendBuffer(struct ByteBuffer *destination, struct ByteBuffer *source)
{
    int i;
    int nextIndex = -1;
    unsigned char indexType;
    unsigned int destinationSize = destination->index;
    struct IndexOffset *indexOffset;

    indexOffset = source->head;
    if(indexOffset != NULL)
    {
        nextIndex = indexOffset->indexOffset;
        indexType = indexOffset->indexType;
    }
    for(i = 0; i < source->index; i++)
    {
        if(i == nextIndex)
        {
            addIndexOffsetToBuffer(destination, indexType);
            indexOffset = indexOffset->next;
            if(indexOffset != NULL)
            {
                nextIndex = indexOffset->indexOffset;
                indexType = indexOffset->indexType;
            }
        }
        addByteToBuffer(destination, source->bytes[i]);
    }
}

void freeIndexOffset(struct IndexOffset *indexOffset)
{
    if(indexOffset != NULL)
    {
        freeIndexOffset(indexOffset->next);
        free(indexOffset);
    }
}

void resetByteBuffer(struct ByteBuffer *byteBuffer)
{
    byteBuffer->index = 0;
    freeIndexOffset(byteBuffer->head);
    byteBuffer->head = NULL;
    byteBuffer->tail = NULL;
}

void freeByteBuffer(struct ByteBuffer *byteBuffer)
{
    resetByteBuffer(byteBuffer);
    free(byteBuffer->bytes);
}

void bb_BLOCK(struct ByteBuffer *byteBuffer, unsigned char returnType)
{
    addByteToBuffer(byteBuffer, OP_BLOCK);
    addByteToBuffer(byteBuffer, returnType);
}

void bb_LOOP(struct ByteBuffer *byteBuffer, unsigned char returnType)
{
    addByteToBuffer(byteBuffer, OP_LOOP);
    addByteToBuffer(byteBuffer, returnType);
}

void bb_IF(struct ByteBuffer *byteBuffer, unsigned char returnType)
{
    addByteToBuffer(byteBuffer, OP_IF);
    addByteToBuffer(byteBuffer, returnType);
}

void bb_BR(struct ByteBuffer *byteBuffer, unsigned int relativeDepth)
{
    addByteToBuffer(byteBuffer, OP_BR);
    addVarUIntToBuffer(byteBuffer, relativeDepth);
}

void bb_BR_IF(struct ByteBuffer *byteBuffer, unsigned int relativeDepth)
{
    addByteToBuffer(byteBuffer, OP_BR_IF);
    addVarUIntToBuffer(byteBuffer, relativeDepth);
}

void bb_BR_TABLE(struct ByteBuffer *byteBuffer, unsigned int targetCount, unsigned int *targetTable, unsigned int defaultTarget)
{
    int i;

    addByteToBuffer(byteBuffer, OP_BR_TABLE);
    addVarUIntToBuffer(byteBuffer, targetCount);
    for(i = 0; i < targetCount; i++)
    {
        addVarUIntToBuffer(byteBuffer, targetTable[i]);
    }
    addVarUIntToBuffer(byteBuffer, defaultTarget);
}

void bb_CALL(struct ByteBuffer *byteBuffer, unsigned char indexType, unsigned int functionIndex)
{
    addByteToBuffer(byteBuffer, OP_CALL);
    //add bytebuffer index to functionIndex structure in byteBuffer
    addIndexOffsetToBuffer(byteBuffer, indexType);
    addVarUIntToBuffer(byteBuffer, functionIndex);    
}

void bb_CALL_INDIRECT(struct ByteBuffer *byteBuffer, unsigned int typeIndex, unsigned char reserved)
{
    addByteToBuffer(byteBuffer, OP_CALL_INDIRECT);
    //store typeindex. The index to the table element is put on the stack after
    //the parameters for the function, this means if I am to use tables for anything
    //I will probably need a tableLength global and maybe store data about elements
    //in the symbol table, and maybe store something with the function that calculates
    //the index to store the index in the linker section.
    addVarUIntToBuffer(byteBuffer, typeIndex);
    addByteToBuffer(byteBuffer, reserved);
}

void bb_GET_LOCAL(struct ByteBuffer *byteBuffer, unsigned int localIndex)
{
    addByteToBuffer(byteBuffer, OP_GET_LOCAL);
    addVarUIntToBuffer(byteBuffer, localIndex);
}

void bb_SET_LOCAL(struct ByteBuffer *byteBuffer, unsigned int localIndex)
{
    addByteToBuffer(byteBuffer, OP_SET_LOCAL);
    addVarUIntToBuffer(byteBuffer, localIndex);
}

void bb_TEE_LOCAL(struct ByteBuffer *byteBuffer, unsigned int localIndex)
{
    addByteToBuffer(byteBuffer, OP_TEE_LOCAL);
    addVarUIntToBuffer(byteBuffer, localIndex);
}

void bb_GET_GLOBAL(struct ByteBuffer *byteBuffer, unsigned char indexType, unsigned int globalIndex)
{
    addByteToBuffer(byteBuffer, OP_GET_GLOBAL);
    //store global index
    addIndexOffsetToBuffer(byteBuffer, indexType);
    addVarUIntToBuffer(byteBuffer, globalIndex);
}

void bb_SET_GLOBAL(struct ByteBuffer *byteBuffer, unsigned char indexType, unsigned int globalIndex)
{
    addByteToBuffer(byteBuffer, OP_SET_GLOBAL);
    //store global index
    addIndexOffsetToBuffer(byteBuffer, indexType);
    addVarUIntToBuffer(byteBuffer, globalIndex);
}

void bb_CURRENT_MEMORY(struct ByteBuffer *byteBuffer, unsigned char reserved)
{
    addByteToBuffer(byteBuffer, OP_CURRENT_MEMORY);
    addByteToBuffer(byteBuffer, reserved);
}

void bb_GROW_MEMORY(struct ByteBuffer *byteBuffer, unsigned char reserved)
{
    addByteToBuffer(byteBuffer, OP_GROW_MEMORY);
    addByteToBuffer(byteBuffer, reserved);
}

void bb_I32_CONST(struct ByteBuffer *byteBuffer, int value)
{
    addByteToBuffer(byteBuffer, OP_I32_CONST);
    addVarIntToBuffer(byteBuffer, value);
}

void bb_I64_CONST(struct ByteBuffer *byteBuffer, long value)
{
    addByteToBuffer(byteBuffer, OP_I64_CONST);
    addVarIntToBuffer(byteBuffer, value);
}

void bb_F32_CONST(struct ByteBuffer *byteBuffer, float value)
{
    addByteToBuffer(byteBuffer, OP_F32_CONST);
    addFloatToBuffer(byteBuffer, value);
}

void bb_F64_CONST(struct ByteBuffer *byteBuffer, double value)
{
    addByteToBuffer(byteBuffer, OP_F64_CONST);
    addDoubleToBuffer(byteBuffer, value);
}

void bb_memorySpaceStart(struct ByteBuffer *byteBuffer, unsigned char memorySpace)
{
    //if any globals are imported, the global indexes will be wrong
    if(memorySpace == MS_DATA)
    {
        bb_I32_CONST(byteBuffer, 4);
    }
    else if(memorySpace == MS_GLOBAL)
    {
        bb_GET_GLOBAL(byteBuffer, GLOBAL_TYPE_MS, 0);
    }
    else if(memorySpace == MS_HEAP)
    {
        bb_GET_GLOBAL(byteBuffer, GLOBAL_TYPE_MS, 1);
    }
    else if(memorySpace == MS_STACK)
    {
        bb_GET_GLOBAL(byteBuffer, GLOBAL_TYPE_MS, 3);
    }
}

void bb_pointerAddition(struct ByteBuffer *byteBuffer, unsigned char variableSize)
{
    bb_I32_CONST(byteBuffer, variableSize);
    addByteToBuffer(byteBuffer, OP_I32_SHL);
    addByteToBuffer(byteBuffer, OP_I32_ADD);
}

void bb_loadOperator(struct ByteBuffer *byteBuffer, unsigned char memorySpace, unsigned char loadInto, unsigned char variableSize, unsigned char isSigned, unsigned int flags, unsigned int offset)
{
    if(loadInto == VAR_I32)
    {
        if(variableSize == VAR_SIZE_8BIT)
        {
            if(isSigned == VAR_SIGNED)
            {
                addByteToBuffer(byteBuffer, OP_I32_LOAD8_S);
            }
            else if(isSigned == VAR_UNSIGNED)
            {
                addByteToBuffer(byteBuffer, OP_I32_LOAD8_U);
            }
        }
        else if(variableSize == VAR_SIZE_16BIT)
        {
            if(isSigned == VAR_SIGNED)
            {
                addByteToBuffer(byteBuffer, OP_I32_LOAD16_S);
            }
            else if(isSigned == VAR_UNSIGNED)
            {
                addByteToBuffer(byteBuffer, OP_I32_LOAD16_U);
            }
        }
        else if(variableSize == VAR_SIZE_32BIT)
        {
            addByteToBuffer(byteBuffer, OP_I32_LOAD);
        }
    }
    else if(loadInto == VAR_I64)
    {
        if(variableSize == VAR_SIZE_8BIT)
        {
            if(isSigned == VAR_SIGNED)
            {
                addByteToBuffer(byteBuffer, OP_I64_LOAD8_S);
            }
            else if(isSigned == VAR_UNSIGNED)
            {
                addByteToBuffer(byteBuffer, OP_I64_LOAD8_U);
            }
        }
        else if(variableSize == VAR_SIZE_16BIT)
        {
            if(isSigned == VAR_SIGNED)
            {
                addByteToBuffer(byteBuffer, OP_I64_LOAD16_S);
            }
            else if(isSigned == VAR_UNSIGNED)
            {
                addByteToBuffer(byteBuffer, OP_I64_LOAD16_U);
            }
        }
        else if(variableSize == VAR_SIZE_32BIT)
        {
            if(isSigned == VAR_SIGNED)
            {
                addByteToBuffer(byteBuffer, OP_I64_LOAD32_S);
            }
            else if(isSigned == VAR_UNSIGNED)
            {
                addByteToBuffer(byteBuffer, OP_I64_LOAD32_U);
            }
        }
        else if(variableSize == VAR_SIZE_64BIT)
        {
            addByteToBuffer(byteBuffer, OP_I64_LOAD);
        }
    }
    else if(loadInto == VAR_F32)
    {
        if(variableSize == VAR_SIZE_32BIT)
        {
            if(isSigned == VAR_SIGN_NA)
            {
                addByteToBuffer(byteBuffer, OP_F32_LOAD);
            }
        }
    }
    else if(loadInto == VAR_F64)
    {
        if(variableSize == VAR_SIZE_64BIT)
        {
            if(isSigned == VAR_SIGN_NA)
            {
                addByteToBuffer(byteBuffer, OP_F64_LOAD);
            }
        }
    }
    addVarUIntToBuffer(byteBuffer, flags);
    //add offset to the linker section with the memory space
    if(memorySpace != MS_STACK)
    {
        addIndexOffsetToBuffer(byteBuffer, memorySpace);
    }
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_storeOperator(struct ByteBuffer *byteBuffer, unsigned char memorySpace, unsigned char storeFrom, unsigned char variableSize, unsigned int flags, unsigned int offset)
{
    if(storeFrom == VAR_I32)
    {
        if(variableSize == VAR_SIZE_8BIT)
        {
            addByteToBuffer(byteBuffer, OP_I32_STORE8);
        }
        else if(variableSize == VAR_SIZE_16BIT)
        {
            addByteToBuffer(byteBuffer, OP_I32_STORE16);
        }
        else if(variableSize == VAR_SIZE_32BIT)
        {
            addByteToBuffer(byteBuffer, OP_I32_STORE);
        }
    }
    else if(storeFrom == VAR_I64)
    {
        if(variableSize == VAR_SIZE_8BIT)
        {
            addByteToBuffer(byteBuffer, OP_I64_STORE8);
        }
        else if(variableSize == VAR_SIZE_16BIT)
        {
            addByteToBuffer(byteBuffer, OP_I64_STORE16);
        }
        else if(variableSize == VAR_SIZE_32BIT)
        {
            addByteToBuffer(byteBuffer, OP_I64_STORE32);
        }
        else if(variableSize == VAR_SIZE_64BIT)
        {
            addByteToBuffer(byteBuffer, OP_I64_STORE);
        }
    }
    else if(storeFrom == VAR_F32)
    {
        if(variableSize == VAR_SIZE_32BIT)
        {
            addByteToBuffer(byteBuffer, OP_F32_STORE);
        }
    }
    else if(storeFrom == VAR_F64)
    {
        if(variableSize == VAR_SIZE_64BIT)
        {
            addByteToBuffer(byteBuffer, OP_F64_STORE);
        }
    }
    addVarUIntToBuffer(byteBuffer, flags);
    //add to linker section
    if(memorySpace != MS_STACK)
    {
        addIndexOffsetToBuffer(byteBuffer, memorySpace);
    }
    addVarUIntToBuffer(byteBuffer, offset);
}

//void bbNegateI32(struct ByteBuffer *byteBuffer)
//{
    
//}

//load and store values
//promoting and casting number types
//if, for, loops, etc
//stack pointer stuff
