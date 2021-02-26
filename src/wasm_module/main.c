#include <stdio.h>
#include "wasmModule.h"
#include "wasmConstants.h"
#include "opCodes.h"
#include "byteBuffer.h"

unsigned char intParameters[5] = {VAR_I32, VAR_I32, VAR_I32, VAR_I32, VAR_I32};

void setUpModule(struct WasmModule *module, unsigned int memoryMin, unsigned int memoryMax)
{
    //This is all stuff done before generating a final module and not for object files
    //I want this to have an option for imported memory
    struct ByteBuffer globalInit;
    initByteBuffer(&globalInit, 5);
    bb_I32_CONST(&globalInit, 0);
    addByteToBuffer(&globalInit, OP_END);
    if(memoryMin == memoryMax)
    {
        addMemoryToModule(module, CANNOT_GROW, memoryMin, memoryMax);
    }
    else
    {
        addMemoryToModule(module, CAN_GROW, memoryMin, memoryMax);
    }
    addGlobalToModule(module, VAR_I32, MUT, &globalInit);
    addGlobalToModule(module, VAR_I32, MUT, &globalInit);
    addGlobalToModule(module, VAR_I32, MUT, &globalInit);
    addGlobalToModule(module, VAR_I32, MUT, &globalInit);
    freeByteBuffer(&globalInit);
}

void addImports(struct WasmModule *module)
{
    unsigned int typeIndex;
    //import void js.drawBubble(int, int, int);
    typeIndex = addTypeToModule(module, VAR_EMPTY_BLOCK, 3, intParameters);
    addFunctionImportToModule(module, 2, "js", 10, "drawBubble", typeIndex);

    //import void js.setScore(int);
    typeIndex = addTypeToModule(module, VAR_EMPTY_BLOCK, 1, intParameters);
    addFunctionImportToModule(module, 2, "js", 8, "setScore", typeIndex);

    //import void js.popSound()
    typeIndex = addTypeToModule(module, VAR_EMPTY_BLOCK, 0, intParameters);
    addFunctionImportToModule(module, 2, "js", 8, "popSound", typeIndex);
}

void addSetRandomBubble(struct WasmModule *module)
{
    //Does addFunctionToModule return the function index?
    unsigned int typeIndex;
    struct ByteBuffer bytecode;
    initByteBuffer(&bytecode, 40);

    bb_GET_LOCAL(&bytecode, 0);
    bb_I32_CONST(&bytecode, 100);
    addByteToBuffer(&bytecode, OP_I32_GE_U);

    addByteToBuffer(&bytecode, OP_IF);
    addByteToBuffer(&bytecode, VAR_EMPTY_BLOCK);

    bb_GET_LOCAL(&bytecode, 0);
    bb_I32_CONST(&bytecode, 100);
    addByteToBuffer(&bytecode, OP_I32_REM_U);
    bb_SET_LOCAL(&bytecode, 0);
    addByteToBuffer(&bytecode, OP_END);

    bb_memorySpaceStart(&bytecode, MS_GLOBAL);
    bb_GET_LOCAL(&bytecode, 0);//The index in the array
    bb_pointerAddition(&bytecode, VAR_SIZE_32BIT);
    bb_GET_LOCAL(&bytecode, 1);//The value to store
    bb_storeOperator(&bytecode, MS_GLOBAL, VAR_I32, VAR_SIZE_32BIT, VAR_SIZE_32BIT, 400);
    addByteToBuffer(&bytecode, OP_END);

    /*
    unsigned char setRandomBubble[30] = 
    {OP_GET_LOCAL, 0x00, OP_I32_CONST, 0x64, OP_I32_GE_U,
    OP_IF, VAR_EMPTY_BLOCK,
     OP_GET_LOCAL, 0x00, OP_I32_CONST, 0x64, OP_I32_REM_U, OP_SET_LOCAL, 0x00,
    OP_END,
    OP_I32_CONST, 0x90, 0x03, OP_GET_LOCAL, 0x00, OP_I32_CONST, 0x02, OP_I32_SHL, OP_I32_ADD,
    OP_GET_LOCAL, 0x01, OP_I32_STORE, 0x02, 0x00, OP_END};
    */

    //export void setRandomBubble(int index, int value)
    typeIndex = addTypeToModule(module, VAR_EMPTY_BLOCK, 2, intParameters);
    addFunctionToModule(module, typeIndex);
    //add name to name section
    addExportToModule(module, 15, "setRandomBubble", EXTERNAL_FUNC, 3);

    //{if(index >= 100){index = index % 100} randomBubbles[index] = value;}
    addCodeToModule(module, 0, intParameters, &bytecode);
    freeByteBuffer(&bytecode);
}

