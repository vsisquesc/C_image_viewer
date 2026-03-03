#include "ppm.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void skipComments(FILE* f) {
    int c;

    do {
        c = fgetc(f);
    } while (isspace(c));

    while (c == '#') {
        while (c != '\n' && c != EOF) {
            c = fgetc(f);
        }
        do {
            c = fgetc(f);
        } while (isspace(c));
    }

    if (c != EOF) {
        ungetc(c, f);
    }
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
        goto error;
    }

    if (strcmp(img->format, "P3") != 0 && strcmp(img->format, "P6") != 0) {
        fprintf(stderr, "Formato PPM no soportado\n");
        goto error;
    }

    /* --- WIDTH / HEIGHT / DEPTH --- */
    skipComments(f);
    if (fscanf(f, "%d", &img->w) != 1)
        goto error;

    skipComments(f);
    if (fscanf(f, "%d", &img->h) != 1)
        goto error;

    skipComments(f);
    if (fscanf(f, "%d", &img->depth) != 1)
        goto error;

    if (img->w <= 0 || img->h <= 0 || img->depth <= 0) {
        goto error;
    }

    fgetc(f); /* consumir whitespace */

    size_t pixels = (size_t)img->w * img->h;

    img->data = malloc(pixels * 3);
    if (!img->data)
        goto error;

    /* --- DATOS --- */
    if (strcmp(img->format, "P6") == 0) {
        if (fread(img->data, 3, pixels, f) != pixels) {
            goto error;
        }
    } else {
        for (size_t i = 0; i < pixels * 3; i++) {
            int v;
            if (fscanf(f, "%d", &v) != 1)
                goto error;
            if (v < 0)
                v = 0;
            if (v > img->depth)
                v = img->depth;
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

error:
    fclose(f);
    free(img->data);
    free(img);
    return NULL;
}
