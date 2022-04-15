/**
 * @file base64.h
 * @author Davide Castellani (@DavideC03)
 * @brief Base64 encoding and decoding
 *
 * Taken inspiration from: https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
 */

#ifndef BASE64
#define BASE64

// Dependencies
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "binary.h"

// Definitions
//#define DEBUG

// Variabiles
static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};
size_t base64length;

// Function declaration
uint8_t *base64_encode(const u_int8_t *input, size_t input_length, bool verbose);
uint8_t *base64_decode(const u_int8_t *input, size_t input_length, bool verbose);
uint8_t *dirty(uint8_t *input, size_t input_length);
uint8_t *clean(uint8_t *input, size_t input_length);
void build_decoding_table();
size_t getLastBase64Length();

// Functions
/**
 * @brief Base64 encode
 * 
 * @param input data to encode
 * @param input_length size of data
 * @param verbose verbose
 * @return u_int8_t* encoded data
 */
u_int8_t *base64_encode(const u_int8_t *input, size_t input_length, bool verbose)
{
    // Calculate the output length
    size_t output_length = 4 * ((input_length + 2) / 3);

    // Create output variable
    u_int8_t *output = malloc(output_length + 1);
    if (output == NULL)
        return NULL;

    // Encode
    for (int i = 0, j = 0; i < input_length;)
    {
        uint32_t octet_a = i < input_length ? (u_int8_t)input[i++] : 0;
        uint32_t octet_b = i < input_length ? (u_int8_t)input[i++] : 0;
        uint32_t octet_c = i < input_length ? (u_int8_t)input[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        output[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        output[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        output[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        output[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    // Add, eventually, = char(s)
    for (int i = 0; i < mod_table[input_length % 3]; i++)
        output[output_length - 1 - i] = '=';

    // Final corrections
    output[output_length] = '\0';
    base64length = output_length + 1;

    // Print if verbose
    if (verbose)
    {
        printf("⚙️\tEncoded data:\n\tOriginal: ");
        for (size_t i = 0; i < input_length; ++i)
            printf("%s", getBinary(input[i], 8, 8));
        printf("\n\tEncoded: %s\n", output);
    }

    // Return
    return output;
}

/**
 * @brief Base64 decode
 * 
 * @param input data to decode
 * @param input_length size of data
 * @param verbose verbose
 * @return u_int8_t* decoded data 
 */
u_int8_t *base64_decode(const u_int8_t *input, size_t input_length, bool verbose)
{
    // Check
    if (input_length % 4 != 0)
    {
        printf("⚠️⚠️⚠️ Base64 decode error: input length is not a multiple of 4\n");
        exit(1);
    }

    // Create decoding table if not exists
    if (decoding_table == NULL)
        build_decoding_table();

    // Calculate output length
    size_t output_length = input_length / 4 * 3;

    if (input[input_length - 1] == '=')
        (output_length)--;
    if (input[input_length - 2] == '=')
        (output_length)--;
    base64length = output_length;

    // Create output variable
    u_int8_t *output = malloc(output_length);
    if (output == NULL)
        return NULL;

    // Decode
    for (int i = 0, j = 0; i < input_length;)
    {

        uint32_t sextet_a = input[i] == '=' ? 0 & i++ : decoding_table[input[i++]];
        uint32_t sextet_b = input[i] == '=' ? 0 & i++ : decoding_table[input[i++]];
        uint32_t sextet_c = input[i] == '=' ? 0 & i++ : decoding_table[input[i++]];
        uint32_t sextet_d = input[i] == '=' ? 0 & i++ : decoding_table[input[i++]];

        uint32_t triple = (sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + (sextet_d << 0 * 6);

        if (j < output_length)
            output[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < output_length)
            output[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < output_length)
            output[j++] = (triple >> 0 * 8) & 0xFF;
    }

    // Print if verbose
    if (verbose)
    {
        printf("⚙️\tDecoded data:\n\tOriginal: %s\n\tDecoded: ", input);
        for (size_t i = 0; i < output_length; ++i)
            printf("%s", getBinary(output[i], 8, 8));
        printf("\n");
    }

    // Return
    return output;
}

/**
 * @brief Get the Last Base64 Length object
 * 
 * @return size_t last base64 output length
 */
size_t getLastBase64Length()
{
    return base64length;
}

/**
 * @brief Create the build decoding table
 * 
 */
void build_decoding_table()
{
    decoding_table = malloc(256);

    for (int i = 0; i < 64; i++)
        decoding_table[(u_int8_t)encoding_table[i]] = i;
}

/**
 * @brief Add random bytes to the input to ensure the correct convertion to base64
 * 
 * @param input message to add random bytes
 * @param input_length length of the input
 * @return uint8_t* message with random bytes
 */
uint8_t *dirty(uint8_t *input, size_t input_length)
{
    // Get the number of final =
    size_t eq = 0;
    while (input[input_length - 1 - eq] == '=')
        eq++;
        
    // Initialize random seed
    time_t t;
    srand((unsigned)time(&t));
    
    // Initialize output
    uint8_t *output = malloc(input_length + 4);

    // Copy input to output
    for (size_t i = 0; i < input_length - eq; i++)
        output[i] = input[i];

    // Add random bytes
    for (size_t i = input_length - eq; i < input_length - eq + 4; i++)
        output[i] = encoding_table[rand() % 64];

    // Add final =
    for (size_t i = input_length - eq + 4; i < input_length + 4; i++)
        output[i] = '=';
    
    // Return
    return output;
}

uint8_t *clean(uint8_t *input, size_t input_length)
{
    // Get the number of final =
    size_t eq = 0;
    while (input[input_length - 1 - eq] == '=')
        eq++;

    // Initialize output
    uint8_t *output = malloc(input_length - 4);

    // Copy input to output
    for (size_t i = 0; i < input_length - eq - 4; i++)
        output[i] = input[i];

    // Add final =
    for (size_t i = input_length - eq - 4; i < input_length - 4; i++)
        output[i] = '=';

    // Return
    return output;
}

#undef DEBUG

#endif // BASE64