#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H
#include "mathematic.h"
#define ILPF 1
#define BLPF 2
#define GLPF 3

void IdealLPFilter(double *Filter,int width,int height,double cut_off_frequency);
void ButterworthLPFilter(double *Filter,int width,int height,double cut_off_frequency,int n);
void GaussianLPFilter(double *Filter,int width,int height,double cut_off_frequency);
#endif