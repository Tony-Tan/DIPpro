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