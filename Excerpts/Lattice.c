// COPYRIGHT 2021, @JZRY, ALL RIGHTS RESERVED.
// If you would like to use any part of this code, please reach out to me.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define AUTHENT_ERR INT_MIN
#define AUTHENT_OK INT_MAX
#define DEFAULT_SIDE_LENGTH 5
#define LATTICE_OK 1
#define LATTICE_ERR 0
#define NO_PATHWAY 0
#define PATHWAY 1


// Information functions:
typedef struct Data
{
    // Holds user data in a 2D array.
    // Each time user inserts a new piece of information, a new column is added.
    // Rows hold each letter. Columns / files hold each word.
    // Data will look like so:
    // File #: 1   2   3
    //       +---+---+---+
    //   0   | 0 | L | P |
    //       +---+---+---+
    //   1   | x | 3 | a |
    //       +---+---+---+
    //   2   | 1 | 4 | s |
    //       +---+---+---+
    //   3   | 3 | i | w |
    //       +---+---+---+
    //   4   | A |   | o |
    //       +---+   +---+
    // File 1: 0x13a
    // File 2: L34i
    // File 3: Paswo
    char **info;

    // User ID (randomly generated salt).
    char *ID;

} Data;


// Security functions:
typedef struct AccessInfo
{
    // Displays the amount of times private information was accessed.
    unsigned int num_of_times_accessed;

    // User ID of last access (salt confirmation for each user).
    // May also function as login location tracker.
    char *last_access_userID;

    // File size (default is 5).
    int file_size;

    // How many files are currently full.
    int filesInUse;

} AccessInfo;

typedef struct Password
{
    // Holds password.
    char *pw;

    // Authenticator to create, edit, or confirm password.
    int (*authenticator)(char *, char *);

} Password;


// Lattice structure:
typedef enum UsageState { INACTIVE , ACTIVE } UsageState;

typedef struct Lattice
{
    // Holds every layer of the lattice as a 3D array.
    int ***layers;

    // Holds pathway coordinates to solve the lattice cube in O(1) time.
    // Pathway solution changes on every lattice access attempt.
    int **pathways;

    // Holds side lengths of the lattice cube (default is 5).
    int sideLength;

    // Indicates if there is currently information inside Lattice.
    // Usage types: INACTIVE or ACTIVE.
    UsageState usage;

    // Maintains password information.
    Password *password;

    // Holds access information.
    AccessInfo access_stats;

    // Holds user's private data.
    Data *data;

} Lattice;

// Initialize lattice layers.
int ***createLayers(int sideLength)
{
    int ***layers = NULL;
    int i, j, k;

    layers = malloc(sizeof(int **) * sideLength);

    if (layers == NULL)
    {
        fprintf(stderr, "Memory error.\n");
        return NULL;
    }

    // Initialize each layer. O(n^2).
    for (i = 0; i < sideLength; i++)
    {
        layers[i] = malloc(sizeof(int *) * sideLength);

        if (layers[i] == NULL)
        {
            fprintf(stderr, "Memory error.\n");
            return NULL;
        }

        for (j = 0; j < sideLength; j++)
        {
            layers[i][j] = malloc(sizeof(int) * sideLength);

            if (layers[i][j] == NULL)
            {
                fprintf(stderr, "Memory error.\n");
                return NULL;
            }
        }
    }

    // Fill up the lattice. O(n^3) time.
    for (i = 0; i < sideLength; i++)
    {
        for (j = 0; j < sideLength; j++)
        {
            for (k = 0; k < sideLength; k++)
            {
                layers[i][j][k] = NO_PATHWAY;
            }
        }
    }

    return layers;
}

// Initialize lattice solution paths. O(n^2).
int **createPaths(Lattice *l)
{
    int i, randX_axis, randY_axis;
    int **paths = NULL;

    if (l == NULL)
        return NULL;

    if (l->layers == NULL)
        if ((l->layers = createLayers(l->sideLength)) == NULL)
            return NULL;

    // If pathways already exist we don't want to rewrite.
    if (l->pathways != NULL)
        free(l->pathways);

    // For random coordinate generation.
    srand(time(NULL));

    // Allocate paths memory block (rows).
    if ((paths = malloc(sizeof(int *) * l->sideLength)) == NULL)
    {
        fprintf(stderr, "Memory error.\n");
        return NULL;
    }

    // Allocate a paths memory block (columns).
    for (i = 0; i < l->sideLength; i++)
    {
        if ((paths[i] = malloc(sizeof(int) * 2)) == NULL)
        {
            fprintf(stderr, "Memory error.\n");
            return NULL;
        }
    }

    // 3D array construct: (name)[height][y][x]
    // Solution pathways are represented as 1's.

    // Implements solution pathway for first layer.
    // Doing this prevents unnecessary comparisons in for loop.
    randX_axis = rand() % l->sideLength;
    randY_axis = rand() % l->sideLength;
    l->layers[0][randY_axis][randX_axis] = PATHWAY;

    // Creates an X and Y coordinate for first layer.
    paths[0][0] = randX_axis;
    paths[0][1] = randY_axis;

    // Add a solution pathway index.
    for (i = 1; i < l->sideLength; i++)
    {
        // Create a new random (x,y) coordinate on every loop.
        randX_axis = rand() % l->sideLength;
        randY_axis = rand() % l->sideLength;

        // Prevents layers from having the same path as previous layer.
        if (l->layers[i-1][randY_axis][randX_axis] != 1)
        {
            // Since previous coordinate is not 1,
            // current coordinate may be set to 1.

            // Add pathway to layer at (random X, random Y).
            l->layers[i][randY_axis][randX_axis] = PATHWAY;

            // Set solution path for O(1) access.
            paths[i][0] = randX_axis;
            paths[i][1] = randY_axis;
        }
        else
        {
            // Previous (X,Y) is 1, cannot make a solution
            // on different heights same coordinate solution.

            // We create a new random coordinate to place solution in.
            // Chances of new coordinate of being the same solution as above layer are slim.
            randX_axis = rand() % l->sideLength;
            randY_axis = rand() % l->sideLength;

            // Add pathway to layer at (random X, random Y).
            l->layers[i][randY_axis][randX_axis] = PATHWAY;

            // Set solution path for O(1) access.
            paths[i][0] = randX_axis;
            paths[i][1] = randY_axis;
        }
    }

    return paths;
}

