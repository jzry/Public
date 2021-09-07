// COPYRIGHT 2021 @JZRY, ALL RIGHTS RESERVED. DO NOT REPLICATE.

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEBUG 0

// =================================================
// Helper functions
// =================================================

typedef struct Node
{
    int digit;
    struct Node *next;
} Node;

typedef struct List
{
    // The head of a linked list that holds the digits
    // of an integer, stored in reverse order.
    Node *head;

    // Number of digits in the integer (which is equal
    // to the number of nodes in the list).
    int length;
} List;

// Print contents of a linked list in order as is.
void print_list(Node *head)
{
    Node *temp = NULL;

    if (head == NULL)
    {
        printf("(empty list)\n");
        return;
    }

    for (temp = head; temp != NULL; temp = temp->next)
    {
        printf("%d%c", temp->digit, (temp->next == NULL) ? '\n' : ' ');
    }
}

// Create node.
Node *create_node(int data)
{
    Node *node = NULL;

    if ((node = malloc(sizeof(Node))) == NULL)
    {
        // Malloc error.
        return NULL;
    }

    node->digit = data;
    node->next = NULL;

    return node;
}

// Iterate through list & insert at tail. O(n) runtime.
Node *tail_insert(Node *head, int data)
{
    Node *temp = NULL;

    // Tail is head.
    if (head == NULL)
    {
        return create_node(data);
    }

    temp = head;

    // Create a pointer to point to last node's next.
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    // Insert element at end of linked list.
    temp->next = create_node(data);

    return head;
}

// NOT IN USE.
// Insert at head. O(1) runtime.
List *head_insert_char(List *list, char c)
{
    Node *temp = NULL;
    int data;

    if (list == NULL)
    {
        return NULL;
    }

    // Transfer ascii value to corresponding integer.
    data = atoi(&c);

    // Debugging purposes.
    if (DEBUG == 1)
    {
        printf("printing in head_insert_char:\n");
        printf("%d\n", data);
    }

    // Head doesn't exist.
    if (list->head == NULL)
    {
        if ((list->head = create_node(data)) == NULL)
        {
            // Malloc error.
            return NULL;
        }

        return list;
    }

    // Create the new head node.
    if ((temp = create_node(data)) == NULL)
    {
        // Malloc error.
        return NULL;
    }

    // Set new head node next pointer to point to the old head address.
    temp->next = list->head;

    // Set real list pointer to point to the new linked list head.
    list->head = temp;

    return list;
}

// Insert at head. O(1) runtime.
List *head_insert(List *list, int data)
{
    Node *temp = NULL;

    if (list == NULL)
    {
        return NULL;
    }

    // Debugging purposes.
    if (DEBUG == 1)
    {
        printf("head_insert(): %d\n", data);
    }

    // Head doesn't exist.
    if (list->head == NULL)
    {
        if ((list->head = create_node(data)) == NULL)
        {
            // Malloc error.
            return NULL;
        }

        return list;
    }

    // Create the new head node.
    if ((temp = create_node(data)) == NULL)
    {
        // Malloc error.
        return NULL;
    }

    // Set new head node next pointer to point to the old head address.
    temp->next = list->head;

    // Set real list pointer to point to the new linked list head.
    list->head = temp;

    return list;
}

// Destroys list recursively. Gatekeeper is destroy_list().
Node *destroy_list_helper(Node *head)
{
    if (head == NULL)
    {
        return NULL;
    }

    destroy_list_helper(head->next);
    free(head);

    return NULL;
}

// Destroys list recursively. Gatekeeper for destroy_list_helper().
Node *destroy_list(Node *head)
{
    if (head == NULL)
    {
        return NULL;
    }

    destroy_list_helper(head);

    return NULL;
}

// Return new linked list pointer.
List *create_list(void)
{
    return calloc(1, sizeof(List));
}

// =================================================
// End of helper functions
// =================================================

// Return the greatest number passed.
int max(int a, int b)
{
    return (a >= b) ? a : b;
}

