#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

// I will have to pass the return value CType index and the highest index used in an
// expression to the expression bytecode generator and this will have to figure out
// promotions and demotions
//
// I think I need to add void and pointer types too.

struct StructTable
{
    //how about making memory segments and stack frames structs?
};

struct Symbol
{
    unsigned int nameLength;
    char *name;
    unsigned int returnCType;
    unsigned char indexSpace;
    unsigned char symbolType; //primitive type, function, pointer, array, structure
    unsigned int index;
    struct Symbol *left;
    struct Symbol *right;
    //parameters or size
    //symbol list?
};

struct SymbolTable
{
    //local or global?
    struct Symbol *symbolTableHead;
};

struct GlobalSymbolTable
{
    unsigned int dataSegmentLength;
    //Do I want to use a ByteBuffer for data segment?
    unsigned int globalSegmentLength;//This might be in the symbol table
    //I'm thinking that individual symbols could be linked be pointers
    unsigned int numberOfImportedFunctions;
    unsigned int numberOfLocalFunctions;
    unsigned int numberOfExternalFunctions;
    unsigned int numberOfImportedVariables;
    unsigned int numberOfLocalVariables;
    unsigned int numberOfExternalVariables;
    //maybe numberOfTables, numberOfTableElements, numberOfMemories
    struct SymbolTable *symbolTable;
};

#endif

