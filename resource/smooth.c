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
//  Smooth
//  图像平滑，高斯，均值，双边滤波
//  Created by 谭升 on 15/01/23.
//  tony.sheng.tan@gmail.com
//  Copyright (c) 2015年 谭升. All rights reserved.
//
#include "smooth.h"
#define GRAY_LEVEL 256



////////////////////高斯滤波模板生成/////////////////////
void GaussianMask(double *mask,int width,int height,double deta){
    double deta_2=deta*deta;
    double center_x=(double)width/2.0-0.5;
    double center_y=(double)height/2.0-0.5;
    double param=1.0/(2*M_PI*deta_2);
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++){
            double distance=Distance(j, i, center_x, center_y);
            mask[i*width+j]=param*exp(-(distance*distance)/(2*deta_2));
    
    }
    double sum=0.0;
    for(int i=0;i<width*height;i++)
        sum+=mask[i];
    for(int i=0;i<width*height;i++)
        mask[i]/=sum;
}


void GaussianFilter(double *src,double *dst,int width,int height,int m_width,int m_height,double deta){
    double * mask=(double *)malloc(sizeof(double)*m_width*m_height);
    GaussianMask(mask, m_width, m_height, deta);
    RealRelevant(src,dst,mask,width,height,m_width,m_height);
    free(mask);
}
////////////////////////均值滤波模板生成///////////////////////
void MeanMask(double *mask,int width,int height){
    double w=width;
    double h=height;
    double meanvalue=1.0/(w*h);
    for(int i=0;i<width*height;i++)
        mask[i]=meanvalue;
}
void MeanFilter(double *src,double *dst,int width,int height,int m_width,int m_height){

    double * mask=(double *)malloc(sizeof(double)*m_width*m_height);
   
    MeanMask(mask, m_width, m_height);
    RealRelevant(src,dst,mask,width,height,m_width,m_height);
    
    free(mask);
}
///////////////////////////中值滤波及其相关函数/////////////////////////////


//迭代，快速算法
int findMedian(int *hist,int *movein,int *moveout,int movesize,int *cursor,int median,int t){
    for(int i=0;i<movesize;i++){
        hist[movein[i]]++;
        hist[moveout[i]]--;
        if(movein[i]<median)
            (*cursor)++;
        if(moveout[i]<median)
            (*cursor)--;
    }
    
    
    if((*cursor)<t){
        for(int i=median;i<GRAY_LEVEL;i++){
            (*cursor)+=hist[i];
            if(*cursor>=t+1){
                (*cursor)-=hist[i];
                return i;
            }
        }
    }else if((*cursor)>t){
            for(int i=median-1;i>=0;i--){
                (*cursor)-=hist[i];
                if(*cursor<=t){
                    return i;
                }
            }
        }
    else if ((*cursor)==t){
            for(int i=median;i<GRAY_LEVEL;i++){
                if(hist[i]>0)
                    return i;
        }
        
    }
    return -1;
}
//初始化一行
int InitRow(double *src,int width,int height,int *hist,int row,int *cursor,int win_width,int win_height){
    int t=win_width*win_height/2+1;
    *cursor=0;
    for(int i=0;i<GRAY_LEVEL;i++)
        hist[i]=0;
    for(int j=-win_height/2;j<win_height/2+1;j++)
        for(int i=0;i<win_width;i++){
            //int pixvalue=cvGetReal2D(src, j+row, i);
            int pixvalue=src[(j+row)*width+i];
            hist[pixvalue]++;
        }
    for(int i=0;i<GRAY_LEVEL;i++){
        *cursor+=hist[i];
        if(*cursor>=t){
            *cursor-=hist[i];
            return i;
        
        }
    }
    return -1;
    
}

void MedianFilter(double *src,double *dst,int width,int height,int m_width,int m_height){
    int hist[GRAY_LEVEL];
    int median;
    int *movein=(int *)malloc(sizeof(int)*m_height);
    int *moveout=(int *)malloc(sizeof(int)*m_height);
    double *dsttemp=(double *)malloc(sizeof(double)*width*height);
    int t=m_width*m_height/2;
    for(int j=m_height/2;j<height-m_height/2-1;j++){
        int cursor=0;

        median=InitRow(src, width, height,hist,j,&cursor, m_width, m_height);
        dsttemp[j*width+m_width/2]=median;
        for(int i=m_width/2+1;i<width-m_width/2-1;i++){
            for(int k=-m_height/2;k<m_height/2+1;k++){
                movein[k+m_height/2]=src[(j+k)*width+i+m_width/2];
                moveout[k+m_height/2]=src[(j+k)*width+i-m_width/2-1];
            }
            median=findMedian(hist, movein, moveout, m_height, &cursor, median, t);
            dsttemp[j*width+i]=median;
        }
    }
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
           dst[j*width+i]=dsttemp[j*width+i];
            
        }
    }
    free(dsttemp);
    free(movein);
    free(moveout);
}