// Add two values represented by linked lists together to get a sum linked list.
List *listAdd(List *p, List *q)
{
    List *sum = NULL;
    Node *temp_p = NULL, *temp_q = NULL, *temp_sum = NULL;
    int data, modded_data, i, longest, carry = 0;

    if (p == NULL || q == NULL)
    {
        return NULL;
    }

    if ((sum = create_list()) == NULL)
    {
        // Malloc error.
        return NULL;
    }

    if (DEBUG == 1)
    {
        printf("p->length in listAdd(): %d\n", p->length);
        printf("q->length in listAdd(): %d\n", q->length);
    }

    // Return the longest length between the two.
    longest = max(p->length, q->length);

    // Form the first head for sum->head

    // Get the first integer of the linked list.
    data = (p->head->digit + q->head->digit);

    // Mod data to only get the final digit of data.
    modded_data = (data % 10);

    // Place modded data in the new linked list.
    if ((sum->head = tail_insert(sum->head, modded_data)) == NULL)
    {
        // Malloc error.
        sum = destroyList(sum);

        return NULL;
    }

    // Get the carry of the data (only possible carry is 0 or 1).
    carry = (data / 10);

    // Temp variables to loop with.
    temp_sum = sum->head;
    temp_p = p->head->next;
    temp_q = q->head->next;

    if (DEBUG == 1)
    {
        printf("\nmodded_data in listAdd(): %d\n", modded_data);
        printf("printing sum list in listAdd() before loop:\n");
        print_list(sum->head);
        printf("entering listAdd loop:\n");
    }

    // Add each node head in linked list (skipped if p and q are both length of 1).
    for (i = 1; i < longest; i++)
    {
        if (temp_p != NULL && temp_q != NULL)
        {
            data = (temp_p->digit + temp_q->digit + carry);
            // temp_p = temp_p->next;
            // temp_q = temp_q->next;

            if (DEBUG == 1)
            {
                printf("1) data in listAdd: %d\n", data);
            }
        }

        if (temp_p != NULL && temp_q == NULL)
        {
            // Temp_q has to be 0 from now on.
            data = (temp_p->digit + carry);
            // temp_p = temp_p->next;

            if (DEBUG == 1)
            {
                printf("2) data in listAdd: %d\n", data);
            }
        }

        if (temp_p == NULL && temp_q != NULL)
        {
            // Temp_p has to be 0 from now on.
            data = (temp_q->digit + carry);
            // temp_q = temp_q->next;

            if (DEBUG == 1)
            {
                printf("3) data in listAdd: %d\n", data);
            }
        }

        if (temp_p == NULL && temp_q == NULL)
        {
            data = carry;

            if (DEBUG == 1)
            {
                printf("4) data in listAdd: %d\n", data);
            }
        }

        // Move to the next digit in linked list p without segmentation fault.
        if (temp_p != NULL)
        {
            temp_p = temp_p->next;
        }

        // Move to the next digit in linked list q without segmentation fault.
        if (temp_q != NULL)
        {
            temp_q = temp_q->next;
        }

        // What we put into each node
        modded_data = (data % 10);

        // Place data into tail.
        if ((sum->head = tail_insert(sum->head, modded_data)) == NULL)
        {
            // Malloc error.
            sum = destroyList(sum);

            return NULL;
        }

        // New node added, increment length.
        sum->length++;

        // Update carry for next loop or to check on the final loop
        carry = (data / 10);

        if (DEBUG == 1)
        {
            printf("\nprinting sum list in listAdd():\n");
            print_list(sum->head);
        }
    }

    // If there is a leftover carry, make the tail node hold a 1 value.
    if (carry == 1)
    {
        // Attach node onto the end of new sum linked list.
        if ((sum->head = tail_insert(sum->head, 1)) == NULL)
        {
            // Malloc error.

            // Free linked list malloc.
            sum = destroyList(sum);

            return NULL;
        }

        // Increase the length of the linked list.
        sum->length++;
    }

    return sum;
}

// Destroy a linked list.
List *destroyList(List *listy)
{
    if (listy == NULL)
    {
        return NULL;
    }

    // Should free all nodes from memory andset list->head to NULL.
    if (listy->head != NULL)
    {
        listy->head = destroy_list(listy->head);
    }

    // Free list from memory.
    free(listy);
    listy = NULL;

    return NULL;
}

// Convert number from string format to linked list format in O(n) runtime.
List *stringToList(char *str)
{
    List *list = NULL;
    int str_len, i, data;

    if (str == NULL)
    {
        return NULL;
    }

    // Get length of string.
    str_len = strlen(str);

    // Initialize linked list struct pointer.
    if ((list = create_list()) == NULL)
    {
        // Malloc error.
        return NULL;
    }

    // Convert each character in string to integer and
    // insert each integer into linked list in reverse order.
    for (i = 0; i < str_len; i++)
    {
        // Head insert into data.
        if ((list = head_insert_char(list, str[i])) == NULL)
        {
            // Malloc error.
            return NULL;
        }
    }

    // The amount of nodes in the linked list is equal to the length of the string.
    list->length = str_len;

    return list;
}

// Convert characters into integers. Assuming only integers 0 - 9 are passed.
char convert_to_char(int data)
{
    return (data + '0');
}

