#include "nameSection.h"
#include "fileOuput.h"
#include <stdlib.h>

void initWasmNameBucket(struct WasmNameBucket *nameBucket)
{
    //Should I set module name here and just be done with it?
    nameBucket->payloadSize = 0;
    nameBucket->moduleNamePayloadSize = 0;
    nameBucket->moduleName = NULL;
    nameBucket->functionNamePayloadSize = 0;
    nameBucket->numberOfFunctionNames = 0;
    nameBucket->functionNameHead = NULL;
    nameBucket->functionNameTail = NULL;
    nameBucket->localNamePayloadSize = 0;
    nameBucket->numberOfLocalNameLists = 0;
    nameBucket->localNameListListHead = NULL;
    nameBucket->localNameListListTail = NULL;
}

void addWasmName(struct WasmNameBucket *nameBucket, unsigned char nameSection, unsigned int nameLength, char *name, unsigned int nameIndex, unsigned int functionIndex)
{
    struct WasmName *currentName;
    struct WasmNameList *currentNameList;
    int i;

    if(nameSection == 0)
    {
        //module name
        if(nameBucket->moduleNmaeLength == 0)
        {
            //adding varUIntSize allows for names longer than 127
            //adding moduleNamePayloadSize to payload size instead
            nameBucket->payloadSize = nameLength + 1
            nameBucket->moduleNamePayloadSize = nameLength + 1;
            nameBucket->moduleNameLength = nameLength;
            //copy name to new array
            nameBucket->moduleName = malloc(sizeof(char) * nameLength);
            for(i = 0 i < nameLength; i++)
            {
                nameBucket->moduleName[i] = name[i];
            }
        }
        //else replace name or error
    }
    else if(nameSection == 1)
    {
        //function name
        //add one to payload size and functionNamePayload size if first name for subsection
        //code. make sure to add count and subsection payload size to payload size when
        //dumping section
        if(nameBucket->functionNameHead == NULL)
        {
            //allocate WasmName and copy pointer to functionNameHead and functionNameTail
        }
        else
        {
            //allocate WasmName and copy pointer to functionNameHead->next and functionNameTail
        }
    }
    else if(nameSection == 2)
    {
        //local name
        //add one to payload size and localNamePayloadSize if first name for subsection
        //code
    }
}

int dumpWasmNameBucket(struct WasmNameBucket *nameBucket, FILE *fp)
{
    int returnValue = 0;
    int i;
    struct WasmName *current = NULL;
    //This might have to be a three step process
    //Check if there are names
    //Would it be simpler to check payload size in this case?
    //Section code 0, payload lenth(not including name of section),
    //"name" length, "name", payload
    //
    //each subsection begins with subsection code followed by subsection payload size
    //(yeah, I need to count that as well).
}

