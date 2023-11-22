#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

struct node
{
    int data;
    struct node *prev;
    struct node *next;
};
struct node *head = NULL;

struct node **lists = NULL; // Array of pointers to the linked lists
int listCount = 0;          // Number of linked lists

struct node **mulList = NULL;
int mulCount = 0;

void insert_at_beginning(int data)
{
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = head;

    if (head != NULL)
    {
        head->prev = new_node;
    }

    head = new_node;
}
void insert_at_beginning101(struct node **head, int data)
{
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = *head;

    if (*head != NULL)
    {
        (*head)->prev = new_node;
    }

    *head = new_node;
}

void insert_at_end(int data)
{
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = NULL;

    if (head == NULL)
    {
        head = new_node;
    }
    else
    {
        struct node *current = head;
        while (current->next != NULL)
        {
            current = current->next;
        }

        current->next = new_node;
        new_node->prev = current;
    }
}

void insert_after_node(struct node *node, int data)
{
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->data = data;
    new_node->prev = node;
    new_node->next = node->next;

    if (node->next != NULL)
    {
        node->next->prev = new_node;
    }

    node->next = new_node;
}

void delete_node(struct node **head, struct node *node)
{
    if (*head == NULL || node == NULL)
        return;

    if (*head == node)
        *head = node->next;

    if (node->next != NULL)
        node->next->prev = node->prev;

    if (node->prev != NULL)
        node->prev->next = node->next;

    free(node);
}

void print_list(struct node *list)
{
    struct node *current = list;
    while (current != NULL)
    {
        printf("%d ", current->data);
        current = current->next;
    }

    printf("\n");
}
int readFile()
{
    FILE *file = fopen("data.txt", "r");

    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    char line[100]; // Assuming maximum line length of 100 characters

    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Process the line and create a new linked list for each line
        line[strcspn(line, "\n")] = '\0'; // Remove newline character if present

        struct node *list = NULL; // Linked list for the current line
        int isNegative = 0;       // Flag to indicate negative number
        int startIndex = 0;       // Index to start processing digits

        // Check if the line starts with a negative sign
        if (line[0] == '-')
        {
            isNegative = 1;
            startIndex = 1;
        }

        // Split the line into individual digits and create the linked list
        for (int i = startIndex; line[i] != '\0'; i++)
        {
            if (isdigit(line[i]))
            {
                int digit = line[i] - '0'; // Convert character digit to integer
                if (isNegative)
                {
                    digit = -digit; // Apply negative sign if necessary
                    isNegative = 0; // Reset the flag
                }
                insert_at_end(digit);
            }
        }

        // Add the new linked list to the array
        lists = realloc(lists, (listCount + 1) * sizeof(struct node *));
        lists[listCount++] = head;
        head = NULL;
    }

    fclose(file);

    return 0;
}

void isEmpty()
{
    if (lists == NULL)
    {
        readFile(); // Linked list is empty
        // printf("emptyyy");
    }
    // Linked list is not empty
}
void writeToFile(struct node *result)
{
    FILE *file = fopen("result.txt", "a");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    struct node *current = result;
    while (current != NULL)
    {
        fprintf(file, "%d", current->data);
        current = current->next;
    }

    fprintf(file, "\n");
    fclose(file);
    printf("Result has been written to result.txt.\n");
}

void displayMenu()
{

    // displays the main menu
    printf("##Welcome to The Main Menu##\n");
    printf("Please Select an Operation (1-6):\n");
    printf("1-Find Addition\n");
    printf("2-Find Subtraction\n");
    printf("3-Find Division\n");
    printf("4-Find Multiplication\n");
    printf("5-Display numbers\n");
    printf("6-EXIT\n");
}
void displayLists(struct node **lists, int listCount)
{
    if (listCount == 0)
    {
        printf("No lists to display.\n");
        return;
    }

    printf("--- Lists ---\n");
    for (int i = 0; i < listCount; i++)
    {
        printf("List %d: ", i + 1);
        print_list(lists[i]);
    }
}
int getListLength(struct node *list)
{
    int length = 0;
    while (list != NULL)
    {
        length++;
        list = list->next;
    }
    return length;
}
void addition(struct node **lists, int listCount)
{
    struct node *currentNodes[listCount]; // Array to store the current node of each list
    struct node *addition = NULL;
    int carry = 0;

    // Initialize current nodes to the last node of each list
    for (int i = 0; i < listCount; i++)
    {
        currentNodes[i] = lists[i];

        // Traverse to the last node of the list
        while (currentNodes[i] != NULL && currentNodes[i]->next != NULL)
        {
            currentNodes[i] = currentNodes[i]->next;
        }
    }

    int maxLength = 0; // Length of the longest list

    // Find the length of the longest list
    for (int i = 0; i < listCount; i++)
    {
        int length = getListLength(lists[i]);
        if (length > maxLength)
        {
            maxLength = length;
        }
    }

    // Traverse the lists from the last node to the first node
    for (int position = 0; position < maxLength; position++)
    {
        int sum = carry; // Sum of the nodes at the current position

        // Traverse the current node of each list
        for (int i = 0; i < listCount; i++)
        {

            struct node *current = currentNodes[i];

            // Move to the previous node if it exists, otherwise consider the node as zero

            if (current != NULL)
            {
                sum = sum + current->data;

                currentNodes[i] = current->prev;
            }
        }
        if (sum >= 10)
        {
            carry = sum / 10;
            sum = sum % 10;
        }
        else if (sum < 10)
        {
            carry = 0;
        }

        insert_at_beginning101(&addition, sum);
    }
    // Check if carry still has a number

    if (carry > 0)
    {
        insert_at_beginning101(&addition, carry);
    }

    print_list(addition);
    writeToFile(addition);

    // Clean up the memory allocated for the result
    struct node *current = addition;
    while (current != NULL)
    {
        struct node *temp = current;
        current = current->next;
        free(temp);
    }

    // printf("\n");
}

