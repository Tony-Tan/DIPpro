#ifndef FILTER_H
#define FILTER_H


#include "highpassfilter.h"
#include "lowpassfilter.h"
#include "Fourer.h"
#include "Image_FFT.h"
#include "Homomorphicfilter.h"
#define HOMPF 7
#define HFEPF 8

void MatrixMulti_R_C(double *src1,Complex *src2,Complex *dst,int size);//m(1,1)=a(1,1)*b(1,1);
int ChangtoPower2(int size);
void ResizeMatrix4FFT(IplImage *src,IplImage **dst);
double FrequencyFiltering(IplImage *src,IplImage *dst,int filter_type,double param1,int param2,double param3,double param4,double param5,int getpowerflag);
double getPower(Complex *src,int totalsize);
#endif