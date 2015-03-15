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
//  Pseudo_Color
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/05/15.
//  Copyright (c) 2015年 谭升. All rights reserved.
//

#include "Pseudo_Color.h"

void Gray2Color(double *src,RGB* dst,int width,int height,int type){
    HSV* temp=(HSV*)malloc(sizeof(HSV)*width*height);
    for(int i=0;i<width*height;i++){
        double gray_value=src[i];
        if(type==HIGHVALUE_EQU_RED)
            temp[i].c1=HSVMAX-GRAY2HSV*gray_value;
        else if(type==LOWVALUE_EQU_RED)
            temp[i].c1=GRAY2HSV*gray_value;
        temp[i].c2=1.0;
        temp[i].c3=1.0;
    }
    HSV2RGB(temp, dst, width, height);
    free(temp);
}