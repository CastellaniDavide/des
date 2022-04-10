/**
 * @file base64.h
 * @author Davide Castellani (@DavideC03)
 * @brief Base64 encoding and decoding
 *
 * Source: https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
 */

#ifndef BASE64
#define BASE64

// Dependencies
#include <stdint.h>
#include <stdlib.h>

// Definitions
#define DEBUG

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
char *base64_encode(const u_int8_t *data, size_t input_length);
u_int8_t *base64_decode(const char *data, size_t input_length);
void build_decoding_table();
size_t getLastBase64Length();

// Functions
char *base64_encode(const u_int8_t *data,
                    size_t input_length)
{
    size_t output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = malloc(output_length); // +1
    if (encoded_data == NULL)
        return NULL;

    for (int i = 0, j = 0; i < input_length;)
    {

        uint32_t octet_a = i < input_length ? (u_int8_t)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (u_int8_t)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (u_int8_t)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[output_length - 1 - i] = '=';

    encoded_data[output_length] = '\0';
    base64length = output_length + 1;

    return encoded_data;
}

u_int8_t *base64_decode(const char *data,
                             size_t input_length)
{

    if (decoding_table == NULL)
        build_decoding_table();

    if (input_length % 4 != 0)
        return NULL;

    size_t output_length = input_length / 4 * 3;

    if (data[input_length - 1] == '=')
        (output_length)--;
    if (data[input_length - 2] == '=')
        (output_length)--;
    base64length = output_length;

    u_int8_t *decoded_data = malloc(output_length);
    if (decoded_data == NULL)
        return NULL;

    for (int i = 0, j = 0; i < input_length;)
    {

        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        uint32_t triple = (sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + (sextet_d << 0 * 6);

        if (j < output_length)
            decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < output_length)
            decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < output_length)
            decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    return decoded_data;
}

size_t getLastBase64Length()
{
    return base64length;
}

void build_decoding_table()
{

    decoding_table = malloc(256);

    for (int i = 0; i < 64; i++)
        decoding_table[(u_int8_t)encoding_table[i]] = i;
}

#undef DEBUG

#endif // BASE64