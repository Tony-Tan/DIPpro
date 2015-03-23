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
//  Remap
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/03/23.
//  Copyright (c) 2015年 谭升. All rights reserved.
//

#include "Resize.h"
//|----|----|
//| 11 | 12 |
//|----|----|
//| 21 | 22 |
//|----|----|

void Resize(double *src,int s_width,int s_height,double *dst,int d_width,int d_height){
    double r_width=(double)s_width/(double)d_width;
    double r_height=(double)s_height/(double)d_height;
    double x,y;
    double v11,v12,v21,v22;
    double value;
    for(int j=0;j<d_height;j++){
        y=(double)j*r_height;
        for(int i=0;i<d_width;i++){
            x=(double)i*r_width;
            if((int)x==s_width-1&&(int)y<s_height-1){
                v11=src[(int)y*s_width+(int)x];
                v21=src[((int)y+1)*s_width+(int)x];
                double d_y=y-(double)((int)y);
                value=d_y*v21+(1-d_y)*v11;
            }
            else if((int)x<s_width-1&&(int)y==s_height-1){
                v11=src[(int)y*s_width+(int)x];
                v12=src[(int)y*s_width+(int)x+1];
                double d_x=x-(double)((int)x);
                value=d_x*v12+(1-d_x)*v11;
            }
            else if((int)x==s_width-1&&(int)y==s_height-1){
                value=src[(int)y*s_width+(int)x];
            }
            else{
                v11=src[(int)y*s_width+(int)x];
                v12=src[(int)y*s_width+(int)x+1];
                v21=src[((int)y+1)*s_width+(int)x];
                v22=src[((int)y+1)*s_width+(int)x+1];
                double d_x=x-(double)((int)x);
                double d_y=y-(double)((int)y);
                value=(v22*d_x+(1-d_x)*v21)*d_y+(1-d_y)*(v12*d_x+(1-d_x)*v11);
                //value=0.0;
            }
            dst[j*d_width+i]=value;
        }
    }
}
