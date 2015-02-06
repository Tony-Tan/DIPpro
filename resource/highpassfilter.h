#ifndef HIGHPASSFILTER_H
#define HIGHPASSFILTER_H
#include "mathematic.h"
#define IHPF 4
#define BHPF 5
#define GHPF 6

void IdealHPFilter(double *Filter,int width,int height,double cut_off_frequency);
void ButterworthHPFilter(double *Filter,int width,int height,double cut_off_frequency,int n);
void GaussianHPFilter(double *Filter,int width,int height,double cut_off_frequency);
void HighFrequencyEmphasisFilter(double *Filter,int width,int height,int filter_type,double cut_off_frequency,double k1,double k2,double param1);
#endif