// Checks if password and confirmation are equal.
int authenticator(char *pw, char *confirmation)
{
    if (pw == NULL)
        return AUTHENT_ERR;

    if (confirmation == NULL)
        return AUTHENT_ERR;

    return (strcmp(pw, confirmation) == 0) ? AUTHENT_OK : AUTHENT_ERR;
}

// Set authentication system.
int setAuthenticator(Lattice *l, int (*authenticator)(char *, char *))
{
    if (l == NULL)
        return LATTICE_ERR;

    l->password->authenticator = authenticator;
    return LATTICE_OK;
}

// Create hexadecimal userID string.
char *createUserID(void)
{
    int IDLength = 8, i;
    char *ID;

    // Implement srand() for random number generation.
    srand(time(NULL));

    // Allocate memory to store ID.
    if ((ID = malloc(sizeof(char) * (IDLength + 1))) == NULL)
        return NULL;

    // Making Hexadecimal ID (8 Digits).
    for (i = 0; i < IDLength; i++)
    {
        ID[i] = (rand() % 16);
        // printf("%X", ID[i]); // Debugging purposes
    }

    // for (i = 0; i < IDLength; i++) // Debugging purposes
    // printf("%X", ID[i]);

    return ID;
}

// Initialize lattice structure.
Lattice *createLattice(int sideLength)
{
    Lattice *l = NULL;

    // Allocate Lattice memory block.
    if ((l = malloc(sizeof(Lattice))) == NULL)
        return NULL; // Memory error

    // Allocate Password memory block.
    if ((l->password = malloc(sizeof(Password))) == NULL)
    {
        // Memory error. Free what came before it.
        free(l);
        return NULL;
    }

    // Allocate Data memory block.
    if ((l->data = malloc(sizeof(Data))) == NULL)
    {
        // Memory error. Free what came before it.
        free(l->password);
        free(l);
        return NULL;
    }

    // Set sideLength.
    if (sideLength <= 1)
    {
        // SideLength is 5 if user input is less than or equal to 1.
        l->sideLength = DEFAULT_SIDE_LENGTH;
    }
    else
    {
        l->sideLength = sideLength;
    }

    // Initialize lattice members.
    l->usage = INACTIVE;
    l->password->pw = NULL;
    l->password->authenticator = NULL;
    l->access_stats.num_of_times_accessed = 0;
    l->access_stats.last_access_userID = 0;
    l->access_stats.filesInUse = 0;
    l->access_stats.file_size = 5;
    l->data->info = NULL;
    l->data->ID = NULL;

    // Create 3D dynamically allocated array as the lattice.
    // Lattice dimensions = sideLength * sideLength * sideLength.
    if ((l->layers = createLayers(l->sideLength)) == NULL)
    {
        // Memory error. Free what came before it.
        free(l->password);
        free(l->data);
        free(l);
        return NULL;
    }

    // Create the coordinates to get into the lattice cube in O(1) time.
    // Each access into the lattice cube creates a new random path.
    if ((l->pathways = createPaths(l)) == NULL)
    {
        // Memory error. Free what came before it.
        free(l->password);
        free(l->layers);
        free(l->data);
        free(l);
        return NULL;
    }

    // Set hash authenticator to access information.
    if (setAuthenticator(l, authenticator) == LATTICE_ERR)
    {
        // Memory error. Free what came before it.
        free(l->pathways);
        free(l->password);
        free(l->layers);
        free(l->data);
        free(l);
        return NULL;
    }

    if ((l->data->ID = createUserID()) == NULL)
    {
        // Memory error. Free what came before it.
        free(l->pathways);
        free(l->password);
        free(l->layers);
        free(l->data);
        free(l);
        return NULL;
    }

    return l;
}

