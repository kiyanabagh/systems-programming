#ifndef MAJOR1_H
#define MAJOR1_H

#include <stdio.h>
#include <stdint.h>

unsigned int countLeadingZeros(uint32_t num);
uint32_t endianSwap(uint32_t num);
uint32_t rotateRight(uint32_t num, unsigned int positions);
unsigned int parity(uint32_t num);

#endif
