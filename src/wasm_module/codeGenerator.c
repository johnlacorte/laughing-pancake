#include "codeGenerator.h"
#include "opCodes.h"
#include "wasmConstants.h"

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

void bb_CALL(struct ByteBuffer *byteBuffer, unsigned int functionIndex)
{
    addByteToBuffer(byteBuffer, OP_CALL);
    //add bytebuffer index to functionIndex structure in byteBuffer
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

void bb_GET_GLOBAL(struct ByteBuffer *byteBuffer, unsigned int globalIndex)
{
    addByteToBuffer(byteBuffer, OP_GET_GLOBAL);
    //store global index
    addVarUIntToBuffer(byteBuffer, globalIndex);
}

void bb_SET_GLOBAL(struct ByteBuffer *byteBuffer, unsigned int globalIndex)
{
    addByteToBuffer(byteBuffer, OP_SET_GLOBAL);
    //store global index
    addVarUIntToBuffer(byteBuffer, globalIndex);
}

void bb_I32_LOAD(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I32_LOAD);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I64_LOAD(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I64_LOAD);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_F32_LOAD(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_F32_LOAD);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_F64_LOAD(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_F64_LOAD);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I32_LOAD8_S(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I32_LOAD8_S);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I32_LOAD8_U(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I32_LOAD8_U);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I32_LOAD16_S(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I32_LOAD16_S);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I32_LOAD16_U(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I32_LOAD16_U);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I64_LOAD8_S(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I64_LOAD8_S);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I64_LOAD8_U(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I64_LOAD8_U);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I64_LOAD16_S(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I64_LOAD16_S);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I64_LOAD16_U(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I64_LOAD16_U);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I64_LOAD32_S(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I64_LOAD32_S);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I64_LOAD32_U(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I64_LOAD32_U);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I32_STORE(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I32_STORE);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I64_STORE(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I64_STORE);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_F32_STORE(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_F32_STORE);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_F64_STORE(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_F64_STORE);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I32_STORE8(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I32_STORE8);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I32_STORE16(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I32_STORE16);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I64_STORE8(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I64_STORE8);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I64_STORE16(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I64_STORE16);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
}

void bb_I64_STORE32(struct ByteBuffer *byteBuffer, unsigned int flags, unsigned int offset)
{
    addByteToBuffer(byteBuffer, OP_I64_STORE32);
    addVarUIntToBuffer(byteBuffer, flags);
    addVarUIntToBuffer(byteBuffer, offset);
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

void bb_I32_CONST(struct ByteBuffer *byteBuffer, unsigned int value)
{
    addByteToBuffer(byteBuffer, OP_I32_CONST);
    addVarUIntToBuffer(byteBuffer, value);
}

void bb_I64_CONST(struct ByteBuffer *byteBuffer, unsigned long value)
{
    addByteToBuffer(byteBuffer, OP_I64_CONST);
    addVarUIntToBuffer(byteBuffer, value);
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
        bb_GET_GLOBAL(byteBuffer, 0);
    }
    else if(memorySpace == MS_HEAP)
    {
        bb_GET_GLOBAL(byteBuffer, 1);
    }
    else if(memorySpace == MS_STACK)
    {
        bb_GET_GLOBAL(byteBuffer, 2);
    }
}

void bb_pointerAddition(struct ByteBuffer *byteBuffer, unsigned char variableSize, unsigned int index)
{
    if(index > 0)
    {
        bb_I32_CONST(byteBuffer, index);
        bb_I32_CONST(byteBuffer, variableSize);
        addByteToBuffer(byteBuffer, OP_I32_SHL);
        addByteToBuffer(byteBuffer, OP_ADD);
    }
}

void bb_loadOperator(struct ByteBuffer, *byteBuffer, unsigned char memorySpace, unsigned char loadInto, unsigned char variableSize, unsigned char isSigned, unsigned int, flags, unsigned int offset)
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
            else if(isSigned == VAR_SIZE_UNSIGNED)
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
                addByteToBuffer(byteBuffer, OP_F64_LOAD)
            }
        }
    }
    addVarUIntToBuffer(byteBuffer, flags);
    //add offset to the linker section with the memory space
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
        else if(variableSize == VAR_Size_32BIT)
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
    addVarUIntToBuffer(offset);
}

//void bbNegateI32(struct ByteBuffer *byteBuffer)
//{
    
//}

//load and store values
//promoting and casting number types
//if, for, loops, etc
//stack pointer stuff
