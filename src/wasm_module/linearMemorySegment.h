#ifndef LINEARMEMORYSEGMENT_H
#define LINEARMEMORYSEGMENT_H

struct LinearMemorySegment
{
    unsigned char currentAlignment;
    unsigned int currentSize;
};

void initLinearMemorySegment(struct LinearMemorySegment *);

void padToAlignment(struct LinearMemorySegment *, unsigned char);

unsigned int addToLinearMemorySegment(struct LinearMemorySegment *, unsigned char, unsigned int);

#endif
