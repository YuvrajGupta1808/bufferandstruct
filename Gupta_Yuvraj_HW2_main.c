/**********************
* Class:  CSC-415-01 Summer 2024
* Name: Yuvraj Gupta
* Student ID: 922933190
* GitHub Name: YuvrajGupta1808
* Project: Assignment 2 – Command Line Arguments
*
* File: Gupta_Yuvraj_HW2_main.c
*
* Description: This file implements the main function for Assignment 2,
* handling command line arguments, buffering, and structure population.
*
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assignment2.h"

int main(int argc, char *argv[])
{
    //Check if the user provides the correct number of command line arguments includes
    //firstname, lastname and message.
    if (argc < 4)
    {
        fprintf(stderr, "Usage: %s <FirstName> <LastName> <Message>\n", argv[0]);
        return 1;
    }


    // Step 4 STARTS

    /*
    Allocate memory for the personalInfo structure to store user details.
    Dynamic allocation allows for flexible memory usage, but we need to ensure we free 
    this memory later to prevent leaks.
    */
    struct personalInfo *pInfo = (personalInfo *)malloc(sizeof(struct personalInfo));
    if (!pInfo)
    {
        perror("Failed to allocate memory for personalInfo");
        return 1;
    }

    /*
    Allocate memory for the first, last name and copy the command-line argument 
    into the allocated space.
    The Dynamic allocation ensures the exact amount of memory needed is used.
    */
    pInfo->firstName = (char *)malloc(strlen(argv[1]) + 1);
    if (!pInfo->firstName)
    {
        perror("Failed to allocate memory for first name");
        free(pInfo);
        return 1;
    }
    strcpy(pInfo->firstName, argv[1]);

    pInfo->lastName = (char *)malloc(strlen(argv[2]) + 1);
    if (!pInfo->lastName)
    {
        perror("Failed to allocate memory for last name");
        free(pInfo->firstName);
        free(pInfo);
        return 1;
    }
    strcpy(pInfo->lastName, argv[2]);

    pInfo->studentID = 922933190; 
    pInfo->level = JUNIOR;        

    // This method allows multiple values in a single field.
    pInfo->languages = KNOWLEDGE_OF_C | KNOWLEDGE_OF_JAVA | KNOWLEDGE_OF_PYTHON;

    // Copy the user-provided message into the structure, ensuring it fits within the allocated space.
    strncpy(pInfo->message, argv[3], sizeof(pInfo->message) - 1);


    // Step 5: STARTS

    /*
    To write the personalInfo data using the provided function and check for success.
    Proper error handling ensures that we can identify and respond to issues during execution.
    */
    if (writePersonalInfo(pInfo) != 0)
    {
        fprintf(stderr, "Failed to write personal info\n");
        free(pInfo->firstName);
        free(pInfo->lastName);
        free(pInfo);
        return 1;
    }


    // Step 6 STARTS

    /*
    Allocate a buffer to hold strings temporarily as they are read from an input source.
    Using a fixed buffer size helps manage memory efficiently and simplifies the logic
    for processing data in blocks.
    */
    char *block = (char *)malloc(BLOCK_SIZE);
    if (!block)
    {
        perror("Failed to allocate block");
        free(pInfo->firstName);
        free(pInfo->lastName);
        free(pInfo);
        return 1;
    }
 
    const char *data;
    // Initialize offset variable that keep track of current position in buffer. 
    size_t offset = 0; 

    /*
    Continuously read strings using the getNext function until no more strings are available.
    This loop ensures that we process all input strings, committing the buffer whenever it 
    becomes full.
    */
    while ((data = getNext()) != NULL)
    {
        size_t len = strlen(data);
        size_t space_left = BLOCK_SIZE - offset;

        // Check if the data length exceeds the remaining space in the block
        if (len > space_left)
        {
            /*
            If the data does not fit, copy only the part that fits into the block
            Commit the filled block to process it
            Adjust the data pointer and length to handle the remaining data
            */
            memcpy(block + offset, data, space_left);
            commitBlock(block);
            offset = 0;

            data += space_left;
            len -= space_left;
        }

        // Copy any remaining data into the block
        // This happens either if the entire data fits initially or after the partial copy
        memcpy(block + offset, data, len);
        offset += len;
    }

    // Commit any remaining data in the block so that no data is lost.
    if (offset > 0)
    {
        commitBlock(block);
    }
    // Free the buffer memory to prevent leaks.
    free(block);


    // Step 7 STARTS
    // Perform a final check using the provided function and return its result as the program's exit status.
    int check_result = checkIt();
    printf("Check result: %d\n", check_result); // Debug output

    // Free the dynamically allocated memory for the personalInfo structure to prevent memory leaks.
    free(pInfo->firstName);
    free(pInfo->lastName);
    free(pInfo);
    return check_result;
}