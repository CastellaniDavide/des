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
#include "lib/stopwatch.h"

// Definitions
//#define DEBUG

int main(int argc, char *argv[])
{
    // Get verbose
    bool verbose = getVerbose(argc, argv);

    // Logo
    getLogo();

    // Start stopwatch
    startStopwatch();

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
            verbose && i == 0); // If verbose, print only the first message

    // Encode base64
    size_t len = getMessageLength() * sizeof(uint64_t);
    uint8_t *m8bit = malloc(len);
    for (size_t i = 0; i < getMessageLength(); ++i)
        for (size_t j = 0; j < 8; ++j)
            m8bit[8 * i + j] = (u_int8_t)(m[i] >> (64 - 8 * (j + 1)));

    while (m8bit[len - 1] == 0) // "Remove" \0 bytes
        len--;

    char *output = base64_encode(dirty(m8bit, len), len + 4, verbose);

    // Stop stopwatch
    stopStopwatch();

    // Print output
    if (getIfFile(argc, argv))
    {
        // Calculate extra padding
        size_t padding = 0;
        if (!getEncrypt(argc, argv))
            while (((u_int64_t)0xff << 8 * (8 - padding - 1) & m[getMessageLength() - 1]) == (uint8_t)0x00)
                padding++;

        // Save output to file
        FILE *file = fopen(getOutputFileName(argc, argv), "wb");
        fwrite(m, getMessageLength() * sizeof(u_int64_t) - padding, 1, file);
        fclose(file);

        // Print
        printf("📝\t%s to: %s (%ld Byte)\n",
            (getEncrypt(argc, argv) ? "Encrypted" : "Decrypted"),
            getOutputFileName(argc, argv),
            getMessageLength() * sizeof(u_int64_t) - padding);
    }
    else if (getIfText(argc, argv) && !getEncrypt(argc, argv))
    {
        // Print
        printf("📝\tDecrypted message: %s\n", (char *)clean((char *)clean((char *)m8bit, len), len - 4));
    }
    else // base64
    {
        // Print
        printf("📝\t%s message: %s\n",
            (getEncrypt(argc, argv) ? "Encrypted" : "Decrypted"),
            clean(output, strlen(output)));
    }

    // Print stopwatch
    printf("⏰\tIn: %s\n", getStopwatch());

    return 0;
}

#undef DEBUG
