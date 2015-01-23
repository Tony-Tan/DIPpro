#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H
#include <math.h>
static double Distance(int x,int y,int c_x,int c_y);
void IdealLPFilter(double *Filter,int width,int height,double cut_off_frequency);
void ButterworthLPFilter(double *Filter,int width,int height,double cut_off_frequency,int n);
void GaussianLPFilter(double *Filter,int width,int height,double cut_off_frequency);
#endif