#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H
#include "byteBuffer.h"
//include wasmModule and whatever else

//global variables have a type of some kind to indicate whether it is a i32, i64, f32, f64, or a structure
//a structure would need to have some information about how to access individual elements and a pointer to the start
//and some way of generating code to set initial value and store the name and type somewhere

//function declarations with and without function body

//structures

//linker section can store function declarations without function body

//type system, name system, bytecode system, linker stuff
//generating types of bytecode such as conditions and statements
//linearMemoryDataSize linearMemoryStaticSize
//
//three types of variables: global, static, local

//function indexes are imported, external, or local
//global indexes are imported, external, or local
//linear memory indexes are either data, global, heap, or stack
//elements would be really hard to relocate since the index might be calculated
//I probably would need each object file to have a numberOfElements
//If I allow for external globals I will need a name section for them

struct codeGenerator
{
    WasmModule *module;
    GlobalSymbolTable *symbolTable;
}
#endif
