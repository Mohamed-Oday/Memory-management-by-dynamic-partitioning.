// Memory management by dynamic partitioning: Bit map
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <conio.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


// Our Program Structure
typedef struct Program
{
    char name[20];
    int size;
    int executionTime;
    char* Status;
    int units;
    int indexI;
    int indexJ;
    struct Program* next;
} Program;

// Our Memory Structure
typedef struct memory
{
    int size;
    int units;
    float externalFragmentation;

} memory;

// Our BitMap Structure
typedef struct BitMap
{
    char p_name[20];
    int start;
    int length;
    struct BitMap *next;
} BitMap;

// Our Global Variables
Program *head = NULL;
BitMap *bhead = NULL;


// controlling the input to be a number and prints wrong input if the input is not a number and not 0
int getValidNumber(char* message) {
    int number;
    char input[100];
    bool valid = false;
    do {
        printf("%s", message);
        scanf("%s", input);
        valid = true;
        for (int i = 0; i < strlen(input); i++) {
            if (!isdigit(input[i])) {
                valid = false;
                break;
            }
        }
        if (!valid) {
            printf("\033[1;31m"); // Error color
            printf("Wrong input. Please enter a number.\n");
            printf("\033[0m"); // Reset to default color
        }
    } while (!valid);
    number = atoi(input);
    return number;
}

// Creating Our Display Queue Function
void DisplayQueue(Program *head) {
    // Print header with table-like borders
    printf("Do you want to display the queue? (y/n): ");
    char responce;
    scanf(" %c", &responce);
    if (responce == 'n' || responce == 'N') {
        return;
    }
    printf("\033[1;35m"); // Header color
    printf("+---------------+--------------+----------------+------------------+-------+\n");
    printf("|                            The Programs Queue                            |\n");
    printf("+---------------+--------------+----------------+------------------+-------+\n");
    printf("| Program Name  | Program Size | Execution Time |      Status      | Units |\n");
    printf("+---------------+--------------+----------------+------------------+-------+\n");
    printf("\033[0m"); // Reset to default color

    if (head == NULL) {
        printf("\033[1;31m"); // Empty queue color
        printf("|                            The Queue is empty.                           |\n");
        printf("+---------------+--------------+----------------+------------------+-------+\n");
        printf("\033[0m"); // Reset to default color
    }
    Program *temp = head;
    while (temp != NULL) {
        // Print each row with table-like borders
        printf("\033[0;36m"); // Values color
        printf("| %-13s | %-11d  | %-14d | %-16s | %-5d |\n", temp->name, temp->size, temp->executionTime, temp->Status, temp->units);
        printf("+---------------+--------------+----------------+------------------+-------+\n");
        printf("\033[0m"); // Reset to default color
        temp = temp->next;
    }
    printf("\033[1;35m");
    printf("|                             End of The Queue                             |\n");
    printf("+---------------+--------------+----------------+------------------+-------+\n");
    printf("\033[0m");
}

// Calculating the number of units in the Program
void CalculatePUnits(Program* head, int unit_Size)
{
    Program* temp = head;
    while (temp != NULL)
    {
        if (temp->size % unit_Size == 0)
        {
            temp->units = temp->size / unit_Size;
        }
        else
        {
            temp->units = (temp->size / unit_Size) + 1;
        }
        temp = temp->next;
    }
}

// Creating Our Queue
Program* Queue(Program* head, int unit_Size)
{
    char responce;
    do
    {
        Program* newProgram = malloc(sizeof(Program));
        // our colored print statement to enter the program name and size and execution time
        printf("\033[0;32mEnter Program Name: \033[0m");
        scanf("%s", newProgram->name);
        newProgram->size = getValidNumber("\033[0;32mEnter Program Size: \033[0m");  
        newProgram->executionTime = getValidNumber("\033[0;32mEnter Execution Time: \033[0m");
        newProgram->Status = "Waiting";
        if (head == NULL)
        {
            head = newProgram;
            newProgram->next = NULL;
        }
        else
        {
            Program* temp = head;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = newProgram;
            newProgram->next = NULL;
        }
        printf("Do you want to add another program? (y/n): ");
        scanf(" %c", &responce);
    } while (responce == 'y' || responce == 'Y');

    CalculatePUnits(head, unit_Size);
    // Displaying the Queue
    DisplayQueue(head);
    return head;
}

