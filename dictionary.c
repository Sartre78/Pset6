/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include "dictionary.h"


// initialize hash table array
node* table[26];

int hash (char* string);
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // TODO
    // allocate memory for a string that will contain the adulterated word
    changed = malloc((strlen(word) + 1) * sizeof(char));
    if (changed == NULL) {free(changed); return false;} // check for NULL
    
    // convert word to lowercase to compare it to the all lower-care dictionary
    for (int i = 0, n = strlen(word); i < n; i++) {changed[i] = tolower(word[i]);}

    node* current = table[hash(changed)]; // set temporary node to head of list (hash function embedded)
    
    while (current->word != NULL && word != NULL) // NULL check and loop until end of list
    { 
        // Scenario 1:  Word matches current word in list   
        if (strcmp(changed, current->word) == 0) {return true; break;}
        /* Scenario 2: Word does not match current word.
           Apocryphal Buddhist and/or Confucian proverb:
           "To advace further, look within" */
        else {current = current->next;}
    }
    // Inform user that word was not in the dictionary            
    if (current->word == NULL) {return false;}
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // Open dictionary (for the purpose of counting total words)   
    FILE* fp = fopen(dictionary, "r");
    if (fp == NULL) {printf("Could not open dictionary.\n"); return false;}

   // initialize variables
    int index = 0;
    words = 0;
    char word[LENGTH+1];

    // begin reading each word
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        // allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // append character to word
            word[index] = c; index++;
            
            // ignore alphabetical strings too long to be words
            // consume remainder of alphabetical string; prepare for new word
            if (index > LENGTH) {while ((c = fgetc(fp)) != EOF && isalpha(c)); index = 0;}
        }
        // ignore words with numbers (like MS Word can)
        // consume remainder of alphanumeric string; prepare for new word
        else if (isdigit(c)) {while ((c = fgetc(fp)) != EOF && isalnum(c)); index = 0;}

        // we must have found a whole word.  terminate word and update counter
        else if (index > 0) {word[index] = '\0'; words++; index = 0;}
    }
    fclose(fp); // close file
    
    // Open dictionary (for the purposes of loading)   
    FILE* file = fopen(dictionary, "r");
    if (file == NULL) {printf("Could not open dictionary.\n"); return false;}
    
    for (int i = 0; i < words; i++)
    {          
        // allocate new node and check for NULL
        node* new_node = malloc(sizeof(node)); {if (new_node == NULL) return false;}
    
        // scan word from file into the "word" portion of the new node struct
        fscanf(file, "%s", new_node->word);

        // initial insertion scenario (i.e., the spot in the table array is empty)
        // The hash function has been embedded
        if (table[hash(new_node->word)] == NULL)  {table[hash(new_node->word)] = new_node;}
    
        /* otherwise, put the new string in the first position and link the
           previous node to the new head of the list */
        else 
        {
            // create a node equal to the current value of the table
            node* curr = table[hash(new_node->word)];  
            /* point "new_node" (the struct holding the value of the desired
               word to be inserted) to the newly created node that equals what
               to this point was the start of the list */ 
            new_node->next = curr;
            /* set the head pointer of the first spot in the array (i.e., the 
               "head") to be the node containing the desired string to be 
               inserted.  */ 
            table[hash(new_node->word)] = new_node;
        }        
    }
    fclose(file);
    return true;
}    
/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // Return the gloabl variable that was iterated while the dictionary was being loaded
    return words;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // free every "i"th spot of the table
    for (int i = 0; i <= 25; i++) 
    {
        node* cursor = table[i]; // temp node
        
        // free every node in the list
        while (cursor != NULL) 
        {
            node* temp = cursor; 
            cursor = cursor->next; 
            free(temp);
        }        
    }
    free(changed);
    return true;
}
// HASH FUNCTION: hash on first letter of string
int hash (char* string) {int h = toupper(string[0]) - 'A'; return h % 26;}
