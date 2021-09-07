// COPYRIGHT 2021 @JZRY, ALL RIGHTS RESERVED. DO NOT REPLICATE.

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED INT_MIN
#define LPA_SUCCESS -1911318925
#define LPA_FAILURE -1911318926

typedef struct DynamicArray
{
    int size;                  // number of occupied cells across all fragments
    int num_fragments;         // number of fragments (arrays) in this struct
    int fragment_length;       // number of cells per fragment
    int num_active_fragments;  // number of allocated (non-NULL) fragments
    int **fragments;           // array of pointers to individual fragments
    int *fragment_sizes;       // stores number of used cells in each fragment
} DynamicArray;

typedef DynamicArray LPA;

// Create a new LPA structure.
DynamicArray *createDynamicArray(int num_fragments, int fragment_length)
{
    // Make new LPA of type LPA (struct defined).
    LPA *new_LPA = NULL;
    int i, capacity;

    // Parameters must be greater than 0.
    if (num_fragments <= 0 || fragment_length <= 0)
    {
        // Malloc error.
        return NULL;
    }

    // Set memory block to new LPA.
    if ((new_LPA = malloc(sizeof(LPA))) == NULL)
    {
        // Malloc error.
        return NULL;
    }

    // Initialize variables.
    new_LPA->fragment_length = fragment_length;
    new_LPA->num_fragments = num_fragments;
    new_LPA->num_active_fragments = 0;
    new_LPA->size = 0;

    // Allocate contiguous memory array to *fragments.
    if ((new_LPA->fragments = malloc(sizeof(int *) * num_fragments)) == NULL)
    {
        // Free memory to avoid memory leak.
        free(new_LPA);

        // Malloc error.
        return NULL;
    }

    // Allocate contiguous memory array to *fragment_sizes
    if ((new_LPA->fragment_sizes = malloc(sizeof(int) * num_fragments)) == NULL)
    {
        // Free fragments memory to avoid memory leak.
        free(new_LPA->fragments);

        // Free memory to avoid memory leak.
        free(new_LPA);

        // Malloc error.
        return NULL;
    }

    // Initialize each fragment to NULL since it's not being used.
    for (i = 0; i < num_fragments; i++)
    {
        new_LPA->fragments[i] = NULL;
    }

    // Initialize fragment_sizes to 0 since no fragments are used.
    for (i = 0; i < num_fragments; i++)
    {
        new_LPA->fragment_sizes[i] = 0;
    }

    capacity = (fragment_length * num_fragments);

    printf("-> A new DynamicArray has emerged from the void. ");
    printf("(capacity: %d, fragments: %d)\n", capacity, num_fragments);

    return new_LPA;
}

// Destroy LPA return NULL.
DynamicArray *destroyDynamicArray(DynamicArray *arr)
{
    int i, j;

    if (arr == NULL)
    {
        return NULL;
    }

    // Remove the fragments malloc.
    if (arr->fragments != NULL)
    {
        for (i = 0; i < arr->num_fragments; i++)
        {
            // Remove individual blocks of memory if they exist.
            if (arr->fragments[i] != NULL)
            {
                free(arr->fragments[i]);
            }
        }
        free(arr->fragments);
    }

    // Remove fragment sizes malloc.
    if (arr->fragment_sizes != NULL)
    {
        free(arr->fragment_sizes);
    }

    if (arr != NULL)
    {
        // Free arr malloc.
        free(arr);
    }

    printf("-> The DynamicArray has returned to the void.\n");

    return NULL;
}