/////////////////////////双边性滤波///////////////////////////////


//高斯函数
double gaussian(double x,double deta){
   return exp(-0.5*(x*x)/(deta*deta));
}
//计算当前模板系数
double BilateralWindow(double *window,int width,int height,double deta_d,double deta_r){
    double *mask=(double *)malloc(sizeof(double)*width*height);
    if(mask==NULL){
        printf("bilateral window malloc wrong\n");
        exit(0);
    }
    GaussianMask(mask,width,height,deta_d);
    double detavalue=0.0;
    double center_value=window[height/2*width+width/2];
    double k=0.0;
    double result=0.0;
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
            detavalue=center_value-window[j*width+i];
            mask[j*width+i]*=gaussian(detavalue,deta_r);
            k+=mask[j*width+i];
        }
    }
    for(int i=0;i<width*height;i++){
        result+=mask[i]*window[i];
    }
    free(mask);
    return result/k;
}
//双边滤波
void BilateralFilter(double *src,double *dst,int width,int height,int m_width,int m_height,double deta_d,double deta_r){
    double *window=(double *)malloc(sizeof(double)*m_width*m_height);
    for(int j=m_height/2;j<height-m_height/2;j++){
        for(int i=m_width/2;i<width-m_width/2;i++){
            for(int m=-m_height/2;m<m_height/2+1;m++){
                for(int n=-m_width/2;n<m_width/2+1;n++)
                    window[(m+m_height/2)*m_width+n+m_width/2]=src[(j+m)*width+i+n];
            }
            double value=BilateralWindow(window,m_width,m_height,deta_d,deta_r);
            dst[j*width+i]=value;
        }
    }
    free(window);
}


//////////////////////////NoLinearMeanFilter////////////////////////////

void HarmonicAve(double *src,int width,int height){
    for(int i=0;i<width*height;i++)
        if(src[i]!=0)
            src[i]=1.0/src[i];

}
void uHarmonicAve(double *src,int width,int height){
    for(int i=0;i<width*height;i++)
        if(src[i]!=0)
            src[i]=1.0/src[i];

}
void GeometricMean(double *src,int width,int height){
    for(int i=0;i<width*height;i++)
        if(src[i]!=0)
            src[i]=log(src[i]);

}
void uGeometricMean(double *src,int width,int height){
    for(int i=0;i<width*height;i++)
        if(src[i]!=0)
            src[i]=exp(src[i]);
    
}
void NoLinearMeanFilter(double *src,double *dst,int width,int height,int Mask_type,int m_width,int m_height,int function_type,double param){
    double * pixarry=(double *)malloc(sizeof(double)*width*height);
    double * dstarry=(double *)malloc(sizeof(double)*width*height);
    double * mask=(double *)malloc(sizeof(double)*width*height);
    
    matrixCopy(src, pixarry, width, height);
    //变换函数
    switch (function_type) {
        case NLMF_FUN_A:
            break;
        case NLMF_FUN_G:
            GeometricMean(pixarry,width,height);
            break;
        case NLMF_FUN_H:
            HarmonicAve(pixarry,width,height);
            break;
        default:
            break;
    }
    
    
    //均值为同态，高斯不是
    switch (Mask_type) {
        case NLMF_MASK_G:
        {
            GaussianMask(mask, m_width, m_height, param);
            RealRelevant(pixarry,dstarry,mask,width,height,width,height);
            break;
        }
        case NLMF_MASK_M:
        {
            MeanMask(mask, m_width, m_height);
            RealRelevant(pixarry,dstarry,mask,width,height,width,height);
            break;
        }
        default:
            matrixCopy(pixarry, dstarry, width, height);
            break;
    }
    
    //函数反变换
    switch (function_type) {
        case NLMF_FUN_A:
            break;
        case NLMF_FUN_G:
            uGeometricMean(dstarry,width,height);
            break;
        case NLMF_FUN_H:
            uHarmonicAve(dstarry,width,height);
            break;
        default:
            break;
    }
    matrixCopy(dstarry, dst, width, height);
    free(pixarry);
    free(dstarry);
    free(mask);
}



//////////////////////robust smoothing filter/////////////////////////
void findMaxMin(double *src,int width,int height,double *max,double *min){
    int mid=width*height/2;
    double max_t=-1,min_t=256;
    for(int i=0;i<width*height;i++){
        if(i!=mid){
            max_t=src[i]>max_t&&src[i]<255?src[i]:max_t;
            min_t=src[i]<min_t?src[i]:min_t;
            
        }
    }
    *min=min_t;
    *max=max_t;

}
