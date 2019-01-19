#include <stdio.h>
#include "wasmModule.h"
#include "wasmConstants.h"
#include "opCodes.h"
#include "byteBuffer.h"

int main()
{
    FILE *fp;
    char test[3] = {VAR_I32, VAR_I32};
    char fieldName[] = {"addn"};
    char localType[1] = {VAR_I32};
    char codeBlock[6] = {OP_GET_LOCAL, 0x00, OP_GET_LOCAL, 0x01, OP_I32_ADD, OP_END};
    char globalInit[3] = {OP_I32_CONST, 0x33, OP_END};
    struct WasmModule module;

    fp = fopen("output.bin", "wb");
    initWasmModule(&module);
    //try replacing test with {VAR_I32, VAR_I32}
    addTypeToModule(&module, VAR_I32, 2, test);
    addFunctionToModule(&module, 0);
    addExportToModule(&module, 4, fieldName, 0, 0);
    addCodeToModule(&module, 1, localType, 6, codeBlock);
    addGlobalToModule(&module, VAR_I32, MUT, 3, globalInit);
    addMemoryToModule(&module, 1, 1, 5);
    addTableToModule(&module, VAR_ANYFUNC, 1, 1, 5);
    dumpWasmModule(&module, fp, 1);
    fclose(fp);
    return 0;
}

