/*
 * Purpose: Swap the byte order (endianness) of a 32-bit integer
 */

#include "major.h"

/*
 * endian_swap()
 * Takes: 0xAABBCCDD
 * Returns: 0xDDCCBBAA
 * Uses masking and shifting for each byte.
 */
uint32_t endian_swap(uint32_t x)
{
    // Move byte 0 to byte 3 position
    uint32_t b0 = (x & 0x000000FFU) << 24;
    // Move byte 1 to byte 2 position
    uint32_t b1 = (x & 0x0000FF00U) << 8;
    // Move byte 2 to byte 1 position
    uint32_t b2 = (x & 0x00FF0000U) >> 8;
    // Move byte 3 to byte 0 position
    uint32_t b3 = (x & 0xFF000000U) >> 24;

    // Combine all bytes into swapped result
    return b0 | b1 | b2 | b3;
}

