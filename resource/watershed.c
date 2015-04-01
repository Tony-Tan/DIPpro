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
//  Watershed
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/03/03.
//  Copyright (c) 2015年 谭升. All rights reserved.
//
/*
 typedef struct PriQueueNode_ PriQueueHead;
 typedef struct NLevelPriQueueNode_ * NLevelPriQueue;
 typedef struct NLevelPriQueueNode_ NLevelPriNode;
 typedef struct ExPix_ Pix_Label;
 
 *          ___    ____________________
 *         | P |->|   NLevelPriQueue   |
 *         |---|  |--------------------|
 *         | r |->|   NLevelPriQueue   |
 *         |---|  |--------------------|
 *         | i |->|   NLevelPriQueue   |
 *         |---|  |--------------------|
 *         | Q |->|   NLevelPriQueue   |
 *         |---|  |--------------------|
 *         | u |->|   NLevelPriQueue   |
 *         |---|  |--------------------|
 *         | e |->|   NLevelPriQueue   |
 *         |---|  |--------------------|
 *         | u |->|   NLevelPriQueue   |
 *         |---|  |--------------------|
 *         | e |->|   NLevelPriQueue   |
 *         |___|  |____________________|
 
 struct NLevelPriNode_{
 int x;
 int y;
 NLevelPriQueue  next;
 };
 struct PriQueueNode_{
 int nodeNum;
 NLevelPriQueue  head;
 NLevelPriQueue  tail;
 };
 struct ExPix_{
 int grayvalue;
 int label;
 };
 */
#include "watershed.h"
#include <stdio.h>

//将非极小值且与极小值相邻的元素入队
void inQueue(PriQueueHead* priQueue,int gray_level,int x,int y){
    priQueue[gray_level].nodeNum++;
    ///malloc new node
    NLevelPriNode* newNode=(NLevelPriNode *)malloc(sizeof(NLevelPriNode));
    newNode->x=x;
    newNode->y=y;
    newNode->next=NULL;
    if(priQueue[gray_level].head==NULL){
        priQueue[gray_level].head=newNode;
        priQueue[gray_level].tail=newNode;
    }else{
        priQueue[gray_level].tail->next=newNode;
        priQueue[gray_level].tail=newNode;
    }

}
//判断极小值是平底锅型还是台阶形状，使用图的深度优先搜索
int isPan(int *src,int width,int height,double value,int x,int y){
    src[y*width+x]=-value;
    
    for(int j=-1;j<2;j++)
        for(int i=-1;i<2;i++)
            if(j+y>=0&&i+x>=0&&j+y<height&&i+x<width&&(i!=0||j!=0)){
                if(src[(j+y)*width+i+x]<value&&src[(j+y)*width+i+x]>0)
                    return 0;
                }
    for(int j=-1;j<2;j++)
        for(int i=-1;i<2;i++)
            if(j+y>=0&&i+x>=0&&j+y<height&&i+x<width&&(i!=0||j!=0)){
                if(src[(j+y)*width+i+x]==value){
                    return(isPan(src,width, height, value, i+x, j+y));
                    
                }
            }
    return 1;

}
//由于判断平底锅时使部分数据损坏，现进行恢复
void repairPan(int *src ,int width,int height,double value,int x,int y){
    src[y*width+x]=-value;
    for(int j=-1;j<2;j++)
        for(int i=-1;i<2;i++)
            if(j+y>=0&&i+x>=0&&j+y<height&&i+x<width&&(i!=0||j!=0)){
                if(src[(j+y)*width+i+x]==value){
                    repairPan(src, width, height, value, x+i, y+j);
                }
            }
}
//平底锅形状，标记极小值为255
void setMinimal(int *src,double *dst,int width,int height,int value,int x,int y){
    dst[y*width+x]=255.0;
    for(int j=-1;j<2;j++)
        for(int i=-1;i<2;i++)
            if(j+y>=0&&i+x>=0&&j+y<height&&i+x<width&&(i!=0||j!=0)){
                if(src[(j+y)*width+x+i]==value&&dst[(j+y)*width+x+i]==0.0)
                    setMinimal(src,dst,width, height, value, x+i,y+j);
            }
}
//台阶形状，标记非极小值127
void setUnMinimal(int *src,double *dst,int width,int height,int value,int x,int y){
    dst[y*width+x]=127.0;
    for(int j=-1;j<2;j++)
        for(int i=-1;i<2;i++)
            if(j+y>=0&&i+x>=0&&j+y<height&&i+x<width&&(i!=0||j!=0)){
                if(src[(j+y)*width+x+i]==value&&dst[(j+y)*width+x+i]==0.0)
                    setUnMinimal(src,dst,width, height, value, x+i,y+j);
            }
}

