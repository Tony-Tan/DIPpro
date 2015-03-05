#ifndef REGIONSEGMENT_H
#define REGIONSEGMENT_H
#include "mathematic.h"

void RegionGrow(double *src,double *dst,double *seed,int width,int height,double param);
void RegionSplit(double *src,double *dst,int width,int height,double mean_param1,double mean_param2,double variance_param1,double variance_param2);



#endif