#ifndef SIFT_H
#define SIFT_H
#include "mathematic.h"
#include "smooth.h"
#include "Resize.h"
double ScaleSpace(double *src,double * dst[],int width,int height,double delta_max,int k);
double * SIFT_Resize(double *src,int width,int height);
void ReleaseMatArr(double * dst[],int k);
void DOG_Scale(double *src[],double * dst[],int width,int height,int k);
void SIFT(double *src,double *dst,int width,int height,int scale_k,int octave);
#endif