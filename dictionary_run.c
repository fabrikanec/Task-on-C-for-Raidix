#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"



int main(int argc, char ** argv)
{
    char word[MAX_WORD_SIZE+1];
    char answer[MAX_DESC_SIZE+1];
    char search_word[MAX_WORD_SIZE];
    char desc[MAX_DESC_SIZE];
    char buff [MAX_DESC_SIZE + MAX_WORD_SIZE + 1];
    short flag = 0; // flag for closing first new_dictionary

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
	    flag = 1;
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
	    scanf("%s", search_word);
	    fgets (desc, MAX_DESC_SIZE, stdin);
	    strcpy(buff,"");
	    strncat (buff, search_word, MAX_WORD_SIZE);
	    strncat (buff, desc, MAX_DESC_SIZE);
	    buff[MAX_DESC_SIZE + MAX_WORD_SIZE + 1] = '\0';	    
	    fprintf (new_dictionary, buff);
	    if (trie_insert(&tree.root, search_word, desc))
            {
                printf("failed to insert: memory is full");
            }
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
    if (flag)
    fclose (new_dictionary);
    return 0;

}