//数据类型从double到int
void Double2Int(double *src,int* dst,int width,int height){
    for(int i=0;i<width*height;i++)
            dst[i]=(int)src[i]+1;
}
//寻找极小值，包括单点极小值和平底锅
void findMinimal(double *src,double *dst,int width,int height){
    
    int *temp=(int *)malloc(sizeof(int)*width*height);
    double *dsttemp=(double *)malloc(sizeof(double)*width*height);
    Zero(dsttemp, width, height);
    Double2Int(src, temp, width, height);
    int lessthan=0;
    int equ=0;
    double min=findMatrixMin(src, width, height,NULL);
    for(int i=0;i<width*height;i++)
        if(src[i]==min)
            dsttemp[i]=255.0;
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
            lessthan=0;
            equ=0;
            int pix=temp[j*width+i];
            if(dsttemp[j*width+i]==0.0){
                for(int m=-1;m<2;m++)
                    for(int n=-1;n<2;n++)
                        if(j+m>=0&&i+n>=0&&j+m<height&&i+n<width){
                            if(m!=0||n!=0){
                                if(temp[(j+m)*width+i+n]<pix)
                                    lessthan=1;
                                if(temp[(j+m)*width+i+n]==pix)
                                    equ=1;
                            }
                        }
                
                if(equ==1&&lessthan==0){
                    if(isPan(temp, width, height,pix, i, j)){
                        setMinimal(temp,dsttemp, width, height, -pix, i, j);
                    }else {
                        repairPan(temp,width, height, -pix, i,j);
                        setUnMinimal(temp,dsttemp, width, height, pix, i, j);
                    }
                }
                if(lessthan==1)
                    dsttemp[j*width+i]=127.0;
                if(0==lessthan&&0==equ)
                    dsttemp[j*width+i]=255.0;
            
            }
        }
    }
    matrixCopy(dsttemp, dst, width, height);
    free(dsttemp);
    free(temp);
}
//标记极小值的label，从-1开始向下增长 -2 -3 -4 -5 -6 -7.....
void LabelMinimal(double * src,Pix_Label* dst,int width,int height,int x,int y,int label){
    dst[y*width+x].label=label;
    for(int i=-1;i<2;i++){
        for(int j=-1;j<2;j++)
            if(x+i>=0&&x+i<width&&y+j>=0&&y+j<height&&(i!=0||j!=0))
                if(src[(y+j)*width+x+i]==255.0&&dst[(y+j)*width+x+i].label==0){
                    LabelMinimal(src, dst, width, height, x+i, y+j, label);
                }
    }

}
//初始化label数组，此数组与图像数组多加了label
void InitLabelMat(double *src,Pix_Label* dst,double *mask,int width,int height){
    for(int i=0;i<width*height;i++){
        dst[i].grayvalue=src[i];
        dst[i].label=0;
    }
    int label_minimal=-1;
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            if(mask[j*width+i]==255.0&&dst[j*width+i].label==0){
                LabelMinimal(mask, dst, width,height,i,j, label_minimal);
                label_minimal--;
            }
    }
}
//初始化队列头数组
void InitPriQueue(PriQueueHead* priQueue,Pix_Label *srclabel,int width,int height){
    for(int i=0;i<GRAY_LEVEL;i++){
        priQueue[i].head=NULL;
        priQueue[i].nodeNum=0;
        priQueue[i].tail=NULL;
    }
    int inqueue=0;
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            inqueue=0;
            if(srclabel[j*width+i].label==0){
                for(int m=-1;m<2;m++)
                    for(int n=-1;n<2;n++){
                        if(m+j>=0&&m+j<height&&n+i>=0&&n+i<width&&(m!=0||n!=0))
                            if(srclabel[(m+j)*width+n+i].label<0)
                                inqueue=1;
                    }
                if(inqueue){
                    inQueue(priQueue,srclabel[j*width+i].grayvalue,i,j);
                    srclabel[j*width+i].label=INQUEUE;
                }
            }
        }

}