// Destroy layers.
int destroyLayers(Lattice *l)
{
    int i, j;

    // Check to make sure lattice exists.
    if (l == NULL || l->layers == NULL)
        return LATTICE_ERR;

    // Deallocate layers from memory.
    for (i = 0; i < l->sideLength; i++)
    {
        for (j = 0; j < l->sideLength; j++)
        {
            if (l->layers[i][j] != NULL)
                free(l->layers[i][j]);
        }
        if (l->layers[i] != NULL)
            free(l->layers[i]);
    }
    if (l->layers != NULL)
        free(l->layers);

    l->layers = NULL;

    return LATTICE_OK;
}

// Destroy pathways.
int destroyPaths(Lattice *l)
{
    int i, j;

    // Check to make sure lattice exists.
    if (l == NULL || l->pathways == NULL)
        return LATTICE_ERR;

    // Deallocate paths from memory.
    for (i = 0; i < l->sideLength; i++)
    {
        if (l->pathways[i] != NULL)
            free(l->pathways[i]);
    }
    if (l->pathways != NULL)
        free(l->pathways);

    l->pathways = NULL;

    return LATTICE_OK;
}

// Destroy lattice password.
Lattice *destroyPassword(Lattice *l)
{
    // Check to make sure lattice exists.
    if (l == NULL || l->password == NULL)
        return NULL;

    if (l->password->pw != NULL)
        free(l->password->pw);

    return NULL;
}

// Destroy lattice data.
Lattice *destroyData(Lattice *l)
{
    int i;

    // Check to make sure lattice exists.
    if (l == NULL || l->data == NULL)
        return NULL;

    if (l->data->info == NULL)
        return NULL;

    // Deallocate each file.
    for (i = 0; i < l->access_stats.file_size; i++)
    {
        if (l->data->info[i] != NULL)
            free(l->data->info[i]);
    }

    // Deallocate array pointer.
    free(l->data->info);

    return NULL;
}

// Destroy lattice.
Lattice *destroyLattice(Lattice *l)
{
    if (l == NULL)
        return NULL;

    if (l->layers != NULL)
        if (destroyLayers(l) == LATTICE_ERR)
            return NULL;

    if (l->pathways != NULL)
        if (destroyPaths(l) == LATTICE_ERR)
            return NULL;

    if (l->password != NULL)
        destroyPassword(l);

    if (l->data != NULL)
        destroyData(l);

    if (l != NULL)
        free(l);

    printf("\nSuccessfully destroyed lattice.\n");

    return NULL;
}

// Create slots for files.
char **createFiles(Lattice *l)
{
    int i;
    char **info = NULL;

    // Lattice does not exist.
    if (l == NULL || l->layers == NULL || l->pathways == NULL)
    {
        fprintf(stderr, "ERROR: Must create lattice with \"new\" command to save data.\n");
        return NULL;
    }

    // Allocate info memory block (rows).
    if ((info = malloc(sizeof(char *) * l->access_stats.file_size)) == NULL)
    {
        // Memory error.
        return NULL;
    }

    // Initialize each info slot to NULL.
    for (i = 0; i < l->access_stats.file_size; i++)
    {
        info[i] = NULL;
    }

    return info;
}

// If l->data->info is at MAX CAPACITY, take all data that is inside, copy to a temporary
// array, create an array 2 times the size of the current array, and copy from temporary array to new array.
// Free up old array.
int expandFileSlots(Lattice *l)
{
    char **tempHolder = NULL;
    int oldFileSize, i;

    if (l == NULL || l->layers == NULL || l->pathways == NULL)
        return LATTICE_ERR;

    if (l->data == NULL || l->data->info == NULL || l->data->ID == NULL)
        return LATTICE_ERR;

    // Store old file size for looping purposes.
    oldFileSize = l->access_stats.file_size;

    // Double the file size.
    l->access_stats.file_size = (l->access_stats.file_size * 2);

    if ((tempHolder = createFiles(l)) == NULL)
        return LATTICE_ERR;

    // Copy data in old file array to new file array.
    for (i = 0; i < oldFileSize; i++)
    {
        tempHolder[i] = l->data->info[i];
    }

    // Free old data from memory.
    free(l->data->info);

    // Set to NULL to be sure it was erased properly without memory leaks.
    l->data->info = NULL;

    // Now change info file storage to the tempHolder which holds the new array.
    l->data->info = tempHolder;

    return LATTICE_OK;
}

// If the number of files in use is less than half of file slot size, return 1; else, 0.
// Automatically returns false if file size is default (5).
// If this returns true, cut the file in half to save space.
int isLessThanHalf(Lattice *l)
{
    if (l == NULL || l->layers == NULL || l->pathways == NULL)
        return 0;

    if (l->data == NULL || l->data->info == NULL || l->data->ID == NULL)
        return 0;

    // Default file size can not be smaller than 5.
    if (l->access_stats.file_size <= 5)
        return -1;

    return (l->access_stats.filesInUse < l->access_stats.file_size / 2) ? 1 : 0;
}

