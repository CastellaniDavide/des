/**
 * @file key.h
 * @author Davide Castellani (@DavideC03)
 * @brief
 */

#ifndef KEY
#define KEY

// Dependencies
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>

// Definitions
#define DEBUG

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
uint64_t generateKey();
char *getNewKey();
bool checkKey(uint64_t key);
uint64_t *getSubKeys(u_int64_t key);

// Functions
uint64_t generateKey()
{
    uint64_t key = 0;
    uint8_t checksum = 0;
    time_t t;

    // Initialize random
    srand((unsigned)time(&t));

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
    }

    return key;
}

char *getNewKey()
{
    uint64_t key = generateKey();
    uint8_t *ptr = (uint8_t *)&key;

    return base64_encode(ptr, 8);
}

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

uint64_t *getSubKeys(uint64_t key)
{
    uint64_t *keys = malloc(16 * sizeof(uint64_t));
    u_int32_t C = 0;
    u_int32_t D = 0;

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

    // printf("C0: %x\nD0: %x\n", C, D);

    for (size_t i = 0; i < 16; ++i)
    {
        // left shift of bitsRotation[i] bits
        for (size_t j = 0; j < bitsRotation[i]; ++j)
        {
            C = 0x0fffffff & (C << 1) | 0x00000001 & (C >> 27);
            D = 0x0fffffff & (D << 1) | 0x00000001 & (D >> 27);
        }

        
        //printf("C%d: %x\nD%d: %x\n", i + 1, C, i + 1, D);

        u_int64_t tmpKey = (((uint64_t)C) << 28) | (uint64_t)D; // C + D

        // Save key applying PC2 table
        keys[i] = 0;
        for (size_t j = 0; j < 48; ++j)
        {
            keys[i] <<= 1;
            keys[i] |= (tmpKey >> (56 - PC2[j])) & 1;
        }
        for(size_t j = 0; j < 48; ++j)
            printf((keys[i] >> j) & 1 ? "1": "0");
        printf("sk%ld\n", i + 1);

        // printf("Sub-key %ld: 0x%lx\n", i, keys[i]);
    }

    return keys;
}

#undef DEBUG

#endif // KEY