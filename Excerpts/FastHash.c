// COPYRIGHT 2021 @JZRY, ALL RIGHTS RESERVED. DO NOT REPLICATE.

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define UNUSED   INT_MIN
#define DIRTY    INT_MAX
#define HASH_ERR 0
#define HASH_OK  1
#define DEFAULT_CAPACITY 5

typedef enum ProbingType { LINEAR, QUADRATIC } ProbingType;

typedef struct HashStats
{
    // This field keeps track of how many insert, search, or delete operations
    // take place over this hash table's lifetime.
    int opCount;

    // This field keeps track of how many collisions occur when performing
    // insert, search, or delete operations on this hash table.
    int collisions;

} HashStats;

typedef struct HashTable
{
    // Your hash table will store integer keys in this array.
    int *array;

    // The current capacity of your hash table (the length of 'array').
    int capacity;

    // The size of your hash table (the number of elements it contains).
    int size;

    // A pointer to the hash function for this hash table (initially NULL).
    unsigned int (*hashFunction)(int);

    // Probing type: LINEAR or QUADRATIC. Initialize to LINEAR by default.
    ProbingType probing;

    // A struct within a struct for maintaining stats on this hash table:
    // number of operations performed and number of collisions encountered.
    HashStats stats;

} HashTable;


int nextPrime(int n)
{
    int i, root, keepGoing = 1;

    if (n % 2 == 0)
        ++n;

    while (keepGoing)
    {
        keepGoing = 0;

        root = sqrt(n);

        for (i = 3; i <= root; i++)
        {
            if (n % i == 0)
            {
                n += 2;
                keepGoing = 1;

                break;
            }
        }
    }

    return n;
}

// Creates a hash table given the capacity.
HashTable *makeHashTable(int capacity)
{
    HashTable *h = NULL;
    int i;

    // Initialize hash table.
    if ((h = calloc(1, sizeof(HashTable))) == NULL)
    {
        return NULL;
    }

    // Initialize array.
    if (capacity <= 0)
    {
        if ((h->array = malloc(sizeof(int) * DEFAULT_CAPACITY)) == NULL)
        {
            free(h);
            return NULL;
        }

        // Default array contains only UNUSED.
        for (i = 0; i < DEFAULT_CAPACITY; i++)
        {
            h->array[i] = UNUSED;
        }
    }
    else
    {
        if ((h->array = malloc(sizeof(int) * capacity)) == NULL)
        {
            free(h);
            return NULL;
        }

        // Default array contains only UNUSED.
        for (i = 0; i < capacity; i++)
        {
            h->array[i] = UNUSED;
        }
    }

    // Default hash table initializations.
    h->capacity = capacity;
    h->size = 0;
    h->hashFunction = NULL;
    h->probing = LINEAR;

    // Default hash table stats.
    h->stats.opCount = 0;
    h->stats.collisions = 0;

    return h;
}

// Destroys an existing hash table (from memory).
HashTable *destroyHashTable(HashTable *h)
{
    if (h == NULL)
    {
        return NULL;
    }

    if (h->array == NULL)
    {
        free(h);
        return NULL;
    }

    // Frees memory belonging to hash table if it exists.
    free(h->array);
    free(h);

    return NULL;
}

// Sets probing type for hash table.
int setProbing(HashTable *h, ProbingType probing)
{
    if (h == NULL)
    {
        return HASH_ERR;
    }

    // Probing types: LINEAR or QUADRATIC.
    // Anything else results in error.
    if (probing == LINEAR)
    {
        h->probing = LINEAR;
        return HASH_OK;
    }
    else if (probing == QUADRATIC)
    {
        h->probing = QUADRATIC;
        return HASH_OK;
    }
    else
    {
        return HASH_ERR;
    }
}

// Set hashFunction member to hash function passed.
int setHashFunction(HashTable *h, unsigned int (*hashFunction)(int))
{
    if (h == NULL)
    {
        return HASH_ERR;
    }
    h->hashFunction = hashFunction;
    return HASH_OK;
}

// If hash table is at least half empty return 1.
// If hash table is more full than empty return 0.
int isAtLeastHalfEmpty(HashTable *h)
{
    if (h == NULL || h->capacity == 0)
    {
        return 0;
    }
    return (h->size <= h->capacity / 2);
}