// Cut the available file slots in half if user deletes less than half
// of the existing array. Done to maintain tolerable memory space.
int cutFileSlots(Lattice *l)
{
    char **tempHolder = NULL;
    int oldFileSize, i;

    if (l == NULL || l->layers == NULL || l->pathways == NULL)
        return LATTICE_ERR;

    if (l->data == NULL || l->data->info == NULL || l->data->ID == NULL)
        return LATTICE_ERR;

    // Do not cut default info file slot size.
    if (l->access_stats.file_size <= 5)
        return LATTICE_ERR;

    // Store old file size for looping purposes.
    oldFileSize = l->access_stats.file_size;

    // Halve the file size.
    l->access_stats.file_size = (l->access_stats.file_size / 2);

    if ((tempHolder = createFiles(l)) == NULL)
        return LATTICE_ERR;

    // Copy data in old file array to new file array.
    for (i = 0; i < l->access_stats.file_size; i++)
    {
        tempHolder[i] = l->data->info[i];
    }

    // Free old data from memory.
    if (l->data->info != NULL)
        free(l->data->info);

    // Set to NULL to be sure it was erased properly without memory leaks.
    l->data->info = NULL;

    // Now change info file storage to the tempHolder which holds the new array.
    l->data->info = tempHolder;

    return LATTICE_OK;
}

// Checks if file storage is full (1 if yes, 0 if no).
int areFilesFull(Lattice *l)
{
    if (l == NULL || l->layers == NULL || l->pathways == NULL)
        return 0;

    if (l->data == NULL || l->data->info == NULL || l->data->ID == NULL)
        return 0;

    return (l->access_stats.filesInUse >= l->access_stats.file_size) ? 1 : 0;
}

// Get user input.
char *getUserInput(char *prompt)
{
    char *input = NULL, buffer[1024];
    int strLength;

    if (prompt == NULL)
    {
        fprintf(stderr, "ERROR: No prompt given to getUserInput().\n");
        return NULL;
    }

    printf("%s", prompt);
    scanf("%s", buffer);

    strLength = strlen(buffer);

    // Allocate input memory to allow passing outside of function.
    if ((input = malloc(sizeof(char) * (strLength + 1))) == NULL)
    {
        printf("ERROR: Memory error in getUserInput().\n");
        return NULL;
    }

    // Copy buffer contents to input and finish string with NULL sentinel '\0'.
    strcpy(input, buffer);
    input[strLength] = '\0';

    return input;
}

// Get user password.
int getUserPassword(Lattice *l)
{
    char *pw = NULL, *pw_confirm = NULL;
    int strLength;

    if (l == NULL || l->password == NULL)
        return LATTICE_ERR;

    // Let user create password.
    if ((pw = getUserInput("Enter password: ")) == NULL)
    {
        fprintf(stderr, "ERROR: Memory error in createPassword().\n");
        return LATTICE_ERR;
    }

    // Let user confirm password.
    if ((pw_confirm = getUserInput("Confirm password: ")) == NULL)
    {
        fprintf(stderr, "ERROR: Memory error in createPassword().\n");
        return LATTICE_ERR;
    }

    // Checking if passwords match.
    if (strcmp(pw, pw_confirm) == 1)
    {
        free(pw);
        free(pw_confirm);
        fprintf(stderr, "ERROR: Passwords do not match in createPassword().\n");
        return LATTICE_ERR;
    }

    // Beyond this point passwords match.

    if (l->password->pw == NULL)
    {
        // Set lattice password to created password.
        l->password->pw = pw;
    }
    else
    {
        fprintf(stderr, "ERROR: Trying to rewrite existing password in createPassword().\n");
        return LATTICE_ERR;
    }

    // Free helper password pointer.
    if (pw_confirm != NULL)
        free(pw_confirm);

    // Debugging to check password.
    printf("Successfully saved password.\n");

    return LATTICE_OK;
}

// Create password and gate function for getUserPassword().
int createPassword(Lattice *l)
{
    if (l == NULL || l->layers == NULL || l->pathways == NULL)
    {
        fprintf(stderr, "ERROR: Must create lattice with \"new\" command to save data.\n");
        return LATTICE_ERR;
    }

    if (l->password->pw != NULL)
    {
        fprintf(stderr, "ERROR: Password already created in createPassword().\n");
        return LATTICE_ERR;
    }

    if (getUserPassword(l) == 1)
    {
        printf("Password successfully created.\n");
        return LATTICE_OK;
    }
    else
    {
        // Password failed to be created.
        fprintf(stderr, "ERROR: Password creation failed in createPassword().\n");
        return LATTICE_ERR;
    }
}

