#include "typeSection.h"
#include "fileOutput.h"
#include <stdlib.h>

void initWasmTypeBucket(WasmTypeBucket *typeBucket)
{
    typeBucket->wasmTypeListHead = NULL;
    typeBucket->payloadSize = 0;
    typeBucket->numberOfEntries = 0;
}

static int compareTypes(unsigned int numberOfParams, unsigned char *type1, unsigned char *type2)
{
    //Might want to take WasmType pointers instead to check function calls with as well.
}

static struct WasmType *newType(unsigned char returnValueType, unsigned char numberOfParams, char *paramTypes)
{

}

//This may get moved "one level up".
int addType(struct WasmTypeBucket *typeBucket, unsigned char returnType, unsigned char numberOfParams, unsigned char *paramTypes)
{

}

//This may get moved "one level up".
struct WasmType *getWasmTypePointer(struct WasmTypeBucket *typeBucket, unsigned int index)
{
    
}

int dumpWasmTypeBucket(FILE *fp, struct WasmTypeBucket *typeBucket)
{

}
