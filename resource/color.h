#ifndef COLOR_H
#define COLOR_H

#include <stdio.h>
#include <stdlib.h>
#include "mathematic.h"
#include <limits.h>
#include <float.h>
#define MIN3(x,y,z) (((x)>(y))?(((y)>(z))?(z):(y)):((x)>(z)?(z):(x)))
#define GMAX 255
typedef struct Chanel3_ RGB;
typedef struct Chanel3_ XYZ;
typedef struct Chanel3_ CMY;
typedef struct Chanel3_ sRGB;
typedef struct Chanel3_ YIQ;
typedef struct Chanel3_ YUV;
typedef struct Chanel3_ YCbCr;
typedef struct Chanel3_ YC1C2;
typedef struct Chanel3_ LAB;
typedef struct Chanel3_ LUV;
typedef struct Chanel3_ HSV;
typedef struct Chanel3_ HSI;
typedef struct Chanel3_ C3;


struct Chanel3_{
    double c1;
    double c2;
    double c3;
};
struct Chanel4_{
    double c1;
    double c2;
    double c3;
    double c4;
};


void Split(C3 *src ,double *dst1,double *dst2,double *dst3,int width,int height);
void Merge(double *src1,double *src2,double *src3,C3 *dst ,int width,int height);

void RGB2XYZ(RGB *src ,XYZ *dst,int width,int height);
void XYZ2RGB(XYZ *src ,RGB *dst,int width,int height);

void RGB2CMY(RGB *src ,CMY *dst,int width,int height);
void CMY2RGB(CMY *src ,RGB *dst,int width,int height);

void RGB2sRGB(RGB *src ,sRGB *dst,int width,int height);
void sRGB2RGB(sRGB *src ,RGB *dst,int width,int height);

void RGB2YIQ(RGB *src ,YIQ *dst,int width,int height);
void YIQ2RGB(YIQ *src ,RGB *dst,int width,int height);

void RGB2YUV(RGB *src ,YUV *dst,int width,int height);
void YUV2RGB(YUV *src ,RGB *dst,int width,int height);

void RGB2YCbCr(RGB *src ,YCbCr *dst,int width,int height);
void YCbCr2RGB(YCbCr *src ,RGB *dst,int width,int height);

void RGB2YC1C2(RGB *src ,YC1C2 *dst,int width,int height);
void YC1C22RGB(YC1C2 *src ,RGB *dst,int width,int height);

void RGB2LAB(RGB *src ,LAB *dst,int width,int height);
void LAB2RGB(LAB *src ,RGB *dst,int width,int height);

void RGB2LUV(RGB *src ,LUV *dst,int width,int height);
void LUV2RGB(LUV *src ,RGB *dst,int width,int height);

void RGB2HSV(RGB *src ,HSV *dst,int width,int height);
void HSV2RGB(HSV *src ,RGB *dst,int width,int height);

void RGB2HSI(RGB *src ,HSI *dst,int width,int height);
void HSI2RGB(HSI *src ,RGB *dst,int width,int height);
#endif