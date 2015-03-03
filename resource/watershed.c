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
//  Watershed
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/03/03.
//  Copyright (c) 2015年 谭升. All rights reserved.
//

#include "watershed.h"

Pix_Label * ExImageArray(double *src,int width,int height){
    Pix_Label * dst=(Pix_Label *)malloc(sizeof(Pix_Label)*width*height);
    for(int i=0;i<width*height;i++){
        dst[i].grayvalue=(int)src[i];
        dst[i].label=INIT;
    }
    return dst;
}
void MakeNewNode(int x,int y){


}
void inQueue(int gray_level);
NLevelPriNode outQueue(int gray_level);
void releaseNode(NLevelPriNode node);
void releasePix_Label(Pix_Label * src){


}

void findMinimal(double *src,double *dst,int width,int height){
    Zero(dst, width, height);
    int lessthan=0;
    int equ=0;
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
            lessthan=0;
            equ=0;
            double pix=src[j*width+i];
            for(int m=-1;m<2;m++)
                for(int n=-1;n<2;n++)
                    if(j+m>=0&&i+n>=0&&j+m<height&&i+n<width){
                        if(m!=0||n!=0){
                            if(src[(j+m)*width+i+n]<pix)
                                lessthan=1;
                            if(src[(j+m)*width+i+n]==pix)
                                equ=1;
                        }
                    }
            if(lessthan)
                dst[j*width+i]=0;
            else if(equ)
                dst[j*width+i]=127.0;
            if(0==lessthan&&0==equ)
                dst[j*width+i]=255.0;
        }
    }
}
void MeyerWatershed(double *src,double *dst,int width,int height){

    


}