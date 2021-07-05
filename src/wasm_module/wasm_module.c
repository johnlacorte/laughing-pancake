//I moved some stuff from wasm_module over here that should be private
//The plan is to add the name section then build another layer on top of this with a name structure that is easier
//to search and stuff.
//I would like to be able to load modules as well, this would allow me to reuse this in a linker.

//from the other file
#include "type_section.h"
#include "import_section.h"
#include "function_section.h"
#include "table_section.h"
#include "memory_section.h"
#include "global_section.h"
#include "export_section.h"
#include "start_section.h"
#include "element_section.h"
#include "code_section.h"
#include "data_section.h"
//#include "name_section.h"
#include "byteBuffer.h"

typedef struct
{
    //maybe keep file pointer for output here
    wasm_type_bucket_t type_bucket;
    wasm_import_bucket_t import_bucket;
    wasm_function_bucket_t function_bucket;
    wasm_table_bucket_t table_bucket;
    wasm_memory_bucket_t memoryBucket;
    wasm_global_bucket_t globalBucket;
    wasm_export_bucket_t exportBucket;
    wasm_start_bucket_t startBucket;
    wasm_element_bucket_t elementBucket;
    wasm_code_bucket_t codeBucket;
    struct WasmDataBucket dataBucket;
    //struct WasmNameBucket nameBucket;
}wasm_module_t;

//fileOutput.h should be copied to all the other files
#include "file_output.h"
#include "wasm_module.h"
#include <stdlib.h>

static void initWasmModule(wasm_module_t *module);

wasm_module_t new_wasm_module(char *filename);
{
    wasm_module_t *newModule;

    newModule = malloc(sizeof(wasm_module_t));
    initWasmModule(newModule);
    return newModule;
}

uint32_t add_type_to_module(wasm_module_t module,
                            uint8_t      return_count, //varuint1
                            value_type_t *return_value_types,
                            uint32_t     number_of_params, //varuint32
                            value_type_t *param_types)
{
    wasm_type_bucket_t *type_bucket;

    type_bucket = &module->type_bucket;
    return addWasmType(type_bucket, returnValueType, numberOfParams, paramTypes);
}

void add_function_import_to_module(wasm_module_t module,
                                   uint32_t      module_name_length,
                                   char          *module_name,
                                   uint32_t      field_name_length,
                                   char          *field_name,
                                   uint32_t      type_index)
{
    wasm_import_bucket_t *import_bucket;

    import_bucket = &module->import_bucket;
    //0 for externalKind = function
    addWasmImport(import_bucket, moduleLength, moduleName, fieldLength, fieldName, 0);
    setImportTypeToFunction(import_bucket, typeIndex);
    module->function_bucket.numberOfImports++;
}

void add_table_import_to_module(wasm_module_t  module,
                                uint32_t       module_name_length,
                                char           *module_name,
                                uint32_t       field_name_length,
                                char           *field_name,
                                element_type_t element_type,
                                flags_t        flags,
                                uint32_t       initial_size,
                                uint32_t       maximum_size)
{
    wasm_import_bucket_t *import_bucket;

    import_bucket = &module->import_bucket;
    //1 for externalKind = table
    addWasmImport(import_bucket, moduleLength, moduleName, fieldLength, fieldName, 1);
    setImportTypeToTable(import_bucket, elementType, flags, initialSize, maximumSize);
    module->table_bucket.numberOfImports++;
}

void addMemoryImportToModule(wasm_module_t *module, unsigned int moduleLength, char *moduleName, unsigned int fieldLength, char *fieldName, unsigned int flags, unsigned int initialSize, unsigned int maximumSize)
{
    wasm_import_bucket_t *import_bucket;

    import_bucket = &module->import_bucket;
    //2 for externalKind = memory
    addWasmImport(import_bucket, moduleLength, moduleName, fieldLength, fieldName, 2);
    setImportTypeToMemory(import_bucket, flags, initialSize, maximumSize);
    module->memoryBucket.numberOfImports++;
}

void addGlobalImportToModule(wasm_module_t *module, unsigned int moduleLength, char *moduleName, unsigned int fieldLength, char *fieldName, unsigned char variableType, unsigned char mutability)
{
    wasm_import_bucket_t *import_bucket;

    import_bucket = &module->import_bucket;
    //3 for externalKind = global
    addWasmImport(import_bucket, moduleLength, moduleName, fieldLength, fieldName, 3);
    setImportTypeToGlobal(import_bucket, variableType, mutability);
    module->globalBucket.numberOfImports++;
}

unsigned int addFunctionToModule(wasm_module_t *module, unsigned int typeIndex)
{
    wasm_function_bucket_t *function_bucket;

    function_bucket = &module->function_bucket;
    return addWasmFunction(function_bucket, typeIndex);
}

unsigned int addTableToModule(wasm_module_t *module, unsigned char elementType, unsigned int flags, unsigned int initialSize, unsigned int maximumSize)
{
    wasm_table_bucket_t *table_bucket;

    table_bucket = &module->table_bucket;
    addWasmTable(table_bucket, elementType, flags, initialSize, maximumSize);
}

void addMemoryToModule(wasm_module_t *module, unsigned int flags, unsigned int initialSize, unsigned int maximumSize)
{
    wasm_memory_bucket_t *memoryBucket;

    memoryBucket = &module->memoryBucket;
    addWasmMemory(memoryBucket, flags, initialSize, maximumSize);
}


unsigned int addGlobalToModule(wasm_module_t *module, unsigned char variableType, unsigned char mutability, struct ByteBuffer *initExpr)
{
    wasm_global_bucket_t *globalBucket;

    globalBucket = &module->globalBucket;
    return addWasmGlobal(globalBucket, variableType, mutability, initExpr->index, initExpr->bytes);
}