int PirQueueisEmpty(PriQueueHead* priqueue){
    int sum=0;
    for(int i=0;i<GRAY_LEVEL;i++)
        sum+=priqueue[i].nodeNum;
    return !sum;
}


NLevelPriNode* outQueue(PriQueueHead* priqueue){
    NLevelPriNode* node=NULL;
    if(!PirQueueisEmpty(priqueue))
        for(int i=0;i<GRAY_LEVEL;i++)
            if(priqueue[i].nodeNum!=0){
                node=priqueue[i].head;
                priqueue[i].head=node->next;
                priqueue[i].nodeNum--;
                break;
            }
    return node;
}


void findWaterShed(Pix_Label * srclabel,PriQueueHead* priqueue,int width,int height){
    NLevelPriNode* node=outQueue(priqueue);
    while(node!=NULL){
        int y=node->y;
        int x=node->x;
        //printf("x:%d y:%d \n",x,y);
        int label=0;
        int isWatershed=0;
        for(int j=-1;j<2;j++)
            for(int i=-1;i<2;i++){
                if(j+y>=0&&j+y<height&&i+x>=0&&i+x<width&&(j!=0||i!=0)){
                    if(srclabel[(j+y)*width+x+i].label<0){
                        if(label==0)
                            label=srclabel[(j+y)*width+x+i].label;
                        else if(label!=srclabel[(j+y)*width+x+i].label){
                            isWatershed=1;
                        }
                    }
                }
            }
        if(isWatershed)
            srclabel[y*width+x].label=WATERSHED;
        else if(label<0){
            srclabel[y*width+x].label=label;
            for(int j=-1;j<2;j++)
                for(int i=-1;i<2;i++){
                    if(j+y>=0&&j+y<height&&i+x>=0&&i+x<width&&(j!=0||i!=0)){
                        if(srclabel[(j+y)*width+x+i].label==0){
                            inQueue(priqueue, srclabel[(j+y)*width+i+x].grayvalue, i+x, j+y);
                            srclabel[(j+y)*width+i+x].label=INQUEUE;
                        }
                    }
                }
            }
        else if(label==0&&isWatershed==0){
            srclabel[y*width+x].label=0;
            
        }
        free(node);
        node=outQueue(priqueue);
                
    }

}
//meyer分水岭方法
void MeyerWatershed(double *src,double *dst,int width,int height){
    double *dst_temp=(double *)malloc(sizeof(double)*width*height);
    Zero(dst_temp, width, height);
    Pix_Label * srclabel=(Pix_Label*)malloc(sizeof(Pix_Label)*width*height);
    PriQueueHead priqueue[GRAY_LEVEL];
    double *minimal=(double *)malloc(sizeof(double)*width*height);
    Zero(minimal, width, height);
    findMinimal(src, minimal, width, height);
    InitLabelMat(src, srclabel, minimal, width, height);
    InitPriQueue(priqueue, srclabel, width, height);
    findWaterShed(srclabel, priqueue, width, height);
    for(int i=0;i<width*height;i++)
        if(srclabel[i].label==WATERSHED)
            dst_temp[i]=255.0;
    matrixCopy(dst_temp, dst, width, height);
    free(dst_temp);
    free(srclabel);
    free(minimal);

}