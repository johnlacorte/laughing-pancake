#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

struct ByteBuffer
{
    unsigned char *bytes;
    unsigned int index;
    unsigned int size;
};

//size of buffer
void initByteBuffer(struct ByteBuffer*, unsigned int);

size_t addByteToBuffer(struct ByteBuffer*, unsigned char);

size_t addVarUIntToBuffer(struct ByteBuffer*, unsigned int);

size_t addVarIntToBuffer(struct ByteBuffer*, int);

size_t addFloatToBuffer(struct ByteBuffer*, float);

size_t addDoubleToBuffer(struct ByteBuffer*, double);

size_t copyArrayToBuffer(struct ByteBuffer*, unsigned int, char*);

void resetByteBuffer(struct ByteBuffer*);

#endif