struct node *getBiggerNumber(struct node *list1, struct node *list2)
{
    struct node *current1 = list1;
    struct node *current2 = list2;

    // Skip leading zeros
    while (current1->next != NULL && current1->data == 0)
    {
        current1 = current1->next;
    }

    while (current2->next != NULL && current2->data == 0)
    {
        current2 = current2->next;
    }

    // Compare lengths
    int length1 = 0;
    int length2 = 0;

    while (current1 != NULL)
    {
        length1++;
        current1 = current1->next;
    }

    while (current2 != NULL)
    {
        length2++;
        current2 = current2->next;
    }

    if (length1 > length2)
    {
        return list1;
    }
    else if (length1 < length2)
    {
        return list2;
    }

    // Compare digits
    current1 = list1;
    current2 = list2;

    while (current1 != NULL && current2 != NULL)
    {
        if (current1->data > current2->data)
        {
            return list1;
        }
        else if (current1->data < current2->data)
        {
            return list2;
        }

        current1 = current1->next;
        current2 = current2->next;
    }

    // Lists are equal
    return list1; // Return list1 by default
}
void subtraction(struct node **lists, int listCount)
{
    int borrow = 0;
    struct node *currentFirst = lists[0];
    struct node *currentSecond = lists[1];
    struct node *subresult = NULL;

    // Find the longest/bigger number
    struct node *longestNumber = getBiggerNumber(currentFirst, currentSecond);

    // Traverse to the last node of the longest number
    while (currentFirst->next != NULL)
    {
        currentFirst = currentFirst->next;
    }
    while (currentSecond->next != NULL)
    {
        currentSecond = currentSecond->next;
    }

    int maxLength = getListLength(longestNumber);

    // Adjust the order of subtraction based on the input lists
    if (longestNumber != lists[0])
    {
        // Swap currentFirst and currentSecond
        struct node *temp = currentFirst;
        currentFirst = currentSecond;
        currentSecond = temp;
    }

    for (int position = 0; position < maxLength; position++)
    {
        int diff = borrow;

        if (currentFirst != NULL)
        {
            diff += currentFirst->data;
            currentFirst = currentFirst->prev;
        }

        if (currentSecond != NULL)
        {
            diff -= currentSecond->data;
            currentSecond = currentSecond->prev;
        }

        if (diff < 0)
        {
            borrow = -1;
            diff += 10;
        }
        else
        {
            borrow = 0;
        }

        insert_at_beginning101(&subresult, diff);
    }

    // Adjust the sign based on the input lists order
    if (longestNumber != lists[0])
    {
        // Apply the negative sign
        struct node *current = subresult;

        current->data *= -1;
    }

    // Remove leading zeros from the result
    while (subresult->next != NULL && subresult->data == 0)
    {
        struct node *temp = subresult;
        subresult = subresult->next;
        subresult->prev = NULL;
        free(temp);
    }

    print_list(subresult);
    writeToFile(subresult);

    // Clean up the memory allocated for the result
    struct node *current = subresult;
    while (current != NULL)
    {
        struct node *temp = current;
        current = current->next;
        free(temp);
    }
}

