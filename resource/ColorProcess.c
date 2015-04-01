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
//  ColorProcess
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/03/16.
//  Copyright (c) 2015年 谭升. All rights reserved.
//

#include "ColorProcess.h"
/********************************************************************/
//直方图均衡RGB，先转换成HSI，然后均衡I通道
void HistEqualRGB(RGB *src,RGB *dst,int width,int height){
    HSI *temp=(HSI*)malloc(sizeof(HSI)*width*height);
    double *chanel_i=(double *)malloc(sizeof(double)*width*height);
    RGB2HSI(src, temp, width, height);
    for(int i=0;i<width*height;i++){
        chanel_i[i]=(double)((int)temp[i].c3);
    }
    HistogramEqualization(chanel_i, chanel_i, width, height);
    for(int i=0;i<width*height;i++){
        temp[i].c3=chanel_i[i];
    }
    HSI2RGB(temp, dst, width, height);
    free(temp);
    free(chanel_i);
    
}
/*****************************************************************/
//平滑RGB三通道模型
void SmoothRGB(RGB *src,RGB *dst,int width,int height,int m_width,int m_height,double param1,double param2,int Smooth_type){
    double *chanel_r=(double*)malloc(sizeof(double)*width*height);
    double *chanel_g=(double*)malloc(sizeof(double)*width*height);
    double *chanel_b=(double*)malloc(sizeof(double)*width*height);
    double *chanel_r_dst=(double*)malloc(sizeof(double)*width*height);
    double *chanel_g_dst=(double*)malloc(sizeof(double)*width*height);
    double *chanel_b_dst=(double*)malloc(sizeof(double)*width*height);
    Split(src, chanel_r, chanel_g, chanel_b, width, height);
    switch (Smooth_type) {
        case SMOOTH_GAUSSIAN:{
            GaussianFilter(chanel_r, chanel_r_dst, width, height, m_width, m_height, param1);
            GaussianFilter(chanel_g, chanel_g_dst, width, height, m_width, m_height, param1);
            GaussianFilter(chanel_b, chanel_b_dst, width, height, m_width, m_height, param1);
            break;
        }
        case SMOOTH_MEDIAN:{
            MedianFilter(chanel_r, chanel_r_dst, width, height, m_width, m_height);
            MedianFilter(chanel_g, chanel_g_dst, width, height, m_width, m_height);
            MedianFilter(chanel_b, chanel_b_dst, width, height, m_width, m_height);
            break;
        }
        case SMOOTH_BILATERAL:{
            BilateralFilter(chanel_r, chanel_r_dst, width, height, m_width, m_height, param1, param2);
            BilateralFilter(chanel_g, chanel_g_dst, width, height, m_width, m_height, param1, param2);
            BilateralFilter(chanel_b, chanel_b_dst, width, height, m_width, m_height, param1, param2);
            break;
        }
        case SMOOTH_MEAN:{
            MeanFilter(chanel_r, chanel_r_dst, width, height, m_width, m_height);
            MeanFilter(chanel_g, chanel_g_dst, width, height, m_width, m_height);
            MeanFilter(chanel_b, chanel_b_dst, width, height, m_width, m_height);
            break;
            
        }
        default:
            break;
    }

    Merge(chanel_r_dst, chanel_g_dst, chanel_b_dst, dst, width, height);
    free(chanel_r);
    free(chanel_g);
    free(chanel_b);
    free(chanel_r_dst);
    free(chanel_g_dst);
    free(chanel_b_dst);
}

/********************************************************************/
//平滑针对HSI中的I通道
void SmoothHSI(HSI *src,HSI *dst,int width,int height,int m_width,int m_height,double param1,double param2,int Smooth_type){
    double *chanel_i=(double*)malloc(sizeof(double)*width*height);
    double *chanel_i_dst=(double*)malloc(sizeof(double)*width*height);
    Split(src, NULL, NULL, chanel_i, width, height);
    switch (Smooth_type) {
        case SMOOTH_GAUSSIAN:{
            GaussianFilter(chanel_i, chanel_i_dst, width, height, m_width, m_height, param1);
            break;
        }
        case SMOOTH_MEDIAN:{
            MedianFilter(chanel_i, chanel_i_dst, width, height, m_width, m_height);
            break;
        }
        case SMOOTH_BILATERAL:{
            BilateralFilter(chanel_i, chanel_i_dst, width, height, m_width, m_height, param1, param2);
            break;
        }
        case SMOOTH_MEAN:{
            MeanFilter(chanel_i, chanel_i_dst, width, height, m_width, m_height);
            break;
        }
        default:
            break;
    }
    for(int i=0;i<width*height;i++){
        dst[i].c1=src[i].c1;
        dst[i].c2=src[i].c2;
        dst[i].c3=chanel_i_dst[i];
    }
    free(chanel_i);
    free(chanel_i_dst);
}

/********************************************************************/

//分别对RGB进行处理

