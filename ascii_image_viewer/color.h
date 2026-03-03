#ifndef COLOR_H
#define COLOR_H

#define C 3

enum imageMode {
    RGB,
    GRAY
};

enum channel {
    R,
    G,
    B
};

void printColor(enum imageMode m, enum channel c);
void resetColor(enum imageMode m);

#endif