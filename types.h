#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

struct CutPoint {
    uint32_t ip;
    bool is_start;
};

struct Subnet {
    uint32_t ip;
    uint8_t mask;
};

struct SubnetLinked {
    struct Subnet subnet;
    struct SubnetLinked *next;
};

#endif