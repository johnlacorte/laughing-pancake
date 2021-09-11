#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../wasm_module/wasm_constants.h"
#include "ast.h"

ast_t new_ast(char *output_file)
{
    return NULL;
}

void free_ast(ast_t ast)
{

}

char *add_identifier_name(ast_t ast, char *name)
{
    return NULL;
}

value_type_array_t new_value_type_array()
{
    return NULL;
}

void free_value_type_array(value_type_array_t value_type_array)
{

}

bool add_value_type_to_value_type_array(value_type_array_t value_type_array,
                                        char *name,
                                        value_type_t value_type)
{
    return false;
}

int get_named_value_type_index(value_type_array_t value_type_array, char *value_name)
{
    return -1;
}

//name is a little useless. I'm thinking that type entries should go into the
//module exactly how they are because they are probably referenced by index
//that would mean those given within an import or a function would have to have
//their own set of indexes  for any that don't match an existing one that would
//need to be renumbered after file is read
//name can be NULL returns index of type or negative
int add_type_entry(ast_t ast,
                   char *name,
                   value_type_array_t params,
                   value_type_array_t results)
{
    return -1;
}

int get_named_type_entry_index(ast_t ast, char *name)
{
    return -1;
}

//I should probably have a function for the parser that dumps an object file
//and one that reads one or more object files and links them to an object file
//and one that takes an object file and outputs a wasm module
bool write_ast_to_file(ast_t ast)
{
    return false;
}

bool write_ast_to_module(ast_t ast)
{
    return false;
}
