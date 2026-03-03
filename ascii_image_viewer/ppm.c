#include "ppm.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void skipComments(FILE* f) {
    int c;

    // Saltar espacios y saltos de línea iniciales
    do {
        c = fgetc(f);
    } while (isspace(c));

    // Mientras haya un comentario
    while (c == '#') {
        // saltar hasta final de línea
        while (c != '\n' && c != EOF) {
            c = fgetc(f);
        }

        // saltar espacios / saltos de línea siguientes
        do {
            c = fgetc(f);
        } while (isspace(c));
    }

    // Devolver el primer carácter útil al flujo
    if (c != EOF) {
        ungetc(c, f);
    }
}

struct ppmImage loadImage(FILE* f) {
    struct ppmImage img = {0};

    if (!f) {
        perror("fopen");
        return img;
    }

    // --- FORMATO ---
    fscanf(f, "%2s", img.format);

    if (strcmp(img.format, "P3") != 0 && strcmp(img.format, "P6") != 0) {
        fprintf(stderr, "Formato PPM no soportado\n");
        fclose(f);
        return img;
    }

    // --- WIDTH / HEIGHT / DEPTH ---
    skipComments(f);
    fscanf(f, "%d", &img.w);

    skipComments(f);
    fscanf(f, "%d", &img.h);

    skipComments(f);
    fscanf(f, "%d", &img.depth);

    fgetc(f); // consumir un whitespace tras depth

    int pixels = img.w * img.h;
    img.data = malloc(pixels * 3);

    // --- DATOS ---
    if (strcmp(img.format, "P6") == 0) {
        // Binario
        fread(img.data, 3, pixels, f);
    } else {
        // Texto (P3)
        for (int i = 0; i < pixels * 3; i++) {
            int v;
            fscanf(f, "%d", &v);
            img.data[i] = (unsigned char)v;
        }
    }
    if (img.depth != 255) {
        for (int i = 0; i < pixels * 3; i++) {

            img.data[i] = (int)(((float)img.data[i] / img.depth) * 255.0f);
        }
    }

    fclose(f);
    return img;
}