DynamicArray *cloneDynamicArray(DynamicArray *arr)
{
    int i, j, capacity;
    LPA *arrClone = NULL;

    if (arr == NULL)
    {
        return NULL;
    }

    if ((arrClone = malloc(sizeof(DynamicArray))) == NULL)
    {
        // Malloc error.
        return NULL;
    }

    // Initialize variables yada yada.
    arrClone->size = arr->size;
    arrClone->num_fragments = arr->num_fragments;
    arrClone->fragment_length = arr->fragment_length;
    arrClone->num_active_fragments = arr->num_active_fragments;

    // Allocate contiguous memory array to *fragments.
    if ((arrClone->fragments = malloc(sizeof(int *) * arr->num_fragments)) == NULL)
    {
        // Free memory to avoid memory leak.
        free(arrClone);

        // Malloc error.
        return NULL;
    }

    // Allocate contiguous memory array to *fragment_sizes
    if ((arrClone->fragment_sizes = malloc(sizeof(int) * arr->num_fragments)) == NULL)
    {
        // Free fragments memory to avoid memory leak.
        free(arrClone->fragments);

        // Free memory to avoid memory leak.
        free(arrClone);

        // Malloc error.
        return NULL;
    }

    // Initialize all fragments to NULL.
    for (i = 0; i < arr->num_fragments; i++)
    {
        arrClone->fragments[i] = NULL;
    }

    // Copy contents of fragments to new fragments.
    for (i = 0; i < arr->num_fragments; i++)
    {
        // If fragment is not NULL, copy every cell in original to clone.
        if (arr->fragments[i] != NULL)
        {
            arrClone->fragments[i] = malloc(sizeof(int) * arr->fragment_length);

            // Activates if there's a malloc error.
            if (arrClone->fragments[i] == NULL)
            {
                // Loop and free any previously set fragments.
                for (j = 0; j < arr->num_fragments; j++)
                {
                    if (arrClone->fragments[j] != NULL)
                    {
                        free(arrClone->fragments[i]);
                        arrClone->fragments[i] = NULL;
                    }
                }

                // Free fragments memory to avoid memory leak.
                free(arrClone->fragments);

                // Free fragment_sizes memory to avoid memory leak.
                free(arrClone->fragment_sizes);

                // Free memory to avoid memory leak.
                free(arrClone);

                return NULL;
            }

            // Copy all values in active fragments to new clone.
            for (j = 0; j < arr->fragment_length; j++)
            {
                arrClone->fragments[i][j] = arr->fragments[i][j];
            }
        }
    }

    // Copy fragment_sizes.
    for (i = 0; i < arr->num_fragments; i++)
    {
        arrClone->fragment_sizes[i] = arr->fragment_sizes[i];
    }

    capacity = (arr->fragment_length * arr->num_fragments);

    printf("-> Successfully cloned the DynamicArray. ");
    printf("(capacity: %d, fragments: %d)\n", capacity, arr->num_fragments);

    return arrClone;
}

// Check if an index is valid. Assume arr is not NULL. Helper function.
int checkIfValidIndex(DynamicArray *arr, int index)
{
    int upper_lim, lower_lim;

    upper_lim = ((arr->num_fragments * arr->fragment_length) - 1);
    lower_lim = 0;

    if (index < lower_lim || index > upper_lim)
    {
        // Index is invalid.
        return LPA_FAILURE;
    }
    else
    {
        // Index is valid.
        return 1;
    }
}

// Set an element in the LPA.
int set(DynamicArray *arr, int index, int key)
{
    int row, col, i, lo_index, hi_index, len;

    if (arr == NULL)
    {
        printf("-> Bloop! NULL pointer detected in set().\n");
        return LPA_FAILURE;
    }

    // Variables to map to fragment and cell with index.
    row = (index / arr->fragment_length);
    col = (index % arr->fragment_length);

    // Check for invalid index.
    if (checkIfValidIndex(arr, index) == LPA_FAILURE)
    {
        printf("-> Bloop! Invalid access in set(). ");
        printf("(index: %d, fragment: %d, offset: %d)\n", index, row, col);
        return LPA_FAILURE;
    }

    // Fragment is uninitialized and contains nothing.
    if (arr->fragments[row] == NULL)
    {
        if ((arr->fragments[row] = malloc(sizeof(int) * arr->fragment_length)) == NULL)
        {
            // Malloc error.
            return LPA_FAILURE;
        }

        // Set every cell in new fragment UNUSED.
        for (i = 0; i < arr->fragment_length; i++)
        {
            arr->fragments[row][i] = UNUSED;
        }

        // Increment the number of active fragments.
        arr->num_active_fragments++;

        // Increment size.
        arr->size++;

        // Increment size in fragment_sizes.
        arr->fragment_sizes[row]++;

        // Set key to correct index.
        arr->fragments[row][col] = key;

        // Smaller fragment_length variable for readability.
        len = arr->fragment_length;

        // Lowest index in array and highest index.
        lo_index = row * len;
        hi_index = ((row * len) + (len - 1));

        printf("-> Spawned fragment %d. ", row);
        printf("(capacity: %d, indices: %d..%d)\n", len, lo_index, hi_index);
    }

    // Cell is UNUSED.
    if (arr->fragments[row][col] == UNUSED)
    {
        // Increment size.
        arr->size++;

        // Increment size in fragment_sizes.
        arr->fragment_sizes[row]++;

        // Set key to correct index.
        arr->fragments[row][col] = key;
    }

    // Cell is taken up by another number, write over it.
    if (arr->fragments[row][col] != UNUSED)
    {
        // Set key to correct index.
        arr->fragments[row][col] = key;
    }

    return LPA_SUCCESS;
}

