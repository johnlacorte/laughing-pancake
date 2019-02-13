#include "linearMemorySegment.h"

//Dont forget I will probably need to pad to concatenate memory segments

void initLinearMemorySegment(struct LinearMemorySegment *linearMemorySegment)
{
    //set current size to 0
    //set initial alignment to 3
    linearMemorySegment->currentAlignment = 3;
    linearMemorySegment->currentSize = 0;
}

void setCurrentAlignment(struct LinearMemorySegment *linearMemorySegment)
{
    unsigned int number;

    number = linearMemorySegment->currentSize;
    if(number & 1 == 1)
    {
        linearMemorySegment->currentAlignment = 0;
    }
    else if((number >> 1) & 1 == 1)
    {
        linearMemorySegment->currentAlignment = 1;
    }
    else if((number >> 2) & 1 == 1)
    {
        linearMemorySegment->currentAlignment = 2;
    }
    else
    {
        linearMemorySegment->currentAlignment = 3;
    }
}

unsigned int getElementTotalSize(unsigned char elementAlignment, unsigned int numberOfElements)
{
    return numberOfElements << elementAlignment;
}

void padToAlignment(struct LinearMemorySegment *linearMemorySegment, unsigned char elementAlignment)
{
    while(linearMemorySegment->currentAlignment < elementAlignment)
    {
        linearMemorySegment->currentSize++;
        setCurrentAlignment(linearMemorySegment);
    }
}

unsigned int addToLinearMemorySegment(struct LinearMemorySegment *linearMemorySegment, unsigned char elementAlignment, unsigned int numberOfElements)
{
    //element size is given as a power of two alignment and this adds any padding needed
    //before the element.
    unsigned int returnValue;

    padToAlignment(linearMemorySegment, elementAlignment);
    returnValue = linearMemorySegment->currentSize;
    linearMemorySegment->currentSize += getElementTotalSize(elementAlignment, numberOfElements);
    setCurrentAlignment(linearMemorySegment);
    //if current alignment < element alignment, add padding until it matches
    //store the beginning after the padding to a variable to return
    //multiply element number of bytes by number of elements
    //add to current size
    //set current alignment
}

