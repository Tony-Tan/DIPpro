#ifndef PSEUDO_COLOR_H
#define PSEUDO_COLOR_H
#define GRAY2HSV 1.23046875 //HSVMAX/GRAY_LEVEL
#define HSVMAX 315.0
#define HIGHVALUE_EQU_RED 1
#define LOWVALUE_EQU_RED 0
#include "color.h"

void Gray2Color(double *src,RGB* dst,int width,int height,int type);

#endif