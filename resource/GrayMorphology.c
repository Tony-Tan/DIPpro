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
//  GrayMorphology
//  灰度图像形态学
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 14/11/28.
//  Copyright (c) 2014年 谭升. All rights reserved.
//
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include "GrayMorphology.h"
#define TOFINDMAX 0
#define TOFINDMIN 1
#define isSIZEEQU(x,y) (((x)->width)==((y)->width)&&((x)->height)==((y)->height))

//判断结构元是否平滑
int isSmooth(double *src,int width,int height){
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            int v=src[j*width+i];
            if(v!=255.0&&v!=0.0)
                return 0;
        }
    return 1;
}
//判断两幅图像是否相等
int isEqu(double *src1,double *src2,int width,int height){
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            double v1=src1[j*width+i];
            double v2=src2[j*width+i];
            if(v1!=v2)
                return 0;
        }
    return 1;
}

//位移，如果非平滑SE将加上sevalue，即对应的灰度值
void G_Translation(double *src,double *dst,int width,int height,double SEvalue,Position *d,int istoFindMin){
    double *temp=(double*)malloc(sizeof(double)*height*width);
    if(istoFindMin)
        One(temp,width,height);
    else
        Zero(temp,width,height);
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
            int target_x=i+d->x;
            int target_y=j+d->y;
            if(target_x>=0&&target_y>=0&&
               target_x<width&&target_y<height){
                double value=src[j*width+i]+SEvalue;
                value=(value>=255.0?255.0:value);
                temp[target_y*width+target_x]=value;
            }
        }
    }
    matrixCopy(temp,dst,width,height);
    free(&temp);
}