// Calculating the number of units in the Memory
int CalculateMUnits(memory* Memory, int unit_Size)
{
    if (Memory->size % unit_Size == 0)
    {
        Memory->units = Memory->size / unit_Size;
    }
    else
    {
        Memory->units = (Memory->size / unit_Size) + 1;
    }
    // printing the number of units colored
    printf("\033[0;33m");
    printf("The number of units in the memory is: %d\n", Memory->units);
    printf("\033[0m");
    return Memory->units;
}

void DisplayBitTable(int number, char** BitTable)
{
    // ANSI escape codes for colors
    const char* RED = "\x1b[31m";
    const char* GREEN = "\x1b[32m";
    const char* YELLOW = "\x1b[33m";
    const char* BLUE = "\x1b[34m";
    const char* MAGENTA = "\x1b[35m";
    const char* CYAN = "\x1b[36m";
    const char* RESET = "\x1b[0m";

    printf("Bit Table:\n");
    printf("+");
    for (int i = 0; i < number; i++) {
        printf("---+");
    }
    printf("\n");

    for (int i = 0; i < number; i++) {
        printf("|");
        for (int j = 0; j < number; j++) {
            // Choose color based on the character
            const char* color;
            switch (BitTable[i][j]) {
                case '0': color = RED; break;
                case '1': color = GREEN; break;
                case 'X': color = YELLOW; break;
                default: color = RESET; break;
            }
            printf(" %s%c%s |", color, BitTable[i][j], RESET);
        }
        printf("\n+");
        for (int j = 0; j < number; j++) {
            printf("---+");
        }
        printf("\n");
    }
}

// Creating Our BitMap using First Fit Algorithm
int BitTableSize(memory* Memory)
{
    for (int i = 0 ; i < Memory->units ; i++)
    {
        if ((i * i) >= Memory->units)
        {
            return i;
        }
    }
    return -1;
}

char** bitTable(int tableSize, memory* Memory)
{
    char** BitTable = malloc(tableSize * sizeof(char*));
    for (int i = 0 ; i < tableSize ; i++)
    {
        BitTable[i] = malloc(tableSize * sizeof(char));
    }
    for (int i = 0 ; i < tableSize ; i++)
    {
        for (int j = 0 ; j < tableSize ; j++)
        {
            BitTable[i][j] = '0';
        }
    }
    if (Memory->units < tableSize * tableSize)
    {
        int extraUnits;
        int index = 0;
        for (int i = 0 ; i < tableSize ; i++)
        {
            for (int j = 0 ; j < tableSize ; j++)
            {
                index++;
                if (index > Memory->units)
                {
                    BitTable[i][j] = 'X';

                }
            }
        }
    }
    DisplayBitTable(tableSize, BitTable);
    return BitTable;
}

void displayMap(BitMap* head) {
    printf("Do you want to display the memory map? (y/n): ");
    char responce;
    scanf(" %c", &responce);
    if (responce == 'n' || responce == 'N') {
        return;
    }
    // Print header with dynamic spacing and colors
    printf("\033[1;35m"); // Header color
    printf("+---------------+------------+---------------+\n");
    printf("|               The Memory Map               |\n");
    printf("+---------------+------------+---------------+\n");
    printf("| Program Name  |    Start   |    Length     |\n");
    printf("+---------------+------------+---------------+\n");
    printf("\033[0m"); // Reset to default color

    BitMap *temp = head;
    if (temp == NULL) {
        printf("\033[1;31m"); // Empty memory color
        printf("|            The Memory is empty.            |\n");
        printf("+---------------+------------+---------------+\n");
        printf("\033[0m"); // Reset to default color
    } else {
        while (temp != NULL) {
            // Print each field with dynamic spacing and colors
            printf("\033[0;36m"); // Values color
            printf("| %-13s | %-10d | %-13d |\n", temp->p_name, temp->start, temp->length);
            printf("+---------------+------------+---------------+\n");
            printf("\033[0m"); // Reset to default color
            temp = temp->next;
        }
        printf("\033[1;35m");
        printf("|            End of The Memory Map           |\n");
        printf("+---------------+------------+---------------+\n");
        printf("\033[0m");
    }
}

