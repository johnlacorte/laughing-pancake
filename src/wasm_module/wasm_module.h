//This is rewriting to match the style of the rest of the compiler
#ifndef WASM_MODULE_H
#define WASM_MODULE_H

typedef void* wasm_module_t;

typedef unsigned char value_type_t;

typedef unsigned char element_type_t;

typedef unsigned char mutability_t;

typedef unsigned char export_kind_t;

//Maybe the other unsigned char should be named too, maybe they should be enums,
//maybe the char pointers should be named to indicate if they are a utf8 string,
//bytes, or bytecode and the bytecode buffer should maybe be moved to its own
//directory. 
//Maybe these typedefs should be in the same header file as the constants.

wasm_module_t new_wasm_module(char *filename);

//I may change the names of these
//It looks like the return values of these is the index
uint32_t add_type_to_module(wasm_module_t module,
                            value_type_t return_value_type,
                            uint8_t      number_of_params,
                            value_type_t *param_types);

void add_function_import_to_module(wasm_module_t module,
                                   uint32_t      module_name_length,
                                   char          *module_name,
                                   uint32_t      field_name_length,
                                   char          *field_name,
                                   uint32_t      type_index);

void add_table_import_to_module(wasm_module_t  module,
                                uint32_t       module_name_length,
                                char           *module_name,
                                uint32_t       field_name_length,
                                char           *field_name,
                                element_type_t element_type,
                                uint32_t       flags,
                                uint32_t       initial_size,
                                uint32_t       maximum_size);

void add_memory_import_to_module(wasm_module_t module,
                                 uint32_t      module_name_length,
                                 char          *module_name,
                                 uint32_t      field_name_length,
                                 char          *field_name,
                                 uint32_t      flags,
                                 uint32_t      initial_size,
                                 uint32_t      maximum_size);

void add_global_import_to_module(wasm_module_t module,
                                 uint32_t      module_name_length,
                                 char          *module_name,
                                 uint32_t      field_name_length,
                                 char          *field_name,
                                 value_type_t  variable_type,
                                 mutability_t mutability);

uint32_t add_function_to_module(wasm_module_t module, uint32_t typeIndex);

uint32_t add_table_to_module(wasm_module_t  module,
                             element_type_t element_type,
                             uint32_t       flags,
                             uint32_t       initial_size,
                             uint32_t       maximum_size);

void add_memory_to_module(wasm_module_t module,
                          uint32_t      flags,
                          uint32_t      initial_size,
                          uint32_t      maximum_size);

uint32_t add_global_to_module(wasm_module_t module,
                              value_type_t  variableType,
                              mutability_t mutability,
                              struct ByteBuffer *init_expr);

void add_export_to_module(wasm_module_t module,
                          uint32_t      field_name_length,
                          char          *field_name,
                          export_kind_t kind,
                          uint32_t      index);

void add_start_to_module(wasm_module_t module, uint32_t functionIndex);

void add_element_to_module(wasm_module_t module,
                           uint32_t      table_index,
                           struct ByteBuffer *init_expr,
                           uint32_t      number_of_elements,
                           uint32_t      *elements);

void add_code_to_module(wasm_module_t module,
                        uint32_t      variable_count,
                        value_type_t  *variable_types,
                        struct ByteBuffer *bytecode);

void add_data_to_module(wasm_module_t module,
                        uint32_t      memory_index,
                        uint32_t      init_expr_length,
                        unsigned char *init_expr,
                        uint32_t      data_length,
                        unsigned char *data_bytes);

//void addNameToModule(struct WasmModule*);

int dumpWasmModule(wasm_module_t module, uint32_t version);

#endif

