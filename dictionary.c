// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 143093;
int SIZE = 0;

// Hash table
node *table[N];
node *tmpNode;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char *lword = (char *) malloc(sizeof(char) * (strlen(word) + 1));
    if (lword == NULL)
    {
        return false;
    }
    strcpy(lword, word);
    lword = strLower(lword);

    unsigned int hashCode = hash(lword);
    tmpNode = table[hashCode];
    while (tmpNode != NULL)
    {
        //printf("%s\n", tmpNode->word);
        if (strcmp(tmpNode->word, lword) == 0)
        {
            free(lword);
            return true;
        }
        tmpNode = tmpNode->next;
    }

    free(lword);
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // K&R Version 2
    unsigned hashval;

    for (hashval = 0; *word != '\0'; word++)
    {
        hashval = *word + 31 * hashval;
    }
    return hashval % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *dictFile = fopen(dictionary, "r");
    if (dictFile == NULL)
    {
        return false;
    }

    char *end = NULL;
    do
    {
        tmpNode = (node *) malloc(sizeof(node));
        if (tmpNode == NULL)
        {
            free(tmpNode);
            fclose(dictFile);
            return false;
        }

        end = fgets(tmpNode->word, LENGTH, dictFile);
        if (end == NULL)
        {
            free(tmpNode);
            break;
        }
        SIZE++;

        for (int i = 0; tmpNode->word[i] != '\0'; i++)
        {
            if (isspace(tmpNode->word[i]) != 0)
            {
                tmpNode->word[i] = '\0';
            }
        }
        
        unsigned int hashCode = hash(tmpNode->word);
        if (table[hashCode] == NULL)
        {
            table[hashCode] = tmpNode;
            continue;
        }
        tmpNode->next = table[hashCode];
        table[hashCode] = tmpNode;
    }
    while (end != NULL);

    fclose(dictFile);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return SIZE;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *cursor = NULL;

    for (int index = 0; index <= N; index++)
    {
        if (table[index] == NULL)
        {
            continue;
        }
        cursor = table[index];
        tmpNode = cursor;
        while (cursor->next != NULL)
        {
            tmpNode = cursor;
            cursor = cursor->next;
            free(tmpNode);
        }
        free(cursor);
    }
    return true;
}

char *strLower(char *word)
{
    for (int index = 0; word[index] != '\0'; index++)
    {
        if (word[index] > 64 && word[index] < 91)
        {
            word[index] += 32;
        }
    }

    return word;
}
