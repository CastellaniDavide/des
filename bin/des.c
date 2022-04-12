// Dependencies
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include "lib/base64.h"
#include "lib/input.h"
#include "lib/des.h"
#include "lib/logo.h"

// Definitions
//#define DEBUG

int main(int argc, char *argv[])
{
    // Start stopwatch
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // Get verbose
    bool verbose = getVerbose(argc, argv);

    // Logo
    if (verbose)
        getLogo();

    // Manage input
    if (manageInput(argc, argv))
        return 0;

    // DES
    uint64_t *m = malloc(getMessageLength() * sizeof(uint64_t));
    for (size_t i = 0; i < getMessageLength(); ++i)
        m[i] = des(
            getMessage(argc, argv)[i],
            getKey(argc, argv),
            getEncrypt(argc, argv),
            verbose);

    // Encode base64
    size_t len = getMessageLength() * sizeof(uint64_t);
    uint8_t *m8bit = malloc(len);
    for (size_t i = 0; i < getMessageLength(); ++i)
        for (size_t j = 0; j < 8; ++j)
            m8bit[8 * i + j] = (u_int8_t)(m[i] >> (64 - 8 * (j + 1)));

    while (m8bit[len - 1] == 0) // "Remove" \0 bytes
        len--;

    char *output = base64_encode(m8bit, len, verbose);

    // Stop stopwatch
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Print output
    printf("%s message: %s\nIn %0.3fµs\n",
           (getEncrypt(argc, argv) ? "Encrypted" : "Decrypted"),
           output,
           cpu_time_used * 1000);

    return 0;
}

#undef DEBUG
