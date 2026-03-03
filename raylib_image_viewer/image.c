#include "image.h"

#include "png.h"
#include "ppm.h"
#include <stdlib.h>
#include <string.h>

const char* get_file_extension(const char* filename) {
    const char* dotPosition = strrchr(filename, '.');
    if (!dotPosition) {
        return NULL;
    }
    return dotPosition + 1;
}

struct ImageData* loadImage(const char* filename) {
    struct ImageData* img = malloc(sizeof(struct ImageData*));
    if (!img) {
        return NULL;
    }
    const char* extension = get_file_extension(filename);

    if (extension == NULL) {

        fprintf(stderr, "Unsupported format");
        free(img);
        return NULL;

    } else if (strcmp(extension, "ppm") == 0) {
        struct ppmImage* ppmImageData = loadPPM(filename);

        if (ppmImageData == NULL) {
            fprintf(stderr, "Error opening ppm file");
            free(img);
            return NULL;
        }
        img->w = ppmImageData->w;
        img->h = ppmImageData->h;
        img->data = ppmImageData->data;
        free(ppmImageData);

    } else {
        fprintf(stderr, "Unsupported format");
        free(img);
        return NULL;
    }

    return img;
}