void CombineBlock(BitMap* head)
{
    BitMap* temp = head; // creating a temp bit map to iterate through the bit map
    while (temp != NULL)
    {
        // combine any next to each other free blocks they could be a 2 or more
        while (temp->next != NULL && strcmp(temp->p_name, "Free") == 0 && strcmp(temp->next->p_name, "Free") == 0) // if the name of the program is free and the next name of the program is free
        {
            temp->length += temp->next->length;
            BitMap* temp2 = temp->next; 
            temp->next = temp2->next;
            free(temp2);
        }
        temp = temp->next;
    }
}

// creating our bitMap
BitMap* Map(Program* head, BitMap* bhead, memory* Memory)
{
    Program* temp = head; // creating a temp program to iterate through the queue
    while (temp != NULL)
    {
        if (temp->Status == "Running" || temp->Status == "Finished" || temp->Status == "OverFlow") // if the status of the program is running or finished
        {
            temp = temp->next; // move to the next node
        }
        else
        {
            break; // break the loop
        }
    }
    if (bhead == NULL) // if the bit map is null
    {
        BitMap* newBitMap = malloc(sizeof(BitMap)); // create a new bit map
        strcpy(newBitMap->p_name, "Free"); // set the name of the program to free
        newBitMap->start = 0; // set the start of the program to 0
        newBitMap->length = Memory->units; // set the length of the program to the units of the memory
        newBitMap->next = NULL; // set the next of the program to null
        bhead = newBitMap; // set the head of the bit map to the new bit map
    }
    BitMap* tempBitMap = bhead; // creating a temp bit map to iterate through the bit map
    BitMap* prevBitMap = NULL; // creating a prev bit map to keep track of the previous bit map node

    while (tempBitMap != NULL)
    {
        if (strcmp(tempBitMap->p_name, "Free") == 0) // if the name of the program is free
        {
            if (tempBitMap->length == temp->units) // if the length of the program is equal to the units of the program
            {
                strcpy(tempBitMap->p_name, temp->name); // set the name of the program to the name of the temp program
                break;
            }
            else if (tempBitMap->length > temp->units) // if the length of the program is greater than the units of the program
            {
                BitMap* newBitMap = malloc(sizeof(BitMap)); // create a new bit map node
                strcpy(newBitMap->p_name, temp->name); // set the name of the program to the name of the temp program
                newBitMap->start = tempBitMap->start; // set the start of the program to the start of the temp bit map
                newBitMap->length = temp->units; // set the length of the program to the units of the temp program
                newBitMap->next = tempBitMap; // set the next of the program to the temp bit map
                if (prevBitMap == NULL) // if the prev bit map is null
                {
                    bhead = newBitMap; // set the head of the bit map to the new bit map
                }
                else
                {
                    prevBitMap->next = newBitMap; // set the next of the prev bit map to the new bit map
                }
                tempBitMap->start += temp->units; // increase the start of the temp bit map by the units of the temp program
                tempBitMap->length -= temp->units; // decrease the length of the temp bit map by the units of the temp program
                break; // break the loop
            }
        }
        prevBitMap = tempBitMap; // set the prev bit map to the temp bit map
        tempBitMap = tempBitMap->next; // move to the next node
    }
    return bhead;
}



