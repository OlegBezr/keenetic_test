#ifndef PRINT_FUNCTIONS_H
#define PRINT_FUNCTIONS_H

#include <stdio.h>
#include <stdint.h>
#include "types.h"

void print_subnet(struct Subnet subnet) {
    unsigned char a, b, c, d;
    a = subnet.ip >> 24;
    b = (subnet.ip >> 16) & 0xff;
    c = (subnet.ip >> 8) & 0xff;
    d = subnet.ip & 0xff;
    printf("%d.%d.%d.%d/%d\n", a, b, c, d, subnet.mask);
}

void print_bits(uint32_t n) {
    int i;
    for (i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
}

#endif