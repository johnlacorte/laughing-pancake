#ifndef CONSTANTS_H
#define CONSTANTS_H

#define VERSION_NUMBER 1

//variable types
#define VAR_I32 0x7f
#define VAR_I64 0x7e
#define VAR_F32 0x7d
#define VAR_F64 0x7c
#define VAR_ANYFUNC 0x70
#define VAR_FUNC 0x60
#define VAR_EMPTY_BLOCK 0x40

//variable info for type system
#define VAR_SIZE_8BIT 0
#define VAR_SIZE_16BIT 1
#define VAR_SIZE_32BIT 2
#define VAR_SIZE_64BIT 3
#define VAR_UNSIGNED 0
#define VAR_SIGNED 1
#define MS_DATA 0
#define MS_GLOBAL 1
#define MS_HEAP 2
#define MS_STACK 3

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

