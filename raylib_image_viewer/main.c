#include "color.h"
#include "image.h"

#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    struct ImageData* img = loadImage(filename);

    if (img == NULL) {
        fprintf(stderr, "Error opening file");
        return 1;
    }

    enum imageMode imgMode = strcmp(mode, "gray") == 0 ? GRAYSCALE : RGB;

    const int screenWidth = img->w;
    const int screenHeight = img->h;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();

        for (int y = 0; y < img->h; y++) {
            for (int x = 0; x < img->w; x++) {
                int i = (y * img->w + x) * 3;

                unsigned char r = img->data[i];
                unsigned char g = img->data[i + 1];
                unsigned char b = img->data[i + 2];
                struct Color c = {r, g, b, 255};

                if (imgMode == GRAYSCALE) {
                    double value = 0.299f * r + 0.587f * g + 0.114f * b;
                    c.r = value;
                    c.g = value;
                    c.b = value;
                }
                DrawRectangle(x, y, 1, 1, c);
            }
        }

        EndDrawing();
    }

    return 0;
}
