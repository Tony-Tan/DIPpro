//
//  Smooth
//  图像平滑，高斯，均值，双边滤波
//  Created by 谭升 on 15/01/23.
//  tony.sheng.tan@gmail.com
//  Copyright (c) 2015年 谭升. All rights reserved.
//
#include "smooth.h"
#include "convolution.h"
#define GRAY_LEVEL 256
static double Distance(int x,int y,int c_x,int c_y){
    return sqrt((x-c_x)*(x-c_x)+(y-c_y)*(y-c_y));
}
//////////////////////////////////////高斯滤波模板生成/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void GaussianMask(double *mask,int width,int height,double deta){
    double deta_2=deta*deta;
    int center_x=width/2;
    int center_y=height/2;
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


void GaussianFilter(IplImage *src,IplImage *dst,int width,int height,double deta){
    double * pixarry=(double *)malloc(sizeof(double)*src->width*src->height);
    double * dstarry=(double *)malloc(sizeof(double)*src->width*src->height);
    double * mask=(double *)malloc(sizeof(double)*width*height);
    for(int j=0;j<src->height;j++){
        for(int i=0;i<src->width;i++){
            pixarry[j*src->width+i]=cvGetReal2D(src, j, i);
        }
    }
    GaussianMask(mask, width, height, deta);
    RealRelevant(pixarry,dstarry,mask,src->width,src->height,width,height);
    for(int j=0;j<src->height;j++){
        for(int i=0;i<src->width;i++){
            cvSetReal2D( dst,j,i,dstarry[j*src->width+i]);
        }
    }

}
//////////////////////////////////////均值滤波模板生成/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void MeanMask(double *mask,int width,int height){
    double w=width;
    double h=height;
    double meanvalue=1.0/(w*h);
    for(int i=0;i<width*height;i++)
        mask[i]=meanvalue;
    

}
void MeanFilter(IplImage *src,IplImage *dst,int width,int height){
    double * pixarry=(double *)malloc(sizeof(double)*src->width*src->height);
    double * dstarry=(double *)malloc(sizeof(double)*src->width*src->height);
    double * mask=(double *)malloc(sizeof(double)*width*height);
    for(int j=0;j<src->height;j++){
        for(int i=0;i<src->width;i++){
            pixarry[j*src->width+i]=cvGetReal2D(src, j, i);
        }
    }
    MeanMask(mask, width, height);
    RealRelevant(pixarry,dstarry,mask,src->width,src->height,width,height);
    for(int j=0;j<src->height;j++){
        for(int i=0;i<src->width;i++){
            cvSetReal2D( dst,j,i,dstarry[j*src->width+i]);
        }
    }
    
}
//////////////////////////////////中值滤波及其相关函数/////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
/*
//以下为低速普通中值滤波，排序使用计数排序
 void initHist(int *hist,int size){
    for(int i=0;i<size;i++)
        hist[i]=0;
}
int sort(int *src,int size){
    int hist[GRAY_LEVEL];
    int m=0;
    initHist(hist, GRAY_LEVEL);
    for(int i=0;i<size;i++)
        hist[src[i]]++;
    for(int i=0;i<GRAY_LEVEL;i++){
        m+=hist[i];
        if(m>size/2)
            return i;
    
    }
    return 0;
}
void MedianFilter(IplImage *src,IplImage *dst,int width,int height){
    IplImage* temp=cvCreateImage(cvSize(src->width+width, src->height+height), src->depth, src->nChannels);
    IplImage* dsttemp=cvCreateImage(cvSize(src->width+width, src->height+height), src->depth, src->nChannels);
    cvZero(temp);
    for(int j=0;j<src->height;j++){
        for(int i=0;i<src->width;i++){
            double value=cvGetReal2D(src, j, i);
            cvSetReal2D(temp, j+height/2, i+width/2, value);
        
        }
    }
    int *window=(int *)malloc(sizeof(int)*width*height);
    if(window==NULL){
        printf(" ");
        exit(0);
    
    }
    for(int j=height/2;j<temp->height-height/2-1;j++){
        for(int i=width/2;i<temp->width-width/2-1;i++){
            for(int n=-height/2;n<height/2+1;n++)
                for(int m=-width/2;m<width/2+1;m++){
                    window[(n+height/2)*width+m+width/2]=cvGetReal2D(temp, j+n, i+m);
                }
            double pix=sort(window,width*height);
            cvSetReal2D(dsttemp, j, i, pix);
        }
    }
    for(int j=height/2;j<temp->height-height/2-1;j++){
        for(int i=width/2;i<temp->width-width/2-1;i++){
            double value=cvGetReal2D(dsttemp, j, i);
            cvSetReal2D(dst, j-height/2, i-width/2, value);
            
        }
    }
    free(window);
        
}
*/
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
int InitRow(IplImage *src,int *hist,int row,int *cursor,int win_width,int win_height){
    int t=win_width*win_height/2+1;
    *cursor=0;
    for(int i=0;i<GRAY_LEVEL;i++)
        hist[i]=0;
    for(int j=-win_height/2;j<win_height/2+1;j++)
        for(int i=0;i<win_width;i++){
            int pixvalue=cvGetReal2D(src, j+row, i);
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

void MedianFilter(IplImage *src,IplImage *dst,int width,int height){
    int hist[GRAY_LEVEL];
    int median;
    int *movein=(int *)malloc(sizeof(int)*height);
    int *moveout=(int *)malloc(sizeof(int)*height);
    double *dsttemp=(double *)malloc(sizeof(double)*src->width*src->height);
    int t=width*height/2;
    for(int j=height/2;j<src->height-height/2-1;j++){
        int cursor=0;
        median=InitRow(src, hist, j, &cursor, width, height);
        dsttemp[j*src->width+width/2]=median;
        for(int i=width/2+1;i<src->width-width/2-1;i++){
            for(int k=-height/2;k<height/2+1;k++){
                movein[k+height/2]=cvGetReal2D(src, j+k, i+width/2);
                moveout[k+height/2]=cvGetReal2D(src, j+k, i-width/2-1);
            }
            median=findMedian(hist, movein, moveout, height, &cursor, median, t);
            dsttemp[j*src->width+i]=median;
        }
    }
    for(int j=0;j<src->height;j++){
        for(int i=0;i<src->width;i++){
            cvSetReal2D(dst, j, i, dsttemp[j*src->width+i]);
            
        }
    }
    free(dsttemp);
    free(movein);
    free(moveout);
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////双边性滤波///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
double gaussian(double x,double deta){
   return exp(-0.5*(x*x)/(deta*deta));
}
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
void BilateralFilter(IplImage *src,IplImage *dst,int width,int height,double deta_d,double deta_r){
    double *window=(double *)malloc(sizeof(double)*width*height);
    for(int j=height/2;j<src->height-height/2;j++){
        for(int i=width/2;i<src->width-width/2;i++){
            for(int m=-height/2;m<height/2+1;m++){
                for(int n=-width/2;n<width/2+1;n++)
                    window[(m+height/2)*width+n+width/2]=cvGetReal2D(src, j+m, i+n);
            }
            double value=BilateralWindow(window,width,height,deta_d,deta_r);
            cvSetReal2D(dst, j, i, value);
        }
    }
    free(window);
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////



void Smooth(IplImage *src,IplImage *dst,int Smooth_type,int width,int height){





}
