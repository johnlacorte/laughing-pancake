#ifndef PARSER_H
#define PARSER_H

typedef void* parser_t;

typedef enum
{
    OUTPUT_OBJECT_FILE,
    OUTPUT_MODULE,
    OUTPUT_TEXT
} output_type_t;

//I guess I need to pass command line arguments somehow
//I don't know if I need to actually hold a pointer to this
//unless the caller is going to be the one to print error messages
//The ouput of any text file input should just be an object file
//I think the only thing to return needs to be if it was successful
//the reading of object files and outputting the module is the only part
//that needs to take multiple input files
//I think maybe have this return a value back to the OS
//This could either output an object file, a module, or text
int open_file_for_parsing(char *input_file, char *output_file, output_type_t output_type);

#endif

/*** end of file "parser.h" ***/
