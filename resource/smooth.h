#ifndef SMOOTH_H
#define SMOOTH_H
#include "convolution.h"
#include <math.h>
#include <stdlib.h>

#define SMOOTH_GAUSSIAN
#define SMOOTH_MEDIAN
#define SMOOTH_BILATERAL
#define SMOOTH_MEAN
static double Distance(int x,int y,int c_x,int c_y);
void MedianFilter(IplImage *src,IplImage *dst,int width,int height);
void GaussianMask(double *mask,int width,int height,double deta);
void GaussianFilter(IplImage *src,IplImage *dst,int width,int height,double deta);
void MeanMask(double *mask,int width,int height);
double BilateralWindow(double *window,int width,int height,double deta_d,double deta_r);
void BilateralFilter(IplImage *src,IplImage *dst,int width,int height,double deta_d,double deta_r);
void Smooth(IplImage *src,IplImage *dst,int Smooth_type,int width,int height);
void MeanFilter(IplImage *src,IplImage *dst,int width,int height);
#endif