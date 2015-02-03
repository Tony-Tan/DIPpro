//
//  Histogram
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/2/1.
//  Copyright (c) 2015年 谭升. All rights reserved.
//
#include "Histogram.h"
/********************************************************************************************
 直方图基本操作
 *******************************************************************************************/
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
/********************************************************************************************
 直方图均衡
 *******************************************************************************************/
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
/********************************************************************************************
 直方图归一化
 *******************************************************************************************/
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
    
    fillMaptable(histMap);
    for(int i=0;i<GRAY_LEVEL;i++)
        histMap_[(int)histMap[i]]=i;
    double dstMap[GRAY_LEVEL];
    for(int i=0;i<GRAY_LEVEL;i++){
        dstMap[i]=histMap_[(int)srcMap[i]];
    }
    
    fillMaptable(dstMap);
    for(int i=0;i<GRAY_LEVEL;i++)
        if(src_hist[i]!=0)
            printf("%d->%lf\n",i,dstMap[i]);
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++)
            dst[j*width+i]=dstMap[(int)src[j*width+i]];
}