// First Fit Algorithm
void FirstFit(Program* head, memory* Memory, char** BitTable, int tableSize)
{
    Program* temp = head; // creating a temp program to iterate through the queue
    while (temp != NULL) // while the temp program is not null
    {
        int tempUnits = temp->units; // get the units of the program
        int counter = 0; // counter to count the number of units
        int indexI = 0; // index I of the program
        int indexJ = 0; // index J of the program
        if (temp->Status == "Finished" || temp->Status == "Running") // if the status of the program is finished or running
        {
            temp = temp->next; // move to the next node
            continue; // skip the current iteration
        }
        if (temp->Status == "Waiting" || temp->Status == "OverFlow") // if the status of the program is waiting
        {
            for (int i = 0 ; i < tableSize ; i++)
            {
                for (int j = 0 ; j < tableSize ; j++)
                {
                    if (BitTable[i][j] == '0') // if the bit table is 0
                    {
                        indexI = i; // get the index I
                        indexJ = j; // get the index J
                        int x = indexI; // temp index I
                        int y = indexJ; // temp index J
                        while (BitTable[x][y] == '0' && counter < tempUnits) // while the bit table is 0 and the counter is less than the units of the program
                        {
                            counter++; // increase the counter
                            y++; // increase the index J
                            if (y == tableSize) // if the index J is equal to the table size
                            {
                                x++; // increase the index I
                                y = 0; // set the index J to 0
                            }
                        }
                        if (counter < tempUnits) // if the counter is less than the units of the program
                        {
                            temp->Status = "OverFlow";
                            counter = 0; // set the counter to 0
                        }
                        else if (counter == tempUnits)
                        {
                            while (tempUnits != 0) // while the units of the program is not 0
                            {
                                temp->indexI = indexI; // set the index I of the program
                                temp->indexJ = indexJ; // set the index J of the program
                                for (int k = indexI ; k < tableSize ; k++)
                                {
                                    for (int l = indexJ ; l < tableSize ; l++)
                                    {
                                        BitTable[k][l] = '1'; // update the bit table
                                        tempUnits--; // decrease the units of the program
                                        if (tempUnits == 0) // if the units of the program is 0
                                        {
                                            break; // break the loop
                                        }
                                    }
                                    if (tempUnits == 0) // if the units of the program is 0
                                    {
                                        break; // break the loop
                                    }
                                    indexJ = 0; // set the index J to 0
                                }
                            temp->Status = "Allocated"; // change the status of the program to allocated
                            bhead = Map(head, bhead, Memory);
                            break;
                            }
                        }
                    }
                }
            }
        }
        if (strcmp(temp->Status, "Allocated") == 0) // if the status of the program is allocated
        {
            temp->Status = "Running"; // change the status of the program to running
            Memory->externalFragmentation -= (float)temp->size / Memory->size * 100; // update the external fragmentation
            printf("\033[0;32m");
            printf("The program %s has been allocated\n", temp->name);
            printf("\033[0m");

        }
        else if (strcmp(temp->Status, "OverFlow") == 0) // if the status of the program is waiting
        {
            printf("\033[0;31m");
            printf("The program %s has not been allocated duo to not enough space\n", temp->name);
            printf("\033[0m");
        }
        else if (strcmp(temp->Status, "Finished") == 0) // if the status of the program is finished
        {
            printf("\033[0;31m");
            printf("The program %s has been finished\n", temp->name);
            printf("\033[0m");
        }
        else if (strcmp(temp->Status, "Running") == 0) // if the status of the program is running
        {
            printf("\033[0;31m");
            printf("The program %s is already running\n", temp->name);
            printf("\033[0m");
        }
        temp = temp->next; // move to the next node
    }
    // colored print
    printf("\033[0;33m");
    printf("The external fragmentation rate is: %.2f%%\n", Memory->externalFragmentation);
    printf("\033[0m");

    DisplayBitTable(tableSize, BitTable);
    DisplayQueue(head);
    displayMap(bhead);


}

