#include <stdlib.h>
#include "byteBuffer.h"

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
}

size_t addByteToBuffer(struct ByteBuffer *byteBuffer, unsigned char byte)
{
    size_t bytesWritten = 0;

    if(byteBuffer->index < byteBuffer->size)
    {
        byteBuffer->bytes[byteBuffer->index] = byte;
        byteBuffer->index++;
        bytesWritten = 1;
    }
    return bytesWritten;
}

size_t addVarUIntToBuffer(struct ByteBuffer *byteBuffer, unsigned int number)
{
    size_t bytesWritten = 0;

    do
    {
        if(byteBuffer->index < byteBuffer->size)
        {
            byteBuffer->bytes[byteBuffer->index] = number & 127;
            number = number >> 7;
            if(bytesWritten > 0)
            {
                byteBuffer->bytes[byteBuffer->index - 1] += 128;
            }
            bytesWritten++;
            byteBuffer->index++;
        }
        else
        {
            bytesWritten = 0;
            break;
        }
    } while(number > 0);
    return bytesWritten;
}

size_t addVarIntToBuffer(struct ByteBuffer *byteBuffer, int number)
{
    size_t bytesWritten = 0;
    unsigned int uNumber;

    if(number < 0)
    {
        uNumber = 0xffffffff + number + 1;
    }
    else
    {
        uNumber = 0 + number;
    }
    bytesWritten = addVarUIntToBuffer(byteBuffer, uNumber);
    return bytesWritten;
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

void resetByteBuffer(struct ByteBuffer *byteBuffer)
{
    byteBuffer->index = 0;
}