void mul1(struct node **lists, int listCount)
{

    struct node *currentFirst = lists[0];
    struct node *currentSecond = lists[1];
    int carry;

    // Traverse to the last node of the first list
    while (currentFirst->next != NULL)
    {
        currentFirst = currentFirst->next;
    }

    // Traverse to the last node of the second list
    while (currentSecond->next != NULL)
    {
        currentSecond = currentSecond->next;
    }

    // Traverse the first list
    while (currentFirst != NULL)
    {
        carry = 0;
        struct node *lastNodeFirst = currentFirst;
        struct node *lastNodeSecond = currentSecond;

        struct node *ans = NULL; // linked list for current line
        // Traverse the second list for each node in the first list
        while (currentSecond != NULL)
        {

            int mul = lastNodeFirst->data * currentSecond->data;
            mul += carry;

            if (mul >= 10)
            {
                carry = mul / 10;
                mul = mul % 10;
            }
            else
            {
                carry = 0;
            }

            insert_at_beginning(mul);
            currentSecond = currentSecond->prev;
        }
        // Check if carry still has a number
        if (carry > 0)
        {
            // Add carry to the result list
            insert_at_beginning(carry);
        }

        // // Add zeros based on the position in mulList
        for (int i = 0; i < mulCount; i++)
        {
            insert_at_end(0);
        }

        // Add the new linked list to the array
        mulList = realloc(mulList, (mulCount + 1) * sizeof(struct node *));
        mulList[mulCount++] = head;
        head = NULL;

        // Reset the current node of the second list to the head for the next iteration
        currentSecond = lastNodeSecond;
        currentFirst = currentFirst->prev;
    }
    displayLists(mulList, mulCount);

    addition(mulList, mulCount);
    //   writeToFile(addition);

    // displayLists(mulList, mulCount);
}
void sub()
{
    int list1_count = 0;
    int list2_count = 0;
    int longest_linkedlist = 0;

    struct node *currentNodes[2]; // Array to store the current node of each list
    struct node *result = NULL;
    int borrow = 0;

    // Count the number of nodes in each list
    while (lists[0]->next != NULL)
    {
        lists[0] = lists[0]->next;
        list1_count++;
    }

    while (lists[1]->next != NULL)
    {
        lists[1] = lists[1]->next;
        list2_count++;
    }

    // Determine the length of the longest list
    if (list1_count > list2_count)
    {
        longest_linkedlist = list1_count;
    }
    else if (list1_count < list2_count)
    {
        longest_linkedlist = list2_count;
    }
    else if (list1_count == list2_count)
    {
        longest_linkedlist = list1_count;
    }

    // Initialize current nodes to the last node of each list
    currentNodes[0] = lists[0];
    currentNodes[1] = lists[1];
    while (currentNodes[0] != NULL && currentNodes[0]->next != NULL)
    {
        currentNodes[0] = currentNodes[0]->next;
    }
    while (currentNodes[1] != NULL && currentNodes[1]->next != NULL)
    {
        currentNodes[1] = currentNodes[1]->next;
    }

    // Traverse the lists from the last node to the first node
    for (int position = 0; position < longest_linkedlist; position++)
    {
        int diff = borrow; // Difference of the nodes at the current position

        // Traverse the current node of each list
        for (int i = 0; i < 2; i++)
        {
            struct node *current = currentNodes[i];

            // Move to the previous node if it exists, otherwise consider the node as zero
            if (current != NULL)
            {
                diff = diff - current->data;
                currentNodes[i] = current->prev;
            }
        }

        if (diff < 0)
        {
            borrow = -1;
            diff += 10;
        }
        else
        {
            borrow = 0;
        }

        insert_at_beginning101(&result, diff);
    }

    // Remove leading zeros from the result
    while (result->next != NULL && result->data == 0)
    {
        struct node *temp = result;
        result = result->next;
        result->prev = NULL;
        free(temp);
    }

    print_list(result);
    writeToFile(result);

    // Clean up the memory allocated for the result
    struct node *current = result;
    while (current != NULL)
    {
        struct node *temp = current;
        current = current->next;
        free(temp);
    }
}

void printList(struct node *list)

{
    struct node *current = list;
    while (current != NULL)
    {
        printf("%d ", current->data);
        current = current->next;
    }

    printf("\n");
}

// void insert_at_end101(struct node **head, int data)
// {
//     struct node *new_node = create_node(data);

//     if (*head == NULL)
//     {
//         *head = new_node;
//     }
//     else
//     {
//         struct node *current = *head;
//         while (current->next != NULL)
//         {
//             current = current->next;
//         }

//         current->next = new_node;
//         new_node->prev = current;
//     }
// }

// struct node *create_node(int data) {
//     struct node *newNode = (struct node *)malloc(sizeof(struct node));
//     newNode->data = data;
//     newNode->prev = NULL;
//     newNode->next = NULL;
//     return newNode;
// }