void SharpenRGB(RGB *src,RGB *dst,int width,int height,double c,int Sharpen_type){
    double *chanel_r=(double*)malloc(sizeof(double)*width*height);
    double *chanel_g=(double*)malloc(sizeof(double)*width*height);
    double *chanel_b=(double*)malloc(sizeof(double)*width*height);
    double *chanel_r_dst=(double*)malloc(sizeof(double)*width*height);
    double *chanel_g_dst=(double*)malloc(sizeof(double)*width*height);
    double *chanel_b_dst=(double*)malloc(sizeof(double)*width*height);
    Split(src, chanel_r, chanel_g, chanel_b, width, height);
    switch (Sharpen_type) {
        case SHARPEN_LAPLACE:{
            LaplaceSharpen(chanel_r, chanel_r_dst, width, height, c);
            LaplaceSharpen(chanel_g, chanel_g_dst, width, height, c);
            LaplaceSharpen(chanel_b, chanel_b_dst, width, height, c);
            break;
        }
        case SHARPEN_SOBEL:{
            SobelSharpen(chanel_r, chanel_r_dst, width, height, c);
            SobelSharpen(chanel_g, chanel_g_dst, width, height, c);
            SobelSharpen(chanel_b, chanel_b_dst, width, height, c);
            break;
        }
        case SHARPEN_ROBERT:{
            RobertSharpen(chanel_r, chanel_r_dst, width, height, c);
            RobertSharpen(chanel_g, chanel_g_dst, width, height, c);
            RobertSharpen(chanel_b, chanel_b_dst, width, height, c);
            break;
        }
        default:
            break;
    }
    
    Merge(chanel_r_dst, chanel_g_dst, chanel_b_dst, dst, width, height);
    free(chanel_r);
    free(chanel_g);
    free(chanel_b);
    free(chanel_r_dst);
    free(chanel_g_dst);
    free(chanel_b_dst);
}
/*****************************************************************/

//锐化HSI色彩空间，直接对I进行操作
void SharpenHSI(HSI *src,HSI *dst,int width,int height,double c,int Sharpen_type){
    double *chanel_i=(double*)malloc(sizeof(double)*width*height);
    double *chanel_i_dst=(double*)malloc(sizeof(double)*width*height);
    Split(src, NULL, NULL, chanel_i, width, height);
    switch (Sharpen_type) {
        case SHARPEN_LAPLACE:{
            LaplaceSharpen(chanel_i, chanel_i_dst, width, height, c);
            break;
        }
        case SHARPEN_SOBEL:{
            SobelSharpen(chanel_i, chanel_i_dst, width, height, c);
            break;
        }
        case SHARPEN_ROBERT:{
            RobertSharpen(chanel_i, chanel_i_dst, width, height, c);
            break;
        }
        default:
            break;
    }
    for(int i=0;i<width*height;i++){
        dst[i].c1=src[i].c1;
        dst[i].c2=src[i].c2;
        dst[i].c3=chanel_i_dst[i];
    }
    free(chanel_i);
    free(chanel_i_dst);
}
/*********************************************************************/

//3通道向量距离，
//对于点x，y有彩色向量（c1,c2,c3）
//给定标准颜色（r,g,b）
//其距离为|(c1,c2,c3)-(r,g,b)|
double Chanel3Distance(RGB point1,RGB point2){
    return sqrt((point1.c1-point2.c1)*(point1.c1-point2.c1)+
                (point1.c2-point2.c2)*(point1.c2-point2.c2)+
                (point1.c3-point2.c3)*(point1.c3-point2.c3));


}
void SegmentRGB(RGB* src,RGB *dst,int width,int height,RGB * color_center,double threshold){
    double distance=0.0;
    for(int i=0;i<width*height;i++){
        distance=Chanel3Distance(src[i], *color_center);
        if(distance<=threshold){
            dst[i].c1=src[i].c1;
            dst[i].c2=src[i].c2;
            dst[i].c3=src[i].c3;
        }else{
            dst[i].c1=0.;
            dst[i].c2=0.;
            dst[i].c3=0.;
        }
    }
}
/*********************************************************************/

///覆盖彩色图像，cover为的0.0的部分表示透明，否则显示Cover颜色
void Cover_RGB(RGB *src,RGB *dst,RGB *cover,int width,int height){
    double cover_c1,cover_c2,cover_c3;
    for(int i=0;i<width*height;i++){
        cover_c1=cover[i].c1;
        cover_c2=cover[i].c2;
        cover_c3=cover[i].c3;
        if(cover_c1!=0.0&&cover_c2!=0.0&&cover_c3!=0.0){
            dst[i].c1=cover_c1;
            dst[i].c2=cover_c2;
            dst[i].c3=cover_c3;
        }else {
            dst[i].c1=src[i].c1;
            dst[i].c2=src[i].c2;
            dst[i].c3=src[i].c3;
        }
    }
}
//归零化RGB图像
void Zero_RGB(RGB *src,int width,int height){
    for(int i=0;i<width*height;i++){
        src[i].c1=0.0;
        src[i].c2=0.0;
        src[i].c3=0.0;
    }

}
//复制三通道数据
void Copy_RGB(RGB *src,RGB *dst,int width,int height){
    for(int i=0;i<width*height;i++){
        dst[i].c1=src[i].c1;
        dst[i].c2=src[i].c2;
        dst[i].c3=src[i].c3;
    }
    
}
//阈值分割RGB图像
void Threshold_RGB(RGB *src,RGB *dst,RGB *threshold,int width,int height){
    RGB *temp=(RGB *)malloc(sizeof(RGB)*width*height);
    Zero_RGB(temp,width,height);
    double threshold_c1=threshold->c1;
    double threshold_c2=threshold->c2;
    double threshold_c3=threshold->c3;
    for(int i=0;i<width*height;i++){
        if(src[i].c1>=threshold_c1&&
           src[i].c2>=threshold_c2&&
           src[i].c3>=threshold_c3){
            temp[i].c1=src[i].c1;
            temp[i].c2=src[i].c2;
            temp[i].c3=src[i].c3;
        }
            
    }
    Copy_RGB(temp,dst,width, height);
    free(temp);
}