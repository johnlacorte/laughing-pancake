#ifndef CTYPES_H
#define CTYPES_H

struct CType
{
    unsigned char wasmReturnType;
    unsigned int variableSize;
    unsigned char signedness;
    unsigned char integerOrFloat;
};

struct CTypes
{
    unsigned int numberOfTypes;
    struct CType *typeArray;
};

#endif
