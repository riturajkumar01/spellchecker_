#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26
#define MAX_WORD_LENGTH 100
#define MAX_SUGGESTIONS 10

struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool isEndOfWord; // true if the node represents end of a word
};

struct TrieNode* getNode(void) {
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    node->isEndOfWord = false;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        node->children[i] = NULL;

    return node;
}

void insert(struct TrieNode *root, const char *word) {
    struct TrieNode *current = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!current->children[index])
            current->children[index] = getNode();
        current = current->children[index];
    }
    current->isEndOfWord = true;
}

bool search(struct TrieNode *root, const char *word) {
    struct TrieNode *current = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!current->children[index])
            return false; // Word not found
        current = current->children[index];
    }
    return (current != NULL && current->isEndOfWord);
}

bool isLastNode(struct TrieNode *root) {
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i])
            return false;
    return true;
}

void suggestionsRec(struct TrieNode *root, char *currentPrefix) {
    if (root->isEndOfWord) {
        printf("%s\n", currentPrefix);
    }

    if (isLastNode(root))
        return;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]) {
            char newPrefix[MAX_WORD_LENGTH];
            strcpy(newPrefix, currentPrefix);
            char letter = 'a' + i;
            strncat(newPrefix, &letter, 1);
            suggestionsRec(root->children[i], newPrefix);
        }
    }
}

int printAutoSuggestions(struct TrieNode *root, const char *prefix) {
    struct TrieNode *current = root;
    for (int i = 0; i < strlen(prefix); i++) {
        int index = prefix[i] - 'a';
        if (!current->children[index])
            return 0; // No suggestions found
        current = current->children[index];
    }

    bool isWord = current->isEndOfWord;
    bool isLast = isLastNode(current);

    if (isWord && isLast) {
        printf("%s\n", prefix);
        return -1;
    }

    if (!isLast) {
        char prefixCopy[MAX_WORD_LENGTH];
        strcpy(prefixCopy, prefix);
        suggestionsRec(current, prefixCopy);
        return 1;
    }
    return 0;
}

void loadDictionary(struct TrieNode *root) {
    char word[MAX_WORD_LENGTH];
    printf("Enter words for the dictionary (type 'exit' to stop):\n");
    
    while (true) {
        printf("> ");
        fgets(word, sizeof(word), stdin); // Use fgets to read the entire line

        // Remove newline character if it exists
        word[strcspn(word, "\n")] = 0;

        if (strcmp(word, "exit") == 0) {
            break; // Stop input when 'exit' is typed
        }

        // Split the input line into words and insert each into the Trie
        char *token = strtok(word, " ");
        while (token != NULL) {
            insert(root, token);
            token = strtok(NULL, " ");
        }
    }
}

void searchWords(struct TrieNode *root) {
    char line[MAX_WORD_LENGTH];
    printf("\nEnter words to search (type 'exit' to stop searching):\n");
    
    while (true) {
        printf("> ");
        fgets(line, sizeof(line), stdin); // Use fgets to read the entire line

        // Remove newline character if it exists
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "exit") == 0) {
            break; // Stop input when 'exit' is typed
        }

        // Split the input line into words
        char *word = strtok(line, " ");
        while (word != NULL) {
            // Check if the word is spelled correctly
            if (search(root, word)) {
                printf("'%s' is spelled correctly.\n\n", word);
            } else {
                printf("'%s'\n <-- is misspelled. Suggestions similar to this words are --> :\n", word);
                int result = printAutoSuggestions(root, word);
                if (result == 0) {
                    printf("No suggestions found.\n");
                }
            }
            word = strtok(NULL, " ");
        }
        printf("\n"); // Extra line break after processing each line
    }
}

int main() {
    struct TrieNode *root = getNode();

    // Load words into the dictionary from user input
    loadDictionary(root);

    // Run dynamic test cases after loading words
    searchWords(root);

    // Free dynamically allocated memory (not shown for simplicity)
    // Implement a function to free the Trie nodes to prevent memory leaks

    return 0;
}