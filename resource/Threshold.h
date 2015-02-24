//
//  Threshold
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/02/23.
//  Copyright (c) 2015年 谭升. All rights reserved.
//
#ifndef THRESHOLD_H
#define THRESHOLD_H
#define MAX_VALUE 255.0
#define MIN_VALUE 0.0
/********************************************************************************
 *THRESHOLD_TYPE1       |       dst(x,y)=src(x,y)>T?src(x,y):Minvalue;          *
 *THRESHOLD_TYPE2       |       dst(x,y)=src(x,y)>T?Maxvalue:src(x,y);          *
 *THRESHOLD_TYPE3       |       dst(x,y)=src(x,y)>T?Maxvalue:Minvalue;          *
 *THRESHOLD_TYPE4       |       dst(x,y)=src(x,y)>T?Minvalue:Maxvalue;          *
 ********************************************************************************/
#define THRESHOLD_TYPE1 1
#define THRESHOLD_TYPE2 2
#define THRESHOLD_TYPE3 3
#define THRESHOLD_TYPE4 4

#define DOUBLEHUMP_BOTTOM 1
#define DOUBLEHUMP_MEANHUMP 2
#include "Histogram.h"
#include "mathematic.h"
#include <stdlib.h>
void Threshold(double *src,double *dst,int width,int height,double threshold,int type);
void MeanThreshold(double *src,double *dst,int width,int height,int type);
void PtileThreshold(double *src,double *dst,double p_value,int width,int height,int type);/*0<p_value<1*/
void IterativeThreshold(double *src,double *dst,double deta_t,int width,int height,int type);
void ValleyBottomThreshold(double *src,double *dst,int width,int height,int type);
void Hist_int2double(int *hist,double *hist_d);
void SmoothHist(double *hist,double *dsthist);
int isDoubleHump(double *hist,int returnvalue);
void MeanDoubleHumpThreshold(double *src,double *dst,int width,int height,int type);
#endif