// void division(struct node **lists, int listCount, struct node **quotient)
// {
//     struct node *currentFirst = lists[0];
//     struct node *currentSecond = lists[1];
//     int borrow = 0;

//     // Find the length of the second list
//     int secondListLength = get_length(currentSecond); // Assuming you have a function to get the length of a linked list

//     // Traverse the first list
//     while (currentFirst != NULL && currentSecond != NULL)
//     {
//         borrow = 0;
//         struct node *subresult = NULL; // Linked list to store intermediate division result

//         // Divide the same number of nodes from the first list over the second list
//         int length = secondListLength;
//         while (length > 0 && currentFirst != NULL)
//         {
//             // Insert the current node from the first list into the subresult linked list
//             insert_at_end101(&subresult, currentFirst->data);

//             // Move to the next node in the first list
//             currentFirst = currentFirst->next;
//             length--;
//         }

//         // Multiply the subresult by the remainder
//         int remainder = 0; // The remainder obtained from the previous division
//         struct node *currentSubresult = subresult;

//         while (currentSubresult != NULL)
//         {
//             int product = currentSubresult->data * remainder;
//             currentSubresult->data = product % 10;
//             remainder = product / 10;

//             currentSubresult = currentSubresult->next;
//         }

//         // Join the multiplied subresult with the next node at the end of the current node from the first list
//         if (currentFirst != NULL)
//         {
//             insert_at_end101(&currentFirst->next, remainder);
//         }
//         else
//         {
//             struct node *newNode = create_node(remainder); // Assuming you have a function to create a new node
//             insert_at_end101(&lists[0], newNode->data);    // Join the new node with the existing first list
//         }

//         // Divide the subresult by the current node from the second list
//         currentSubresult = subresult;
//         int divisor = currentSecond->data;

//         while (currentSubresult != NULL)
//         {
//             int quotient_digit = (currentSubresult->data + borrow) / divisor;
//             borrow = (currentSubresult->data + borrow) % divisor;

//             currentSubresult->data = quotient_digit;
//             currentSubresult = currentSubresult->next;
//         }

//         // Insert the quotient digit into the quotient linked list
//         insert_at_beginning101(quotient, subresult->data);

//         // Clean up subresult if necessary

