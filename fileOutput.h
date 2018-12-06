#ifndef FILEOUTPUT_H
#define FILEOUTPUT_H
#include <stdio.h>

//Writes magic number ("\0asm") at the start of a module.
size_t writeMagicNumber(FILE*, unsigned int);

//Writes unsigned char to file.
size_t writeByte(FILE*, unsigned char);

//Returns number of bytes for variable length unsigned int. Useful for payload size.
size_t varUIntSize(unsigned int);

//Writes an unsigned int as a variable length unsigned integer.
size_t writeVarUInt(FILE*, unsigned int);

//Writes unsigned int bytes from unsigned char array.
size_t writeByteArray(FILE*, unsigned int, unsigned char*);

#endif

