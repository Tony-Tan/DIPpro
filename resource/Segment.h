#ifndef SEGMENT_H
#define SEGMENT_H

#include "convolution.h"
#include "smooth.h"
#define SHARPEN_LAP_0 0
#define SHARPEN_LAP_1 1
#define SHARPEN_LAP_2 2
#define SHARPEN_LAP_3 3

#define LAPLACE_MASK_SIZE 3
#define SOBEL_MASK_SIZE 3
#define ROBERT_MASK_SIZE 3

void Sobel(double *src,double *dst,int width,int height);
void Laplace(double *src,double *dst,int width,int height,int mask_type);
void Robert(double *src,double *dst,int width,int height);

#endif