#ifndef RANGES_MERGER_H
#define RANGES_MERGER_H

#include <stdlib.h>
#include <stdint.h>
#include "types.h"

int cutpoint_comparator(const void *a, const void *b) {
    struct CutPoint a_value = *((struct CutPoint *)a);
    struct CutPoint b_value = *((struct CutPoint *)b);
    if (a_value.ip > b_value.ip) {
        return 1;
    }
    if (a_value.ip < b_value.ip) {
        return -1;
    }

    if (a_value.is_start > b_value.is_start) {
        return -1;
    }
    if (a_value.is_start < b_value.is_start) {
        return 1;
    }
    return 0;
}

uint32_t merge_ranges(
    struct CutPoint *points, 
    uint32_t points_count, 
    uint32_t **merged_ranges
) {
    uint32_t important_points_count = 0;
    uint32_t *important_points = NULL;
    uint32_t open_count = 0;
    uint32_t l, r;

    for (int i = 0; i < points_count; i++) {
        if (points[i].is_start) {
            if (open_count == 0) {
                l = points[i].ip;
            }
            open_count++;
        } else {
            open_count--;
            if (open_count == 0) {
                r = points[i].ip;
                important_points_count += 2;
                important_points = (uint32_t *)realloc(
                    important_points, 
                    important_points_count * sizeof(uint32_t)
                );
                important_points[important_points_count - 2] = l;
                important_points[important_points_count - 1] = r;
            }
        }
    }

    *merged_ranges = important_points;
    return important_points_count;
}

#endif