// Retrieve element at given index.
int get(DynamicArray *arr, int index)
{
    int row, col;

    if (arr == NULL)
    {
        printf("-> Bloop! NULL pointer detected in get().\n");
        return LPA_FAILURE;
    }

    // Variables to map to fragment and cell with index.
    row = (index / arr->fragment_length);
    col = (index % arr->fragment_length);

    // Check for invalid index.
    if (checkIfValidIndex(arr, index) == LPA_FAILURE)
    {
        printf("-> Bloop! Invalid access in get(). ");
        printf("(index: %d, fragment: %d, offset: %d)\n", index, row, col);
        return LPA_FAILURE;
    }

    // Fragment is unallocated.
    if (arr->fragments[row] == NULL)
    {
        return UNUSED;
    }

    // Index maps to UNUSED cell.
    if (arr->fragments[row][col] == UNUSED)
    {
        return UNUSED;
    }

    // Return number from index.
    return arr->fragments[row][col];
}

// Delete element at given index.
int delete(DynamicArray *arr, int index)
{
    int row, col, len, lo_index, hi_index;

    if (arr == NULL)
    {
        printf("-> Bloop! NULL pointer detected in delete().\n");
        return LPA_FAILURE;
    }

    // Variables to map to fragment and cell with index.
    row = (index / arr->fragment_length);
    col = (index % arr->fragment_length);

    // Check for invalid index.
    if (checkIfValidIndex(arr, index) == LPA_FAILURE)
    {
        printf("-> Bloop! Invalid access in delete(). ");
        printf("(index: %d, fragment: %d, offset: %d)\n", index, row, col);
        return LPA_FAILURE;
    }

    // Trying to delete an element in fragment that doesn't exist.
    if (arr->fragments[row] == NULL)
    {
        return LPA_FAILURE;
    }

    // Trying to delete an UNUSED cell is not allowed.
    if (arr->fragments[row][col] == UNUSED)
    {
        return LPA_FAILURE;
    }

    // Deleting a number inside a fragment.
    if (arr->fragments[row][col] != UNUSED)
    {
        // Set the element at given index to UNUSED.
        arr->fragments[row][col] = UNUSED;

        // Decrement size.
        arr->size--;

        // Decrement fragment_sizes.
        arr->fragment_sizes[row]--;

        // Deallocate fragment if it contains no occupied cells.
        if (arr->fragment_sizes[row] == 0)
        {
            if (arr->fragments[row] != NULL)
            {
                // Free fragment malloc.
                free(arr->fragments[row]);

                // Set pointer to NULL.
                arr->fragments[row] = NULL;
            }

            // Decrement number of active fragments.
            arr->num_active_fragments--;

            // Smaller fragment_length variable for readability.
            len = arr->fragment_length;

            // Lowest index in array and highest index.
            lo_index = row * len;
            hi_index = ((row * len) + (len - 1));

            printf("-> Deallocated fragment %d. ", row);
            printf("(capacity: %d, indices: %d..%d)\n", len, lo_index, hi_index);
        }
    }

    return LPA_SUCCESS;
}

// Perform linear search to find if key is in LPA.
int containsKey(DynamicArray *arr, int key)
{
    int i, j;

    if (arr == NULL)
    {
        return 0;
    }

    // Linear search.
    for (i = 0; i < arr->num_fragments; i++)
    {
        for (j = 0; j < arr->fragment_length; j++)
        {
            if (arr->fragments[i] == NULL)
            {
                break;
            }
            if (arr->fragments[i][j] == key)
            {
                return 1;
            }
        }
    }

    return 0;
}

// Determine value being stored at index.
int isSet(DynamicArray *arr, int index)
{
    int col, row;

    if (arr == NULL)
    {
        return 0;
    }

    // Variables to map to fragment and cell with index.
    row = (index / arr->fragment_length);
    col = (index % arr->fragment_length);

    // Check if index is invalid.
    if (checkIfValidIndex(arr, index) == LPA_FAILURE)
    {
        return 0;
    }

    // Fragment doesn't exist.
    if (arr->fragments[row] == NULL)
    {
        return 0;
    }

    // Fragment is unused.
    if (arr->fragments[row][col] == UNUSED)
    {
        return 0;
    }

    // Value is being stored at index.
    return 1;
}