// Convert integer represented by linked list into a string in O(n) runtime.
char *listToString(List *listy)
{
    List *list_copy = NULL;
    char *list_string = NULL;
    int count, data, i;
    Node *temp = NULL;

    // Unsure if listy->head should be tested too.
    if (listy == NULL || listy->head == NULL)
    {
        return NULL;
    }

    // Make new malloc for linked list string.
    if ((list_string = malloc(sizeof(char) * (listy->length + 1))) == NULL)
    {
        // Malloc error.
        return NULL;
    }

    // Iterate through the linked list with a temp variable and stick data into string array in reverse.
    for (i = listy->length - 1, temp = listy->head; temp != NULL; temp = temp->next, i--)
    {
        data = temp->digit;
        list_string[i] = convert_to_char(data);

        if (DEBUG == 1)
        {
            printf("inserting %d to listy_string[%d]\n", data, i);
        }
    }

    // Print string.
    if (DEBUG == 1)
    {
        printf("list_string in listToString(): %s\n", list_string);
    }

    // End the new string array with a null sentinal.
    list_string[listy->length] = '\0';

    return list_string;
}

// Return the amount of numbers in an unsigned integer.
int counter(unsigned int n)
{
    int count = 0;

    // Increment count in each iteration. Cut unsigned int by 1 digit each time.
    while (n > 0)
    {
        n /= 10;
        count++;
    }

    return count;
}

// Get the reversed unsigned int in the form of an integer array.
int *get_reversed_unsigned(int count, unsigned int n)
{
    int *reversed_n = NULL, i;

    if ((reversed_n = malloc(sizeof(int) * count)) == NULL)
    {
        // Malloc error.
        return NULL;
    }

    // 1234 -> [4][3][2][1]

    // Put numbers into reversed list.
    for (i = 0; i < count; i++)
    {
        // Put the last number inside of the reversed_n.
        reversed_n[i] = n % 10;
        n /= 10;
    }

    return reversed_n;
}

// Convert an unsigned int to a linked list.
List *uintToList(unsigned int n)
{
    List *list = NULL;
    int *reversed_n = NULL;
    int i, count;
    unsigned int num;

    if ((list = create_list()) == NULL)
    {
        // Malloc error.
        return NULL;
    }

    // Base case. If n is 0.
    if (n == 0)
    {
        // Insert at head.
        if ((list->head = create_node(0)) == NULL)
        {
            // Malloc error.

            // Free list from memory.
            free(list);

            return NULL;
        }

        // Increment length of linked list by one.
        list->length++;

        return list;
    }

    // Find the amount of numbers in the unsigned integer.
    count = counter(n);

    // Set the length of the list equal to the count.
    list->length = count;

    // Get reversed n.
    if ((reversed_n = get_reversed_unsigned(count, n)) == NULL)
    {
        // Mallor error.
        return NULL;
    }

    // Loop through the amount of numbers in n and insert each number into a linked list.
    for (i = list->length - 1; i >= 0; i--)
    {
        // Insert data into head of linked list.
        if ((list = head_insert(list, reversed_n[i])) == NULL)
        {
            // Malloc error.

            // Free reversed unsigned array from memory.
            free(reversed_n);

            // Free linked list from memory.
            free(list);

            return NULL;
        }

        if (DEBUG == 1)
        {
            printf("in uintToList: %d\n", reversed_n[i]);
        }
    }

    // Free reversed unsigned array from memory.
    free(reversed_n);
    reversed_n = NULL;

    return list;
}

// Recursive power function. x is base, y is exponent.
int power(int x, int y)
{
    return (y == 0) ? 1 : (x * power(x, y - 1));
}

