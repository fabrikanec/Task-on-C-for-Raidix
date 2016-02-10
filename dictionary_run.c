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


int main(int argc, char ** argv)
{
    char word[MAX_WORD_SIZE+1];
    char answer[MAX_DESC_SIZE+1];
    char search_word[MAX_WORD_SIZE];
    char desc[MAX_DESC_SIZE];
    char buff [MAX_DESC_SIZE + MAX_WORD_SIZE + 1];

    FILE *new_dictionary;
    dictionary_initialise();

    int i;
    if (argc > 1) 
    {
    	for (i = 1; i < argc; i++)
    	{
       	    dictionary_read_from_file(argv[i]);
    	}
    } else {
	//new_dictionary = fopen ("./new_dictionary.txt","ab+");
	if ((new_dictionary = fopen ("./new_dictionary.txt","r")) == NULL)
	{
	    new_dictionary = fopen ("./new_dictionary.txt","ab+");

	} else {
	    fclose (new_dictionary);
	    dictionary_read_from_file("new_dictionary.txt");
	}
	if (new_dictionary == NULL)
    	{
       	   printf("could not create the file\n");
       	   exit (1);
    	}

    }
    usage();
    scanf("%s",word);

    while(word[0] != '@')
    {	
	if (word[0] == '$')
	{
	    scanf("%s", &search_word);
	    fgets (desc, MAX_DESC_SIZE, stdin);
	    strcpy(buff,"");
	    strncat (buff, search_word, MAX_WORD_SIZE);
	    strncat (buff, desc, MAX_DESC_SIZE);
	    buff[MAX_DESC_SIZE + MAX_WORD_SIZE + 1] = '\0';	    
	    fprintf (new_dictionary, buff);
	    trie_insert(&tree.root, search_word, desc);

	} else {
            if (!dictionary_lookup(word,answer))
            {
                printf("%s:\t%s\n", word, answer);
            } else {
                printf("%s:\t%s\n", word, "Not in dictionary");
            }
	}
        scanf("%s",word);
    }

    return 0;

}