// Insert helper function for expandHashTable().
int *tempArrayInsert(HashTable *h, ProbingType probingType)
{
    int i, j, tempIndex, tempValue, new_array_length, *tempArray = NULL;

    // LINEAR and QUADRATIC probing.
    if (probingType == LINEAR)
    {
        new_array_length = (h->capacity * 2 + 1);

        if ((tempArray = malloc(sizeof(int) * new_array_length)) == NULL)
        {
            // Memory error.
            return HASH_ERR;
        }

        // Initializing the new array.
        for (i = 0; i < new_array_length; i++)
        {
            tempArray[i] = UNUSED;
        }

        // Hash original h->array values to tempArray.
        for (i = 0; i < h->capacity; i++)
        {
            // If statement activated only if h->array[i] contains actual value.
            if (h->array[i] != UNUSED && h->array[i] != DIRTY)
            {
                // Value -> HASHFUNCTION -> tempIndex.
                tempValue = h->hashFunction(h->array[i]);

                for (j = 0; j < new_array_length; j++)
                {
                    tempIndex = (tempValue + j) % new_array_length;

                    // If index in tempArray is UNUSED, replace w/ key from original.
                    if (tempArray[tempIndex] == UNUSED)
                    {
                        tempArray[tempIndex] = h->array[i];
                        break;
                    }
                    else
                    {
                        // Collided into a number.
                        h->stats.collisions++;
                        continue;
                    }
                }
            }
        }
        return tempArray;
    }
    else if (probingType == QUADRATIC)
    {
        new_array_length = nextPrime(h->capacity * 2 + 1);

        if ((tempArray = malloc(sizeof(int) * new_array_length)) == NULL)
        {
            // Memory error.
            return HASH_ERR;
        }

        // Initializing the new array.
        for (i = 0; i < new_array_length; i++)
        {
            tempArray[i] = UNUSED;
        }

        // Hash original h->array values to tempArray.
        for (i = 0; i < h->capacity; i++)
        {
            // If statement activated only if h->array[i] contains actual value.
            if (h->array[i] != UNUSED && h->array[i] != DIRTY)
            {
                // Value -> HASHFUNCTION -> tempIndex.
                tempValue = h->hashFunction(h->array[i]);

                for (j = 0; j < new_array_length; j++)
                {
                    tempIndex = (tempValue + j * j) % new_array_length;

                    // If index in tempArray is UNUSED, replace w/ key from original.
                    if (tempArray[tempIndex] == UNUSED)
                    {
                        tempArray[tempIndex] = h->array[i];
                        break;
                    }
                    else
                    {
                        // Collided into a number
                        h->stats.collisions++;
                        continue;
                    }
                }
            }
        }
        return tempArray;
    }
    else
    {
        // Error. Probing not LINEAR or QUADRATIC.
        return NULL;
    }
}

// Make a temporary array to expand the original array,
// and free original array.
int expandHashTable(HashTable *h)
{
    int new_array_length, *tempArray = NULL;

    if (h == NULL)
        return HASH_ERR;

    if (h->hashFunction == NULL)
        return HASH_ERR;

    // LINEAR and QUADRATIC probing.
    if (h->probing == LINEAR)
    {
        new_array_length = (h->capacity * 2 + 1);

        // Hash original h->array values to tempArray linearly.
        if ((tempArray = tempArrayInsert(h, LINEAR)) == NULL)
        {
            // tempArray not filled properly.
            return HASH_ERR;
        }
    }
    else if (h->probing == QUADRATIC)
    {
        new_array_length = nextPrime(h->capacity * 2 + 1);

        // Hash original h->array values to tempArray quadratically.
        if ((tempArray = tempArrayInsert(h, QUADRATIC)) == NULL)
        {
            // tempArray not filled properly.
            return HASH_ERR;
        }
    }
    else
    {
        // Error. Probing not LINEAR or QUADRATIC.
        return HASH_ERR;
    }

    // Connect new array and delete original.
    free(h->array);
    h->array = tempArray;
    h->capacity = new_array_length;

    return HASH_OK;
}