void addExportToModule(wasm_module_t *module, unsigned int fieldLength, char *field, unsigned char kind, unsigned int index)
{
    wasm_export_bucket_t *exportBucket;

    exportBucket = &module->exportBucket;
    addWasmExport(exportBucket, fieldLength, field, kind, index);
}

void addStartToModule(wasm_module_t *module, unsigned int functionIndex)
{
    wasm_start_bucket_t *startBucket;

    startBucket = &module->startBucket;
    addWasmStart(startBucket, functionIndex);
}

void addElementToModule(wasm_module_t *module, unsigned int tableIndex, struct ByteBuffer *initExpr, unsigned int numberOfElements, unsigned int *elements)
{
    wasm_element_bucket_t *elementBucket;

    elementBucket = &module->elementBucket;
    addWasmElement(elementBucket, tableIndex, initExpr->index, initExpr->bytes, numberOfElements, elements);
}

void addCodeToModule(wasm_module_t *module, unsigned int variableCount, unsigned char *variableTypes, struct ByteBuffer *bytecode)
{
    wasm_code_bucket_t *codeBucket;

    codeBucket = &module->codeBucket;
    addWasmCode(codeBucket, variableCount, variableTypes, bytecode->index, bytecode->bytes);
}

void addDataToModule(wasm_module_t *module, unsigned int memoryIndex, unsigned int initExprLength, unsigned char *initExpr, unsigned int dataLength, unsigned char *dataBytes)
{
    struct WasmDataBucket *dataBucket;

    dataBucket = &module->dataBucket;
    addWasmData(dataBucket, memoryIndex, initExprLength, initExpr, dataLength, dataBytes);
}

/*
void addNameToModule(wasm_module_t *module)
{
    struct WasmNameBucket *nameBucket;

    nameBucket = &module->nameBucket;
    addWasmName(nameBucket);
}
*/

int dumpWasmModule(wasm_module_t *module, FILE *fp, unsigned int version)
{
    int returnValue = 0;
    wasm_type_bucket_t *type_bucket;
    wasm_import_bucket_t *import_bucket;
    wasm_function_bucket_t *function_bucket;
    wasm_table_bucket_t *table_bucket;
    wasm_memory_bucket_t *memoryBucket;
    wasm_global_bucket_t *globalBucket;
    wasm_export_bucket_t *exportBucket;
    wasm_start_bucket_t *startBucket;
    wasm_element_bucket_t *elementBucket;
    wasm_code_bucket_t *codeBucket;
    struct WasmDataBucket *dataBucket;
    //struct WasmNameBucket *nameBucket;

    writeMagicNumber(fp, version);
    type_bucket = &module->type_bucket;
    dumpWasmtype_bucket(type_bucket, fp);
    import_bucket = &module->import_bucket;
    dumpWasmimport_bucket(import_bucket, fp);
    function_bucket = &module->function_bucket;
    dumpWasmfunction_bucket(function_bucket, fp);
    table_bucket = &module->table_bucket;
    dumpWasmtable_bucket(table_bucket, fp);
    memoryBucket = &module->memoryBucket;
    dumpWasmMemoryBucket(memoryBucket, fp);
    globalBucket = &module->globalBucket;
    dumpWasmGlobalBucket(globalBucket, fp);
    exportBucket = &module->exportBucket;
    dumpWasmExportBucket(exportBucket, fp);
    startBucket = &module->startBucket;
    dumpWasmStartBucket(startBucket, fp);
    elementBucket = &module->elementBucket;
    dumpWasmElementBucket(elementBucket, fp);
    codeBucket = &module->codeBucket;
    dumpWasmCodeBucket(codeBucket, fp);
    dataBucket = &module->dataBucket;
    dumpWasmDataBucket(dataBucket, fp);
    //nameBucket = &module->nameBucket;
    //dumpWasmNameBucket;
    return 0;
}

static void initWasmModule(wasm_module_t *module)
{
    wasm_type_bucket_t *type_bucket;
    wasm_import_bucket_t *import_bucket;
    wasm_function_bucket_t *function_bucket;
    wasm_table_bucket_t *table_bucket;
    wasm_memory_bucket_t *memoryBucket;
    wasm_global_bucket_t *globalBucket;
    wasm_export_bucket_t *exportBucket;
    wasm_start_bucket_t *startBucket;
    wasm_element_bucket_t *elementBucket;
    wasm_code_bucket_t *codeBucket;
    struct WasmDataBucket *dataBucket;
    //struct WasmNameBucket *nameBucket;

    type_bucket = &module->type_bucket;
    import_bucket = &module->import_bucket;
    function_bucket = &module->function_bucket;
    table_bucket = &module->table_bucket;
    memoryBucket = &module->memoryBucket;
    globalBucket = &module->globalBucket;
    exportBucket = &module->exportBucket;
    startBucket = &module->startBucket;
    elementBucket = &module->elementBucket;
    codeBucket = &module->codeBucket;
    dataBucket = &module->dataBucket;
    //nameBucket = &module->nameBucket;

    initWasmtype_bucket(type_bucket);
    initWasmimport_bucket(import_bucket);
    initWasmfunction_bucket(function_bucket);
    initWasmtable_bucket(table_bucket);
    initWasmMemoryBucket(memoryBucket);
    initWasmGlobalBucket(globalBucket);
    initWasmExportBucket(exportBucket);
    initWasmStartBucket(startBucket);
    initWasmElementBucket(elementBucket);
    initWasmCodeBucket(codeBucket);
    initWasmDataBucket(dataBucket);
    //initWasmNameBucket(nameBucket);
}