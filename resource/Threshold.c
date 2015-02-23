//
//  Threshold
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/02/23.
//  Copyright (c) 2015年 谭升. All rights reserved.
//

/********************************************************************************
 *THRESHOLD_TYPE1       |       dst(x,y)=src(x,y)>T?src(x,y):Minvalue;          *
 *THRESHOLD_TYPE2       |       dst(x,y)=src(x,y)>T?Maxvalue:src(x,y);          *
 *THRESHOLD_TYPE3       |       dst(x,y)=src(x,y)>T?Maxvalue:Minvalue;          *
 *THRESHOLD_TYPE4       |       dst(x,y)=src(x,y)>T?Minvalue:Maxvalue;          *
 ********************************************************************************/
#include "Threshold.h"

void Threshold(double *src,double *dst,int width,int height,double threshold,int type){
    if(type==THRESHOLD_TYPE1){
        for(int i=0;i<width*height;i++)
            dst[i]=src[i]>threshold?src[i]:MIN_VALUE;
    }else if(type==THRESHOLD_TYPE2){
        for(int i=0;i<width*height;i++)
            dst[i]=src[i]>threshold?MAX_VALUE:src[i];
    }else if(type==THRESHOLD_TYPE3){
        for(int i=0;i<width*height;i++)
            dst[i]=src[i]>threshold?MAX_VALUE:MIN_VALUE;
    }else if(type==THRESHOLD_TYPE4){
        for(int i=0;i<width*height;i++)
            dst[i]=src[i]>threshold?MIN_VALUE:MAX_VALUE;
    }
    
}
/*********************************************************************************/
/*********************************************************************************/
////计算从start到end的直方图的平均值
double getMeaninHist(int start,int end,int *hist){
    int hist_count=0;
    double hist_value=0;
    for(int i=start;i<end;i++){
        hist_count+=hist[i];
        hist_value+=(double)hist[i]*i;
    }
    return hist_value/(double)hist_count;
}
/*********************************************************************************/
/*********************************************************************************/
//均值法求阈值
//阈值等于全图的像素的平均值
void MeanThreshold(double *src,double *dst,int width,int height,int type){
    int hist[GRAY_LEVEL];
    double threshold_value=0.0;
    InitHistogram(hist);
    setHistogram(src, hist, width,height);    
    threshold_value=getMeaninHist(0, GRAY_LEVEL, hist);
    Threshold(src,dst, width, height, threshold_value,type);
}
/*********************************************************************************/
/*********************************************************************************/
//阈值法，p分位法
//p分位为统计学方法
//当p为5时为中位数
void PtileThreshold(double *src,double *dst,double p_value,int width,int height,int type){/*0<p_value<1*/
    int total_pix_count=width*height;
    int pix_count=0;
    int hist[GRAY_LEVEL];
    double threshold_value=0.0;
    InitHistogram(hist);
    setHistogram(src, hist, width,height);
    for(int i=0;i<GRAY_LEVEL;i++){
        pix_count+=hist[i];
        if(pix_count>=(int)((double)total_pix_count*p_value)){
            threshold_value=(double)i;
            break;
        }
    }
    Threshold(src,dst, width, height, threshold_value,type);
}
/*********************************************************************************/
/*********************************************************************************/
//迭代法求阈值，初始化一个阈值
//将直方图分为两部分
//求出两部分的均值
//这两个均值的均值为新的阈值，迭代这些步骤
void IterativeThreshold(double *src,double *dst,double deta_t,int width,int height,int type){
    
    int hist[GRAY_LEVEL];
    InitHistogram(hist);
    setHistogram(src, hist, width,height);
    int hist_min=findHistogramMax(hist);
    int hist_max=findHistogramMin(hist);
    double threshold_value=(hist_max+hist_min)/2.0;
    double threshold_last=threshold_value;
    while (threshold_last-threshold_value>=deta_t||
           threshold_last-threshold_value<=-deta_t) {
        threshold_last=threshold_value;
        double mean1=getMeaninHist(0, (int)threshold_value, hist);
        double mean2=getMeaninHist((int)threshold_value,hist_max+1, hist);
        threshold_value=(mean1+mean2)/2.0;
    }
    Threshold(src,dst, width, height, threshold_value,type);
}