#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define MAX_WORD_LENGTH 1023

// ------------------------------------------------------------------------------
// Program automatically makes a trie data structure to search through a text file.
// ------------------------------------------------------------------------------

typedef struct TrieNode
{
    int count;
    struct TrieNode *children[26];
} TrieNode;

// Helper function for printTrie().
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
    int i;

    if (root == NULL)
        return;

    if (root->count > 0)
        printf("%s (%d)\n", buffer, root->count);

    buffer[k + 1] = '\0';

    for (i = 0; i < 26; i++)
    {
        buffer[k] = i + 'a';
        printTrieHelper(root->children[i], buffer, k + 1);
    }

    buffer[k] = '\0';
}

// Print all the strings represented in the trie rooted at this node.
void printTrie(TrieNode *root)
{
    char buffer[MAX_WORD_LENGTH + 1];
    buffer[0] = '\0';

    printTrieHelper(root, buffer, 0);
}

// Malloc creation function.
TrieNode *createNode(void)
{
    return calloc(1, sizeof(TrieNode));
}

// Insert a string into a trie. This function returns the root of the trie.
TrieNode *insert(TrieNode *root, char *str)
{
    int i, index, len = strlen(str);
    TrieNode *temp;

    if (root == NULL)
    {
        root = createNode();
    }

    temp = root;

    for (i = 0; i < len; i++)
    {

        index = tolower(str[i]) - 'a';

        // If next node doesn't exist, create it.
        if (temp->children[index] == NULL)
        {
            temp->children[index] = createNode();
        }

        // Jump forward to insert the next letter.
        temp = temp->children[index];
    }

    // Increment the count of the word in the final letter of the word we inserted.
    temp->count++;

    return root;
}

TrieNode *buildTrie(void)
{
    TrieNode *root = NULL;
    char buffer[MAX_WORD_LENGTH + 1], filename[512];
    FILE *ifp = NULL;

    while (1)
    {
        printf("Enter the name of the file to pull definitions from (include extension like .txt): ")
        scanf("%s", filename);

        if ((ifp = fopen(filename, "r")) == NULL)
        {
            printf("The file %s was not found.\n");
            printf("Place a text file with the name %s in the same");
            printf(" folder as the executable and try again.\n");
            continue;
        }

        break;
    }

    // Insert strings one-by-one into the trie.
    while (fscanf(ifp, "%s", buffer) != EOF)
    {
        root = insert(root, buffer);
    }

    fclose(ifp);
    return root;
}

// Delete a trie recursively
void deleteTrie(TrieNode *tn)
{

    return NULL;
}

// ----------------------------------------------
// Main menu driven interface portion of program.
// ----------------------------------------------

void printMenu(void)
{
    printf("Menu:\n");
    printf("find --  Find if a word is in a selected text document\n");
    printf("define --  Find a word's definition\n");
    printf("? --  Help\n");
    printf("exit --  Quit program\n");
}

// Receive user input and process it as a string.
int getWord(char *prompt)
{
    char buffer[1024];
    char *word;

    printf("%s");
    scanf("%s", word);

    // Malloc for functional decomposition (+1 for null sentinal).
    word = malloc(sizeof(char) * (strlen(buffer) + 1));

    // Copy from buffer to word.
    strcpy(word, buffer);

    // Make the final character a null sentinal.
    word[strlen(buffer)] = '\0';

    return word;
}

// Free entity from memory safely.
void deleteMalloc(char *entity)
{
    if (entity != NULL)
    {
        free(entity);
    }
}

int main(void)
{
    char *word, buffer[512];

    printMenu();

    while (1)
    {
        printf("Enter a command ('?' for help // 'exit' to quit).\n");
        printf("%% ");
        scanf("%s", buffer);

        if (strcmp(buffer, "find") == 0)
        {
            word = getWord("Enter the word you want to find: ");

            // Retrieve if word is in .txt file.
            result = findWord(word);

            // Print out results.
            printf("Found %d result%c for (%s).\n", result, ((result == 1) ? '' : 's') , word);
        }

        else if (strcmp(buffer, "define") == 0)
        {
            word = getWord("Enter a word to find its definition: ");
            getDefinition(word);
        }

        else if (strcmp(buffer, "?") == 0)
        {
            printMenu();
        }

        else if (strcmp(buffer, "exit") == 0)
        {
            break;
        }

        else
        {
            printf("Command not found. Enter \"?\" for help.\n");
        }

        printf("\n");
    }

    printf("Exiting program...\n");

    deleteMalloc(word);

    return 0;
}
