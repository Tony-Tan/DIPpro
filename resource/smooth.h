#ifndef SMOOTH_H
#define SMOOTH_H
#include <stdlib.h>
#include "convolution.h"
#include "mathematic.h"


#define SMOOTH_GAUSSIAN 0
#define SMOOTH_MEDIAN 1
#define SMOOTH_BILATERAL 2
#define SMOOTH_MEAN 3
#define NLMF_FUN_A 0   //arithmetic
#define NLMF_FUN_H 1
#define NLMF_FUN_G 2
#define NLMF_MASK_M 0
#define NLMF_MASK_G 1
void MedianFilter(double *src,double *dst,int width,int height,int m_width,int m_height);
void GaussianMask(double *mask,int width,int height,double deta);
void GaussianFilter(double *src,double *dst,int width,int height,int m_width,int m_height,double deta);
void MeanFilter(double *src,double *dst,int width,int height,int m_width,int m_height);
void MeanMask(double *mask,int width,int height);
double BilateralWindow(double *window,int width,int height,double deta_d,double deta_r);
void BilateralFilter(double *src,double *dst,int width,int height,int m_width,int m_height,double deta_d,double deta_r);
void MeanFilter(double *src,double *dst,int width,int height,int m_width,int m_height);
void NoLinearMeanFilter(double *src,double *dst,int width,int height,int Mask_type,int m_width,int m_height,int function_type,double param);
#endif