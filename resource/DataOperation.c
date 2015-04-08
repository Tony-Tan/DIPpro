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
//  DataOperation
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/04/08.
//  Copyright (c) 2015年 谭升. All rights reserved.
//

#include "DataOperation.h"
void swap_dbl(double * s1,double * s2){
    double temp=*s1;
    *s1=*s2;
    *s2=temp;
}

int Sort_quick_Rec(double *src,int left,int right){
    if(right<=left)
        return 0;
    double std_value=src[left];
    int empty_space=left;
    for(int j=right-1,i=left;j>i;j--){
        if(src[j]<std_value){
            src[empty_space]=src[j];
            empty_space=j;
            for(;i<j;i++){
                if(src[i]>=std_value){
                    src[empty_space]=src[i];
                    empty_space=i;
                    break;
                }
            }
        }
    }
    src[empty_space]=std_value;
    Sort_quick_Rec(src, left, empty_space);
    Sort_quick_Rec(src, empty_space+1,right);
    return 1;
}

void Sort_quick(double *src,double *dst ,int size){
    double *temp=(double *)malloc(sizeof(double)*size);
    matrixCopy(src, temp,1, size);
    Sort_quick_Rec(temp, 0, size);
    matrixCopy(temp, dst, 1, size);
    free(temp);


}


