#include "color.h"
#include <stdio.h>

void printColor(enum imageMode m, enum channel c) {
    if (m == RGB) {
        switch (c) {
        case R:
            printf("\033[31m");
            break;
        case G:
            printf("\033[32m");
            break;
        case B:
            printf("\033[34m");
            break;
        default:
            break;
        }
    }
}

void resetColor(enum imageMode m) {
    if (m == RGB) {
        printf("\033[0m");
    }
}