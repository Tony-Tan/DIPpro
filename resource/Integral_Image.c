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
//  Integral_Image
//  积分图像
//  Created by 谭升 on 14/12/23.
//  tony.sheng.tan@gmail.com
//  Copyright (c) 2014年 谭升. All rights reserved.
//
#include "Integral_Image.h"
/*
 图像积分，即一个像素点等于其左上方全部像素值之和
 */
void Inte_Image(IplImage * src,IplImage *dst){
    if(dst->depth!=IPL_DEPTH_64F||
       src->width!=dst->width   ||
       src->height!=dst->height ||
       src->nChannels!=1        ||
       dst->nChannels!=1)
        exit(0);
    int width=src->width;
    int height=src->height;
    double value;
    for(int j=1;j<height;j++)
        for(int i=1;i<width;i++){
            value=cvGetReal2D(src,j,i-1)+cvGetReal2D(src, j-1, i);
            cvSetReal2D(dst,j,i, value);
        }
    for(int i=1;i<width;i++){
        value=cvGetReal2D(src,0, i)+cvGetReal2D(src, 0, i-1);
        cvSetReal2D(dst,  0, i,value);
    }
    for(int i=1;i<height;i++){
        value=cvGetReal2D(src, i, 0)+cvGetReal2D(src, i-1, 0);
        cvSetReal2D(dst, i, 0,value);
    }
    value=cvGetReal2D(src, 0, 0);
    cvSetReal2D(dst, 0, 0, value);

}