#ifndef HIGHPASSFILTER_H
#define HIGHPASSFILTER_H
#include <math.h>
#include "fr.h"
static double Distance(int x,int y,int c_x,int c_y);
void IdealHPFilter(double *Filter,int width,int height,double cut_off_frequency);
void ButterworthHPFilter(double *Filter,int width,int height,double cut_off_frequency,int n);
void GaussianHPFilter(double *Filter,int width,int height,double cut_off_frequency);
void HighFrequencyEmphasisFilter(double *Filter,int width,int height,int filter_type,double cut_off_frequency,double k1,double k2,double param1);
#endif