// Convert the linked list number representation to an unsigned integer.
unsigned int *listToUint(List *listy)
{
    unsigned int *temp_array = NULL, *n = NULL;
    unsigned int sum = 0;
    Node *temp = NULL;
    int i, j;

    if (listy == NULL)
    {
        return NULL;
    }

    if (listy->length > counter(UINT_MAX))
    {
        // If number of digits in the linked list exceeds INT_MAX, terminate function.
        // The reason for this is so we're not forcing a number
        // into n that exceeds C's unsigned int amount, which will cause issues.

        return NULL;
    }

    // Malloc an unsigned int to place the value of the linked list.
    if ((n = malloc(sizeof(unsigned int))) == NULL)
    {
        // Malloc error.
        return NULL;
    }

    // Malloc a temporary unsigned int list for summation.
    if ((temp_array = malloc(sizeof(unsigned int) * listy->length)) == NULL)
    {
        // Malloc error.
        return NULL;
    }

    // Iterate through the linked list, take each digit out and put it into an array.
    for (i = listy->length - 1, temp = listy->head; i >= 0; i--, temp = temp->next)
    {
        // Put the number in the temporary int array.
        temp_array[i] = temp->digit;

        if (DEBUG == 1)
        {
            printf("putting %u in temp_array[%d] listToUint()\n", temp_array[i], i);
        }
    }

    // Creating multiples for each digit.
    for (i = listy->length - 1, j = 0; i >= 0; i--, j++)
    {
        temp_array[i] = (temp_array[i] * power(10, j));

        if (DEBUG == 1)
        {
            printf("getting multiple for %u with temp_array[%d] listToUint()\n", temp_array[i], i);
        }
    }

    // Sum corresponding decimal to create the whole unsigned integer.
    for (i = 0; i < listy->length; i++)
    {
        sum += temp_array[i];

        if (DEBUG == 1)
        {
            printf("sum in temp_array[%d]: %u\n", i, temp_array[i]);
        }
    }

    if (DEBUG == 1)
    {
        printf("final sum: %u\n", sum);
    }

    // Set the sum to the actual unsigned int value.
    *n = sum;

    if (DEBUG == 1)
    {
        printf("printing *n: %u\n", *n);
    }

    // Free the temporary array.
    free(temp_array);
    temp_array = NULL;

    return n;
}

// Make a linked list representing the number 1.
List *make_one_linkedlist(void)
{
    List *one = NULL;

    // Make a linked list containing one.
    if ((one = create_list()) == NULL)
    {
        // Malloc error.
        return NULL;
    }

    // Make a linked list containing one.
    if ((one = head_insert(one, 1)) == NULL)
    {
        // Malloc error.
        return NULL;
    }

    one->length++;

    return one;
}

// Increment the value represented by a linked list by one.
void plusPlus(List *listy)
{
    Node *temp = NULL;
    int data;

    if (listy == NULL)
    {
        return;
    }

    // Make a temporary variable to loop with.
    temp = listy->head;

    // Get data in case of malloc failure.
    data = listy->head->digit;

    // Increment digit at first node (last number).
    listy->head->digit++;

    // If the number in first node is now 10, each number in linked list
    // becomes 0 and a 1 is added to tail.
    if (listy->head->digit == 10)
    {
        // As long as the node exists, replace digit with one.
        while (temp != NULL)
        {
            temp->digit = 0;

            // Move temporary variable to next node.
            temp = temp->next;
        }

        // Add a one node at the tail of the linked list.
        if ((listy->head = tail_insert(listy->head, 1)) == NULL)
        {
            listy->head->digit--;

            // Malloc error.
            return;
        }
    }
}

List *clone(List *p, List *q)
{
    // Copies information from p to q, frees q
    // p = F(n-2), q = F(n-1)
    p->head = q->head;
    p->length = q->length;
    free(q);
    return p;
}

List *fib(unsigned int n)
{
    List *fib = NULL, *fib1 = NULL, *fib2 = NULL, *fib3 = NULL;
    unsigned int i;

    // Returns F(0) or F(1)
    if (n < 2)
    {
        fib = calloc(1, sizeof(List));
        if (fib == NULL)
            return NULL;
        fib->head = calloc(1, sizeof(List));
        if (fib->head == NULL)
            return NULL;
        fib->length++;
        fib->head->digit = n;
        return fib;
    }

    // Setting up first two conditions outside of loop F(0) = 0 F(1) = 1
    fib1 = calloc(1, sizeof(List));

    if (fib1 == NULL)
    {
        return NULL;
    }

    fib2 = calloc(1, sizeof(List));
    if (fib2 == NULL)
    {
        return NULL;
    }

    fib1->length++;
    fib1->head = create_node(0);

    fib2->length++;
    fib2->head = create_node(1);

    // Looping for F(N), starts at 3rd term since first 2 terms are set
    for (i = 2; i <= n; i++)
    {
        // Set a pointer to fib3, increment length
        fib3 = listAdd(fib1, fib2);
        fib3->length++;

        // Destroy fib1, transfer fib2->fib1 to be new fib1, destroy fib2
        destroyList(fib1);
        fib1 = malloc(sizeof(List));
        if (fib1 == NULL)
            return NULL;
        fib1 = clone(fib1, fib2); // fib1 == fib2

        // Transfer fib3->fib2 to be new fib2, destroy fib2
        fib2 = malloc(sizeof(List));
        if (fib2 == NULL)
            return NULL;
        fib2 = clone(fib2, fib3); // fib3 == fib2
    }

    // Frees fib2, fib1, just in case
    if (fib2 != NULL)
    {
        fib2 = destroyList(fib2);
    }

    if (fib1 != NULL)
    {
        fib1 = destroyList(fib2);
    }

    return fib3;
}
