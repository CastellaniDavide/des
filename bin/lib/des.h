/**
 * @file des.h
 * @author Davide Castellani (@DavideC03)
 * @brief
 */

#ifndef DES
#define DES

// Dependencies
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>
#include "key.h"
#include "binary.h"

// Definitions
//#define DEBUG

// Variabiles
uint8_t IP_schema[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7};
uint8_t FP_schema[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25};
uint8_t E[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1};
uint8_t S[8][4][16] = {{// S1
                        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
                        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
                        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
                        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
                       {// S2
                        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
                        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
                        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
                        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
                       {// S3
                        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
                        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
                        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
                        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
                       {// S4
                        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
                        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
                        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
                        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
                       {// S5
                        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
                        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
                        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
                        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
                       {// S6
                        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
                        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
                        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
                        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
                       {// S7
                        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
                        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
                        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
                        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
                       {// S8
                        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
                        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
                        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
                        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}};
size_t P[32] = {16, 7, 20, 21, 29, 12, 28, 17,
                1, 15, 23, 26, 5, 18, 31, 10,
                2, 8, 24, 14, 32, 27, 3, 9,
                19, 13, 30, 6, 22, 11, 4, 25};

// Function declaration
uint64_t des(uint64_t input, uint64_t key, bool encrypt, bool verbose);
uint64_t IP(uint64_t input, bool verbose);
uint64_t FP(uint64_t input, bool verbose);
uint64_t rounds(uint64_t message, uint64_t *keys, bool encrypt, bool verbose);
uint32_t feistel(uint32_t input, uint64_t *keys, size_t round, bool encrypt, bool verbose);
uint64_t expansion(uint32_t input);

// Functions
/**
 * @brief des
 *
 * @param input input message
 * @param key key
 * @param encrypt true if encrypt, false if decrypt
 * @param verbose verbose
 * @return uint64_t output message
 */
uint64_t des(uint64_t input, uint64_t key, bool encrypt, bool verbose)
{
    // Print
    if (verbose)
        printf("⚙️ DES:\n\tOriginal message: %s\n", getBinary(input, 64, 8));

    // IP
    uint64_t IP_ = IP(input, verbose);
    if (verbose)
        printf("\tAfter IP: %s\n", getBinary(IP_, 64, 8));

    // Rounds
    uint64_t rounds_ = rounds(IP_, getSubKeys(key, verbose), encrypt, verbose);

    // IP
    uint64_t FP_ = FP(rounds_, verbose);
    if (verbose)
        printf("\tAfter FP: %s\n", getBinary(FP_, 64, 8));

    // Return
    return FP_;
}

/**
 * @brief Initial Permutation
 *
 * @param input input message
 * @param verbose verbose
 * @return uint64_t output message
 */
uint64_t IP(uint64_t input, bool verbose)
{
    uint64_t output = 0;

    for (size_t i = 0; i < 64; ++i)
    {
        output <<= 1;
        output |= (input >> (64 - IP_schema[i])) & 1;
    }

    return output;
}

/**
 * @brief Final Permutation
 *
 * @param input input message
 * @param verbose verbose
 * @return uint64_t output message
 */
uint64_t FP(uint64_t input, bool verbose)
{
    uint64_t output = 0;

    for (size_t i = 0; i < 64; ++i)
    {
        output <<= 1;
        output |= (input >> 64 - FP_schema[i]) & 1;
    }

    return output;
}

/**
 * @brief 16 Rounds
 *
 * @param message input message
 * @param keys 16 keys
 * @param encrypt encrypt/ decrypt
 * @param verbose verbose
 * @return uint64_t message after the 16 rounds
 */
uint64_t rounds(uint64_t message, uint64_t *keys, bool encrypt, bool verbose)
{
    // Initial split
    uint32_t L = (uint32_t)(message >> 32) & UINT32_MAX;
    uint32_t R = (uint32_t)message & UINT32_MAX;

    // Print
    if (verbose)
        printf("\tRounds:\n\t\tInitial:\n\t\t\tL0: %s\n\t\t\tR0: %s\n",
               getBinary(L, 32, 8),
               getBinary(R, 32, 8));

    for (size_t i = 0; i < 16; ++i)
    {
        // Print
        if (verbose)
            printf("\t\tRound %ld:\n", i + 1);

        // Do a round
        uint32_t tmp = R;
        R = L ^ feistel(R, keys, i, encrypt, verbose);
        L = tmp;

        // Print
        if (verbose)
            printf("\t\t\tL%ld: %s\n\t\t\tR%ld: %s\n",
                   i + 1,
                   getBinary(L, 32, 8),
                   i + 1,
                   getBinary(R, 32, 8));
    }

    return (((uint64_t)R) << 32) | (uint64_t)L;
}

/**
 * @brief Feistel
 *
 * @param input input
 * @param keys 16 sub-keys
 * @param round n° of the round
 * @param encrypt encrypt/ decrypt
 * @param verbose verbose
 * @return uint32_t feistel output
 */
uint32_t feistel(uint32_t input, uint64_t *keys, size_t round, bool encrypt, bool verbose)
{
    // Print
    if (verbose)
        printf("\t\t\tFeistel:\n\t\t\t\tInitial: %s\n",
               getBinary(input, 32, 8));

    // Expand right part of the message
    uint64_t input2 = expansion(input);
    if (verbose)
        printf("\t\t\t\tAfter expansion: %s\n",
               getBinary(input2, 48, 8));

    // Get the key
    uint64_t key = (encrypt ? keys[round] : keys[15 - round]);
    if (verbose)
        printf("\t\t\t\tKey: %s\n",
               getBinary(key, 48, 8));

    // key XOR input2
    uint64_t keyXORinput2 = key ^ input2;
    if (verbose)
        printf("\t\t\t\tKey XOR after expansion: %s\n",
               getBinary(keyXORinput2, 48, 8));

    // Sbox
    uint32_t sbox = 0;
#ifdef DEBUG
    if (verbose)
        printf("\t\t\t\tSbox:\n");
#endif // DEBUG
    for (size_t i = 0; i < 8; ++i)
    {
        // Get 6bit piece
        uint8_t piece = (uint8_t)(keyXORinput2 >> (6 * (8 - i - 1))) & 0b111111;
#ifdef DEBUG
        if (verbose)
            printf("\t\t\t\t\tPart %ld:\n\t\t\t\t\t\tpiece: %s\n",
                   i + 1,
                   getBinary(piece, 6, 8));
#endif // DEBUG

        // Get row ?xxxx?
        uint8_t row = (piece & 0b1) | ((piece & 100000) >> 4);
#ifdef DEBUG
        if (verbose)
            printf("\t\t\t\t\t\trow: %s\n",
                   getBinary(row, 2, 8));
#endif // DEBUG

        // Get column x????x
        uint8_t column = (piece & 0b011110) >> 1;
#ifdef DEBUG
        if (verbose)
            printf("\t\t\t\t\t\tcolumn: %s\n",
                   getBinary(column, 4, 8));
#endif // DEBUG

        // Get value
        uint8_t value = S[i][row][column];
#ifdef DEBUG
        if (verbose)
            printf("\t\t\t\t\t\tnew value: %s\n",
                   getBinary(value, 4, 8));
#endif // DEBUG

        // Set value into output;
        sbox <<= 4;
        sbox |= (uint32_t)value;
    }
    if (verbose)
        printf("\t\t\t\tAfter sbox: %s\n",
               getBinary(sbox, 32, 8));

    // Final permutation
    uint32_t output = 0;
    for (size_t i = 0; i < 32; ++i)
    {
        output <<= 1;
        output |= (sbox >> (32 - P[i])) & 1;
    }
    if (verbose)
        printf("\t\t\t\tAfter final permutation: %s\n",
               getBinary(output, 32, 8));

    return output;
}

/**
 * @brief Expansion
 *
 * @param input input
 * @return uint64_t output
 */
uint64_t expansion(uint32_t input)
{
    uint64_t output = 0;

    for (size_t i = 0; i < 48; ++i)
    {
        output <<= 1;
        output |= (uint64_t)(input >> (32 - E[i]) & 1);
    }

    // printf("Before expansion: 0x%x\nAfter expansion: 0x%lx\n", input, output);

    return output;
}

#undef DEBUG

#endif // DES