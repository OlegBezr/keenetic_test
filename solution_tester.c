#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "types.h"
#include "print_functions.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Provide two arguments: input file and answer file");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    FILE *answer = fopen(argv[2], "r");

    struct Subnet *input_subnets = NULL;
    uint32_t input_subnets_count = 0;
    struct Subnet *answer_subnets = NULL;
    uint32_t answer_subnets_count = 0;

    uint8_t a, b, c, d, mask;
    while (fscanf(input, "%hhu.%hhu.%hhu.%hhu/%hhu", &a, &b, &c, &d, &mask) != EOF) {
        input_subnets_count++;
        input_subnets = realloc(input_subnets, input_subnets_count * sizeof(struct Subnet));
        input_subnets[input_subnets_count - 1] = (struct Subnet) { .ip = (a << (uint32_t)24) + (b << (uint32_t)16) + (c << (uint32_t)8) + d, .mask = mask };
    }

    while (fscanf(answer, "%hhu.%hhu.%hhu.%hhu/%hhu", &a, &b, &c, &d, &mask) != EOF) {
        answer_subnets_count++;
        answer_subnets = realloc(answer_subnets, answer_subnets_count * sizeof(struct Subnet));
        answer_subnets[answer_subnets_count - 1] = (struct Subnet) { .ip = (a << (uint32_t)24) + (b << (uint32_t)16) + (c << (uint32_t)8) + d, .mask = mask };
    }

    printf("Input subnets: %u\n", input_subnets_count);
    printf("Answer subnets: %u\n", answer_subnets_count);

    for (uint32_t i = 0; i < 0xffffffff; i++) {
        bool input_has = false;
        bool answer_has = false;
        for (uint32_t j = 0; j < input_subnets_count; j++) {
            struct Subnet subnet = input_subnets[j];
            uint32_t ip = subnet.ip;
            uint32_t mask_number = (((1 << (subnet.mask - 1)) - 1) * 2 + 1) << (32 - subnet.mask);
            uint32_t actual_ip = (ip & mask_number);
            uint32_t max_ip = ((~mask_number) | actual_ip);

            if (i >= actual_ip && i <= max_ip) {
                input_has = true;
                break;
            }
        }

        for (uint32_t j = 0; j < answer_subnets_count; j++) {
            struct Subnet subnet = answer_subnets[j];
            uint32_t ip = subnet.ip;
            uint32_t mask_number = (((1 << (subnet.mask - 1)) - 1) * 2 + 1) << (32 - subnet.mask);
            uint32_t actual_ip = (ip & mask_number);
            uint32_t max_ip = ((~mask_number) | actual_ip);

            if (i >= actual_ip && i <= max_ip) {
                answer_has = true;
                break;
            }
        }

        if (input_has == answer_has) {
            printf("Error: addresses inclusion does not match input\n");
            printf("%d.%d.%d.%d\n", (i >> 24) & 0xff, (i >> 16) & 0xff, (i >> 8) & 0xff, i & 0xff);
            printf("INPUT: %d\n", input_has);
            printf("ANSWER: %d\n", answer_has);
            print_bits(i);
            return 1;
        }
    }

    printf("All tests passed\n");

    free(input_subnets);
    free(answer_subnets);

    return 0;
}