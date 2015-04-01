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
//  convolution
//  滤波相关
//  Created by 谭升 on 14/12/23.
//  tony.sheng.tan@gmail.com
//  Copyright (c) 2014年 谭升. All rights reserved.
//
#include "filter.h"

double getPower(Complex *src,int totalsize){
    double power=0.0;
    double real;
    double imagin;
    for(int i=0;i<totalsize;i++){
        real=src[i].real;
        imagin=src[i].imagin;
        power+=real*real+imagin*imagin;
    }
    return power;
}
void MatrixMulti_R_C(double *src1,Complex *src2,Complex *dst,int size){//m(1,1)=a(1,1)*b(1,1);
    for(int i=0;i<size;i++){
        dst[i].real=src2[i].real*src1[i];
        dst[i].imagin=src2[i].imagin*src1[i];
    }
}
int ChangtoPower2(int size){
    size--;//避免为2的幂的size会被扩大
    int i=0;
    while ((size/=2)>0) {
        i++;
    }
    return 2<<i;
    
}
//将图像伸缩到2的幂次大小，并填充
void ResizeMatrix4FFT(double *src,double **dst,int width,int height){
    
    int re_width=ChangtoPower2(width);
    int re_height=ChangtoPower2(height);
    double *temp=(double *)malloc(sizeof(double)*re_width*re_height);
    Resize(src, width, height, temp, re_width, re_height);
    *dst=(double *)malloc(sizeof(double)*re_width*re_height*4);

    Zero(*dst, re_width*2, re_height*2);
    matrixCopy(temp, *dst, re_width, re_height);
    free(temp);
}
//将扩充后的图像还原为左上角的
void CutImage421(double *src,int s_width,int s_height,double *dst,int d_width,int d_height){
    Position p;
    p.x=0;
    p.y=0;
    matrixCopyLocal(src, dst, s_width, s_height, d_width, d_height, &p);
}
//频域滤波
double FrequencyFiltering(double *src,int width,int height,double *dst,int filter_type,double param1,int param2,double param3,double param4,double param5,int isgetPower){
    //IplImage *temp=NULL;
    double *temp=NULL;
    double power=0.0;
    //调整至2的幂，并用黑色填充，防止周期缠绕
    ResizeMatrix4FFT(src, &temp, width, height);
    int fft_width=width*2;
    int fft_height=height*2;
    //产生滤波器
    double *filter=(double *)malloc(sizeof(double)*fft_height*fft_width);
    if(filter==NULL){
        printf("frequency filter malloc faile");
        exit(0);
    }
    //生成滤波器
    switch(filter_type){
        case ILPF:
            IdealLPFilter(filter, fft_width, fft_height, param1);
            break;
        case BLPF:
            if(param2<0)
                param2=2;
            ButterworthLPFilter(filter, fft_width, fft_height, param1, param2);
            break;
        case GLPF:
            GaussianLPFilter(filter, fft_width, fft_height, param1);
            break;
        case IHPF:
            IdealHPFilter(filter, fft_width, fft_height, param1);
            break;
        case BHPF:
            if(param2<0)
                param2=2;
            ButterworthHPFilter(filter, fft_width, fft_height, param1, param2);
            break;
        case GHPF:
            GaussianHPFilter(filter, fft_width, fft_height, param1);
            break;
        case HOMPF :
            HomorphicFilter(filter, fft_width, fft_height,param1 , param2/*lamda_l*/, param3/*lamda_h*/,param4/*c*/);
            break;
        case HFEPF:
            HighFrequencyEmphasisFilter(filter, fft_width, fft_height, param4, param1, param2, param3,param5);
            break;

    }
    //FFT
    Complex *temp_complex=(Complex*)malloc(sizeof(Complex)*fft_height*fft_width);
    //fft结果
    if(temp_complex==NULL){
        exit(0);
    }
    ImageFFT(temp, temp_complex,fft_width,fft_height);
    if(isgetPower)
        power=getPower(temp_complex,fft_width*fft_height);
    //相乘
    MatrixMulti_R_C(filter,temp_complex,temp_complex,fft_width*fft_height);
    
    //IFFT
    ImageIFFT(temp_complex, temp, fft_width, fft_height);
   
    //还原图像
    double *result2=(double *)malloc(sizeof(double)*width*height);
    CutImage421(temp,fft_width,fft_height,result2,width,height);
    matrixCopy(result2, dst, width, height);
    free(result2);
    free(filter);
    free(temp_complex);
    free(temp);
    if(isgetPower)
        power=getPower(temp_complex,fft_width*fft_height)/power;
    return power;

}