// Print value at corresponding index if it's valid.
int printIfValid(DynamicArray *arr, int index)
{
    int row, col;

    if (arr == NULL)
    {
        return LPA_FAILURE;
    }

    // Check if index is invalid.
    if (checkIfValidIndex(arr, index) == LPA_FAILURE)
    {
        return LPA_FAILURE;
    }

    // Variables to map to fragment and cell with index.
    row = (index / arr->fragment_length);
    col = (index % arr->fragment_length);

    // Row is not allocated.
    if (arr->fragments[row] == NULL)
    {
        return LPA_FAILURE;
    }

    // Index cell is unused.
    if (arr->fragments[row][col] == UNUSED)
    {
        return LPA_FAILURE;
    }

    // Avoid seg fault.
    if (arr->fragments[row] != NULL)
    {
        // Print out digit at specified index.
        printf("%d\n", arr->fragments[row][col]);
    }

    return LPA_SUCCESS;
}

// Reset LPA back to nascent state.
DynamicArray *resetDynamicArray(DynamicArray *arr)
{
    int i, capacity;

    if (arr == NULL)
    {
        printf("-> Bloop! NULL pointer detected in resetDynamicArray().\n");
        return arr;
    }

    // Free any fragments and set all fragments to NULL.
    for (i = 0; i < arr->num_fragments; i++)
    {
        if (arr->fragments[i] != NULL)
        {
            free(arr->fragments[i]);
            arr->fragments[i] = NULL;
        }

        // Set all fragment_sizes to 0.
        arr->fragment_sizes[i] = 0;
    }

    // Variables reset.
    arr->size = 0;
    arr->num_active_fragments = 0;
    capacity = (arr->fragment_length * arr->num_fragments);

    printf("-> The DynamicArray has returned to its nascent state. ");
    printf("(capacity: %d, fragments: %d)\n", capacity, arr->num_fragments);

    return arr;
}

// Return current size in LPA. -1 if arr is NULL.
int getSize(DynamicArray *arr)
{
    return (arr == NULL) ? -1 : arr->size;
}

// Return LPA capacity (num_frag * frag_length). -1 if arr is NULL.
int getCapacity(DynamicArray *arr)
{
    return (arr == NULL) ? -1 : (arr->num_fragments * arr->fragment_length);
}

// Return max capacity of available cells.
int getAllocatedCellCount(DynamicArray *arr)
{
    int i, allocated_sum = 0;

    if (arr == NULL)
    {
        return -1;
    }

    // Loop to check if any fragment is not NULL.
    for (i = 0; i < arr->num_fragments; i++)
    {
        if (arr->fragments[i] != NULL)
        {
            // Add total amount of cells in each fragment for every non NULL fragment.
            allocated_sum += arr->fragment_length;
        }
    }

    return allocated_sum;
}

// Get size taken up in bytes of a normal array.
long long unsigned int getArraySizeInBytes(DynamicArray *arr)
{
    long long unsigned int array_bytes;

    if (arr == NULL)
    {
        return 0;
    }

    // Number of bytes taken up by traditional array.
    array_bytes = (
                  (long long unsigned int)arr->num_fragments *
                  (long long unsigned int)arr->fragment_length *
                  (long long unsigned int)sizeof(int)
                  );

    return array_bytes;
}

// Size in bytes taken up with LPA.
long long unsigned int getCurrentSizeInBytes(DynamicArray *arr)
{
    long long unsigned int LPA_bytes;

    if (arr == NULL)
    {
        return 0;
    }

    // Number of bytes taken up by LPA.
    LPA_bytes = (
                // Pointers & Struct.
                sizeof(LPA*) +
                sizeof(LPA) +

                // Number of bytes taken up by active fragments.
                (sizeof(int) * arr->num_active_fragments * arr->fragment_length) +

                // Bytes taken up by fragments array.
                (sizeof(arr->fragments) * arr->num_fragments) +

                // Bytes taken up by fragment_sizes array.
                (sizeof(int) * arr->num_fragments)
                );

    return LPA_bytes;
}
