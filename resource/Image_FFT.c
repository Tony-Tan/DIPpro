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
//  Image_FFT
//  快速傅里叶变换
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 14/11/28.
//  Copyright (c) 2014年 谭升. All rights reserved.
//
#include "Image_FFT.h"

/*
 中心化，根据傅里叶性质的平移性质
 */
void FFT_Shift(double * src,int size_w,int size_h){
    for(int j=0;j<size_h;j++)
        for(int i=0;i<size_w;i++){
            if((i+j)%2)
                src[j*size_w+i]=-src[j*size_w+i];
        
    }
    
}
/*
 图像快速傅里叶变换，图像大小为2的N次幂
 */
void ImageFFT(double * src,int width,int height,Complex * dst){
    double *image_data=(double*)malloc(sizeof(double)*width*height);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            image_data[j*width+i]=cvGetReal2D(src, j, i);
        
    }
    FFT_Shift(image_data,width, height);//图像中心化
    FFT2D(image_data, dst, width, height);
    free(image_data);
    
}
/*
 将幅度值伸缩到0到255，用于频谱显示
 */
void Nomalsize(double *src,double *dst,int size_w,int size_h){
    double  max=0.0,min=DBL_MAX;
    for(int i=0;i<size_w*size_h;i++){
        max=src[i]>max?src[i]:max;
        min=src[i]<min?src[i]:min;
    }
    double step=255.0/(max-min);
    //printf("%d",test);
    //printf("max:%lf   min:%lf\n",max,min);
    for(int i=0;i<size_w*size_h;i++){
        dst[i]=(src[i]-min)*step;
        dst[i]*=45.9*log((double)(1+dst[i]));
    }
    
}
/*
 获得频谱
 */
void getAmplitudespectrum(Complex * src,double *dst,int size_w,int size_h){
    double *despe=(double *)malloc(sizeof(double)*size_w*size_h);
    if(despe==NULL)
        exit(0);
    double real=0.0;
    double imagin=0.0;
    
    for(int j=0;j<size_h;j++)
        for(int i=0;i<size_w;i++){
            real=src[j*size_w+i].real;
            imagin=src[j*size_w+i].imagin;
            despe[j*size_w+i]=sqrt(real*real+imagin*imagin);
            
        }
    Nomalsize(despe, despe, size_w, size_h);
    matrixCopy(despe, dst, size_w, size_h);
    free(despe);
    
}
/*
 图像傅里叶反变换
 */
void ImageIFFT(Complex *src,double *dst,int size_w,int size_h){
    Complex *temp_c=(Complex*)malloc(sizeof(Complex)*size_w*size_h);
    if(temp_c==NULL)
        exit(0);
    for(int i=0;i<size_w*size_h;i++)
        Copy_Complex(&src[i],&temp_c[i]);
    Complex *temp=(Complex*)malloc(sizeof(Complex)*size_w*size_h);
    if(temp==NULL)
        exit(0);
    double *temp_d=(double *)malloc(sizeof(double)*size_w*size_h);
    if(temp_d==NULL)
        exit(0);
    IFFT2D(temp_c,temp,size_w,size_h);
    for(int j=0;j<size_h;j++)
        for(int i=0;i<size_w;i++){
            temp_d[j*size_w+i]=temp[j*size_w+i].real;
        }
    FFT_Shift(temp_d, size_w, size_h);
    matrixCopy(temp_d, dst, size_w, size_h);
    free(temp);
    free(temp_c);
    free(temp_d);

}

