#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>
#include <stdlib.h>
#include "mathematic.h"
#include <limits.h>
#include <float.h>
#define MIN3(x,y,z) (((x)>(y))?(((y)>(z))?(z):(y)):((x)>(z)?(z):(x)))
typedef struct RGB_ RGB;
typedef struct HSI_ HSI;
struct RGB_{
    double r;
    double g;
    double b;
};
struct HSI_{
    double h;
    double s;
    double i;
};

void RGB2HSV(RGB *src ,HSI *dst,int width,int height);
void HSV2RGB(HSI *src ,RGB *dst,int width,int height);


#endif