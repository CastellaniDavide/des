/**
 * @file key.h
 * @author Davide Castellani (@DavideC03)
 * @brief Manage keys
 */

#ifndef KEY
#define KEY

// Dependencies
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>
#include "binary.h"

// Definitions
//#define DEBUG

// Variabiles
size_t bitsRotation[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
size_t PC1[56] = {
    // Left
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    // Right
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4};
size_t PC2[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32};

// Function declaration
uint64_t generateKey(bool verbose);
char *getNewKey(bool verbose);
bool checkKey(uint64_t key);
uint64_t *getSubKeys(u_int64_t key, bool verbose);

// Functions
/**
 * @brief Generate a new key
 *
 * @param verbose verbose
 * @return uint64_t key
 */
uint64_t generateKey(bool verbose)
{
    // Dependencies declaration
    uint64_t key = 0;
    uint8_t checksum = 0;
    time_t t;

    // Initialize random
    srand((unsigned)time(&t));

    // Print
    if (verbose)
        printf("⚙️ Generating key:\n");

    // Create key
    for (size_t i = 0; i < 8; ++i)
    {
        uint8_t tmp = rand() % 128;
        bool checksum = 0;

        for (size_t j = 0; j < 7; ++j)
            checksum ^= (tmp >> i) & 1;

        tmp <<= 1;
        tmp |= checksum;

        key <<= 8;
        key |= (uint64_t)tmp;

        // Print
        if (verbose)
            printf("\tPart %ld: %02x with checksum: %d\n", i, tmp, checksum);
    }

    // Print
    if (verbose)
        printf("\tKey: %lx\n", key);

    // Return
    return key;
}

/**
 * @brief Get the New Key object
 *
 * @param verbose verbose
 * @return char* printable key
 */
char *getNewKey(bool verbose)
{
    uint64_t key = generateKey(verbose);
    uint8_t *ptr = (uint8_t *)&key;

    return base64_encode(ptr, 8, false);
}

/**
 * @brief Check the key
 *
 * @param key the key to check
 * @return true key is valid
 * @return false key is damaged
 */
bool checkKey(uint64_t key)
{
    uint8_t *ptr = (uint8_t *)&key;

    for (size_t i = 0; i < 8; ++i)
    {
        bool checksum = 0;
        for (size_t j = 0; j < 8; ++j)
            checksum ^= (ptr[i] >> i) & 1;

        if (checksum)
            return false;
    }

    return true;
}

/**
 * @brief Get the Sub Keys object
 *
 * @param key key
 * @param verbose verbose
 * @return uint64_t* sub-keys
 */
uint64_t *getSubKeys(uint64_t key, bool verbose)
{
    // Dependencies declaration
    uint64_t *keys = malloc(16 * sizeof(uint64_t));
    u_int32_t C = 0;
    u_int32_t D = 0;

    // Print
    if (verbose)
        printf("\tGenerating sub-keys:\n");

    // Permutation table 1 left (C) and right(D)
    for (size_t i = 0; i < 56 / 2; ++i)
    {
        D <<= 1;
        D |= (key >> PC1[i]) & 1;
    }

    for (size_t i = 56 / 2; i < 56; ++i)
    {
        C <<= 1;
        C |= (key >> PC1[i]) & 1;
    }

    // Print
    if (verbose)
        printf("\t\tInitial sub-key parts:\n\t\t\tC: %s\n\t\t\tD: %s\n",
               getBinary(C, 32, 8),
               getBinary(D, 32, 8));

    for (size_t i = 0; i < 16; ++i)
    {
        // left shift of bitsRotation[i] bits
        for (size_t j = 0; j < bitsRotation[i]; ++j)
        {
            C = 0x0fffffff & (C << 1) | 0x00000001 & (C >> 27);
            D = 0x0fffffff & (D << 1) | 0x00000001 & (D >> 27);
        }

        // Print
        if (verbose)
            printf("\t\tSub-key %ld:\n\t\tC: %s\n\t\t\tD: %s\n",
                i + 1,
                getBinary(C, 32, 8),
                getBinary(D, 32, 8));

        u_int64_t tmpKey = (((uint64_t)C) << 28) | (uint64_t)D; // C + D

        // Save key applying PC2 table
        keys[i] = 0;
        for (size_t j = 0; j < 48; ++j)
        {
            keys[i] <<= 1;
            keys[i] |= (tmpKey >> (56 - PC2[j])) & 1;
        }

        // Print
        if (verbose)
            printf("\t\t\tsub-key: %s\n", getBinary(keys[i], 48, 8));
    }

    return keys;
}

#undef DEBUG

#endif // KEY