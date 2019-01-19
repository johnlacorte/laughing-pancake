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

//external types
#define EXTERNAL_FUNC 0x00
#define EXTERNAL_TABLE 0x01
#define EXTERNAL_MEMORY 0x02
#define EXTERNAL_GLOBAL 0x03

//for globals
#define IMM 0
#define MUT 1

#endif

