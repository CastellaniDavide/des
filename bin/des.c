// Dependencies
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include "lib/base64.h"
#include "lib/input.h"
#include "lib/key.h"
#include "lib/des.h"

// Definitions
#define DEBUG

int main(int argc, char *argv[])
{
    // Test input
    char *tmp[] = {"-", "--key=EzRXeZu83/E=", "--message=1vdapnyQx2k="};
    argv = tmp;
    argc = 3;

    // Check if help
    if (getHelp(argc, argv))
    {
        system("man 1 des");
        return 0;
    }

    // Check if new key is asked
    if (getNewKeyRequest(argc, argv))
    {
        printf("New key generated: %s\n", getNewKey());
        return 0;
    }

    // Check key
    uint64_t key = getKey(argc, argv);
    if (!checkKey(key))
    {
        printf(
            "%s\n%s\n",
            "The key is damaged.",
            "Please generate a new one.");
        return 0;
    }

    // Get if encryption or decryption asked
    bool encrypt = getEncrypt(argc, argv);

    // Get sub-keys
    uint64_t *subKeys = getSubKeys(key);

    // Get message
    uint64_t *message = getMessage(argc, argv);
//*
    uint64_t tmp222 = IP(0b0000000100100011010001010110011110001001101010111100110111101111);
    tmp222 = rounds(tmp222, subKeys, encrypt);

    for(size_t j = 0; j < 64; ++j)
            printf((tmp222 >> j) & 1 ? "1": "0");
        printf("\n");
/* /
    for (size_t i = 0; i < getMessageLength(); ++i)
    {

        // Intial Permutation
        message[i] = IP(message[i]);

        // Rounds
        message[i] = rounds(message[i], subKeys, encrypt);

        // Final permuation
        message[i] = FP(message[i]);
    }

    // Encode base64
    uint8_t *m = (uint8_t *)&message[0];
    char *output = base64_encode(m, 8 * getMessageLength());

    // Print output
    printf("%s message: %s",
           (encrypt ? "Encrypted" : "Decrypted"),
           output);
*/
    return 0;
}

#undef DEBUG
