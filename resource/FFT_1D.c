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
//  1D.c
//  Fourer
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 14/11/25.
//  Copyright (c) 2014年 谭升. All rights reserved.
//

#include "Fourer.h"

////////////////////////////////////////////////////////////////////
//计算WN
///////////////////////////////////////////////////////////////////
void getWN(double n,double size_n,Complex * dst){
    double x=2.0*M_PI*n/size_n;
    dst->imagin=-sin(x);
    dst->real=cos(x);
}
////////////////////////////////////////////////////////////////////
//随机初始化输入
///////////////////////////////////////////////////////////////////
void setInput(double * data,int  n){
    printf("Setinput signal:\n");
    srand((int)time(0));
    for(int i=0;i<n;i++){
        data[i]=rand()%VALUE_MAX;
        
    }
    
}
////////////////////////////////////////////////////////////////////
//标准DFT
///////////////////////////////////////////////////////////////////
void DFT(double * src,Complex * dst,int size){

    for(int m=0;m<size;m++){
        double real=0.0;
        double imagin=0.0;
        for(int n=0;n<size;n++){
            double x=M_PI*2*m*n;
            real+=src[n]*cos(x/size);
            imagin+=src[n]*(-sin(x/size));
            
        }
        dst[m].imagin=imagin;
        dst[m].real=real;
        
    }
}
////////////////////////////////////////////////////////////////////
//IDT，复原傅里叶
///////////////////////////////////////////////////////////////////
void IDFT(Complex *src,Complex *dst,int size){
    for(int m=0;m<size;m++){
        double real=0.0;
        double imagin=0.0;
        for(int n=0;n<size;n++){
            double x=M_PI*2*m*n/size;
            real+=src[n].real*cos(x)-src[n].imagin*sin(x);
            imagin+=src[n].real*sin(x)+src[n].imagin*cos(x);
            
        }
        real/=SIZE;
        imagin/=SIZE;
        if(dst!=NULL){
            dst[m].real=real;
            dst[m].imagin=imagin;
        }
    }
    
    
}
////////////////////////////////////////////////////////////////////
//FFT前，对输入数据重新排序
///////////////////////////////////////////////////////////////////
int FFTReal_remap(double * src,int size_n){
    
    if(size_n==1)
        return 0;
    double * temp=(double *)malloc(sizeof(double)*size_n);
    for(int i=0;i<size_n;i++)
        if(i%2==0)
            temp[i/2]=src[i];
        else
            temp[(size_n+i)/2]=src[i];
    for(int i=0;i<size_n;i++)
        src[i]=temp[i];
    free(temp);
    FFTReal_remap(src, size_n/2);
    FFTReal_remap(src+size_n/2, size_n/2);
    return 1;
    
    
}
/*
 将数据分为两组，奇数一组，偶数一组，递归调用
 */
int FFTComplex_remap(Complex * src,int size_n){
    
    if(size_n==1)
        return 0;
    Complex * temp=(Complex *)malloc(sizeof(Complex)*size_n);
    for(int i=0;i<size_n;i++)
        if(i%2==0)
            Copy_Complex(&src[i],&(temp[i/2]));
        else
            Copy_Complex(&(src[i]),&(temp[(size_n+i)/2]));
    for(int i=0;i<size_n;i++)
        Copy_Complex(&(temp[i]),&(src[i]));
    free(temp);
    FFTComplex_remap(src, size_n/2);
    FFTComplex_remap(src+size_n/2, size_n/2);
    return 1;
    
    
}
////////////////////////////////////////////////////////////////////
//FFT公式，蝶形算法
///////////////////////////////////////////////////////////////////
void FFT(Complex * src,Complex * dst,int size_n){
    
    int k=size_n;
    int z=0;
    while (k/=2) {
        z++;
    }
    k=z;
    if(size_n!=(1<<k))
        exit(0);
    Complex * src_com=(Complex*)malloc(sizeof(Complex)*size_n);
    if(src_com==NULL)
        exit(0);
    for(int i=0;i<size_n;i++){
        Copy_Complex(&src[i], &src_com[i]);
    }
    FFTComplex_remap(src_com, size_n);
    for(int i=0;i<k;i++){
        z=0;
        for(int j=0;j<size_n;j++){
            if((j/(1<<i))%2==1){
                Complex wn;
                getWN(z, size_n, &wn);
                Multy_Complex(&src_com[j], &wn,&src_com[j]);
                z+=1<<(k-i-1);
                Complex temp;
                int neighbour=j-(1<<(i));
                temp.real=src_com[neighbour].real;
                temp.imagin=src_com[neighbour].imagin;
                Add_Complex(&temp, &src_com[j], &src_com[neighbour]);
                Sub_Complex(&temp, &src_com[j], &src_com[j]);
            }
            else
                z=0;
        }
        
    }
    
    
    for(int i=0;i<size_n;i++){
        Copy_Complex(&src_com[i], &dst[i]);
    }
    free(src_com);
    

}
/*
 实数的快速傅里叶变换
 */
void RealFFT(double * src,Complex * dst,int size_n){
    

    int k=size_n;
    int z=0;
    while (k/=2) {
        z++;
    }
    k=z;
    if(size_n!=(1<<k))
        exit(0);
    Complex * src_com=(Complex*)malloc(sizeof(Complex)*size_n);
    if(src_com==NULL)
        exit(0);
    for(int i=0;i<size_n;i++){
        src_com[i].real=src[i];
        src_com[i].imagin=0;
    }
    FFTComplex_remap(src_com, size_n);
    for(int i=0;i<k;i++){
        z=0;
        for(int j=0;j<size_n;j++){
            if((j/(1<<i))%2==1){
                Complex wn;
                getWN(z, size_n, &wn);
                Multy_Complex(&src_com[j], &wn,&src_com[j]);
                z+=1<<(k-i-1);
                Complex temp;
                int neighbour=j-(1<<(i));
                temp.real=src_com[neighbour].real;
                temp.imagin=src_com[neighbour].imagin;
                Add_Complex(&temp, &src_com[j], &src_com[neighbour]);
                Sub_Complex(&temp, &src_com[j], &src_com[j]);
            }
            else
                z=0;
        }
        
    }
    
    
    for(int i=0;i<size_n;i++){
        Copy_Complex(&src_com[i], &dst[i]);
    }
    free(src_com);
    
}

////////////////////////////////////////////////////////////////////
//IFFT实现
////////////////////////////////////////////////////////////////////
void IFFT(Complex * src,Complex * dst,int size_n){
    for(int i=0;i<size_n;i++)
        src[i].imagin=-src[i].imagin;
    FFTComplex_remap(src, size_n);
    int z,k;
    if((z=isBase2(size_n))!=-1)
        k=isBase2(size_n);
    else
        exit(0);
    for(int i=0;i<k;i++){
        z=0;
        for(int j=0;j<size_n;j++){
            if((j/(1<<i))%2==1){
                Complex wn;
                getWN(z, size_n, &wn);
                Multy_Complex(&src[j], &wn,&src[j]);
                z+=1<<(k-i-1);
                Complex temp;
                int neighbour=j-(1<<(i));
                Copy_Complex(&src[neighbour], &temp);
                Add_Complex(&temp, &src[j], &src[neighbour]);
                Sub_Complex(&temp, &src[j], &src[j]);
            }
            else
                z=0;
        }
        
    }
    for(int i=0;i<size_n;i++){
            dst[i].imagin=(1./size_n)*src[i].imagin;
            dst[i].real=(1./size_n)*src[i].real;
        }
    
}