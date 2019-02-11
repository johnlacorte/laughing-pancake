#include "cTypes.h"
#include "wasmConstants.h"
#include <stdlib.h>

void initCTypes(struct CTypes *cTypes)
{
    //add void and pointer types, maybe others
    struct CType *typeArray;
    int i;
    int varsize = 0;

    typeArray = malloc(sizeof(struct CType) * 10);
    for(i = 0; i < 8; i = i + 2)
    {
        if(i < 6)
        {   
            typeArray[i].wasmReturnType = VAR_I32;
            typeArray[i + 1].wasmReturnType = VAR_I32;
        }
        else
        {
            typeArray[i].wasmReturnType = VAR_I64;
            typeArray[i + 1].wasmReturnType = VAR_I64;
        }
        typeArray[i].signedness = VAR_SIGNED;
        typeArray[i + 1].signedness = VAR_UNSIGNED;
        typeArray[i].integerOrFloat = VAR_INTEGER;
        typeArray[i + 1].integerOrFloat = VAR_INTEGER;
        typeArray[i].variableSize = varsize;
        typeArray[i + 1].variableSize = varsize;
        varsize++;
    }
    typeArray[8].wasmReturnType = VAR_F32;
    typeArray[8].variableSize = VAR_SIZE_32BIT;
    typeArray[8].signedness = VAR_SIGN_NA;
    typeArray[8].integerOrFloat = VAR_FLOAT;
    typeArray[9].wasmReturnType = VAR_F64;
    typeArray[9].variableSize = VAR_SIZE_64BIT;
    typeArray[9].signedness = VAR_SIGN_NA;
    typeArray[9].integerOrFloat = VAR_FLOAT;

    cTypes->numberOfTypes = 10;
    cTypes->typeArray = typeArray;
}

unsigned char getCTypeWasmReturnType(struct CTypes *cTypes, unsigned int index)
{
    //want to check index and print error message
    return cTypes->typeArray[index].wasmReturnType;
}

unsigned int getCTypeVariableSize(struct CTypes *cTypes, unsigned int index)
{
    return cTypes->typeArray[index].variableSize;
}

unsigned char getCTypeSignedness(struct CTypes *cTypes, unsigned int index)
{
    return cTypes->typeArray[index].signedness;
}

unsigned char getCTypeIntegerOrFloat(struct CTypes *cTypes, unsigned int index)
{
    return cTypes->typeArray[index].integerOrFloat;
}
