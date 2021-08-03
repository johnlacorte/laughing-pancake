#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct constant_node
{
    char *constant_string;
    struct constant_node *next;
} typedef constant_node_t;

typedef struct
{
    constant_node_t *head;
    constant_node_t *tail;
} constants_list_t;

struct trie_node
{
    char *token_constant_name;
    trie_node *node_array[256];
} typedef trie_node_t;

int read_all_tokens(trie_node_t *head,
                    FILE *input_file,
                    FILE *constant_file,
                    FILE *matcher_file
                   );

//I imagine this should take three command line arguments; file to read from,
//constants file to write to, and keyword matcher c source file.
int main()
{
    trie_node_t *trie_head = calloc(1, sizeof(trie_node_t));
    constants_list_t *constants_list = calloc(1, sizeof(constant_list_t));
    FILE *input_file = fopen("input_file", "r");
    FILE *constants_file = fopen("keyword_tokens.h", "w");
    FILE *matcher_file = fopen("keyword_matcher.c", "w");
    if((trie_head != NULL) &&
       (constants_list != NULL) &&
       (input_file != NULL) &&
       (constant_file != NULL) && 
       (matcher_file != NULL)
      )
    {
        return read_all_tokens(trie_head,
                               constants_list,
                               input_file,
                               constant_file,
                               matcher_file);
    }

    else
    {
        fprintf(stderr, "Failed to open files or allocate memory.\n"); 
        return 1;
    } 
}

bool read_token(FILE *input_file, trie_node_t *trie_head, constant_list_t *constants_list);

void dump_trie_to_file(trie_node_t *trie_head, FILE *matcher_file);

void dump_constants_to_file(constants_list_t *constants_list, FILE *constants_file);

int read_all_tokens(trie_node_t *trie_head,
                    constants_list *constants_list
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

bool read_token(FILE *input_file, trie_node_t *trie_head, constant_list_t *constants_list)
{
    char buffer[64];
    int i = 0;
    int ch = fgetc(input_file);
    while(ch != ' ' && i < 64)
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
            buffer[i] = ch;
            i++;
            ch = fgetc(input_file);
        }
        
    }

    if(i < 64)
    {
        int j = i;
        //To be thorough this should check if the next character is whitespace
        ch = fgetc(input_file);
        while(ch != \n && ch != EOF)
        {
            if()
        }
    }

    else
    {
        fprintf(stderr, "Constant line too long.\n");
        return false;
    }
    
}

void dump_trie_to_file(trie_node_t *trie_head, FILE *matcher_file)
{

}

void dump_constants_to_file(constant_node_t *constants_head, FILE *constants_file)
{

}