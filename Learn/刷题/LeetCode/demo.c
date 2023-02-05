#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 10

// Structure of a hash entry
struct HashEntry
{
    int key;
    int value;
};

struct HashTable
{
    int size;
    struct HashEntry **table;
};

// Initialize a hash table
struct HashTable *createHashTable(int size)
{
    struct HashTable *hashTable = NULL;
    if (size < 1)
        return NULL;
    // Allocate memory for hash table
    if ((hashTable = malloc(sizeof(struct HashTable))) == NULL)
    {
        return NULL;
    }
    // Allocate memory for size elements of the table
    if ((hashTable->table = malloc(sizeof(struct HashEntry *) * size)) == NULL)
    {
        return NULL;
    }
    // Initialize table with NULL pointers
    int i;
    for (i = 0; i < size; i++)
    {
        hashTable->table[i] = NULL;
    }
    hashTable->size = size;
    return hashTable;
}

// Hash a key
int hashKey(struct HashTable *hashTable, int key)
{
    return key % hashTable->size;
}

// Add a key-value pair in the hash table
void addElement(struct HashTable *hashTable, int key, int value)
{
    // Create a new element
    struct HashEntry *newElement = NULL;
    if ((newElement = malloc(sizeof(struct HashEntry))) == NULL)
    {
        return;
    }
    // Set key and value
    newElement->key = key;
    newElement->value = value;
    // Get the hash index of the key
    int hashIndex = hashKey(hashTable, key);
    // Add the element to the hash table
    while (hashTable->table[hashIndex] != NULL)
    {
        // Go to next cell
        ++hashIndex;
        // Wrap around the table
        hashIndex %= hashTable->size;
    }
    hashTable->table[hashIndex] = newElement;
}

// Search the table for the value corresponding to the given key
int search(struct HashTable *hashTable, int key)
{
    // Find the element using the key
    int hashIndex = hashKey(hashTable, key);
    while (hashTable->table[hashIndex] != NULL)
    {
        if (hashTable->table[hashIndex]->key == key)
        {
            return hashTable->table[hashIndex]->value;
        }
        // Go to next cell
        ++hashIndex;
        // Wrap around the table
        hashIndex %= hashTable->size;
    }
    return -1;
}

int main(int argc, char const *argv[])
{
    struct HashTable *hashtab = createHashTable(12);

    addElement(hashtab, 0, 666);
    addElement(hashtab, 1, 12312);
    addElement(hashtab, 2, 324);
    addElement(hashtab, 99, 661231456);

    int val = search(hashtab, 99);
    if(val != -1)
        printf("%d", val);


    return 0;
}
