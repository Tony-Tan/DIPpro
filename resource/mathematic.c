//
//  Mathematic
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 14/11/28.
//  Copyright (c) 2014年 谭升. All rights reserved.
//

#include "mathematic.h"

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