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
/*********************************************************************************/
/*********************************************************************************/
/*双峰型直方图，经过直方图平滑后呈现出双峰后找出谷底，以此值为阈值划分灰度值
 *平滑直方图采用1/4[1 2 1]的模板
 *判断是否是双峰采用1阶微分，判断正负性
 *直方图非双峰不能使用该方法。
 */

//直方图从int转换为double
void Hist_int2double(int *hist,double *hist_d){
    for(int i=0;i<GRAY_LEVEL;i++)
        hist_d[i]=(double)hist[i];
}
//平滑直方图，是指呈现双峰形状
void SmoothHist(double *hist,double *dsthist){
    double *histtemp=(double *)malloc(sizeof(double)*GRAY_LEVEL);
    histtemp[0]=0.0;
    histtemp[GRAY_LEVEL]=0.0;
    for(int i=0;i<GRAY_LEVEL;i++)
        histtemp[i]=hist[i];
    for(int i=1;i<GRAY_LEVEL-1;i++){
        histtemp[i]=0.25*histtemp[i-1]+0.5*histtemp[i]+0.25*histtemp[i+1];
    }
    for(int i=0;i<GRAY_LEVEL;i++)
        dsthist[i]=histtemp[i];
    free(histtemp);
}
//判断是否是双峰直方图，如果是返回谷底，否则返回0
//#define DOUBLEHUMP_BOTTOM 1
//#define DOUBLEHUMP_MEANHUMP 2
int isDoubleHump(double *hist,int returnvalue){
    double * diffHist=(double *)malloc(sizeof(double)*GRAY_LEVEL);
    int * statusHist=(int *)malloc(sizeof(int)*GRAY_LEVEL);
    diffHist[0]=0.0;
    for(int i=1;i<GRAY_LEVEL-1;i++){
        diffHist[i]=hist[i+1]-hist[i];
    }
    for(int i=1;i<GRAY_LEVEL;i++){
        if(diffHist[i]>0)
            statusHist[i]=1;
        else if(diffHist[i]<0)
            statusHist[i]=-1;
        else if(diffHist[i]==0&&statusHist[i-1]>=0)
            statusHist[i]=1;
        else if(diffHist[i]==0&&statusHist[i-1]<0)
            statusHist[i]=-1;
    }
/*1st order:
 *______________                ________________
 *              |              |
 *              |              |
 *              |______________|
 *status:       1             -1
 *hist:
 *0 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0 0 0
 */
    for(int i=1;i<GRAY_LEVEL-1;i++)
        if(statusHist[i]*statusHist[i+1]<0){
            if(statusHist[i]>0)
                statusHist[i]=1;
            else if(statusHist[i]<0)
                statusHist[i]=-1;
        }else{
            statusHist[i]=0;
        }
    statusHist[GRAY_LEVEL-1]=0;
 /*double hump diff:
  *______________                _______________
  *              |              |               |
  *              |              |               |
  *              |______________|               |______________
  *status:       1             -1               1
  *             top           bottom           top
  *0 0 0 0 0 0 0 1 0 0 0 0 0 0 -1 0 0 0 0 0 0 0 1 0 0 0 0
  *the arry test store nonzero
  */
    int test[4]={0,0,0,0};
    int test_num=0;
    for(int i=1;i<GRAY_LEVEL;i++){
        if(statusHist[i]!=0){
            test[test_num]=statusHist[i];
            if(test_num>=3){
                free(diffHist);
                free(statusHist);
                return 0;
            }
            test_num++;
        }
    }
    
    if(test_num==3&&test[0]==1&&test[1]==-1&&test[2]==1){
        if(returnvalue==DOUBLEHUMP_BOTTOM){
            for(int i=0;i<GRAY_LEVEL;i++)
                if(statusHist[i]==-1){
                    free(diffHist);
                    free(statusHist);
                    return i;
                }
        }else if(returnvalue==DOUBLEHUMP_MEANHUMP){
            int hump[2];
            for(int i=0,k=0;i<GRAY_LEVEL;i++)
                if(statusHist[i]==1){
                    hump[k]=i;
                    k++;
                }
            free(diffHist);
            free(statusHist);
            return (hump[0]+hump[1])/2;
        }
    }
    free(diffHist);
    free(statusHist);
    return 0;
}
//谷底法阈值分割，适用于直方图是双峰的。
void ValleyBottomThreshold(double *src,double *dst,int width,int height,int type){
    int *hist=(int *)malloc(sizeof(int)*GRAY_LEVEL);
    double *hist_d=(double *)malloc(sizeof(double)*GRAY_LEVEL);
    setHistogram(src, hist, width, height);
    Hist_int2double(hist, hist_d);
    double threshold=0.0;
#define MAXLOOP 1000
    for(int i=0;i<MAXLOOP;i++){
        SmoothHist(hist_d, hist_d);
        if(0.0!=(threshold = (double)isDoubleHump(hist_d,DOUBLEHUMP_BOTTOM))){
            Threshold(src, dst, width, height, threshold, type);
            break;
        }
    }

}
//与谷底法类似，不是使用最小谷底值，而是使用峰值位置平均值
void MeanDoubleHumpThreshold(double *src,double *dst,int width,int height,int type){
    int *hist=(int *)malloc(sizeof(int)*GRAY_LEVEL);
    double *hist_d=(double *)malloc(sizeof(double)*GRAY_LEVEL);
    setHistogram(src, hist, width, height);
    Hist_int2double(hist, hist_d);
    double threshold=0.0;
    for(int i=0;i<MAXLOOP;i++){
        SmoothHist(hist_d, hist_d);
        if(0.0!=(threshold = (double)isDoubleHump(hist_d,DOUBLEHUMP_MEANHUMP))){
            Threshold(src, dst, width, height, threshold, type);
            break;
        }
    }
    
}
/*
 *OTSU 算法
 *otsu 算法使用贝叶斯分类原理得到最好聚类
 *
 *
 */
