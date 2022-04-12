/**
 * @file binary.h
 * @author Davide Castellani (@DavideC03)
 * @brief
 */

#ifndef BINARY
#define BINARY

// Dependencies
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

// Definitions
//#define DEBUG

// Function declaration
char *getBinary(uint64_t input, int size, int group_size);

// Functions
/**
 * @brief Get the Binary object
 * 
 * @param input binary input
 * @param size size of input
 * @param group_size size of each group
 * @return char* char version of the input binary
 */
char *getBinary(uint64_t input, int size, int group_size)
{
    // Dependencies declaration
    char *output = malloc((size + (int)(size / group_size) + 1) * sizeof(char));
    size_t index = 0;

    // Get bit-bit
    for (size_t i = 0; i < size; ++i)
    {
        output[index++] = (input >> (size - i - 1)) & 1 ? '1' : '0';
        if ((i + 1) % group_size == 0)
            output[index++] = ' ';
    }

    // Final byte
    output[index] = '\0';

    // Return
    return output;
}

#undef DEBUG

#endif // BINARY