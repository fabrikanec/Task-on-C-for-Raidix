#ifndef DICTIONARY_H
#define DICTIONARY_H

#define MAX_WORD_SIZE  128
#define MAX_DESC_SIZE  200

#define ALPHABET_SIZE 26

/**
* A dictionary is a collection of mappings from WORDs to DESCRIPTIONs
* A WORD is a sequence of characters up to MAX_WORD_SIZE in length
*/

struct trie tree;

struct trie_node
{
    char *value;

    // *2 as we must account for a-z as well as A-Z
    struct trie_node *children[ALPHABET_SIZE * 2];
};

struct trie
{
    struct trie_node root;
};


void dictionary_initialise();

int dictionary_read_from_file(const char * filename);

int dictionary_lookup(const char * word, char * meaning);

void usage();

int trie_insert(struct trie_node *node, const char *word, char *description);

#endif
