#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define _CRT_SECURE_NO_WARNINGS

// Trie node structure
struct TrieNode
{
    struct TrieNode* children[26]; // Assuming only lowercase alphabets
    int word_count; // Number of times the word is inserted in the trie
};

// Initializes a trie node
struct TrieNode* createTrieNode()
{
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    for (int i = 0; i < 26; i++)
        newNode->children[i] = NULL;
    newNode->word_count = 0;
    return newNode;
}

// Inserts the word to the trie structure
void insert(struct TrieNode* root, char* word)
{
    struct TrieNode* currentNode = root; // This line is causing the warning
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (currentNode->children[index] == NULL)
            currentNode->children[index] = createTrieNode(); // Possible NULL pointer dereference
        currentNode = currentNode->children[index];
    }
    currentNode->word_count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct TrieNode* root, char* word)
{
    struct TrieNode* currentNode = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (currentNode->children[index] == NULL)
            return 0; // Word not found in the trie
        currentNode = currentNode->children[index];
    }
    return currentNode->word_count;
}

// Deallocates the trie structure
void deallocateTrie(struct TrieNode* root)
{
    if (root == NULL)
        return;

    for (int i = 0; i < 26; i++)
    {
        if (root->children[i] != NULL)
            deallocateTrie(root->children[i]);
    }
    free(root);
}

// This function will return the number of words in the dictionary
// and read all the words in the dictionary to the structure words
int readDictionary(char* filename, char** pInWords)
{
    FILE* file;
    if (fopen_s(&file, filename, "r") != 0)
    {
        printf("Error opening the file '%s'\n", filename);
        exit(1);
    }

    int numWords = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file))
    {
        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0'; // Remove the trailing newline character

        int hasDigit = 0;
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (isdigit(buffer[i]))
            {
                hasDigit = 1;
                break;
            }
        }

        if (hasDigit)
            continue; // Skip lines with numbers

        pInWords[numWords] = _strdup(buffer);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char* inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct TrieNode* pTrie = createTrieNode();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Words to be searched for occurrences
    char* pWords[] = { "notaword", "ucf", "no", "note", "corg" };
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    deallocateTrie(pTrie);
    pTrie = NULL; // To prevent dangling pointer

    return 0;
}