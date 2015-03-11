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
//  Color
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/03/10.
//  Copyright (c) 2015年 谭升. All rights reserved.
//

#include "color.h"
 
void RGB2HSV(RGB *src ,HSI *dst,int width,int height){
    double r,g,b;
    for(int i=0;i<width*height;i++){
        r=src[i].r;
        g=src[i].g;
        b=src[i].b;
        double delta=acos((2*r-g-b)/(2*sqrt((r-g)*(r-g)+(r-b)*(g-b))));
        dst[i].h=b<=g?delta:2*M_PI-delta;
        dst[i].s=1.0-3.0*(MIN3(r,g,b)/(r+g+b));
        dst[i].i=(r+g+b)/3.0;
    }

}
void HSV2RGB(HSI *src ,RGB *dst,int width,int height){
    double src_h,src_s,src_i;
    double r,g,b;
    for(int i=0;i<width*height;i++){
        src_h=src[i].h;
        src_s=src[i].s;
        src_i=src[i].i;
        if(src_h>=0&&src_h<(2./3.*M_PI)){
            b=src_i*(1.0-src_s);
            r=src_i*(1.0+(src_s*cos(src_h)/cos(1./3.*M_PI-src_h)));
            g=3*src_i-(r+b);
        }else if(src_h>=(2./3.*M_PI)&&src_h<(4./3.*M_PI)){
            src_h-=(2./3.*M_PI);
            r=src_i*(1.0-src_s);
            g=src_i*(1.0+(src_s*cos(src_h)/cos(1./3.*M_PI-src_h)));
            b=3*src_i-(r+g);
        }else if(src_h>=(4./3.*M_PI)&&src_h<(2.*M_PI)){
            src_h-=(4./3.*M_PI);
            g=src_i*(1.0-src_s);
            b=src_i*(1.0+(src_s*cos(src_h)/cos(1./3.*M_PI-src_h)));
            r=3*src_i-(r+b);
        }
        dst[i].r=r;
        dst[i].g=g;
        dst[i].b=b;
    }
}