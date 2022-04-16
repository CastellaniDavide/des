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
#include <math.h>
#include "base64.h"
#include "key.h"

// Definitions
//#define DEBUG
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// Variabiles
bool error = false;
uint64_t salvedKey = 0;
uint64_t *salvedMessage = NULL;
size_t messageLength;

// Function declaration
bool getHelp(int argc, char *argv[]);
bool getVerbose(int argc, char *argv[]);
bool getVersion(int argc, char *argv[]);
bool getIfText(int argc, char *argv[]);
bool getIfFile(int argc, char *argv[]);
char *getInputFileName(int argc, char *argv[]);
char *getOutputFileName(int argc, char *argv[]);
uint64_t getKey(int argc, char *argv[]);
uint64_t *getMessage(int argc, char *argv[]);
size_t getMessageLength();
bool getEncrypt(int argc, char *argv[]);
bool getNewKeyRequest(int argc, char *argv[]);
bool startsWith(const char *a, const char *b);
int getIndex(int argc, char *argv[], char *string);
bool manageInput(int argc, char *argv[]);

// Functions
/**
 * @brief Get the Help object
 *
 * @param argc number of command line arguments
 * @param argv command line arguments
 * @return true help is needed
 * @return false help isn't needed
 */
bool getHelp(int argc, char *argv[])
{
    // Check if help is asked
    for (size_t i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
            return true;
    }

    // Check inputs
    if (!getNewKeyRequest(argc, argv))
    {
        getKey(argc, argv);
        getMessage(argc, argv);
    }

    return error;
}

/**
 * @brief Get the Verbose object
 *
 * @param argc number of command line arguments
 * @param argv command line arguments
 * @return true verbose is asked
 * @return false verbose isn't asked
 */
bool getVerbose(int argc, char *argv[])
{
    // Check if verbose
    for (size_t i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0)
            return true;
    }
    return false;
}

/**
 * @brief Get the Version object
 *
 * @param argc number of command line arguments
 * @param argv command line arguments
 * @return true version is asked
 * @return false version isn't asked
 */
bool getVersion(int argc, char *argv[])
{
    // Check if version
    for (size_t i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "--version") == 0)
            return true;
    }
    return false;
}

/**
 * @brief Get the If Text object
 *
 * @param argc number of command line arguments
 * @param argv command line arguments
 * @return true text version is asked
 * @return false text version isn't asked
 */
bool getIfText(int argc, char *argv[])
{
    // Check if text input/ output
    for (size_t i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "--text") == 0 || strcmp(argv[i], "-txt") == 0)
            return true;
    }
    return false;
}

/**
 * @brief Get the If File object
 *
 * @param argc number of command line arguments
 * @param argv command line arguments
 * @return true file version is asked
 * @return false file version isn't asked
 */
bool getIfFile(int argc, char *argv[])
{
    return (getIndex(argc, argv, "--file-input=") != -1 && getIndex(argc, argv, "--file-output=") != -1);
}

/**
 * @brief Get the File Input Name object
 *
 * @param argc number of command line arguments
 * @param argv command line arguments
 * @return char* file name
 */
char *getInputFileName(int argc, char *argv[])
{
    // Get the file input name
    char *fileName = "--file-input=";

    // Return the file input name
    return argv[getIndex(argc, argv, fileName)] + strlen(fileName);
}

/**
 * @brief Get the File Output Name object
 *
 * @param argc number of command line arguments
 * @param argv command line arguments
 * @return char* file name
 */
char *getOutputFileName(int argc, char *argv[])
{
    // Get the file output name
    char *fileName = "--file-output=";

    // Return the file output name
    return argv[getIndex(argc, argv, fileName)] + strlen(fileName);
}

/**
 * @brief Get the Key object
 *
 * @param argc number of command line arguments
 * @param argv command line arguments
 * @return uint64_t key
 */
uint64_t getKey(int argc, char *argv[])
{
    // Check if key was already readed
    if (salvedKey == 0)
    {
        // check if key argument is passed
        char *k = "--key=";
        int index = getIndex(argc, argv, k);

        if (index == -1)
        {
            error = true;
        }
        else
        {
            // Get base64 key
            u_int8_t *tmp = argv[index] + strlen(k);

            // Convert key
            uint64_t key = 0;

            tmp = base64_decode(tmp, strlen(tmp), false);

            for (int i = 7; i >= 0; --i)
            {
                key <<= 8;
                key |= (uint64_t)(tmp[i]);
            }

            salvedKey = key;
        }
    }

    // printf("key: %lx\n", salvedKey);
    return (uint64_t)salvedKey;
}

/**
 * @brief Get the Message object
 *
 * @param argc number of command line arguments
 * @param argv command line arguments
 * @return uint64_t* message
 */
