#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct trie_node
{
    char *token_constant_name;
    trie_node *node_array[256];
} typedef trie_node_t;

int read_next_token(trie_node_t *head, FILE *input_file, FILE *constant_file);

void dump_to_file(trie_node_t *head, char *matcher_file);

//I imagine this should take three command line arguments; file to read from,
//constants file to write to, and keyword matcher c source file.
int main()
{
    trie_node_t *head = calloc(1, sizeof(trie_node_t));
    FILE *input_file = fopen("input_file", "r");
    FILE *constant_file = fopen("keyword_tokens.h", "w");
    if(head != NULL && input_file != NULL && constant_file != NULL)
    {
        while(read_next_token(head, input_file, constant_file));
        return 0;
    }

    else
    {
        return 1;
    } 
}

int read_next_token_char(trie_node_t *node);

int read_token_constant_name(trie_node_t *node);

int read_next_token(trie_node_t *head, FILE *input_file, FILE *constant_file)
{
    return 0;
}

void dump_to_file(trie_node_t *head, char *matcher_file)
{

}

int read_next_token_char(trie_node_t *node)
{
    return 0;
}

int read_token_constant_name(trie_node_t *node)
{
    return 0;
}