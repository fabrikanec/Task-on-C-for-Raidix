#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

#define ALPHABET_SIZE 26

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

/**
* Converts a character to an integer value based upon its position in
* the alphabet, or returns -1 if the character is not in the alphabet.
*/
int letter_to_int(char letter)
{
    if (letter >= 'A' && letter <= 'Z')
    {
        return letter - 'A' + ALPHABET_SIZE;
    }
    else if (letter >= 'a' && letter <= 'z')
    {
        return letter - 'a';
    }
    return -1;
}

/**
* Prints and identifies an invalid character in a word
* based upon its index within the word.           ^
*/
void print_invalid_word(const char *word, int index)
{
    printf("  word: \"%s\"\n", word);
    printf("         ");
    int i;
    for (i = 0; i < index; i++)
    {
        printf(" ");
    }
    printf("^\n");
}

/**
* Inserts a word and its meaning into the trie.
*/
int trie_insert(struct trie_node *node, const char *word, char *description)
{
    int i;
    for (i = 0; i < strlen(word); i++)
    {
        int letter = letter_to_int(word[i]);
        if (letter == -1)
        {
            // invalid character in the string, cannot be inserted into the trie
            printf("failed to insert due to invalid character in word\n");
            print_invalid_word(word, i);
            printf("  description: \"%s\"\n", description);
            return 1;
        }

        struct trie_node *parent = node;
        node = node->children[letter];

        if (!node)
        {
            node = malloc(sizeof(struct trie_node));
	    if (node == NULL)
	    {
 	        printf("failed to insert: memory is full");
		return 1;
	    }
            parent->children[letter] = node;
        }
    }

    /*
    * this will prove very efficient we are only allocating enough memory
    * for every character in the description
    */
    int len = strlen(description);
    if ((node->value = malloc(len + 1)) == NULL)
    {
	printf("failed to insert: memory is full");
	return 1;
    }
    strncpy(node->value, description, len);
    return 0;
}

/**
* Traverses through a trie from the root node and searches
* for a word's description based on the individual letters
* that make up the word.
*/
char *trie_get(struct trie_node *node, const char *word)
{
    int i;
    for (i = 0; i < strlen(word); i++)
    {
        int letter = letter_to_int(word[i]);
        if (letter == -1)
        {
            return 0;
        }

        node = node->children[letter];
        if (!node)
        {
            return 0; // not found
        }
    }
    return node->value;
}

void dictionary_initialise()
{
    tree = (struct trie) {}; // zeroes the tree
}

int dictionary_read_from_file(const char * filename)
{
    // attempts to open the file in read only mode
    FILE *file = fopen(filename, "r");
    
    if (!file)
    {
        printf("could not find/open file \"%s\"\n", filename);
        exit (1);
    }

    char word[MAX_WORD_SIZE];
    //char desc[MAX_DESC_SIZE]; 
    char* desc = NULL;

    int count = 0;

    // ensure that at least two items are being parsed (word & desc)
    while (fscanf(file, "%s %[^\n]", word, desc) > 1)
    {
        if (trie_insert(&tree.root, word, desc))
        {
            fclose(file);
            return 1;
        }
        else
        {
            count++;
        }
    }

    fclose(file);
    printf("parsed file \"%s\" with %i entries\n", filename, count);
    return 0;
}

int dictionary_lookup(const char *word, char *meaning)
{
    // check for invalid letters in input
    int i;
    for (i = 0; i < strlen(word); i++)
    {
        int letter = letter_to_int(word[i]);
        if (letter == -1)
        {
            printf("invalid character in word\n");
            print_invalid_word(word, i);
            return 1;
        }
    }

    // grab the description from the tree
    char *description = trie_get(&tree.root, word);

    if (!description)
    {
        return 1;
    }

    // copy the description into the user supplied buffer
    strcpy(meaning, description);

    return 0;
}

void usage()
{
	printf("to know meaning of word  -  type only word\nto add new word  -  type $ and then type word and after space its meaning\nto exit  -  type @\n");
}
