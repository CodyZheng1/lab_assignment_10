#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


struct TrieNode {
    struct TrieNode* children[26];
    int count;
};

struct Trie {
    struct TrieNode* root;
};


struct TrieNode* createTrieNode() {
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    newNode->count = 0;
    for (int i = 0; i < 26; ++i) {
        newNode->children[i] = NULL;
    }
    return newNode;
}

//Inserts the word to the trie structure
void insert(struct Trie* pTrie, char* word) {
    struct TrieNode* currentNode = pTrie->root;
    for (int i = 0; word[i] != '\0'; ++i) {
        int index = tolower(word[i]) - 'a';
        if (!currentNode->children[index]) {
            currentNode->children[index] = createTrieNode();
        }
        currentNode = currentNode->children[index];
    }
    currentNode->count++;
}

// computes the number of occuances of the word
int numberOfOccurrences(struct Trie* pTrie, char* word) {
    struct TrieNode* currentNode = pTrie->root;
    for (int i = 0; word[i] != '\0'; ++i) {
        int index = tolower(word[i]) - 'a';
        if (!currentNode->children[index]) {
            return 0;
        }
        currentNode = currentNode->children[index];
    }
    return currentNode->count;
}

void deallocateTrieNode(struct TrieNode* node) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < 26; ++i) {
        deallocateTrieNode(node->children[i]);
    }
    free(node);
}

struct Trie* createTrie() {
    struct Trie* pTrie = (struct Trie*)malloc(sizeof(struct Trie));
    pTrie->root = createTrieNode();
    return pTrie;
}
//this function will return number of words in the dicitonary,
// and read all the words in the dicitonary to the strucutre words
int readDictionary(char* filename, char** pInWords) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    
    int numWords = 0;
    char buffer[256];
    
    while (fgets(buffer, sizeof(buffer), file)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        pInWords[numWords] = (char*)malloc((len + 1) * sizeof(char));
        strcpy(pInWords[numWords], buffer);
        numWords++;
    }
    
    fclose(file);
    return numWords;
}

int main(void) {
    char* inWords[256];

  // read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    
    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }
    // parse line by line, and insert each word to the trie data structure
    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }
    
    deallocateTrieNode(pTrie->root);
    free(pTrie);
    
    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }

    return 0;
}