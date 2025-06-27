// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 131071;

// Hash table
node *table[N];

// ADDED: Count of words loaded
unsigned int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO--------------------------------- START
    // compute bucket index
    unsigned int idx = hash(word);

    // traverse linked list at table[idx]
    node *cursor = table[idx];
    while (cursor)
    {
        // case-insensitive compare
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    // TODO--------------------------------- END
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function --- START
    // return toupper(word[0]) - 'A';

    // polynomial rolling hash on lowercase letters
    unsigned int h = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        h = (h * 31 + tolower((unsigned char) word[i])) % N;
    }
    return h;
    // TODO --------------------------------- END
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO --------------------------------- START
    // open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }
    // read each word
    char buffer[LENGTH + 1];
    while (fscanf(file, "%s", buffer) != EOF)
    {
        // create new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(file);
            return false;
        }
        // copy word into node
        strcpy(n->word, buffer);
        // get hash index
        unsigned int idx = hash(buffer);
        // insert at head of list
        n->next = table[idx];
        table[idx] = n;
        // increment word count
        word_count++;
    }

    fclose(file);
    return true;
    // TODO  --------------------------------- END
    // return false;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO --------------------------------- START
    // return 0;
    return word_count;
    // TODO --------------------------------- END
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO --------------------------------- START
    // free all nodes in each bucket
    for (unsigned int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
        table[i] = NULL;
    }
    // TODO --------------------------------- END
    // return false; Haha! This is CS50!
    return true;
}
