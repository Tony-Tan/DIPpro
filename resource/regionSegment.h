#ifndef REGIONSEGMENT_H
#define REGIONSEGMENT_H
#include "Mathematic.h"

void RegionGrow(double *src,double *dst,Position * position,int p_size,int width,int height,double param);
void RegionSplit(double *src,double *dst,int width,int height,double mean_param1,double mean_param2,double variance_param1,double variance_param2);



#endif