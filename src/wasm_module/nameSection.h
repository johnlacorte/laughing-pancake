#ifndef NAMESECTION_H
#define NAMESECTION_H
#include <stdio.h>

struct WasmName
{
    //I may need a couple different types of names and lists
    unsigned int nameIndex;
    unsigned int nameLength;
    char *name;
    struct WasmName *next;
};

struct WasmLocalNameList
{
    unsigned int numberOfLocalNames;
    unsigned int functionIndex;
    struct WasmName *localNameHead;
    struct WasmName *localNameTail;
    struct WasmLocalNameList *next;
}

struct WasmNameBucket
{
    size_t payloadSize;
    unsigned int moduleNamePayloadSize;
    unsigned int moduleNameLength;
    char *moduleName;
    unsigned int functionNamePayloadSize;
    unsigned int numberOfFunctionNames;
    struct WasmName *functionNameHead;
    struct WasmName *functionNameTail;
    unsigned int localNamePayloadSize;
    unsigned int numberOfLocalNamesLists;
    struct WasmLocalNameList *localNameListListHead;
    struct WasmLocalNameList *localNameListListTail;
};

void initWasmNameBucket(struct WasmNameBucket *);

//section code, nameLength, name, (for functions and locals) nameIndex, and
//(for locals) functionIndex
//maybe have a name module, name function, and name local function for the module level
void addWasmName(struct WasmNameBucket *, unsigned char, unsigned int, char *, unsigned int, unsigned int); //other things too of course

int dumpWasmNameBucket(struct WasmNameBucket *, FILE*);

#endif
