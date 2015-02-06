#ifndef IMAGE_FFT_H
#define IMAGE_FFT_H
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include "Fourer.h"
void FFT_Shift(double * src,int size_w,int size_h);
void ImageFFT(IplImage * src,Complex * dst);
void Nomalsize(double *src,double *dst,int size_w,int size_h);
void getAmplitudespectrum(Complex * src,int size_w,int size_h,IplImage *dst);
void ImageIFFT(Complex *src,IplImage *dst,int size_w,int size_h);


#endif