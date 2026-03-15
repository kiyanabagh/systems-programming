/* Implements rotate-right using only bitwise operators and shifts.
 */

#include "major.h"

/*
 * Rotate the 32-bit value x to the right by n positions (0..31).
 */
uint32_t rotate_right(uint32_t x, unsigned int n)
{
    /* Ensure n is within 0..31 to avoid undefined shift widths */
    n &= 31U;

    if (n == 0U) {
        return x;
    }

    /* Right part and wrapped-around left part */
    return (x >> n) | (x << (32U - n));
}
