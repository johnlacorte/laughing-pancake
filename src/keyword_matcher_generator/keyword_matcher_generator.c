#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct trie_node
{
    char *token_constant_name;
    trie_node *node_array[256];
};

int read_next_token(struct trie_node *node);

void dump_to_file(struct trie_node *head, char *matcher_file);

//I imagine this should take three command line arguments; file to read from,
//constants file to write to, and keyword matcher c source file.
int main()
{
    trie_node *head = calloc(1, sizeof(struct trie_node));
    while(read_next_token(head));    
    return 0;
}

int read_next_token_char(struct trie_node *node);

int read_token_constant_name(struct trie_node *node);

int read_next_token(struct trie_node *node)
{
    return 0;
}

void dump_to_file(struct trie_node *head, char *matcher_file)
{

}

int read_next_token_char(struct trie_node *node)
{
    return 0;
}

int read_token_constant_name(struct trie_node *node)
{
    return 0;
}