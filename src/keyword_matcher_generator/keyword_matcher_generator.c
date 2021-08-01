#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    trie_node_t *head = calloc(1, sizeof(trie_node_t));
    FILE *input_file = fopen("input_file", "r");
    FILE *constant_file = fopen("keyword_tokens.h", "w");
    FILE *matcher_file = fopen("keyword_matcher.c", "w");
    if((head != NULL) &&
       (input_file != NULL) &&
       (constant_file != NULL) && 
       (matcher_file != NULL)
      )
    {
        return read_all_tokens(head, input_file, constant_file, matcher_file);
    }

    else
    {
        return 1;
    } 
}

int read_all_tokens(trie_node_t *head,
                    FILE *input_file,
                    FILE *constant_file,
                    FILE *matcher_file
                   )
{
    int ch = fgetc(input_file);
    while(ch != EOF)
    {
        //push back ch
        if(!read_token())
        {

        }

        ch = fgetc(input_file);
    }

    return 0;
}

void dump_to_file(trie_node_t *head, FILE *matcher_file)
{

}
