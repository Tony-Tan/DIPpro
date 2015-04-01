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
//  Hough
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/2/13.
//  Copyright (c) 2015年 谭升. All rights reserved.
//

#include "Hough.h"
#include <cv.h>
#include <highgui.h>
#include "Histogram.h"
#define POLARWIDTH (180)
#define POLARSTEP (M_PI/POLARWIDTH)
#define POLARHEIGHT_ZOOM 1

void DrawLine(double *image,int width,int height,double theta,int r){
    if(theta==M_PI_2||theta==-M_PI_2){
        for(int j=0;j<height;j++){
            if(r<height&&r>=0)
                image[j*width+r]=255.0;
        }
    }else{
        double tan_theta=tan(theta);
        for(int i=0;i<width;i++){
            int j=(int)(tan_theta*i+r);
            if(j<height&&j>=0)
                image[j*width+i]=255.0;
        }
    }
}


void SHT(int x,int y,int zero,double * polar){
    double angle_step=POLARSTEP;
    double angle=-M_PI_2;
    for(int i=0;i<POLARWIDTH;i++){

        int p_y=(int)(((sin(angle)*y+cos(angle)*x)+0.5)*POLARHEIGHT_ZOOM)+zero;
   
        polar[p_y*POLARWIDTH+i]++;
        angle+=angle_step;
    }
}

///////////////////////////////for test//////////////////////////////////////
void show_polar(double *src,int width,int height){
    IplImage *show=cvCreateImage(cvSize(width, height), 8, 1);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++)
            if(src[j*width+i]>0)
                cvSetReal2D(show, j, i, src[j*width+i]+128);
    cvSaveImage("/Users/Tony/DIPImage/matlab_polar.jpg", show, 0);
    cvNamedWindow("Polar", 1);
    cvShowImage("Polar",show);
    cvWaitKey(0);


}



/////////////////////////////////////////////////////////////////////////////
void HoughLine(double *src,double *dst,int width,int height,int lineLength){
    int polar_height=2*POLARHEIGHT_ZOOM*(int)(sqrt(width*width+height*height)+1);
    int polar_width=POLARWIDTH;
    double *polar=(double *)malloc(sizeof(double)*polar_height*polar_width);
    Zero(polar,polar_width,polar_height);
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
            if(src[j*width+i]==255.0)
                SHT(i, j,polar_height/2,polar);
        }
    }
    for(int j=0; j<polar_height;j++)
        for(int i=0;i<polar_width;i++){
            if(polar[j*polar_width+i]>lineLength){
                double theta=i*POLARSTEP;
                if(theta==M_PI_2)
                    DrawLine(dst, width, height, theta, abs(j-polar_height/2)/POLARHEIGHT_ZOOM);
                else if (theta==0)
                    DrawLine(dst, width, height, theta, abs(j-polar_height/2)/POLARHEIGHT_ZOOM);
                else{
                    DrawLine(dst, width, height, theta, -(int)((j-polar_height/2)/cos(i*POLARSTEP))/POLARHEIGHT_ZOOM);
                }
            }
        }
    free(polar);
}
