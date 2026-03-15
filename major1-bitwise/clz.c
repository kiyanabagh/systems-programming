#include "major1.h"

unsigned int countLeadZeros(unsigned int num) {
    unsigned int count = 0;
    int i;

    for (i = 31; i >= 0; i--) {
        if ((num >> i) & 1) {
            break;
        }
        count++;
    }

    return count;
}
