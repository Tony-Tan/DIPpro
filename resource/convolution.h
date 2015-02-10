#ifndef CONVOLUTION_H
#define CONVOLUTION_H
#include "Fourer.h"
//void RotateRealMatrix(double *matrix,int width,int height);
void RotateComplexMatrix(Complex *matrix,int width,int height);
void RealRelevant(double *src,double *dst,double *mask,int width,int height,int m_width,int m_height);
void ComplexRelevant(Complex* src,Complex *dst,Complex *mask,int width,int height,int m_width,int m_height);
void RealConvolution(double *src,double *dst,double *mask,int width,int height,int m_width,int m_height);
void RotateRealMatrix(double *matrix,double *dst,int width,int height);
void ComplexCovolution(Complex* src,Complex *dst,Complex *mask,int width,int height,int m_width,int m_height);
#endif