void Free(Program* head, memory* Memory, char** BitTable, int tableSize, BitMap* bhead)
{
    char name[100]; // the name of the program
    printf("Enter the name of the program: "); // asking the user to enter the name of the program
    scanf("%s", name); // reading the name of the program
    Program* temp = head; // creating a temp program to iterate through the queue
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0 && temp->Status == "Running") // if the name of the program is found
        {
            if (temp->Status == "Running")
            {
                temp->Status = "Finished";
            }
            else
            {
                break;
            }
            int indexI = temp->indexI; // get the index I of the program
            int indexJ = temp->indexJ; // get the index J of the program
            int tempUnits = temp->units; // get the units of the program
            for (int i = indexI ; i < tableSize ; i++)
            {
                for (int j = indexJ ; j < tableSize ; j++)
                {
                    BitTable[i][j] = '0'; // update the bit table
                    tempUnits--; // decrease the units of the program
                    if (tempUnits == 0) // if the units of the program is 0
                    {
                        break; // break the loop
                    }
                }
                if (tempUnits == 0) // if the units of the program is 0
                {
                    break; // break the loop
                }
                indexJ = 0;
            }
            // updating the bit map
            BitMap* tempBitMap = bhead; // creating a temp bit map to iterate through the bit map
            while (tempBitMap != NULL) // while the temp bit map is not null
            {
                if (strcmp(tempBitMap->p_name, name) == 0) // if the name of the program is found
                {
                    strcpy(tempBitMap->p_name, "Free"); // change the name of the program to free
                    break; // break the loop
                }
                tempBitMap = tempBitMap->next; // move to the next node
            }
            Memory->externalFragmentation += (float)temp->size / Memory->size * 100; // update the external fragmentation
            printf("\033[0;32m");
            printf("The program %s has been deallocated\n", temp->name);
            printf("\033[0m");
            break; // break the loop
        }
        temp = temp->next; // move to the next node
    }
    printf("\033[0;33m");
    printf("The external fragmentation rate is: %.2f%%\n", Memory->externalFragmentation);
    printf("\033[0m");

    DisplayBitTable(tableSize, BitTable);
    DisplayQueue(head);
    CombineBlock(bhead);
    displayMap(bhead);
}


int main()
{
    printf(ANSI_COLOR_GREEN "Welcome to the Memory Management System!" ANSI_COLOR_RESET "\n");
    memory* Memory = malloc(sizeof(memory));
    int unit_Size;
    Memory->size = getValidNumber("Enter the size of the memory: ");
    unit_Size = getValidNumber("Enter the allocation unit size: ");
    Memory->externalFragmentation = (float)Memory->size / Memory->size * 100;
    printf("Thank you for entering the memory size and the allocation unit size\n");
    Memory->units = CalculateMUnits(Memory, unit_Size);
    int tableSize = BitTableSize(Memory);
    char** BitTable = bitTable(tableSize, Memory);
    int choice = -1;
    do
    {
        // Our Menu : Add Program, Allocate Program , Deallocate Program, Exit colored
        printf("\033[0;35m"); // Setting the color to purple for the title
        printf("+--------+-----------------------------+\n");
        printf("|       Memory Management System       |\n");
        printf("+--------+-----------------------------+\n");
        printf("\033[0m"); // Resetting the color for the table headers
        printf("\033[0;36m"); // Setting the color to cyan for the table headers
        printf("| Option |         Description         |\n");
        printf("+--------+-----------------------------+\n");
        printf("| 1      | Add Program to the Queue    |\n");
        printf("| 2      | Allocate Program            |\n");
        printf("| 3      | Deallocate Program          |\n");
        printf("| 4      | Exit                        |\n");
        printf("+--------+-----------------------------+\n");
        printf("\033[0m"); // Resetting the color for the table rows
        choice = getValidNumber("Enter your choice: ");
        switch (choice)
        {
        case 1:
            head = Queue(head, unit_Size);
            break;
        case 2:
            FirstFit(head, Memory, BitTable, tableSize);
            break;
        case 3:
            Free(head, Memory, BitTable, tableSize, bhead);
            break;
        case 4:
            printf(ANSI_COLOR_RED "Thank you for using the Memory Management System. Goodbye!" ANSI_COLOR_RESET "\n");
            exit(0);
            break;
        default:
            printf("Invalid Choice\n");
            break;
        }
    } while (choice != 4);


}
