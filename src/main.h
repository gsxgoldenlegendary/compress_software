//
// Created by administrator Guo on 23/11/2021 15:46.
//

#ifndef DSHW2_V1_MAIN_H
#define DSHW2_V1_MAIN_H
#include "iostream"
#include "fstream"
#include "string"
#include "vector"
#include "algorithm"
struct HTNode {
    unsigned long long number;
    unsigned code;
    unsigned bit_of_code;
    unsigned long long weight;
    unsigned child[16];
    unsigned parent;
    bool is_in_tree;

    bool operator==(const unsigned long long i) const {
        return number == i;
    }
};
#endif //DSHW2_V1_MAIN_H