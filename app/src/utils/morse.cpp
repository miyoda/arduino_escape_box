#include <ctype.h>
#include <stdlib.h>

#include "morse.h"

/* 
 * Function morse_to_index by cypherpunks on Reddit.
 * See: http://goo.gl/amr6A3
 */
int morse_to_index (const char* str)
{
        unsigned char sum = 0, bit;

        for (bit = 1; bit; bit <<= 1) {
                switch (*str++) {
                case 0:
                        return sum | bit;
                default:
                        return 0;
                case '-':
                        sum |= bit;
                        /* FALLTHROUGH */
                case '.':
                        break;
                }
        }

        return 0;
}

const char* char_to_morse (char c)
{
        if (islower(c))
                c += ('A' - 'a');

        return CHAR_TO_MORSE[(int) c];
}

const char* morse_to_char (const char* str)
{
        return MORSE_TO_CHAR[morse_to_index(str)];
}