uint64_t *getMessage(int argc, char *argv[])
{
    if (!salvedMessage) // Get message from file
    {

        if (getIfFile(argc, argv))
        {
            // Get the file
            FILE *file = fopen(getInputFileName(argc, argv), "rb");

            if (file == NULL)
            {
                error = true;
            }
            else
            {
                // Get the file size
                fseek(file, 0L, SEEK_END);
                size_t fileSize = MIN(ftell(file), 1024 * 1024 * 16); // up to 16MB
                fseek(file, 0L, SEEK_SET);

                // Allocate memory
                messageLength = (fileSize * sizeof(char) + sizeof(uint64_t) - 1) / sizeof(uint64_t);
                salvedMessage = malloc(messageLength * sizeof(uint64_t));
                size_t extra_bytes = messageLength * sizeof(uint64_t) / sizeof(char) - fileSize;

                // Read the file
                fread(salvedMessage, fileSize, 1, file);

                // Close the file
                fclose(file);
            }
        }
        else
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
                if (getIfText(argc, argv) && getEncrypt(argc, argv)) // Get message from text input
                {
                    messageLength = (strlen(message) * sizeof(char) + sizeof(uint64_t) - 1) / sizeof(uint64_t);
                    uint64_t *result = malloc(sizeof(uint64_t) * messageLength);

                    for (size_t i = 0; i < messageLength; ++i)
                    {
                        for (size_t j = 0; j < sizeof(uint64_t) / sizeof(char); ++j)
                        {
                            result[i] <<= 8;
                            if (i * sizeof(uint64_t) + j < strlen(message))
                                result[i] |= (uint64_t)(message[i * sizeof(uint64_t) + j]);
                            else
                                result[i] |= (uint64_t)('\0');
                        }
                    }

                    salvedMessage = result;
                }
                else // Get message from base64 input
                {
                    // Add extra bytes to the message to ensure correct base64 conversion
                    if (getEncrypt(argc, argv))
                        message = dirty(message, strlen(message));

                    // Convert message
                    uint8_t *tmp = base64_decode(message, strlen(message), false);
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
    }
    return salvedMessage;
}

/**
 * @brief Get the Message Length object
 *
 * @return size_t message length
 */
size_t getMessageLength()
{
    return messageLength;
}

/**
 * @brief Get the Encrypt object
 *
 * @param argc number of command line arguments
 * @param argv command line arguments
 * @return true is encryption is asked
 * @return false is decryption is asked
 */
bool getEncrypt(int argc, char *argv[])
{
    for (size_t i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "--decrypt") == 0 || strcmp(argv[i], "-d") == 0)
            return false;
    }
    return true;
}

/**
 * @brief Get the New Key Request object
 *
 * @param argc number of command line arguments
 * @param argv command line arguments
 * @return true if new key is asked
 * @return false if new key isn't asked
 */
bool getNewKeyRequest(int argc, char *argv[])
{
    for (size_t i = 0; i < argc; ++i)
    {
        if (strcmp(argv[i], "--new-key") == 0 || strcmp(argv[i], "-nk") == 0 || strcmp(argv[i], "--generate-key") == 0 || strcmp(argv[i], "-gk") == 0)
            return true;
    }
    return false;
}

/**
 * @brief Check if a string starts with another one
 *
 * @param a "long" string
 * @param b "short" string
 * @return true if a starts with b
 * @return false if a don't start with b
 */
bool startsWith(const char *a, const char *b)
{
    if (strncmp(a, b, strlen(b)) == 0)
        return 1;
    return 0;
}

/**
 * @brief Get the Index object
 *
 * @param argc number of command line arguments
 * @param argv command line arguments
 * @param string string to search
 * @return int the index in argv of the string; -1 if didn't exist
 */
int getIndex(int argc, char *argv[], char *string)
{
    for (size_t i = 0; i < argc; ++i)
    {
        if (startsWith(argv[i], string))
            return i;
    }
    return -1;
}

/**
 * @brief Manage the input, eg. help
 *
 * @param argc number of command line arguments
 * @param argv command line arguments
 * @return true if it's needed to end the execution
 * @return false if it's not needed to end the execution
 *
 */
bool manageInput(int argc, char *argv[])
{
    // get verbose
    bool verbose = getVerbose(argc, argv);

    // if verbose print command line arguments
    if (verbose)
    {
        printf("ℹ️\tCommand line arguments:\n");

        for (size_t i = 0; i < argc; ++i)
        {
            printf("\t- %ld: %s\n", i, argv[i]);
        }
    }

    // Get verbose
    if (verbose)
        printf("ℹ️\tVerbose: %s\n", (verbose ? "true" : "false"));

    // Check if new key is asked
    if (verbose)
        printf("ℹ️\tCheck new key request\n");
    if (getNewKeyRequest(argc, argv))
    {
        printf("➡️ New key generated: %s\n", getNewKey(verbose));
        return true;
    }

    // Check if version asked
    if (verbose)
        printf("ℹ️\tCheck version\n");
    if (getVersion(argc, argv))
    {
        system("dpkg -s des | grep '^Version:'");
        return true;
    }

    // Check if help
    if (verbose)
        printf("ℹ️\tCheck help\n");
    if (getHelp(argc, argv))
    {
        system("man 1 des");
        return true;
    }

    // Check if new key is asked
    if (verbose)
        printf("ℹ️\tCheck new key request\n");
    if (getNewKeyRequest(argc, argv))
    {
        printf("➡️\tNew key generated: %s\n", getNewKey(verbose));
        return true;
    }

    // Check key
    if (verbose)
        printf("ℹ️\tCheck given key\n");
    if (!checkKey(getKey(argc, argv)))
    {
        printf("⚠️⚠️⚠️\nThe key is damaged.\nPlease generate a new one.\n");
        return true;
    }

    // Get encryption
    if (verbose)
        printf("ℹ️\tEncryption: %s\n", (verbose ? "true" : "false"));

    // End message
    if (verbose)
        printf("ℹ️\tCommand line arguments: ok\n");

    return false;
}

#undef DEBUG

#endif // INPUT