// Search for file to add data.
int fileAddSearch(Lattice *l, char *data, int file)
{
    char *pw = NULL;
    int i;

    // IF THERE IS ALREADY SOMETHING IN THIS FILE, DO NOT WRITE OVER IT.
    // STORE DATA IN A NEW FILE, TELL USER THAT THE PREVIOUS FILE WAS FULL.
    // IF USER WANTS TO DELETE A SPECIFIC PIECE OF DATA THEY DO IT THEMSELVES.
    // THERE SHOULD ALWAYS BE AT LEAST ONE OPEN SPACE IF REACHING THIS POINT.
    if (l->data->info[file - 1] == NULL)
    {
        if (l->password->pw == NULL)
        {
            // Create user password if not made.
            if (createPassword(l) == LATTICE_ERR)
            {
                fprintf(stderr, "ERROR: createPassword() failed.\n");
                return LATTICE_ERR;
            }
        }
        else
        {
            // Password already exists, prompt user for it.
            if ((pw = getUserInput("Enter password: ")) == NULL)
            {
                fprintf(stderr, "ERROR: Memory error in addData.\n");
                return LATTICE_ERR;
            }

            // Compare if user input and existing password match.
            if (strcmp(l->password->pw, pw) == 0)
            {
                printf("Password correct.\n");
            }

            // Free temporary password comparator.
            if (pw != NULL)
                free(pw);
        }

        l->data->info[file-1] = data;
    }
    else
    {
        // User requested file input already contains data.
        printf("File %d is currently in use. ", file);
        printf("Adding data to next available file slot...\n");

        // Look for next available file to add data.
        for (i = 0; i < l->access_stats.file_size; i++)
        {
            if (l->data->info[i] == NULL)
            {
                if (l->password->pw == NULL)
                {
                    // Create user password if not made.
                    if (createPassword(l) == LATTICE_ERR)
                    {
                        fprintf(stderr, "ERROR: createPassword() failed.\n");
                        return LATTICE_ERR;
                    }

                    // Add data and stop searching for available file.
                    printf("Added data (%s) to file (%d).\n", data, file);
                    l->data->info[i] = data;
                    break;
                }
                else
                {
                    // Password already exists, prompt user for it.
                    if ((pw = getUserInput("Enter password: ")) == NULL)
                    {
                        fprintf(stderr, "ERROR: Memory error in getUserInput().\n");
                        return LATTICE_ERR;
                    }

                    // Compare if user input and existing password match.
                    if (strcmp(l->password->pw, pw) == 0)
                    {
                        printf("Password correct.\n");
                    }
                    else
                    {
                        printf("Invalid password.\n");
                        return LATTICE_ERR;
                    }

                    // Free temporary password comparator.
                    free(pw);

                    // Add data and stop searching for available file.
                    printf("Added data (%s) to file (%d).\n", data, file);
                    l->data->info[i] = data;
                    break;
                }
            }
        }
    }

    // Increment number of files being used.
    l->access_stats.filesInUse++;

    return LATTICE_OK;
}

// ------------------------- SOLVE / CHANGE -------------------------

// Solves the lattice cube pathways in O(n) time.
int solvePathways(Lattice *l)
{
    int i;

    if (l == NULL || l->layers == NULL || l->pathways == NULL)
        return LATTICE_ERR;

    // Loop through every layer and solve in O(n) time.
    for (i = 0; i < l->sideLength; i++)
    {
        if (l->layers[i][l->pathways[i][1]][l->pathways[i][0]] == PATHWAY)
            continue;
        else
        {
            printf("Invalid login.\n");
            return LATTICE_ERR;
        }
    }

    // Cube was solved properly.
    printf("\nPathways solved.\n");
    return LATTICE_OK;
}

// Changes lattice cube solution pathways after every attempted access.
int changePathways(Lattice *l)
{
    if (l == NULL || l->layers == NULL || l->pathways == NULL)
        return LATTICE_ERR;

    // Destroy existing layers if they exist.
    if (l->layers != NULL)
    {
        if (destroyLayers(l) == LATTICE_ERR)
            return LATTICE_ERR;
    }

    // Destroy existing pathways if they exist.
    if (l->pathways != NULL)
    {
        if (destroyPaths(l) == LATTICE_ERR)
            return LATTICE_ERR;
    }

    // Create new layers.
    if (createLayers(l->sideLength) == NULL)
        return LATTICE_ERR;

    // Create new paths.
    if (createPaths(l) == NULL)
        return LATTICE_ERR;

    return LATTICE_OK;
}

// ------------------------- END OF PROGRAM FUNCTIONS -------------------------
// ------------------------- LOOP STYLE MENU -------------------------

