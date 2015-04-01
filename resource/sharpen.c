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
//  Sharpen
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/1/30.
//  Copyright (c) 2014年 谭升. All rights reserved.
//

#include "sharpen.h"



void LaplaceSharpen(double *src,double *dst,int width,int height,double c){
    
    Laplace(src,dst,width,height);
    matrixMultreal(dst,dst,c,width,height);
    matrixAdd(src,dst,dst,width,height);

}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

//非锐化掩蔽
void UnsharpMasking(double *src,double *dst,int width,int height,int smooth_type,int smooth_mask_width,int smooth_mask_height,double gaussian_deta,double k){
    switch (smooth_type) {
        case SMOOTH_GAUSSIAN:
            GaussianFilter(src, dst,width,height, smooth_mask_width, smooth_mask_height,gaussian_deta);
            break;
        case SMOOTH_MEAN:
            MeanFilter(src, dst,width,height, smooth_mask_width, smooth_mask_height);
            break;
        default:
            break;
    }
    matrixSub(src, dst, dst, width, height);
    matrixMultreal(dst, dst, k, width, height);
    matrixAdd(src, dst, dst, width, height);


}

////////////////////////////////////////////////////////////////////////////////////
//sobel锐化
////////////////////////////////////////////////////////////////////////////////////

void SobelSharpen(double *src,double *dst,int width,int height,double c){
    Sobel(src,dst,NULL,width,height,3);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            dst[j*width+i]=src[j*width+i]+c*dst[j*width+i];
        }
    
    
}
////////////////////////////////////////////////////////////////////////////////////
//robert锐化
////////////////////////////////////////////////////////////////////////////////////

void RobertSharpen(double *src,double *dst,int width,int height,double c){
    Robert(src,dst,NULL,width,height);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            dst[j*width+i]=src[j*width+i]+c*dst[j*width+i];
        }
    
    
}
