/**
 * @file logo.h
 * @author Davide Castellani (@DavideC03)
 * @brief
 */

#ifndef LOGO
#define LOGO

// Dependencies
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>

// Definitions
//#define DEBUG

// Variabiles
char logo[10][49] = {
    "8 888888888o.      8 8888888888     d888888o.   ",
    "8 8888    `^888.   8 8888         .`8888:' `88. ",
    "8 8888        `88. 8 8888         8.`8888.   Y8 ",
    "8 8888         `88 8 8888         `8.`8888.     ",
    "8 8888          88 8 888888888888  `8.`8888.    ",
    "8 8888          88 8 8888           `8.`8888.   ",
    "8 8888         ,88 8 8888            `8.`8888.  ",
    "8 8888        ,88' 8 8888        8b   `8.`8888. ",
    "8 8888    ,o88P'   8 8888        `8b.  ;8.`8888 ",
    "8 888888888P'      8 888888888888 `Y8888P ,88P' "};
char *colors[7] = {
    "\x1B[31m", // Red
    "\x1B[32m", // Green
    "\x1B[33m", // Yellow
    "\x1B[34m", // Blu
    "\x1B[35m", // Magenta
    "\x1B[36m", // Cyan
    "\x1B[37m"  // White
};
char *rst = "\033[0m"; // Reset

// Function declaration
void getLogo();

// Functions
void getLogo()
{
    // Dependencies declaration
    time_t t;
    u_int8_t c[49 + 10];

    // Initialize random
    srand((unsigned)time(&t));

    // Colors
    for (size_t i = 0; i < 49 + 10; ++i)
        c[i] = rand() % 8;

    // Print logo
    for (size_t i = 0; i < 10; ++i)
    {
        for (size_t j = 0; j < 49; ++j)
        {
            printf("%s%c", colors[c[i+j]], logo[i][j]);
        }
        printf("\n");
    }

    // Reset 
    printf("%s", rst);
}

#undef DEBUG

#endif // LOGO