// Add data to a specific lattice file. Returns LATTICE_ERR or LATTICE_OK
int addData(Lattice *l, char *data, int file)
{
    // Protects against incomplete lattice.
    if (l == NULL || l->layers == NULL || l->pathways == NULL || l->data == NULL)
    {
        fprintf(stderr, "ERROR: Must create lattice with \"new\" command to save data.\n");
        return LATTICE_ERR;
    }

    // Protects against incorrect file user input.
    if (file < 1 || file > l->access_stats.file_size)
    {
        fprintf(stderr, "FILE ERROR: Valid files are from 1 - %d.\n", l->access_stats.file_size);
        return LATTICE_ERR;
    }

    // Create file slots if not made.
    if (l->data->info == NULL)
    {
        if ((l->data->info = createFiles(l)) == NULL)
        {
            fprintf(stderr, "ERROR: createFiles() failed in addData().\n");
            return LATTICE_ERR;
        }
    }

    // Create user ID if not made.
    if (l->data->ID == NULL)
    {
        if ((l->data->ID = createUserID()) == NULL)
        {
            fprintf(stderr, "ERROR: createUserID() failed in addData().\n");
            return LATTICE_ERR;
        }
    }

    // Checks to make sure file slots are not full.
    // If they are, expand file slots.
    if (areFilesFull(l) == 1)
    {
        if (expandFileSlots(l) == LATTICE_ERR)
        {
            fprintf(stderr, "ERROR: expandFilesSlots() failed in addData().\n");
            return LATTICE_ERR;
        }
    }

    // If file slots in use are less than half (exluding default 5), cut available file slots to save space.
    if (isLessThanHalf(l) == 1)
    {
        if (cutFileSlots(l) == LATTICE_ERR)
        {
            fprintf(stderr, "ERROR: cutFileSlots() failed in addData().\n");
            return LATTICE_ERR;
        }
    }

    // Get user password to access cube.
    if (getUserPassword(l) == LATTICE_ERR)
        return LATTICE_ERR;

    // Solve the cube before adding data.
    if (solvePathways(l) == LATTICE_ERR)
        return LATTICE_ERR;

    // Permutate the cube to secure entry.
    if (changePathways(l) == LATTICE_ERR)
        return LATTICE_ERR;

    // Search for file to add data to.
    return (fileAddSearch(l, data, file) == 1) ? LATTICE_OK : LATTICE_ERR;
}

// Search for file slot to delete.
int fileDeleteSearch(Lattice *l, int file)
{
    char *pw = NULL;

    // Search and delete for selected file data. If NULL, return OK.
    if (l->data->info[file - 1] == NULL)
    {
        printf("File (%d) is empty.\n", file);
        return LATTICE_OK;
    }
    else
    {
        // User requested to delete file that exists.

        if (l->password->pw == NULL)
        {
            // Password should be made.
            fprintf(stderr, "ERROR: Password should be created before deletion.\n");
            return LATTICE_ERR;
        }
        else
        {
            // Password exists.
            if ((pw = getUserInput("Enter password: ")) == NULL)
            {
                fprintf(stderr, "ERROR: Memory error in fileDeleteSearch().\n");
                return LATTICE_ERR;
            }

            // Compare if user input and existing password match.
            if (strcmp(l->password->pw, pw) == 0)
            {
                printf("Password correct.\n");
            }
            else
            {
                printf("Incorrect password.\n");
                return LATTICE_ERR;
            }

            // Free temporary password comparator.
            if (pw != NULL)
                free(pw);

            l->data->info[file - 1] = NULL;
            printf("Deleted data from file (%d).\n", file);
        }
    }

    // Decrement the number of files being used.
    l->access_stats.filesInUse--;

    return LATTICE_OK;
}

// Delete data from a specific lattice file.
int deleteData(Lattice *l, int file)
{
    // Protects against incomplete lattice.
    if (l == NULL || l->layers == NULL || l->pathways == NULL)
    {
        fprintf(stderr, "Must create lattice with \"new\" command to save data.\n");
        return LATTICE_ERR;
    }

    // Protects against incomplete data.
    if (l->data == NULL || l->data->info == NULL || l->data->ID == NULL)
    {
        fprintf(stderr, "Must create lattice with \"new\" command to save data.\n");
        return LATTICE_ERR;
    }

    // Protects against incorrect file bounds.
    if (file < 1 || file > l->access_stats.file_size)
    {
        fprintf(stderr, "File can not be less than 1 or greater than %d.\n", l->access_stats.file_size);
        return LATTICE_ERR;
    }

    // Get user password to access cube.
    if (getUserPassword(l) == LATTICE_ERR)
        return LATTICE_ERR;

    // Solve the cube before adding data.
    if (solvePathways(l) == LATTICE_ERR)
        return LATTICE_ERR;

    // Permutate the cube to secure entry.
    if (changePathways(l) == LATTICE_ERR)
        return LATTICE_ERR;

    // If file is deleted return 1.
    return (fileDeleteSearch(l, file) == LATTICE_OK) ? LATTICE_OK : LATTICE_ERR;
}

// Show data from a specific lattice file.
void showData(Lattice *l, int file)
{
    // Protects against incomplete lattice.
    if (l == NULL || l->layers == NULL || l->pathways == NULL)
    {
        fprintf(stderr, "ERROR: Must create lattice with \"new\" command to save data.\n");
        return;
    }

    // Protects against incomplete data.
    if (l->data == NULL || l->data->info == NULL || l->data->ID == NULL)
    {
        fprintf(stderr, "ERROR: Must create lattice with \"new\" command to save data.\n");
        return;
    }

    // Protects against incorrect file bounds.
    if (file < 1 || file > l->access_stats.file_size)
    {
        fprintf(stderr, "FILE ERROR: File can not be less than 1 or greater than ");
        fprintf(stderr, "%d.\n", l->access_stats.file_size);
        return;
    }

    // Prints data in selected file if entered in O(1) time.
    // Must access all solution pathways correctly in timely manner
    // and enter password correctly and match userID correctly.
    printf("Data in file #%d:\n", file);
    printf("%s\n", l->data->info[file]);
}

