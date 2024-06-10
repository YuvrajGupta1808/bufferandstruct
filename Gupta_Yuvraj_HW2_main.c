/**********************
* Class:  CSC-415-02 Summer 2024
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
    if (argc < 4)
    {
        fprintf(stderr, "Usage: %s <FirstName> <LastName> <Message>\n", argv[0]);
        return 1;
    }

    // Step 4: Allocate and populate the personalInfo structure
    personalInfo *pInfo = (personalInfo *)malloc(sizeof(personalInfo));
    if (!pInfo)
    {
        perror("Failed to allocate memory for personalInfo");
        return 1;
    }

    // Allocate and copy first name
    pInfo->firstName = (char *)malloc(strlen(argv[1]) + 1);
    if (!pInfo->firstName)
    {
        perror("Failed to allocate memory for first name");
        free(pInfo);
        return 1;
    }
    strcpy(pInfo->firstName, argv[1]);

    // Allocate and copy last name
    pInfo->lastName = (char *)malloc(strlen(argv[2]) + 1);
    if (!pInfo->lastName)
    {
        perror("Failed to allocate memory for last name");
        free(pInfo->firstName);
        free(pInfo);
        return 1;
    }
    strcpy(pInfo->lastName, argv[2]);

    // Set student ID and level
    pInfo->studentID = 922933190; // Replace with your actual student ID
    pInfo->level = JUNIOR;        // Set appropriately

    // Set languages field (update this to include all languages you know)
    pInfo->languages = KNOWLEDGE_OF_C | KNOWLEDGE_OF_JAVA | KNOWLEDGE_OF_PYTHON;

    // Copy the message
    strncpy(pInfo->message, argv[3], sizeof(pInfo->message) - 1);

    // Step 5: Write personal information
    if (writePersonalInfo(pInfo) != 0)
    {
        fprintf(stderr, "Failed to write personal info\n");
        free(pInfo->firstName);
        free(pInfo->lastName);
        free(pInfo);
        return 1;
    }

    // Step 6: Buffer data into blocks
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
size_t offset = 0;

while ((data = getNext()) != NULL)
{
    size_t len = strlen(data);
    size_t space_left = BLOCK_SIZE - offset;

    if (len > space_left)
    {
        // Partial copy and commit the block
        memcpy(block + offset, data, space_left);
        commitBlock(block);
        offset = 0;

        // Handle remaining part of the data
        data += space_left;
        len -= space_left;
    }

    // Copy remaining data into the buffer
    memcpy(block + offset, data, len);
    offset += len;

}

// Commit any remaining data in the block
if (offset > 0)
{
    commitBlock(block);
}

free(block);
    // Step 7: Call checkIt and exit
    int check_result = checkIt();
    printf("Check result: %d\n", check_result); // Debug output
        free(pInfo->firstName);
    free(pInfo->lastName);
    free(pInfo);
    return check_result;
}
