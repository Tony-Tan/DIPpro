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
//  Mathematic
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 14/11/28.
//  Copyright (c) 2014年 谭升. All rights reserved.
//

#include "mathematic.h"
#include <stdio.h>
int isBase2(int size_n){
    int k=size_n;
    int z=0;
    while (k/=2) {
        z++;
    }
    k=z;
    if(size_n!=(1<<k))
        return -1;
    else
        return k;
}

double Distance(int x,int y,int c_x,int c_y){
    
    return sqrt((x-c_x)*(x-c_x)+(y-c_y)*(y-c_y));
}
////////////////////////////////////////////////////////////////////
//复数基本运算
///////////////////////////////////////////////////////////////////
void Add_Complex(Complex * src1,Complex *src2,Complex *dst){
    dst->imagin=src1->imagin+src2->imagin;
    dst->real=src1->real+src2->real;
}
void Sub_Complex(Complex * src1,Complex *src2,Complex *dst){
    dst->imagin=src1->imagin-src2->imagin;
    dst->real=src1->real-src2->real;
}
void Multy_Complex(Complex * src1,Complex *src2,Complex *dst){
    double r1=0.0,r2=0.0;
    double i1=0.0,i2=0.0;
    r1=src1->real;
    r2=src2->real;
    i1=src1->imagin;
    i2=src2->imagin;
    dst->imagin=r1*i2+r2*i1;
    dst->real=r1*r2-i1*i2;
}
void Copy_Complex(Complex * src,Complex *dst){
    dst->real=src->real;
    dst->imagin=src->imagin;
}
void Show_Complex(Complex * src,int size_n){
    if(size_n==1){
        if(src->imagin>=0.0)
            printf("%lf+%lfj  ",src->real,src->imagin);
        else
            printf("%lf%lfj  ",src->real,src->imagin);
        
    }
    else if(size_n>1){
        for(int i=0;i<size_n;i++)
            if(src[i].imagin>=0.0){
                printf("%lf+%lfj  ",src[i].real,src[i].imagin);
            }
            else
                printf("%lf%lfj  ",src[i].real,src[i].imagin);
        
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void matrixAdd(double *src1,double *src2,double *dst,int width,int height){
    
    for(int i=0;i<width*height;i++)
        dst[i]=src1[i]+src2[i];
}
void matrixSub(double *src1,double *src2,double *dst,int width,int height){
    
    for(int i=0;i<width*height;i++)
        dst[i]=src1[i]-src2[i];
}
void matrixCopy(double *src,double *dst,int width,int height){
    
    for(int i=0;i<width*height;i++)
        dst[i]=src[i];
}
void matrixMultreal(double *src,double *dst,double k,int width,int height){
    for(int i=0;i<width*height;i++)
        dst[i]=src[i]*k;
}
double findMatrixMax(double *src,int width,int height){
    double max=-1.0;
    for(int i=0;i<width*height;i++)
        max=src[i]>max?src[i]:max;
    return max;
}
double matrixMean(double *mat,int width,int height){
    double sum=0.0;
    for(int i=0;i<width*height;i++)
        sum+=mat[i];
    return sum/(double)(width*height);
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

//找出两幅等大图像中同一位置中相对较大的像素值
void MaxPix(double *src1 ,double *src2,double *dst,int width,int height){
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            double value1=src1[j*width+i];
            double value2=src2[j*width+i];
            dst[j*width+i]=value1>=value2?value1:value2;
        }
}
//找出两幅等大图像中同一位置中相对较小的像素值
void MinPix(double *src1 ,double *src2,double *dst,int width,int height){
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            double value1=src1[j*width+i];
            double value2=src2[j*width+i];
            dst[j*width+i]=value1<=value2?value1:value2;
        }
}
void One(double *src,int width,int height){
    for(int i=0;i<width*height;i++)
        src[i]=1.0;
}
void Zero(double *src,int width,int height){
    for(int i=0;i<width*height;i++)
        src[i]=0.0;
}
void Mask(double *src,double *dst,double *mask,int width,int height){
    double *temp=(double*)malloc(sizeof(double)*width*height);
    Zero(temp, width, height);
    for(int i=0;i<height*width;i++){
        if(mask[i]==255.0){
            temp[i]=src[i];
        }
    }
    matrixCopy(temp, dst, width, height);
    free(temp);
}

