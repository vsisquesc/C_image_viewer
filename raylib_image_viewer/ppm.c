#include "ppm.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void trimSpaces(FILE* f, char* c) {

    do {
        *c = fgetc(f);
    } while (isspace(*c));
}

static void skipComments(FILE* f) {
    char c;

    // trim
    trimSpaces(f, &c);

    while (c == '#') {
        while (c != '\n' && c != EOF) {
            c = fgetc(f);
        }

        trimSpaces(f, &c);
    }

    if (c != EOF) {
        ungetc(c, f);
    }
}

static void* error(FILE* f, struct ppmImage* img) {
    fclose(f);
    free(img->data);
    free(img);
    img = NULL;
    return NULL;
}

struct ppmImage* loadPPM(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        perror("fopen");
        return NULL;
    }

    struct ppmImage* img = calloc(1, sizeof *img);
    if (!img) {
        fclose(f);
        return NULL;
    }

    /* --- FORMATO --- */
    if (fscanf(f, "%2s", img->format) != 1) {
        return error(f, img);
    }

    if (strcmp(img->format, "P3") != 0 && strcmp(img->format, "P6") != 0) {
        fprintf(stderr, "Formato PPM no soportado\n");
        return error(f, img);
    }

    /* --- WIDTH / HEIGHT / DEPTH --- */
    skipComments(f);
    if (fscanf(f, "%d", &img->w) != 1) {
        return error(f, img);
    }

    skipComments(f);
    if (fscanf(f, "%d", &img->h) != 1) {
        return error(f, img);
    }

    skipComments(f);
    if (fscanf(f, "%d", &img->depth) != 1) {
        return error(f, img);
    }

    if (img->w <= 0 || img->h <= 0 || img->depth <= 0) {
        return error(f, img);
    }
    skipComments(f);

    size_t pixels = (size_t)img->w * img->h;
    img->data = malloc(pixels * 3);
    if (!img->data) {
        return error(f, img);
    }

    /* --- DATOS --- */
    if (strcmp(img->format, "P6") == 0) {
        // Binario
        if (fread(img->data, 3, pixels, f) != pixels) {
            return error(f, img);
        }
    } else {
        // Texto (P3)
        for (size_t i = 0; i < pixels * 3; i++) {
            int v;
            if (fscanf(f, "%d", &v) != 1) {
                return error(f, img);
            }
            if (v < 0) {
                v = 0;
            }
            if (v > img->depth) {
                v = img->depth;
            }
            img->data[i] = (unsigned char)v;
        }
    }

    if (img->depth != 255) {
        for (size_t i = 0; i < pixels * 3; i++) {
            img->data[i] =
                (unsigned char)((img->data[i] * 255) / img->depth);
        }
    }

    fclose(f);
    return img;
}
