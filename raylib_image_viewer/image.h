#ifndef IMAGE_H
#define IMAGE_H
#include <stdio.h>

struct ImageData {
    int w;
    int h;
    char* data;
};
struct ImageData* loadImage(const char* filename);

#endif