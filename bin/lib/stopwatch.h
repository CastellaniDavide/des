/**
 * @file stopwatch.h
 * @author Davide Castellani (@DavideC03)
 * @brief
 */

#ifndef STOPWATCH
#define STOPWATCH

// Dependencies
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>

// Definitions
//#define DEBUG

// Variabiles
clock_t start, end;
double cpu_time_used;

// Function declaration
void startStopwatch();
void stopStopwatch();
char *getStopwatch();

// Functions
/**
 * @brief Start the stopwatch
 * 
 */
void startStopwatch()
{
    // Dependencies declaration
    start = clock();
}

/**
 * @brief Stop the stopwatch
 * 
 */
void stopStopwatch()
{
    // Dependencies declaration
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
}

/**
 * @brief Get the Stopwatch object "MM m SS s MS ms US µs"
 * 
 * @return char* printable version of the stopwatch
 */
char *getStopwatch()
{
    // Dependencies declaration
    char *output = malloc(sizeof(char) * 255);

    // Get time
    int m = (int)(cpu_time_used / 60);
    int s = (int)(cpu_time_used - m * 60);
    int ms = (int)(((cpu_time_used) - m * 60 - s) * 1000);
    int us = (int)(((cpu_time_used) - m * 60 - s - (double) ms / 1000) * 1000000);

    // Return
    sprintf(output, "%dm %ds %dms %dµs", m, s, ms, us);
    return output;
}

#undef DEBUG

#endif // STOPWATCH