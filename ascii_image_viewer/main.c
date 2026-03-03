#include "color.h"
#include "ppm.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct imageBuffer {
    char* data;
    enum imageMode m;
    int w;
    int h;
    int c;
};

struct imageBuffer initBuffer(struct ppmImage* img, enum imageMode mode) {

    int c = mode == RGB ? C : 1;
    struct imageBuffer output = {
        .data = malloc(sizeof(char) * img->w * img->h * c),
        .m = mode,
        .w = img->w,
        .h = img->h,
        .c = c};
    // static const char* grad = "@%#*+=-:. ";
    static const char* grad = " .:-=+*#%@";
    int levels = strlen(grad);
    for (int y = 0; y < img->h; y++) {
        for (int x = 0; x < img->w; x++) {
            int i = (y * img->w + x) * 3;

            unsigned char r = img->data[i];
            unsigned char g = img->data[i + 1];
            unsigned char b = img->data[i + 2];

            if (mode == RGB) {
                int iR, iG, iB;
                iR = (0 * img->w * img->h) + y * img->w + x;
                iG = (1 * img->w * img->h) + y * img->w + x;
                iB = (2 * img->w * img->h) + y * img->w + x;

                output.data[iR] = grad[(r * (levels - 1)) / 255];
                output.data[iG] = grad[(g * (levels - 1)) / 255];
                output.data[iB] = grad[(b * (levels - 1)) / 255];

            } else {
                int iGray = y * img->w + x;
                double value = 0.299f * r + 0.587f * g + 0.114f * b;

                output.data[iGray] = grad[((int)value * (levels - 1)) / 255];
            }
        }
    }
    return output;
}

void printBuffer(struct imageBuffer* buffer) {
    int plane_size = buffer->w * buffer->h;

    for (enum channel c = R; c < buffer->c; c++) {
        printf("\033[H"); // mover cursor una sola vez

        printColor(buffer->m, c);

        char* plane = buffer->data + c * plane_size;

        for (int y = 0; y < buffer->h; y++) {
            fwrite(plane + y * buffer->w, 1, buffer->w, stdout);
            putchar('\n');
        }
    }

    resetColor(buffer->m);
    fflush(stdout);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <gray|rgb> <archivo>\n", argv[0]);
        return 1;
    }

    const char* mode = argv[1];
    if (strcmp(mode, "gray") && strcmp(mode, "rgb")) {
        fprintf(stderr, "Uso: %s <gray|rgb> <archivo>\n", argv[0]);
        return 1;
    }
    const char* filename = argv[2];

    FILE* f = fopen(filename, "rb");
    if (!f) {
        perror("fopen");
        return 1;
    }

    clrscr();
    struct ppmImage img = loadImage(f);

    struct imageBuffer buffer = initBuffer(&img, strcmp(mode, "gray") == 0 ? GRAY : RGB);

    while (true) {
        printBuffer(&buffer);
    }

    return 0;
}