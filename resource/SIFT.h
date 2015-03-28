#ifndef SIFT_H
#define SIFT_H
#include "mathematic.h"
#include "smooth.h"
#include "Resize.h"
#include "smooth.h"
typedef struct SIFT_Feature_ SIFT_Feature;

struct SIFT_Feature_{
    double x;
    double y;
    double scale;
    double orientation;
    int des_vector[128];
    struct SIFT_Feature_ *next;
};

void ScaleSpace(double *src,double * dst[],double *delta_arry,int width,int height,double delta_max,int k);
double * SIFT_Resize(double *src,int width,int height);
void ReleaseMatArr(double * dst[],int k);
void DOG_Scale(double *src[],double * dst[],int width,int height,int k);
void SIFT(double *src,SIFT_Feature **dst,int width,int height,int scale_k,int octave);
void ReleaseSIFTlist(SIFT_Feature * head);
#endif