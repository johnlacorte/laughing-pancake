#ifndef AST_H
#define AST_H

typedef void* ast_t;

//move to wasm_constants header file


typedef void* value_type_array_t;

ast_t new_ast();

void free_ast(ast_t ast);

value_type_array_t new_value_type_array();

bool add_value_type_to_value_type_array(value_type_array_t value_type_array,
                                        value_type_t value_type);

//name can be NULL returns index of type or negative
int add_type_entry(ast_t ast,
                   char *name,
                   value_type_array_t params,
                   value_type_array_t results);

//I should probably have a function for the parser that dumps an object file
//and one that reads one or more object files and links them to an object file
//and one that takes an object file and outputs a wasm module
bool dump_ast_to_module(ast_t ast, char *filename);

#endif

/*** end of file "ast.h" ***/