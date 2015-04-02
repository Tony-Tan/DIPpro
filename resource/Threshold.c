/*
 * 中文说明
 * 如果你下载或使用本代码说明你已阅读并同意本声明
 * 此代码由谭升（Tony）开发，并允许任何人，或团体下载，使用此代码用于任何商业或非商业用途
 * 使用本代码时必须复制此声明
 * 本函数库版权归谭升所有.
 * 如有第三方，例如部分使用OpenCV函数，OpenCV函数库版权属于Intel公司，将在后续版本中去除这些函数，特此声明
 
 * English
 * If you download or use the code that you have read and agree to this statement,
 * This code by Tan Sheng (Tony) development, and allow any person, or group to download,
 * use for any commercial or non-commercial use
 * Use the code must be copied to this statement
 * Copyright (C) 2015,Tony, all rights reserved.
 * Part of the use of the OpenCV function, OpenCV function library copyright belongs
 * to Intel company, will remove these functions in subsequent versions, hereby declare
 */
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
//当p为0.5时为中位数
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
//deta_t 精确度，当迭代n次以后阈值tn与第n-1次迭代结果tn-1相差小于deta_t时，迭代停止。
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
    printf("deta_t:%g  threshold:%g\n",deta_t,threshold_value);
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
    diffHist[GRAY_LEVEL-1]=0.0;
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
    for(int i=1;i<GRAY_LEVEL-1;i++){
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
            for(int i=1;i<GRAY_LEVEL;i++)
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
    free(hist);
    free(hist_d);

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
    free(hist);
    free(hist_d);
    
}
/*
 *OTSU 算法
 *otsu 算法使用贝叶斯分类原理得到最好聚类
 *
 *
 */
//归一化直方图

void setHist2One(double *hist_d,double *dst_hist_d){
    double sum=0.0;
    for(int i=0;i<GRAY_LEVEL;i++)
        sum+=hist_d[i];
    if(sum!=0)
        for(int i=0;i<GRAY_LEVEL;i++)
            dst_hist_d[i]=hist_d[i]/sum;
    
}
//计算公式中最大的deta，并返回直方图灰度
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
        for(int j=0;j<=i;j++){
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
    printf("threshold:%g\n",threshold);
    Threshold(src, dst, width, height, threshold, type);
}
/*对于小目标物体
 *使用边缘检测结果作为MASK
 *得到MASK为1处的原图灰度集合
 *对这个集合做阈值分割
 *的到最终的结果
 */
void SobelThreshold(double *src,double *dst,int width,int height,double sobel_threshold,int type){
    double *mask=(double *)malloc(sizeof(double)*width*height);
    double *temp=(double *)malloc(sizeof(double)*width*height);
    //use 0.05*width and 0.05*height gaussian mask smooth src
    GaussianFilter(src, temp, width, height, width/25,height/25, (double)width/150.);
    double max=Sobel(temp, mask, NULL, width, height, 5);
    Threshold(mask, mask, width, height, max*sobel_threshold, THRESHOLD_TYPE3);
    ///////////////////////////////////////////////////////////////////////////
    int hist[GRAY_LEVEL];
    double hist_d[GRAY_LEVEL];
    InitHistogram(hist);
    for(int i=0;i<width*height;i++)
        if(mask[i]!=0.0)
            hist[(int)src[i]]++;
    Hist_int2double(hist, hist_d);
    setHist2One(hist_d, hist_d);
    double threshold=findMaxDeta(hist_d);//
    printf("Threshold:%g \n",threshold);
    Threshold(src, dst, width, height, threshold, type);
    free(mask);
    free(temp);

}


/*局部阈值
 *使用均值和标准差作为判定依据
 *输入参数包括邻域大小，均值系数，以及标准差系数
 *
 *
 */
void LocalThreshold(double *src,double *dst,int width,int height,int w_size,double mean_param,double std_dev_param){
    double *temp=(double *)malloc(sizeof(double)*width*height);

    Zero(temp, width,height);
    double mean_g=matrixMean(src, width, height);
    for(int j=w_size/2;j<height-w_size/2;j++){
        for(int i=w_size/2;i<width-w_size/2;i++){
            double deta=0.0;
            double mean=0.0;
            double pix_value=src[j*width+i];
            //local mean
            for(int m=-w_size/2;m<w_size/2+1;m++){
                for(int n=-w_size/2;n<w_size/2+1;n++){
                    mean+=src[(j+m)*width+i+n];
                }
            }
            mean/=(double)(w_size*w_size);
            //local deta
            for(int m=-w_size/2;m<w_size/2+1;m++){
                for(int n=-w_size/2;n<w_size/2+1;n++){
                    deta+=(src[(j+m)*width+i+n]-mean)*(src[(j+m)*width+i+n]-mean);
                }
            }
            
            deta/=(double)(w_size*w_size);
            deta=sqrt(deta);
            if(pix_value>mean_param*mean_g&&pix_value>std_dev_param*deta){
                temp[j*width+i]=255.0;
            }
        }
    }
    matrixCopy(temp, dst, width, height);
    free(temp);
}