////
void setHist2One(double *hist_d,double *dst_hist_d){
    double sum=0.0;
    for(int i=0;i<GRAY_LEVEL;i++)
        sum+=hist_d[i];
    if(sum!=0)
        for(int i=0;i<GRAY_LEVEL;i++)
            dst_hist_d[i]=hist_d[i]/sum;
    
}
double findMaxDeta(double *hist_d){
    double max_deta=-1.0;
    double max_deta_location=0.0;
    double m_g=0.0;
    
    for(int i=0;i<GRAY_LEVEL;i++)
        m_g+=i*hist_d[i];
    
    
    for(int i=0;i<GRAY_LEVEL;i++){
        double p1=0.0;
        double m1=0.0;
        double deta=0.0;
        for(int j=0;j<i;j++){
            p1+=hist_d[j];
            m1+=j*hist_d[j];
        }
        deta=p1*(m1-m_g)*(m1-m_g)/(1-p1);
        if(deta>max_deta){
            max_deta_location=i;
            max_deta=deta;
        }
    }
    return max_deta_location;
}
void OTSUThreshold(double *src,double *dst,int width,int height,int type){
    int hist[GRAY_LEVEL];
    double hist_d[GRAY_LEVEL];
    setHistogram(src, hist, width, height);
    Hist_int2double(hist, hist_d);
    setHist2One(hist_d, hist_d);
    double threshold=findMaxDeta(hist_d);
    
    Threshold(src, dst, width, height, threshold, type);
}
/*
 *
 *
 *
 *
 */
void LoGThreshold(double *src,double *dst,int width,int height,double threshold,int type){
    double *mask=(double *)malloc(sizeof(double)*width*height);
    double *temp=(double *)malloc(sizeof(double)*width*height);
    GaussianFilter(src, temp, width, height, 10, 10, 1.6);
    double max=LoG(temp, mask, width, height, 3, 3, 0.5, threshold);
    Threshold(mask, mask, width, height, max*0.7, THRESHOLD_TYPE3);
    Mask(src, temp, mask, width, height);
    ///////////////////////////////////////////////////////////////////////////
    int hist[GRAY_LEVEL];
    double hist_d[GRAY_LEVEL];
    setHistogram(temp, hist, width, height);
    Hist_int2double(hist, hist_d);
    setHist2One(hist_d, hist_d);
    double threshold_=findMaxDeta(hist_d);
    Threshold(src, dst, width, height, threshold_, type);
    free(mask);
    free(temp);

}