void addGetRandomBubble(struct WasmModule *module)
{
    unsigned int typeIndex;
    struct ByteBuffer bytecode;
    initByteBuffer(&bytecode, 30);
    bb_GET_LOCAL(&bytecode, 0);
    bb_I32_CONST(&bytecode, 100);
    addByteToBuffer(&bytecode, OP_I32_GE_U);

    addByteToBuffer(&bytecode, OP_IF);
    addByteToBuffer(&bytecode, VAR_EMPTY_BLOCK);

    bb_GET_LOCAL(&bytecode, 0);
    bb_I32_CONST(&bytecode, 100);
    addByteToBuffer(&bytecode, OP_I32_REM_U);
    bb_SET_LOCAL(&bytecode, 0);
    addByteToBuffer(&bytecode, OP_END);

    bb_memorySpaceStart(&bytecode, MS_GLOBAL);
    bb_GET_LOCAL(&bytecode, 0);
    bb_pointerAddition(&bytecode, VAR_SIZE_32BIT);
    bb_loadOperator(&bytecode, MS_GLOBAL, VAR_I32, VAR_SIZE_32BIT, VAR_SIGNED, VAR_SIZE_32BIT, 400);
    addByteToBuffer(&bytecode, OP_END);
    /*
    unsigned char getRandomBubble[28] =
    {OP_GET_LOCAL, 0x00, OP_I32_CONST, 0x64, OP_I32_GE_U,
    OP_IF, VAR_EMPTY_BLOCK,
     OP_GET_LOCAL, 0x00, OP_I32_CONST, 0x64, OP_I32_REM_U, OP_SET_LOCAL, 0x00,
    OP_END,
    OP_I32_CONST, 0x90, 0x03, OP_GET_LOCAL, 0x00, OP_I32_CONST, 0x02, OP_I32_SHL, OP_I32_ADD,
    OP_I32_LOAD, 0x02, 0x00, OP_END};
    */

    //export int getRandomBubble(int index)
    typeIndex = addTypeToModule(module, VAR_I32, 1, intParameters);
    addFunctionToModule(module, typeIndex);
    //add name to name section
    addExportToModule(module, 15, "getRandomBubble", EXTERNAL_FUNC, 4);

    //{if(index >= 100){index = index % 100} return randomBubbles[index];}
    addCodeToModule(module, 0, intParameters, &bytecode);
    freeByteBuffer(&bytecode);
}
 
void addStartFunction(struct WasmModule *module, int dataLength, int globalLength, unsigned int numberOfMemoryPages)
{
    //This needs to generate constants from values given
    //I need to make sure there is a constant for memory segment pointers to pass to GET_GLOBAL and SET_GLOBAL
    unsigned int typeIndex;
    int globalSectionStart, heapSectionStart, stackSectionStart, stackPointer;
    unsigned char startFunction[10] =
    {OP_I32_CONST, 0x00, OP_SET_GLOBAL, 0x00, OP_I32_CONST, 0xec, 0x06, OP_SET_GLOBAL, 0x00, OP_END};
    struct ByteBuffer bytecode;
    initByteBuffer(&bytecode, 20);
    
    globalSectionStart = dataLength + 4;
    heapSectionStart = globalSectionStart + globalLength;
    stackPointer = numberOfMemoryPages * 16000;
    stackSectionStart = (heapSectionStart + stackPointer) / 2;

    bb_I32_CONST(&bytecode, globalSectionStart);
    bb_SET_GLOBAL(&bytecode, GLOBAL_TYPE_MS, 0);//global segment start
    bb_I32_CONST(&bytecode, heapSectionStart);
    bb_SET_GLOBAL(&bytecode, GLOBAL_TYPE_MS, 1);//heap segment start
    bb_I32_CONST(&bytecode, stackSectionStart);
    bb_SET_GLOBAL(&bytecode, GLOBAL_TYPE_MS, 2);//stack segment start
    addByteToBuffer(&bytecode, OP_END);
    //start function
    typeIndex = addTypeToModule(module, VAR_EMPTY_BLOCK, 0, intParameters);
    addFunctionToModule(module, typeIndex);
    //This has a return value of it's index doesn't it?
    addCodeToModule(module, 0, intParameters, &bytecode);
    addStartToModule(module, 5);
    freeByteBuffer(&bytecode);
}

int main()
{
    unsigned char stackPointerInit[5] = {OP_I32_CONST, 0xFF, 0xFF, 0x03, OP_END};

    //Maybe set stack pointer here too
    int dataLength = 0;
    int globalLength = 876;

    FILE *fp;
    struct WasmModule module;

    fp = fopen("output.bin", "wb");
    initWasmModule(&module);

    setUpModule(&module, 1, 5);
    addImports(&module);
    addSetRandomBubble(&module);
    addGetRandomBubble(&module);
    //reserve space for global variables. 219 integers, 876 bytes
    //last step is to add moduleStart function to set globals
    addStartFunction(&module, dataLength, globalLength, 1);


//load needs type to load into, size in bytes(mabye 2^x), signedness, and offset
//storing needs type stored from, size in bytes, offset, and bytecode to produce the value
//also if it is global, stack, or local
    //Do I need to make sure there is some javascript function for error messages?
    dumpWasmModule(&module, fp, 1);
    fclose(fp);
    return 0;
}

