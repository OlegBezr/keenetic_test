#include <stdio.h>
#include <stdlib.h>
#include "print_functions.h"

int main() {
    unsigned int a, b, c, d;
    unsigned int mask;
    unsigned int ip_number;
    unsigned int mask_number;
    unsigned int actual_ip;

    while (scanf("%d.%d.%d.%d/%d", &a, &b, &c, &d, &mask) != EOF) {
        ip_number = (a << 24) + (b << 16) + (c << 8) + d;
        mask_number = (((1 << (mask - 1)) - 1) * 2 + 1) << (32 - mask);
        actual_ip = ip_number & mask_number;
        printf("INPUT: %d.%d.%d.%d/%d\n", a, b, c, d, mask);
        unsigned int max_ip = ~mask_number | actual_ip;
        a = max_ip >> 24;
        b = (max_ip >> 16) & 0xff;
        c = (max_ip >> 8) & 0xff;
        d = max_ip & 0xff;
        // printf("M IP : %d.%d.%d.%d\n", a, b, c, d);

        printf("INPUT BITS:");
        print_bits(ip_number);
        printf("\n");
        // printf("MASK BITS :");
        // print_bits(mask_number);
        // printf("\n");
        // printf("M IP BITS :");
        // print_bits(max_ip);
        // printf("\n\n");
    }

    return 0;
}