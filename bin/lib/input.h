/**
 * @file input.h
 * @author Davide Castellani (@DavideC03)
 * @brief Manage the input data
 */

#ifndef INPUT
#define INPUT

// Dependencies
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#include "base64.h"

// Definitions
#define DEBUG

// Variabiles
bool error = false;
uint64_t salvedKey = 0;
uint64_t *salvedMessage = NULL;
size_t messageLength;

// Function declaration
bool getHelp(int argc, char *argv[]);
uint64_t getKey(int argc, char *argv[]);
uint64_t *getMessage(int argc, char *argv[]);
size_t getMessageLength();
bool getEncrypt(int argc, char *argv[]);
bool getNewKeyRequest(int argc, char *argv[]);
bool startsWith(const char *a, const char *b);
int getIndex(int argc, char *argv[], char *string);

// Functions
bool getHelp(int argc, char *argv[])
{
    // Check if help is asked
    for (size_t i = 0; i < argc; ++i)
    {
        if (argv[i] == "--help" || argv[i] == "-h")
            return true;
    }

    // Check inputs
    if (!getNewKeyRequest(argc, argv))
        getKey(argc, argv);
    getMessage(argc, argv);

    return error;
}

uint64_t getKey(int argc, char *argv[])
{
    if (salvedKey == 0)
    {
        char *k = "--key=";
        int index = getIndex(argc, argv, k);

        if (index == -1)
        {
            error = true;
        }
        else
        {
            u_int8_t *tmp = argv[index] + strlen(k);
            uint64_t key = 0;

            // printf("%ld, %s\n", strlen(tmp), tmp);

            tmp = base64_decode(tmp, strlen(tmp));

            for (int i = 7; i >= 0; --i)
            {
                key <<= 8;
                key |= (uint64_t)(tmp[i]);
                // printf("%d ==> %lx\n", i, (uint64_t)key);
            }

            salvedKey = key;
        }
    }
    // printf("key: %lx\n", salvedKey);
    return (uint64_t)salvedKey;
}

uint64_t *getMessage(int argc, char *argv[])
{
    if (!salvedMessage)
    {
        char *m = "--message=";
        int index = getIndex(argc, argv, m);
        char *message;

        if (index == -1)
        {
            error = true;
        }
        else
        {
            message = argv[index] + strlen(m);
            if (getEncrypt(argc, argv))
            {
                messageLength = (strlen(message) + 8 - 1) / 8;
                uint64_t *result = malloc(sizeof(uint64_t) * messageLength);
                for (size_t i = 0; i < messageLength; ++i)
                {
                    for (int j = 7; j >= 0; --j)
                    {
                        result[i] <<= 8;
                        result[i] |= (uint64_t)(message[8 * i + j]);
                    }
                }
                salvedMessage = result;
            }
            else
            {
                uint8_t *tmp = base64_decode(message, sizeof(message));
                messageLength = (getLastBase64Length() + 8 - 1) / 8;
                uint64_t *result = malloc(sizeof(uint64_t) * messageLength);

                for (size_t i = 0; i < messageLength; ++i)
                {
                    result[i] = 0;
                    for (size_t j = 0; j < 8; ++j)
                    {
                        result[i] <<= 8;
                        if (i * 8 + j < getLastBase64Length())
                            result[i] |= tmp[i * 8 + j] & 0xff;
                    }
                }
                salvedMessage = result;
            }
        }
    }
    return salvedMessage;
}

size_t getMessageLength()
{
    return messageLength;
}

bool getEncrypt(int argc, char *argv[])
{
    for (size_t i = 0; i < argc; ++i)
    {
        if (argv[i] == "--decrypt" || argv[i] == "-d")
            return false;
    }
    return true;
}

bool getNewKeyRequest(int argc, char *argv[])
{
    for (size_t i = 0; i < argc; ++i)
    {
        if (argv[i] == "--new-key" || argv[i] == "-nk")
            return true;
    }
    return false;
}

bool startsWith(const char *a, const char *b)
{
    if (strncmp(a, b, strlen(b)) == 0)
        return 1;
    return 0;
}

int getIndex(int argc, char *argv[], char *string)
{
    for (size_t i = 0; i < argc; ++i)
    {
        if (startsWith(argv[i], string))
            return i;
    }
    return -1;
}

#undef DEBUG

#endif // INPUT