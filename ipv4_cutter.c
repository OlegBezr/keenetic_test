#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "types.h"
#include "ranges_merger.h"
#include "print_functions.h"

struct SubnetLinked *range_subnets(uint32_t min, uint32_t max) {
    if (min == 0b010111111101010000000000100000000) {
        print_subnet((struct Subnet) {min, 32});
        print_subnet((struct Subnet) {max, 32});
    }

    if (min == max) {
        struct SubnetLinked *subnet = malloc(sizeof(struct SubnetLinked));
        subnet->subnet.ip = min;
        subnet->subnet.mask = 32;
        subnet->next = NULL;
        return subnet;
    }

    uint8_t last_min_one = 0;
    for (int i = 1; i <= 32; i++) {
        if (((min >> (32 - i)) & 1) == 1) {
            last_min_one = i;
        }
    }

    uint8_t first_min_less = 0;
    for (uint32_t i = 1; i <= 32; i++) {
        if (((max >> (32 - i)) & 1) == 1 && ((min >> (32 - i)) & 1) == 0) {
            first_min_less = i;
            break;
        }
    }

    uint8_t first_max_zero = 0;
    for (uint32_t i = 32; i >= 1; i--) {
        if (((max >> (32 - i)) & 1) == 0) {
            first_max_zero = i;
            break;
        }
    }
    
    uint8_t mask = last_min_one;
    if (first_min_less > last_min_one) {
        mask = first_min_less;
        if (first_max_zero < mask) {
            mask -= 1;
        }
    }

    uint32_t mask_number = (((1 << (mask - 1)) - 1) * 2 + 1) << (32 - mask);
    uint32_t middle = ((~mask_number) | min);

    struct SubnetLinked *subnets = calloc(1, sizeof(struct SubnetLinked));
    subnets->subnet = (struct Subnet) { .ip = min, .mask = mask };

    if (middle == max) {
        subnets->next = NULL;
    } else {
        subnets->next = range_subnets(middle + 1, max);
    }

    return subnets;
}

int main() {
    // READ INPUT AND TURN IT INTO "OPEN" / "CLOSE" RANGE EVENTS
    uint32_t points_count = 0;
    struct CutPoint *points = NULL;
    uint8_t a, b, c, d, mask;
    while (scanf("%hhu.%hhu.%hhu.%hhu/%hhu", &a, &b, &c, &d, &mask) != EOF) {
        uint32_t ip_number = (a << 24) + (b << 16) + (c << 8) + d;
        uint32_t mask_number = (((1 << (mask - 1)) - 1) * 2 + 1) << (32 - mask);
        uint32_t actual_ip = (ip_number & mask_number);
        uint32_t max_ip = ((~mask_number) | actual_ip);

        points_count += 2;
        points = realloc(points, points_count * sizeof(struct CutPoint));
        points[points_count - 2] = (struct CutPoint){ .ip = actual_ip, .is_start = true };
        points[points_count - 1] = (struct CutPoint){ .ip = max_ip, .is_start = false };
    }
    if (points_count == 0) {
        printf("0.0.0.0/32\n");
        return 0;
    }
    qsort(points, points_count, sizeof(struct CutPoint), cutpoint_comparator);

    // REMOVE OVERLAPPING RANGES - MERGE THEM INTO ONE
    uint32_t *merged_points;
    uint32_t merged_points_count = merge_ranges(points, points_count, &merged_points);
    free(points);
    
    // FIND SUBNETS FOR THE RANGES
    struct SubnetLinked *subnets_ans = NULL;
    struct SubnetLinked *subnets_ans_last = NULL;
    if (merged_points[0] != 0) {
        subnets_ans = range_subnets(0, merged_points[0] - 1);
        subnets_ans_last = subnets_ans;
        while (subnets_ans_last->next != NULL) {
            subnets_ans_last = subnets_ans_last->next;
        }
    }

    for (int i = 1; i < merged_points_count - 1; i += 2) {
        if (merged_points[i] == merged_points[i + 1] 
            || merged_points[i] + 1 == merged_points[i + 1]) {
            continue;
        }
        subnets_ans_last->next = range_subnets(merged_points[i] + 1, merged_points[i + 1] - 1);
        while (subnets_ans_last->next != NULL) {
            subnets_ans_last = subnets_ans_last->next;
        }
    }

    if (merged_points[merged_points_count - 1] != 0xffffffff) {
        subnets_ans_last->next = range_subnets(merged_points[merged_points_count - 1] + 1, 0xffffffff);
        while (subnets_ans_last->next != NULL) {
            subnets_ans_last = subnets_ans_last->next;
        }
    }
    free(merged_points);

    // PRINT ANSWER
    while (subnets_ans != NULL) {
        print_subnet(subnets_ans->subnet);
        struct SubnetLinked *tmp = subnets_ans;
        subnets_ans = subnets_ans->next;
        free(tmp);
    }

    return 0;
}
