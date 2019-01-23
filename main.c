#include <stdio.h>
#include "wasmModule.h"
#include "wasmConstants.h"
#include "opCodes.h"
#include "byteBuffer.h"

int main()
{
    //char test[3] = {VAR_I32, VAR_I32};
    //char fieldName[] = {"addn"};
    //char localType[1] = {VAR_I32};
    //char codeBlock[6] = {OP_GET_LOCAL, 0x00, OP_GET_LOCAL, 0x01, OP_I32_ADD, OP_END};

    unsigned char globalInit[3] = {OP_I32_CONST, 0x00, OP_END};
    unsigned char stackPointerInit[5] = {OP_I32_CONST, 0xFF, 0xFF, 0x03, OP_END};

    //If I remember all my functions have int parameters of four or less. The
    //addTypeToModule function only reads how many it expects.
    unsigned char intParameters[5] = {VAR_I32, VAR_I32, VAR_I32, VAR_I32, VAR_I32};

    //The value of dataLength global needs to be added to address for these
    unsigned char setRandomBubble[30] = 
    {OP_GET_LOCAL, 0x00, OP_I32_CONST, 0x64, OP_I32_GE_U,
    OP_IF, VAR_EMPTY_BLOCK,
     OP_GET_LOCAL, 0x00, OP_I32_CONST, 0x64, OP_I32_REM_U, OP_SET_LOCAL, 0x00,
    OP_END,
    OP_I32_CONST, 0x90, 0x03, OP_GET_LOCAL, 0x00, OP_I32_CONST, 0x02, OP_I32_SHL, OP_I32_ADD,
    OP_GET_LOCAL, 0x01, OP_I32_STORE, 0x02, 0x00, OP_END};

    unsigned char getRandomBubble[28] =
    {OP_GET_LOCAL, 0x00, OP_I32_CONST, 0x64, OP_I32_GE_U,
    OP_IF, VAR_EMPTY_BLOCK,
     OP_GET_LOCAL, 0x00, OP_I32_CONST, 0x64, OP_I32_REM_U, OP_SET_LOCAL, 0x00,
    OP_END,
    OP_I32_CONST, 0x90, 0x03, OP_GET_LOCAL, 0x00, OP_I32_CONST, 0x02, OP_I32_SHL, OP_I32_ADD,
    OP_I32_LOAD, 0x02, 0x00, OP_END};

    //Maybe set stack pointer here too
    unsigned char startFunction[10] =
    {OP_I32_CONST, 0x00, OP_SET_GLOBAL, 0x00, OP_I32_CONST, 0xec, 0x06, OP_SET_GLOBAL, 0x00, OP_END};
    unsigned int Void_None;
    unsigned int Void_Int;
    unsigned int Void_IntInt;
    unsigned int Void_IntIntInt;
    unsigned int Int_Int;

    unsigned int dataLength = 0;
    unsigned int globalLength = 0;

    FILE *fp;
    struct WasmModule module;

    fp = fopen("output.bin", "wb");
    initWasmModule(&module);

    //set up memory, this should be a command line option
    addMemoryToModule(&module, CAN_GROW, 1, 5); //between one and five pages

    //data section of linear memory length
    addGlobalToModule(&module, VAR_I32, MUT, 3, globalInit);

    //global section of linear memory length
    addGlobalToModule(&module, VAR_I32, MUT, 3, globalInit);

    //stack pointer for linear memory
    addGlobalToModule(&module, VAR_I32, MUT, 5, stackPointerInit);

    //import void js.drawBubble(int, int, int);
    Void_IntIntInt = addTypeToModule(&module, VAR_EMPTY_BLOCK, 3, intParameters);
    addFunctionImportToModule(&module, 2, "js", 10, "drawBubble", Void_IntIntInt);

    //import void js.setScore(int);
    Void_Int = addTypeToModule(&module, VAR_EMPTY_BLOCK, 1, intParameters);
    addFunctionImportToModule(&module, 2, "js", 8, "setScore", Void_Int);

    //import void js.popSound()
    Void_None = addTypeToModule(&module, VAR_EMPTY_BLOCK, 0, intParameters);
    addFunctionImportToModule(&module, 2, "js", 8, "popSound", Void_None);

    //reserve space for global variables. 219 integers, 876 bytes
    //last step is to add moduleStart function to set globals

    //export void setRandomBubble(int index, int value)
    Void_IntInt = addTypeToModule(&module, VAR_EMPTY_BLOCK, 2, intParameters);
    addFunctionToModule(&module, Void_IntInt);
    //add name to name section
    addExportToModule(&module, 15, "setRandomBubble", EXTERNAL_FUNC, 3);

    //{if(index >= 100){index = index % 100} randomBubbles[index] = value;}
    addCodeToModule(&module, 0, intParameters, 30, setRandomBubble);

    //export int getRandomBubble(int index)
    Int_Int = addTypeToModule(&module, VAR_I32, 1, intParameters);
    addFunctionToModule(&module, Int_Int);
    //add name to name section
    addExportToModule(&module, 15, "getRandomBubble", EXTERNAL_FUNC, 4);

    //{if(index >= 100){index = index % 100} return randomBubbles[index];}
    addCodeToModule(&module, 0, intParameters, 28, getRandomBubble);

    //start function
    addFunctionToModule(&module, Void_None);
    addCodeToModule(&module, 0, intParameters, 10, startFunction);
    addStartToModule(&module, 5);


/*
    addTypeToModule(&module, VAR_I32, 2, test);
    addFunctionToModule(&module, 0);
    addExportToModule(&module, 4, fieldName, 0, 0);
    addCodeToModule(&module, 1, localType, 6, codeBlock);
    addGlobalToModule(&module, VAR_I32, MUT, 3, globalInit);
    addMemoryToModule(&module, 1, 1, 5);
    addTableToModule(&module, VAR_ANYFUNC, 1, 1, 5);
*/
//load needs type to load into, size in bytes(mabye 2^x), signedness, and offset
//storing needs type stored from, size in bytes, offset, and bytecode to produce the value
//also if it is global, stack, or local
    //Do I need to make sure there is some javascript function for error messages?
    dumpWasmModule(&module, fp, 1);
    fclose(fp);
    return 0;
}