//         // Move to the next node in the second list
//         currentSecond = currentSecond->next;
//     }
// }
int main()
{

    int x = -1;
    struct node *selectedLists[] = {NULL}; // Array to store the selected lists
    int maxListIndex = listCount;

    do
    {
        displayMenu();
        printf("Please enter your operation:\n");
        scanf("%d", &x);
        switch (x)
        {

        case 1:
            isEmpty();
            printf("Addition\n");
            displayLists(lists, listCount);
            if (listCount >= 1)
            {
                int numListsSelected;
                printf("Enter the number of lists to select (maximum %d): ", listCount);
                scanf("%d", &numListsSelected);

                if (numListsSelected >= 1 && numListsSelected <= listCount)
                {
                    printf("Enter the indices of the lists to select: ");
                    int selectedIndices[numListsSelected];
                    for (int i = 0; i < numListsSelected; i++)
                    {
                        scanf("%d", &selectedIndices[i]);
                    }

                    bool validIndices = true;
                    for (int i = 0; i < numListsSelected; i++)
                    {
                        int listIndex = selectedIndices[i];
                        if (listIndex < 1 || listIndex > listCount)
                        {
                            validIndices = false;
                            break;
                        }

                        selectedLists[i] = lists[listIndex - 1];
                    }

                    if (validIndices)
                    {

                        printf("Result of addition: ");

                        addition(selectedLists, numListsSelected);
                    }
                    else
                    {
                        printf("Invalid list index. Please try again.\n");
                    }
                }
                else
                {
                    printf("Invalid number of lists to select. Please try again.\n");
                }
            }
            else
            {
                printf("Insufficient number of lists to perform addition.\n");
            }
            break;

        case 2:
            isEmpty();

            printf("Subtraction\n");
            displayLists(lists, listCount);
            if (listCount >= 1)
            {
                int numListsSelected;
                printf("Enter the number of lists to select (maximum %d): ", listCount);
                scanf("%d", &numListsSelected);

                if (numListsSelected >= 1 && numListsSelected <= listCount)
                {
                    printf("Enter the indices of the lists to select: ");
                    int selectedIndices[numListsSelected];
                    for (int i = 0; i < numListsSelected; i++)
                    {
                        scanf("%d", &selectedIndices[i]);
                    }

                    bool validIndices = true;
                    for (int i = 0; i < numListsSelected; i++)
                    {
                        int listIndex = selectedIndices[i];
                        if (listIndex < 1 || listIndex > listCount)
                        {
                            validIndices = false;
                            break;
                        }

                        selectedLists[i] = lists[listIndex - 1];
                    }

                    if (validIndices)
                    {

                        printf("Subtraction Result");
                        subtraction(selectedLists, numListsSelected);
                    }
                    else
                    {
                        printf("Invalid list index. Please try again.\n");
                    }
                }
                else
                {
                    printf("Invalid number of lists to select. Please try again.\n");
                }
            }
            else
            {
                printf("Insufficient number of lists to perform addition.\n");
            }
            break;
        case 3:
            isEmpty();

            printf("Division\n");
            displayLists(lists, listCount);
            if (listCount >= 1)
            {
                int numListsSelected;
                printf("Enter the number of lists to select (maximum %d): ", listCount);
                scanf("%d", &numListsSelected);

                if (numListsSelected >= 1 && numListsSelected <= listCount)
                {
                    printf("Enter the indices of the lists to select: ");
                    int selectedIndices[numListsSelected];
                    for (int i = 0; i < numListsSelected; i++)
                    {
                        scanf("%d", &selectedIndices[i]);
                    }

                    bool validIndices = true;
                    for (int i = 0; i < numListsSelected; i++)
                    {
                        int listIndex = selectedIndices[i];
                        if (listIndex < 1 || listIndex > listCount)
                        {
                            validIndices = false;
                            break;
                        }

                        selectedLists[i] = lists[listIndex - 1];
                    }

                    if (validIndices)
                    {

                        // printf("Subtraction Result");
                        // struct node *quotient = NULL;
                        // division(selectedLists, numListsSelected, &quotient);
                        // printf("Quotient: ");
                        // struct node *current = quotient;

                        // while (current != NULL)
                        // {
                        //     printf("%d ", current->data);
                        //     current = current->next;
                        // }

                        // printf("\n");

                        // division(selectedLists, numListsSelected);
                        // Free the memory allocated for the linked lists
                        for (int i = 0; i < mulCount; i++)
                        {
                            struct node *current = mulList[i];
                            while (current != NULL)
                            {
                                struct node *temp = current;
                                current = current->next;
                                free(temp);
                            }
                        }
                        free(mulList);
                    }
                    else
                    {
                        printf("Invalid list index. Please try again.\n");
                    }
                }
                else
                {
                    printf("Invalid number of lists to select. Please try again.\n");
                }
            }
            else
            {
                printf("Insufficient number of lists to perform addition.\n");
            }

            break;

        case 4:

            isEmpty();

            printf("Mul\n");
            displayLists(lists, listCount);
            if (listCount >= 1)
            {
                int numListsSelected;
                printf("Enter the number of lists to select (maximum %d): ", listCount);
                scanf("%d", &numListsSelected);

                if (numListsSelected >= 1 && numListsSelected <= listCount)
                {
                    printf("Enter the indices of the lists to select: ");
                    int selectedIndices[numListsSelected];
                    for (int i = 0; i < numListsSelected; i++)
                    {
                        scanf("%d", &selectedIndices[i]);
                    }

                    bool validIndices = true;
                    for (int i = 0; i < numListsSelected; i++)
                    {
                        int listIndex = selectedIndices[i];
                        if (listIndex < 1 || listIndex > listCount)
                        {
                            validIndices = false;
                            break;
                        }

                        selectedLists[i] = lists[listIndex - 1];
                    }

                    if (validIndices)
                    {
                        printf("Multiplication Result");
                        mul1(selectedLists, numListsSelected);

                        // Free the memory allocated for the linked lists
                        for (int i = 0; i < mulCount; i++)
                        {
                            struct node *current = mulList[i];
                            while (current != NULL)
                            {
                                struct node *temp = current;
                                current = current->next;
                                free(temp);
                            }
                        }
                        free(mulList);
                    }
                    else
                    {
                        printf("Invalid list index. Please try again.\n");
                    }
                }
                else
                {
                    printf("Invalid number of lists to select. Please try again.\n");
                }
            }
            else
            {
                printf("Insufficient number of lists to perform addition.\n");
            }
            break;

        case 5:
            isEmpty();
            printf("print\n");
            displayLists(lists, listCount);

            break;

        case 6:

            printf("Exiting Program.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");

            break;
        }
    } while (x != 6);

    // Free the memory allocated for the linked lists
    for (int i = 0; i < listCount; i++)
    {
        struct node *current = lists[i];
        while (current != NULL)
        {
            struct node *temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(lists);

    return 0;
}
