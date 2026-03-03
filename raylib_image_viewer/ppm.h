#ifndef PPM_H
#define PPM_H
#include <stdio.h>

struct ppmImage {
    char format[2];
    int w;
    int h;
    int depth;
    char* data;
};
struct ppmImage* loadPPM(const char* filename);

#endif