//
//  Fourer.h
//  Fourer
//
//  Created by 谭升 on 14/11/25.
//  Copyright (c) 2014年 谭升. All rights reserved.
//

#ifndef Fourer_h
#define Fourer_h
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mathematic.h"
#define VOID 0
#define UCHAR 1
#define CHAR 2
#define INT 3
#define FLOAT 4
#define DOUBLE 5


#define SIZE 16*16
#define VALUE_MAX 100



void getWN(double n,double size_n,Complex * dst);
void setInput(double * data,int  n);
void DFT(double * src,Complex * dst,int size);
void IDFT(Complex *src,Complex *dst,int size);
int FFTReal_remap(double * src,int size_n);
int FFTComplex_remap(Complex * src,int size_n);
void RealFFT(double * src,Complex * dst,int size_n);
void FFT(Complex * src,Complex * dst,int size_n);
void IFFT(Complex * src,Complex * dst,int size_n);
//////////////////////////////////////////////////////////////////////
void ColumnVector(Complex * src,Complex * dst,int size_w,int size_h);
int DFT2D(double *src,Complex *dst,int size_w,int size_h);
int IDFT2D(Complex *src,Complex *dst,int size_w,int size_h);
int FFT2D(double *src,Complex *dst,int size_w,int size_h);
int IFFT2D(Complex *src,Complex *dst,int size_w,int size_h);
#endif
