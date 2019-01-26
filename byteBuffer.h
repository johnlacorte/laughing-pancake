#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

struct IndexOffset
{
    unsigned char indexType;
    unsigned int indexOffset;
    struct IndexOffset *next;
}

struct ByteBuffer
{
    unsigned char *bytes;
    unsigned int index;
    unsigned int size;
    struct IndexOffset *head;
    struct IndexOffset *tail;
};

//size of buffer
void initByteBuffer(struct ByteBuffer*, unsigned int);

size_t addByteToBuffer(struct ByteBuffer*, unsigned char);

size_t addVarUIntToBuffer(struct ByteBuffer*, unsigned int);

size_t addVarIntToBuffer(struct ByteBuffer*, int);

size_t addFloatToBuffer(struct ByteBuffer*, float);

size_t addDoubleToBuffer(struct ByteBuffer*, double);

size_t copyArrayToBuffer(struct ByteBuffer*, unsigned int, char*);

void appendBuffer(struct ByteBuffer*, struct ByteBuffer*);

void addIndexOffsetToBuffer(struct ByteBuffer*, unsigned char);

void freeIndexOffset(struct IndexOffset*);

void resetByteBuffer(struct ByteBuffer*);

void bb_BLOCK(struct ByteBuffer*, unsigned char);
void bb_LOOP(struct ByteBuffer*, unsigned char);
void bb_IF(struct ByteBuffer*, unsigned char);
void bb_BR(struct ByteBuffer*, unsigned int);
void bb_BR_IF(struct ByteBuffer*, unsigned int);
void bb_BR_TABLE(struct ByteBuffer*, unsigned int, unsigned int*);
void bb_CALL(struct ByteBuffer*, unsigned char, unsigned int);
void bb_CALL_INDIRECT(struct ByteBuffer*, unsigned int, unsigned char);
void bb_GET_LOCAL(struct ByteBuffer*, unsigned int);
void bb_SET_LOCAL(struct ByteBuffer*, unsigned int);
void bb_TEE_LOCAL(struct ByteBuffer*, unsigned int);
void bb_GET_GLOBAL(struct ByteBuffer*, unsigned char, unsigned int);
void bb_SET_GLOBAL(struct ByteBuffer*, unsigned char, unsigned int);
void bb_CURRENT_MEMORY(struct ByteBuffer*, unsigned char);
void bb_GROW_MEMORY(struct ByteBuffer*, unsigned char);
void bb_I32_CONST(struct ByteBuffer*, unsigned int);
void bb_I64_CONST(struct ByteBuffer*, unsigned long);
void bb_F32_CONST(struct ByteBuffer*, float);
void bb_f64_CONST(struct ByteBuffer*, double);

void bb_memorySpaceStart(struct ByteBuffer*, unsigned char);
void bb_pointerAddition(struct ByteBuffer*, unsigned char, unsigned int);

//byteBuffer, memorySpace, loadInto, variableSize, isSigned, flags, offset
void bb_loadOperator(struct ByteBuffer*, unsigned char, unsigned char, unsigned char, unsigned char, unsigned int, unsigned int);

//byteBuffer, memorySpace, storeFrom, variableSize, flags, offset
void bb_storeOperator(struct ByteBuffer*, unsigned char, unsigned char, unsigned char, unsigned int, unsigned int);
#endif