// Indicates if lattice currently holds information.
int isLatticeInUse(Lattice *l)
{
    int inUse = 1, notInUse = 0;
    return (l->access_stats.filesInUse == 0) ? notInUse : inUse;
}

// Print a fancy net 2D view of the pathway coordinates. O(n^2).
void printPaths(Lattice *l)
{
    int i, j;

    if (l == NULL || l->pathways == NULL)
        return;

    for (i = 0; i < l->sideLength; i++)
    {
        printf("Pathway %d solution:\n", i + 1);

        for (j = 0; j < 2; j++)
        {
            // Allows comma to print between coordinates for clarity.
            printf("%d%c ", l->pathways[i][j], (j == 0) ? ',' : ' ');
        }

        printf("\n");
    }
    printf("\n");
}

// Print a net 2D view of the current lattice cube. O(n^3).
void printLattice(Lattice *l)
{
    int i, j, k;

    if (l == NULL || l->layers == NULL)
        return;

    for (i = 0; i < l->sideLength; i++)
    {
        for (j = 0; j < l->sideLength; j++)
        {
            for (k = 0; k < l->sideLength; k++)
            {
                printf("%d ", l->layers[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

/*
// Encrypt a string.
char *encrypt(char *data)
{
    int i, strLength;
    char *hashed;

    if (data == NULL)
    {
        printf("ERROR: Passed empty data in encrypt().\n");
        return NULL;
    }

    hashed = SHA256(data);

    // MUST FREE AFTER USING!
    free(data);

    return hashed;
}

// Decrypt a string.
char *decrypt(char *data)
{
    char *decrypted_pw = NULL;
    int strLength, i;

    if (data == NULL)
    {
        printf("ERROR: Passed empty data in decrypt().\n");
        return NULL;
    }

    strLength = strlen(data);

    if ((decrypted_pw = malloc(sizeof(char) * (strLength + 1))) == NULL)
        return NULL;

    // Displays and creates decrypted data.
    for (i = 0; i < strLength; i++)
    {
        decrypted_pw[i] = (pw[i] + 11);
        printf("%c", decrypted_pw[i]);
    }

    // End off decrypted return with null terminator.
    decrypted_pw[strLength] = '\0';

    printf("\n");

    // MUST FREE AFTER USING!
    return decrypted_pw;
}

// Takes in the encrypted password and a decrypted password.
// If decrypted password is encrypted and it matches, then
// user is granted access to lattice.
int checkPassword2(char *encryptedData, char *data)
{
    char *data_hash = NULL;

    if (encryptedData == NULL || data == NULL)
        return 0;

    // Hash password to get hash value of user input.
    // pw_hash = generateHash(pw);
    data_hash = generateHash(data);

    // Check if original password hash and user input hash match.
    if (strcmp(encryptedData, data_hash) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
*/

// Allow user to check their password.
int checkPassword(Lattice *l)
{
    char *pw_input = NULL;

    // Protects against incomplete lattice.
    if (l == NULL || l->layers == NULL || l->pathways == NULL)
    {
        fprintf(stderr, "ERROR: Must create lattice with \"new\" command to save data.\n");
        return LATTICE_ERR;
    }

    // Protects against incomplete password information.
    if (l->password == NULL || l->password->pw == NULL)
    {
        fprintf(stderr, "ERROR: No password exists in checkPassword().\n");
        return LATTICE_ERR;
    }

    if ((pw_input = getUserInput("Enter password: ")) == NULL)
    {
        fprintf(stderr, "ERROR: Memory error in checkPassword().\n");
        return LATTICE_ERR;
    }

    // Would also like to add their user ID, but I don't know how to get cookies from the user.
    if (strcmp(l->password->pw, pw_input) == 0)
    {
        free(pw_input);
        printf("Password match.\n");
        return LATTICE_OK;
    }
    else
    {
        free(pw_input);
        printf("Invalid login.\n");
        return LATTICE_ERR;
    }
}

// Allow user to change their password.
int changePassword(Lattice *l)
{
    char *new_pw = NULL, *new_pw_confirm = NULL, *pw_input = NULL;

    // Protects against incomplete lattice.
    if (l == NULL || l->layers == NULL || l->pathways == NULL)
    {
        fprintf(stderr, "ERROR: Must create lattice with \"new\" command to save data.\n");
        return LATTICE_ERR;
    }

    // Protects against incomplete password information.
    if (l->password == NULL || l->password->pw == NULL)
    {
        fprintf(stderr, "ERROR: No password exists in checkPassword().\n");
        return LATTICE_ERR;
    }

    if ((pw_input = getUserInput("Enter old password: ")) == NULL)
    {
        fprintf(stderr, "ERROR: Memory error in checkPassword().\n");
        return LATTICE_ERR;
    }

    // Checking if old password is correct.
    if (strcmp(l->password->pw, pw_input) == 0)
    {
        if ((new_pw = getUserInput("Enter new password: ")) == NULL)
        {
            free(pw_input);
            fprintf(stderr, "ERROR: Memory error in changePassword().\n");
            return LATTICE_ERR;
        }

        if ((new_pw_confirm = getUserInput("Confirm new password: ")) == NULL)
        {
            free(new_pw);
            free(pw_input);
            fprintf(stderr, "ERROR: Memory error in changePassword().\n");
            return LATTICE_ERR;
        }

        if (strcmp(new_pw, new_pw_confirm) == 0)
        {
            free(new_pw);
            free(pw_input);
            free(new_pw_confirm);

            // Get rid of old password address, make it point to new address.
            free(l->password->pw);
            l->password->pw = NULL;
            l->password->pw = new_pw;

            printf("Password successfully changed.\n");
            return LATTICE_OK;
        }
        else
        {
            free(new_pw);
            free(pw_input);
            free(new_pw_confirm);
            printf("Passwords don't match, try again.\n");
            return LATTICE_ERR;
        }
    }
    else
    {
        free(pw_input);
        fprintf(stderr, "ERROR: Invalid login in changePassword().\n");
        return LATTICE_ERR;
    }
}

// System interface functions:

// Print list of commands for menu-driven interface.
void printMenu(void)
{
    printf("\n");
    printf("Lattice allows you to manage sensitive data in a safe way.\n");
    printf("\n");
    printf("Lattice commands:\n");
    printf("\n");
    printf("  new <n>     -- create a new lattice with side lengths of <n>\n");
    printf("  destroy     -- completely destroy the current lattice structure\n");
    printf("  add <s> <n> -- add <s> (data) to file <n>\n");
    printf("  delete <n>  -- delete data held in file <n>\n");
    printf("  show <n>    -- show data held in file <n>\n");
    printf("  check       -- check if your password works\n");
    printf("  change      -- change your current password\n");
    // printf("  hint        -- gives you a hint of your password\n");
    printf("  usage       -- show whether there is data currently stored in lattice\n");
    printf("  print       -- display net 2D view of the current lattice cube\n");
    printf("  help        -- display list of commands\n");
    printf("  ?           -- display list of commands\n");
    printf("  exit        -- stop program\n");
}

int main(void)
{
    char buffer[512], data[512]; int n;
    Lattice *myLattice = NULL;

    printMenu();
    printf("\n");

    // Program runs until user enters "exit" or "stop".
    while (1)
    {
        printf("Enter a command ('?' // 'help' for help, or 'exit' to quit).\n");
        printf("%% ");

        scanf("%s", buffer);

        if (strcmp(buffer, "?") == 0)
        {
            printMenu();
        }

        if (strcmp(buffer, "help") == 0)
        {
            printMenu();
        }

        else if (strcmp(buffer, "new") == 0)
        {
            if (myLattice != NULL)
                destroyLattice(myLattice);

            scanf("%d", &n);

            printf("\n");

            if ((myLattice = createLattice(n)) == NULL)
                printf("Lattice failed to be created.\n");
            else
                printf("Lattice successfully created (use 'print' to view).\n");
        }

        else if (strcmp(buffer, "destroy") == 0)
        {
            if (myLattice != NULL)
                myLattice = destroyLattice(myLattice);
            else
                printf("No lattice exists.");
        }

        else if (strcmp(buffer, "add") == 0)
        {
            scanf("%s", data);
            scanf("%d", &n);

            if (addData(myLattice, data, n) == 1)
                printf("Data successfully added to file %d.\n", n);
            else
                printf("Something went wrong with addData().\n");
        }

        else if (strcmp(buffer, "delete") == 0)
        {
            scanf("%d", &n);

            if (deleteData(myLattice, n) == 1)
                printf("Data successfully deleted from file %d.\n", n);
            else
                printf("Something went wrong with deleteData().\n");
        }

        else if (strcmp(buffer, "show") == 0)
        {
            scanf("%d", &n);

            printf("\n");

            showData(myLattice, n);
        }

        else if (strcmp(buffer, "check") == 0)
        {
            if (myLattice == NULL)
                printf("You have not created a password.\n");

            checkPassword(myLattice);
        }

        else if (strcmp(buffer, "change") == 0)
        {
            if (myLattice == NULL)
                printf("Lattice does not exist.\n");

            if (changePassword(myLattice) == 1)
                printf("Password successfully changed.\n");
            else
                printf("Something went wrong with changePassword().\n");
        }

        /*
        else if (strcmp(buffer, "hint") == 0)
        {
            if (myLattice == NULL)
                printf("Lattice does not exist.");

            passwordHint(myLattice);
        }
        */

        else if (strcmp(buffer, "usage") == 0)
        {
            if (myLattice == NULL)
            {
                printf("Lattice does not exist.");
                continue;
            }

            if (isLatticeInUse(myLattice) == 1)
                printf("Lattice is currently protecting data.");
            else
                printf("Lattice is not currently protecting data.");
        }

        else if (strcmp(buffer, "print") == 0)
        {
            if (myLattice == NULL)
                printf("Lattice does not exist.");

            printf("\n");
            printLattice(myLattice);
        }

        else if (strcmp(buffer, "exit") == 0)
        {
            break;
        }
        printf("\n");
    }

    // Clear up lattice from memory.
    if (myLattice != NULL)
        destroyLattice(myLattice);

    return 0;
}
