#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct constant_node
{
    char constant_string[64];
    struct constant_node *next;
} typedef constant_node_t;

typedef struct
{
    constant_node_t *head;
    constant_node_t *tail;
} constants_list_t;

struct trie_node
{
    char token_constant_name[64];
    trie_node *node_array[256];
} typedef trie_node_t;

int read_all_tokens(trie_node_t *trie_head,
                    constants_list_t *constants_list,
                    FILE *input_file,
                    FILE *constant_file,
                    FILE *matcher_file
                   );

//I imagine this should take three command line arguments; file to read from,
//constants file to write to, and keyword matcher c source file.
int main()
{
    trie_node_t *trie_head = calloc(1, sizeof(trie_node_t));
    constants_list_t *constants_list = calloc(1, sizeof(constants_list_t));
    FILE *input_file = fopen("input_file", "r");
    FILE *constants_file = fopen("keyword_tokens.h", "w");
    FILE *matcher_file = fopen("keyword_matcher.c", "w");
    if((trie_head != NULL) &&
       (constants_list != NULL) &&
       (input_file != NULL) &&
       (constants_file != NULL) && 
       (matcher_file != NULL)
      )
    {
        return read_all_tokens(trie_head,
                               constants_list,
                               input_file,
                               constants_file,
                               matcher_file);
    }

    else
    {
        fprintf(stderr, "Failed to open files or allocate memory.\n"); 
        return 1;
    } 
}

bool read_token(FILE *input_file, trie_node_t *trie_head, constants_list_t *constants_list);

void dump_trie_to_file(trie_node_t *trie_head, FILE *matcher_file);

void dump_constants_to_file(constants_list_t *constants_list, FILE *constants_file);

int read_all_tokens(trie_node_t *trie_head,
                    constants_list_t *constants_list,
                    FILE *input_file,
                    FILE *constants_file,
                    FILE *matcher_file
                   )
{
    int ch = fgetc(input_file);
    while(ch != EOF)
    {
        //push back ch
        ungetc(ch, input_file);
        if(!read_token(input_file, trie_head, constants_list))
        {
            return 1;
        }

        ch = fgetc(input_file);
    }

    return 0;
}

bool add_constant_line_to_list(constants_list_t *constants_list, char *constant_line, int length);

bool read_token_line_into_trie(trie_node_t *trie_head, char *constant_name, int length);

bool read_token(FILE *input_file, trie_node_t *trie_head, constants_list_t *constants_list)
{
    //now that I think of it there shouldn't be an eof on the first line at all
    //I think it would be nice if each token was instead three lines with the
    //first line being the Doxygen comment. Maybe storing it in the constants
    //list
    char buffer[64];
    int constant_name_index = 0;
    int ch = fgetc(input_file);
    while(ch != ' ' && constant_name_index < 64)
    {
        //To be thorough this should check for other kinds of whitespace and
        //other characters not allowed like control characters
        if(ch == '\n' || ch == EOF)
        {
            fprintf(stderr, "Constant line must be <constant><space><replacement><newline>\n");
            return false;
        }

        else
        {
            buffer[constant_name_index] = ch;
            constant_name_index++;
            ch = fgetc(input_file);
        }
    }

    if(constant_name_index < 64)
    {
        int constant_line_index = constant_name_index;
        ch = fgetc(input_file);
        while(ch != '\n' &&  constant_line_index < 64)
        {
            if(ch == ' ')
            {
                fprintf(stderr, "Second space in constant line.\n");
                return false;
            }

            else
            {
                if(ch == EOF)
                {
                    fprintf(stderr, "Unexpected EOF in constant line.\n");
                    return false;
                }
                
                else
                {
                    buffer[constant_line_index] = ch;
                    constant_line_index++;
                    ch = fgetc(input_file);
                }
            }
        }

        if(constant_line_index < 64)
        {
            if(add_constant_line_to_list(constants_list, buffer, constant_line_index))
            {
                if(read_token_line_into_trie(trie_head, buffer, constant_name_index))
                {
                    return true;
                }
            }

            return false;
        }
    }

    fprintf(stderr, "Constant line too long.\n");
    return false;
}

void write_trie_node(trie_node_t *node, FILE *matcher_file, int depth);

void dump_trie_to_file(trie_node_t *trie_head, FILE *matcher_file)
{
    //This should at least have comments at the begining of the file describing
    //what it is
    //write beginning of matcher
    fprintf(matcher_file, "\
#include \"token_types.h\"\n\
#include \"keyword_tokens.h\"\n\
\n\
int keyword_matcher(char *token_buffer)\
{\n\
  switch(token_buffer[0])\n");
    //write trie to file
    write_trie_node(trie_head, matcher_file, 1);
    //write end of file
    fprintf(matcher_file, "  }\n}\n\n");
}

void dump_constants_to_file(constants_list_t *constants_list, FILE *constants_file)
{
    //This should at least have comments at the begining of the file describing
    //what it is
    //write beginning of header file
    fprintf(constants_file, "\
#ifndef KEYWORD_TOKENS_H\n\
#define KEYWORD_TOKENS_H\n\
\n");
    //write list
    constant_node_t *current = constants_list->head;
    while(current != NULL)
    {
        fprintf(constants_file, "%s\n", current->constant_string);
        current = current->next;
    }
    //write end of header file
    fprintf(constants_file, "\n#endif\n\n");
}

bool add_constant_line_to_list(constants_list_t *constants_list, char *constant_line, int length)
{
    //allocate new node and copy line with a null byte at length index
    return false;
}

bool read_token_line_into_trie(trie_node_t *trie_head, char *constant_name, int length)
{
    return false;
}

void indent_line(FILE *matcher_file, int depth);

void write_trie_node(trie_node_t *node, FILE *matcher_file, int depth)
{
    //check 0 index is not NULL(this returns the constant name)
    //loop through the array looking for non-NULL pointers
    //if found write case and opening of next switch block then recursively
    //call this function on the pointer.
    //when it returns, add closing curly brace and empty line before moving
    //to the next non-NULL pointer. When the end of the array is reached
    //add default case and return.
}

void indent_line(FILE *matcher_file, int depth)
{
    //(depth + 1) * 2 spaces
}
