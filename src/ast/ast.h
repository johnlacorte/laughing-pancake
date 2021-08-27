#ifndef AST_H
#define AST_H

typedef void* ast_t;

//move to wasm_constants header file
typedef unsigned char value_type_t;

typedef void* value_type_array_t;

ast_t new_ast();

void free_ast(ast_t ast);

value_type_array_t new_value_type_array();

bool add_value_type_to_value_type_array(value_type_array_t value_type_array,
                                        value_type_t value_type);
                                        
bool add_type_entry();

//I might be better off to have functions for each kind of dump
bool dump_ast_to_module(ast_t ast, char *filename);

#endif

/*** end of file "ast.h" ***/