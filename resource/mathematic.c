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
#include <float.h>
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

double Distance(double x,double y,double c_x,double c_y){
    
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

int matrixCopyLocal(double *src,double *dst,int width,int height,int w_width,int w_height,Position *lefttoppoint){
    if(lefttoppoint->x+w_width<width&&lefttoppoint->y+w_height<height){
        for(int j=0;j<w_height;j++)
            for(int i=0;i<w_width;i++){
                dst[j*w_width+i]=src[(j+lefttoppoint->y)*width+i+lefttoppoint->x];
            }
        return 1;
    }
    else
        return 0;
}

void matrixMultreal(double *src,double *dst,double k,int width,int height){
    for(int i=0;i<width*height;i++)
        dst[i]=src[i]*k;
}
void matrixMul_matrix(double *src1,double *src2,double *dst,int width,int height){
    for(int i=0;i<width*height;i++)
        dst[i]=src1[i]*src2[i];
}
double findMatrixMax(double *src,int width,int height){
    double max=-1.0;
    for(int i=0;i<width*height;i++)
        max=src[i]>max?src[i]:max;
    return max;
}
double findMatrixMin(double *src,int width,int height){
    double max=DBL_MAX;
    for(int i=0;i<width*height;i++)
        max=src[i]<max?src[i]:max;
    return max;
}
double matrixMean(double *mat,int width,int height){
    double sum=0.0;
    for(int i=0;i<width*height;i++)
        sum+=mat[i];
    return sum/(double)(width*height);
}
void matrixDBL2INT(double *src,int *dst,int width,int height){
    for(int i=0;i<width*height;i++)
        dst[i]=(int)src[i];
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
//一般差分，返回幅度值和方向矩阵
void matrixOrdinaryDiff(double *src,double *range,double* angle,int width,int height){
    double *temp_rang=(double *)malloc(sizeof(double)*width*height);
    double *temp_angle=(double *)malloc(sizeof(double)*width*height);
    Zero(temp_angle, width, height);
    Zero(temp_rang, width, height);
    for(int j=1;j<height-1;j++)
        for(int i=1;i<width-1;i++){
            double d_x=(src[j*width+i+1]-src[j*width+i-1])/2.0;
            double d_y=(src[(j+1)*width+i]-src[(j-1)*width+i])/2.0;
            temp_rang[j*width+i]=fabs(d_x)+fabs(d_y);
            temp_angle[j*width+i]=atan2(d_y, d_x)*W_PI+180;// atan2(double y,double x)
        }
    matrixCopy(temp_angle, angle, width, height);
    matrixCopy(temp_rang, range, width, height);
    free(temp_rang);
    free(temp_angle);
}

//矩阵旋转
void matrixRotation(double *src,double *dst,int s_width,int s_height,int d_width,int d_height,double theta,Position_DBL* center){
    theta/=W_PI;
    double cos_theta=cos(theta);
    double sin_theta=sin(theta);
    double src_x,src_y;
    int src_x_int,src_y_int;
    double *temp_dst=(double *)malloc(sizeof(double)*d_width*d_height);
    double v11,v12,v21,v22;
    double value=0.0;
    double center_x=(double)center->x;
    double center_y=(double)center->y;
    double dw=center_x/s_width;
    double dh=center_y/s_height;
    Zero(temp_dst, d_width, d_height);
    for(int j=0;j<d_height;j++){
        for(int i=0;i<d_width;i++){
            if(j==128&&i==88)
                printf("stop\n");
            value=0.0;
            src_x=(i-dw*d_width)*cos_theta+(dh*d_height-j)*sin_theta+dw*s_width;
            src_y=(i-dw*d_width)*sin_theta-(dh*d_height-j)*cos_theta+dh*s_height;
            src_x_int=(int)src_x;
            src_y_int=(int)src_y;
            if(src_x<0.0||src_y<0.0||src_x>d_width||src_y>d_height)
                ;//nothing to do
            else{
                double delta_x=(src_x-(double)(src_x_int));
                double delta_y=(src_y-(double)(src_y_int));
                if(delta_x==0.0&&src_y_int<d_height-1){
                    v11=src[src_y_int*s_width+src_x_int];
                    v21=src[(src_y_int+1)*s_width+src_x_int];
                    value=v11*(1.0-delta_y)+delta_y*v21;
                }else if(delta_x<d_width-1&&delta_y==0){
                    v11=src[src_y_int*s_width+src_x_int];
                    v12=src[(src_y_int)*s_width+src_x_int+1];
                    value=v11*(1.0-delta_x)+delta_x*v12;
                }else if(delta_x==0&&src_y_int==0){
                    value=src[(src_y_int)*s_width+src_x_int];
                }
                else if(delta_y>0.0&&delta_x>0.0&&src_y_int<d_height-1&&src_x_int<d_width-1){
                    v11=src[src_y_int*s_width+src_x_int];
                    v12=src[(src_y_int)*s_width+src_x_int+1];
                    v21=src[(src_y_int+1)*s_width+src_x_int];
                    v22=src[(src_y_int+1)*s_width+src_x_int+1];
                    double v1=v12*delta_x+(1-delta_x)*v11;
                    double v2=v22*delta_x+(1-delta_x)*v21;
                    value=v2*delta_y+(1-delta_y)*v1;
                }
            }
            
            
            temp_dst[j*d_width+i]=value;
            //printf("%d ",(int)value);
        }
        //printf("\n");
    }
    matrixCopy(temp_dst, dst, d_width, d_height);
    free(temp_dst);

}
