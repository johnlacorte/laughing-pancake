#include <stdlib.h>
#include "symbolTable.h"

void initSymbolTable(struct SymbolTable *symbolTable)
{
    symbolTable->symbolTableHead = NULL;
}

int compareNames(unsigned int nameLength1, char *name1, unsigned int nameLength2, char *name2)
{
    
}

struct Symbol *searchSymbolTreeForNew(struct Symbol *symbol, unsigned int nameLength, char *name)
{
    struct Symbol *current = NULL;
    struct Symbol *returnValue = symbol;
    int compare;

    if(symbol != NULL)
    {
        compare = compareNames(nameLength, name, symbol->nameLength, symbol->name);
        if(compare == 0)
        {
            returnValue = NULL;
        }
        else if(compare == -1)
        {
            if(symbol->left != NULL)
            {
                current = symbol->left;
                compare = compareNames(nameLength, name, current->nameLength, current->name);
                //decide to continue searching this branch or insert new symbol here
                if(compare == 0)
                {
                    returnValue = NULL;
                }
                else
                {
                    returnValue = searchSymbolTreeForNew(current, nameLength, name);
                }
            }
            else
            {
                //add to left
                returnValue = malloc(suzeof(struct Symbol));
                returnValue->left = NULL;
                returnValue->right = NULL;
                symbol->left = returnValue;
            }
        }
        else
        {
            if(symbol->right != NULL)
            {
                current = symbol->right;
                compare = compareNames(nameLength, name, current->nameLength, current->name);
                if(compare == 0)
                {
                    returnValue = NULL;
                }
                else
                {
                    returnValue = searchSymbolTreeForNew(current, nameLength, name);
                }
            }
            else
            {
                //add to right
            }
        }
    }
    else
    {
        returnValue = NULL;
        //add symbol to head of tree please
    }
    return returnValue;
}

struct Symbol *searchSymbolTreeForExisting(struct Symbol *symbol, unsigned int nameLength, char *name)
{
    struct Symbol *returnValue = NULL;
    int compare;

    if(symbol != NULL)
    {
        compare = compareNames(nameLength, name, symbol->nameLength, symbol->name);
        if(compare == 0)
        {
            returnValue = symbol;
        }
        else
        {
            returnValue = searchSymbolTreeForExisting(symbol->left);
            if(returnValue == NULL)
            {
                returnValue = searchSymbolTreeForExisting(symbol->right);
            }
        }
    }
    return returnValue;
}

void addSymbolToSymbolTable(struct SymbolTable *symbolTable, unsigned int nameLength, char *name, unsigned int returnCType, unsigned char indexSpace, unsigned char symbolType, unsigned int index)
{
    struct Symbol *current;

    if(symbolTable->symbolTableHead == NULL)
    {
        current = malloc(sizeof(struct Symbol));
        current->left = NULL;
        current->right = NULL;
        symbolTable->symbolTableHead = current;
    }
    else
    {
        current = searchSymbolTreeForNew(symbolTable->symbolTableHead, nameLength, name);
    }
    //set structure members
}

struct Symbol *findSymbolInSymbolTable(struct SymbolTable *symbolTable)
{
    return searchSymbolTreeForExisting(symbolTable->symbolTableHead);
}

void freeSymbolTable(struct SymbolTable *symbolTable)
{

}