// Insert key into the hash table.
// If array is not at least half empty it expands.
int insert(HashTable *h, int key)
{
    unsigned int hashValue, index;
    int i;

    if (h == NULL)
        return HASH_ERR;

    if (h->hashFunction == NULL)
        return HASH_ERR;

    // If hash table is not at least half empty (more than half full), expand table.
    if (!isAtLeastHalfEmpty(h))
    {
        if (expandHashTable(h) == HASH_ERR)
        {
            // expandHashTable() fails.
            return HASH_ERR;
        }
    }

    // Key -> HASHFUNCTION -> hashValue.
    hashValue = h->hashFunction(key);

    // LINEAR or QUADRATIC insertion.
    if (h->probing == LINEAR)
    {
        // Loop until open slot is found.
        for (i = 0; i < h->capacity; i++)
        {
            index = (hashValue + i) % h->capacity;

            if (h->array[index] == UNUSED)
            {
                h->array[index] = key;
                break;
            }

            // Available key slot not found.
            h->stats.collisions++;
        }
    }
    else if (h->probing == QUADRATIC)
    {
        // Loop until open slot is found.
        for (i = 0; i < h->capacity; i++)
        {
            index = (hashValue + (i * i)) % h->capacity;

            if (h->array[index] == UNUSED)
            {
                h->array[index] = key;
                break;
            }

            // Available key slot not found.
            h->stats.collisions++;
        }
    }
    else
    {
        // Error. Probing not LINEAR or QUADRATIC.
        return HASH_ERR;
    }

    // Amount of elements in hash table increases.
    h->size++;

    // Insertion operation increases opertion count.
    h->stats.opCount++;

    return HASH_OK;
}

// Searches for a key inside a hash table.
int search(HashTable *h, int key)
{
    int index, keyIndex, hashValue;

    if (h == NULL)
        return -1;

    if (h->hashFunction == NULL)
        return -1;

    // Key -> HASHFUNCTION -> hashValue.
    hashValue = h->hashFunction(key);

    // LINEAR and QUADRATIC probing.
    if (h->probing == LINEAR)
    {
        for (index = 0; index < h->capacity; index++)
        {
            // KeyIndex sized according to capacity.
            keyIndex = (hashValue + index) % h->capacity;

            if (h->array[keyIndex] == key)
            {
                // Key found.
                h->stats.opCount++;
                return keyIndex;
            }
            else if (h->array[keyIndex] == UNUSED)
            {
                // Key cannot be in array, return.
                h->stats.opCount++;
                return -1;
            }
            else if (h->array[keyIndex] == DIRTY)
            {
                // DIRTY in key position, keep searching.
                h->stats.collisions++;
            }
            else
            {
                // Invalid value in key index.
                h->stats.collisions++;
            }
        }
    }
    else if (h->probing == QUADRATIC)
    {
        for (index = 0; index < h->capacity; index++)
        {
            // KeyIndex sized according to capacity.
            keyIndex = (hashValue + (index * index)) % h->capacity;

            if (h->array[keyIndex] == key)
            {
                // Key found.
                h->stats.opCount++;
                return keyIndex;
            }
            else if (h->array[keyIndex] == UNUSED)
            {
                // Key cannot be in array, return.
                h->stats.opCount++;
                return -1;
            }
            else if (h->array[keyIndex] == DIRTY)
            {
                // DIRTY in key position, keep searching.
                h->stats.collisions++;
            }
            else
            {
                // Invalid value in key index.
                h->stats.collisions++;
            }
        }
    }
    else
    {
        // Error. Probing not LINEAR or QUADRATIC.
        return -1;
    }

    h->stats.opCount++;
    return -1;
}

// Finds the key inside the array and deletes it.
// If there is identical keys it deletes the first found.
int delete(HashTable *h, int key)
{
    int keyIndex;

    if (h == NULL)
        return -1;

    if (h->hashFunction == NULL)
        return -1;

    // Finds the key to delete, returns keyIndex.
    keyIndex = search(h, key);

    if (keyIndex == -1)
    {
        // Key not found.
        return -1;
    }
    else
    {
        // Key found and deleted.
        h->array[keyIndex] = DIRTY;
        h->size--;
        return keyIndex;
    }
}

void print_hash_table(HashTable *h)
{
    int i;

    if (h == NULL)
        return;

    if (h->hashFunction == NULL)
        return;

    if (h->array == NULL)
        return;

    for (i = 0; i < h->capacity; i++)
    {
        printf("%d ", h->array[i]);
    }
    printf("\n");
}

unsigned int hash(int key)
{
    return ((key / 10) % 10) * 37 + (key % 10);
}