//灰度图像膨胀
void Dilate_Gray(double *src,double *dst,int width,int height,double *se,int sewidth,int seheight,Position *center){
    int SEissmooth=isSmooth(se,sewidth,seheight);
    //IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    //IplImage *temp_last=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    double *temp=(double*)(malloc(sizeof(double)*width*height));
    double *temp_last=(double*)(malloc(sizeof(double)*width*height));
    Position centerde;
    centerde.x=sewidth/2;
    centerde.y=seheight/2;
    if(center==NULL){
        center=&centerde;
    }
    matrixCopy(src,temp_last,width,height);
    for(int j=0;j<seheight;j++)
        for(int i=0;i<sewidth;i++){
            matrixCopy(src,temp,width,height);
            double value=se[j*width+i];//cvGetReal2D(se, j, i);
            if(value!=0.0){
                Position d;
                d.x=center->x-i;
                d.y=center->y-j;
                if(SEissmooth)
                    G_Translation(temp, temp,width,height, 0.0, &d,TOFINDMAX);
                else
                    G_Translation(temp, temp,width,height, value, &d,TOFINDMAX);
                MaxPix(temp, temp_last, temp_last,width,height);
            }
        }
    matrixCopy(temp_last, dst,width,height);
    free(temp);
    free(temp_last);

}
//灰度图像腐蚀
void Erode_Gray(double *src,double *dst,int width,int height,double *se,int sewidth,int seheight,Position *center){
    int SEissmooth=isSmooth(se,sewidth,seheight);
    double *temp=(double*)(malloc(sizeof(double)*width*height));
    double *temp_last=(double*)(malloc(sizeof(double)*width*height));
    Position centerde;
    centerde.x=sewidth/2;
    centerde.y=seheight/2;
    if(center==NULL){
        center=&centerde;
    }
    matrixCopy(src,temp_last,width,height);
    for(int j=0;j<seheight;j++)
        for(int i=0;i<sewidth;i++){
            matrixCopy(src,temp,width,height);
            double value=se[j*width+i];
            if(value!=0.0){
                Position d;
                
                d.x=i-center->x;
                d.y=j-center->y;
                if(SEissmooth)
                    G_Translation(temp, temp,width,height, 0.0, &d,TOFINDMAX);
                else
                    G_Translation(temp, temp,width,height, -1.0*value, &d,TOFINDMAX);
                MaxPix(temp, temp_last, temp_last,width,height);
            }
        }
    matrixCopy(temp_last, dst,width,height);
    free(temp);
    free(temp_last);
}
//开操作
void Open_Gray(double *src,double *dst,int width,int height,double *se,int sewidth,int seheight,Position *center){
    double *temp=(double*)malloc(sizeof(double)*width*height);
    Erode_Gray(src, temp,width,height, se,sewidth,seheight, center);
    Dilate_Gray(temp, temp, width,height,se, sewidth,seheight,center);
    matrixCopy(temp,dst,width,height);
    free(temp);
}
//闭操作
void Close_Gray(double *src,double *dst,int width,int height,double *se,int sewidth,int seheight,Position *center){
    double *temp=(double*)malloc(sizeof(double)*width*height);
    Dilate_Gray(src, temp,width,height, se,sewidth,seheight, center);
    Erode_Gray(temp, temp, width,height,se, sewidth,seheight,center);
    matrixCopy(temp,dst,width,height);
    free(temp);
    
}
//灰度梯度形态学提取
void Gard_Gray(double *src,double *dst,int width,int height,double *se,int sewidth,int seheight,Position *center){
    double *temp_dilate=(double *)malloc(sizeof(double)*width*height);
    double *temp_erode=(double *)malloc(sizeof(double)*width*height);
    Dilate_Gray(src, temp_dilate,width,height,se,sewidth,seheight, center);
    Erode_Gray(src, temp_erode,width,height,se,sewidth,seheight, center);
    matrixSub(temp_dilate,temp_erode,dst,width,height);
    free(temp_dilate);
    free(temp_erode);

}
//顶帽操作
void TopHat(double *src,double *dst,int width,int height,double *se,int sewidth,int seheight,Position *center){
    double *temp=(double *)malloc(sizeof(double)*width*height);
    Open_Gray(src, temp,width,height,se,sewidth,seheight,center);
    matrixSub(src,temp,dst,width,height);
    free(temp);
}
//底帽操作
void BottomHat(double *src,double *dst,int width,int height,double *se,int sewidth,int seheight,Position *center){
    double *temp=(double *)malloc(sizeof(double)*width*height);
    Close_Gray(src, temp,width,height,se,sewidth,seheight,center);
    matrixSub(temp,src,dst,width,height);
    free(temp);
}
//测地腐蚀
void Erode_Gray_g(double *src,double *ground,double *dst,int width,int height,double *se,int sewidth,int seheight,Position *center){
    int SEissmooth=isSmooth(se,sewidth,seheight);
    double *temp=(double*)malloc(sizeof(double)*width*height);
    double *temp_last=(double*)malloc(sizeof(double)*width*height);
    Position centerde;
    centerde.x=sewidth/2;
    centerde.y=seheight/2;
    if(center==NULL){
        center=&centerde;
    }
    matrixCopy(src,temp_last,width,height);
    for(int j=0;j<seheight;j++)
        for(int i=0;i<sewidth;i++){
            matrixCopy(src,temp,width,height);
            double value=se[j*sewidth+i];
            if(value!=0.0){
                Position d;
                d.x=i-center->x;
                d.y=j-center->y;
                if(SEissmooth)
                    G_Translation(temp, temp,width,height, 0.0, &d,TOFINDMIN);
                else
                    G_Translation(temp, temp,width,height, -1.0*value, &d,TOFINDMIN);
                MinPix(temp, temp_last, temp_last,width,height);
            }
        }
    MaxPix(temp_last,ground,temp_last,width,height);
    matrixCopy(temp_last,dst,width,height);
    free(temp);
    free(temp_last);
}
//测地膨胀
void Dilate_Gray_g(double *src,double *ground,double *dst,int width,int height,double *se,int sewidth,int seheight,Position *center){
    int SEissmooth=isSmooth(se,sewidth,seheight);
    double *temp=(double*)malloc(sizeof(double)*width*height);
    double *temp_last=(double*)malloc(sizeof(double)*width*height);
    Position centerde;
    centerde.x=sewidth/2;
    centerde.y=seheight/2;
    if(center==NULL){
        center=&centerde;
    }
    matrixCopy(src,temp_last,width,height);
    for(int j=0;j<seheight;j++)
        for(int i=0;i<sewidth;i++){
            matrixCopy(src,temp,width,height);
            double value=se[j*sewidth+i];
            if(value!=0.0){
                Position d;
                d.x=center->x-i;
                d.y=center->y-j;
                if(SEissmooth)
                    G_Translation(temp, temp,width,height, 0.0, &d,TOFINDMAX);
                else
                    G_Translation(temp, temp,width,height, value, &d,TOFINDMAX);
                MaxPix(temp, temp_last, temp_last,width,height);
            }
        }
    MinPix(temp_last, ground, temp_last,width,height);
    matrixCopy(temp_last,dst,width,height);
    free(temp);
    free(temp_last);
}
//重建开操作
void Rebuild_Open(double *src,double *dst,double *ground,int width,int height,double *erodeSE,int esewidth,int eseheight,double *dilateSE,int dsewidth,int dseheight,int eroden){
    double *temp=(double*)malloc(sizeof(double)*width*height);
    double *temp_last=(double*)malloc(sizeof(double)*width*height);
    matrixCopy(src,temp,width,height);
    for(int i=0;i<eroden;i++){
        Erode_Gray(temp, temp, width,height,erodeSE,esewidth,eseheight,NULL);
    }
    while(!isEqu(temp, temp_last,width,height)){
        matrixCopy(temp,temp_last,width,height);
        Dilate_Gray_g(temp, ground, temp,width,height, dilateSE,dsewidth,dseheight, NULL);
    }
    matrixCopy(temp_last,dst,width,height);
    free(temp);
    free(temp_last);
}

//重建顶帽操作
void Rebuild_Tophat(double *src,double *dst,double *ground,int width,int height,double *dilateSE,int dsewidth,int dseheight,double *erodeSE,int esewidth,int eseheight,int eroden){
    Rebuild_Open(src,dst,ground,width,height,erodeSE,esewidth,eseheight,dilateSE,dsewidth,dseheight,eroden);
    matrixSub(src,dst,dst,width,height);
}


