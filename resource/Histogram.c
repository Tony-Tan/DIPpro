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
//  Histogram
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/2/1.
//  Copyright (c) 2015年 谭升. All rights reserved.
//
#include "Histogram.h"
/*************************************
 直方图基本操作
 *************************************/
void InitMappingTable(void * arry,int size,int Data_type){
    if(Data_type==TABLE_INT)
        for(int i=0;i<size;i++)
            ((int*)arry)[i]=0;
    else if(Data_type==TABLE_CHAR)
        for(int i=0;i<size;i++)
            ((char*)arry)[i]=0;
    else if(Data_type==TABLE_DOUBLE)
        for(int i=0;i<size;i++)
            ((double*)arry)[i]=0;
    
}
void InitHistogram(int *hist){
    for(int i=0;i<GRAY_LEVEL;i++)
        hist[i]=0;
}

void setHistogram(double *src,int *hist,int width,int height){
    InitHistogram(hist);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            int tempv=src[j*width+i];
            hist[tempv]++;
        }
}
int findHistogramMax(int *hist){
    for(int i=GRAY_LEVEL-1;i>=0;i--){
        if(hist[i]!=0)
            return i;
    }
    return -1;

}
int findHistogramMin(int *hist){
    for(int i=0;i<GRAY_LEVEL;i++){
        if(hist[i]!=0)
            return i;
    }
    return -1;
}
void fillMaptable(double * map){
    
    for(int i=1;i<GRAY_LEVEL;i++){
        if(map[i]==0)
            map[i]=map[i-1];
    
    }

}
/**********************************
 直方图均衡
 **********************************/
//均衡直方图，将原图直方图，经过公式得到目标直方图
void EqualizationHist(int *src_hist,double *dst_map){
    int temphist[GRAY_LEVEL];
    InitHistogram(temphist);
    int max=findHistogramMax(src_hist);
    int min=findHistogramMin(src_hist);
    temphist[min]=src_hist[min];
    for(int i=min+1;i<=max;i++)
        temphist[i]=temphist[i-1]+src_hist[i];
    for(int i=min;i<=max;i++)
        temphist[i]-=temphist[min];
    int total=temphist[max];
    for(int i=min;i<=max;i++){
        dst_map[i]=((double)GRAY_LEVEL-1.0)*temphist[i]/total;
    }
    
}
//直方图均很，用输入图像得到输出图像
void HistogramEqualization(double *src,double *dst,int width,int height){
    int hist[GRAY_LEVEL];
    setHistogram(src, hist, width, height);
    double GrayMappingTable[GRAY_LEVEL];
    InitMappingTable(GrayMappingTable,GRAY_LEVEL,TABLE_DOUBLE);
    EqualizationHist(hist, GrayMappingTable);
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++)
            dst[j*width+i]=GrayMappingTable[(int)src[j*width+i]];
    
}
/******************************
 直方图归一化
 ******************************/
void HistogramSpecification(double *src,double *dst,int* hist,int width,int height){
    int src_hist[GRAY_LEVEL];
    setHistogram(src, src_hist, width, height);
    double srcMap[GRAY_LEVEL];
    double histMap[GRAY_LEVEL];
    InitMappingTable(srcMap,GRAY_LEVEL,TABLE_DOUBLE);
    EqualizationHist(src_hist, srcMap);
    EqualizationHist(hist, histMap);
    int histMap_[GRAY_LEVEL];
    InitHistogram(histMap_);
    for(int i=0;i<GRAY_LEVEL;i++)
        histMap_[(int)histMap[i]]=i;
    double dstMap[GRAY_LEVEL];
    for(int i=0;i<GRAY_LEVEL;i++){
        dstMap[i]=histMap_[(int)srcMap[i]];
    }
    
    fillMaptable(dstMap);
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++)
            dst[j*width+i]=dstMap[(int)src[j*width+i]];
}
