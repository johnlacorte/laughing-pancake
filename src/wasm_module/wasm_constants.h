#ifndef WASM_CONSTANTS_H
#define WASM_CONSTANTS_H

#define VALUE_I32 0x7f
#define VALUE_I64 0x7e
#define VALUE_F32 0x7d
#define VALUE_F64 0x7c

typedef unsigned char value_type_t;

//external types
#define EXTERNAL_FUNC 0x00
#define EXTERNAL_TABLE 0x01
#define EXTERNAL_MEMORY 0x02
#define EXTERNAL_GLOBAL 0x03

//for globals
#define IMM 0
#define MUT 1

//for memory
#define CANNOT_GROW 0x00
#define CAN_GROW 0x01

#endif

