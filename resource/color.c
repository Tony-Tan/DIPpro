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

void Split(C3 *src ,double *dst1,double *dst2,double *dst3,int width,int height){
    for(int i=0;i<width*height;i++){
        if(dst1!=NULL)dst1[i]=src[i].c1;
        if(dst2!=NULL)dst2[i]=src[i].c2;
        if(dst3!=NULL)dst3[i]=src[i].c3;
    }
    
}
void Merge(double *src1,double *src2,double *src3,C3 *dst ,int width,int height){
    for(int i=0;i<width*height;i++){
        dst[i].c1=(src1!=NULL?src1[i]:0.0);
        dst[i].c2=(src2!=NULL?src2[i]:0.0);
        dst[i].c3=(src3!=NULL?src3[i]:0.0);
    }
}
//本文见出现的未声明的常数都为转换系数，详情可以根据函数功能查询原公式
/**********************************************************************************************************************/
void RGB2XYZ(RGB *src ,XYZ *dst,int width,int height){
    double r,g,b;
    for(int i=0;i<width*height;i++){
        r=src[i].c1;
        g=src[i].c2;
        b=src[i].c3;
        dst[i].c1=0.49000*r+0.31000*g+0.20000*b;
        dst[i].c2=0.17697*r+0.81240*g+0.01063*b;
        dst[i].c3=          0.01000*g+0.99000*b;
    }
    
}
void XYZ2RGB(XYZ *src ,RGB *dst,int width,int height){
    double x,y,z;
    for(int i=0;i<width*height;i++){
        x=src[i].c1;
        y=src[i].c2;
        z=src[i].c3;
        dst[i].c1=2.364613846538365*x-0.896540570739668*y-0.468073275798697*z;
        dst[i].c2=-0.515166208447888*x+1.426408103856389*y+0.088758104591499*z;
        dst[i].c3=0.005203699075231*x-0.014408162665216*y+1.009204463589985*z;
    }
}
/**********************************************************************************************************************/

void RGB2nRGB(RGB *src ,nRGB *dst,int width,int height){
    double r,g,b;
    for(int i=0;i<width*height;i++){
        r=src[i].c1;
        g=src[i].c2;
        b=src[i].c3;
        dst[i].c1= 0.842*r+0.156*g+0.091*b;
        dst[i].c2=-0.129*r+1.320*g-0.203*b;
        dst[i].c3= 0.008*r-0.069*g+0.897*b;
    }
    
}
void nRGB2RGB(nRGB *src ,RGB *dst,int width,int height){
    double nr,ng,nb;
    for(int i=0;i<width*height;i++){
        nr=src[i].c1;
        ng=src[i].c2;
        nb=src[i].c3;
        dst[i].c1=1.166653319332446*nr-0.142379679989947*ng-0.086134199577807*nb;
        dst[i].c2=0.114262295661615*nr+0.734773840666534*ng-0.177878437637139*nb;
        dst[i].c3=-0.001615527484959*nr+0.057790894588529*ng+1.101912443031951*nb;
    }
}
/**********************************************************************************************************************/

void RGB2CMY(RGB *src ,CMY *dst,int width,int height){
    for(int i=0;i<width*height;i++){
        dst[i].c1=GMAX-src[i].c1;
        dst[i].c2=GMAX-src[i].c2;
        dst[i].c3=GMAX-src[i].c3;
    }
}
void CMY2RGB(CMY *src ,RGB *dst,int width,int height){
    for(int i=0;i<width*height;i++){
        dst[i].c1=GMAX-src[i].c1;
        dst[i].c2=GMAX-src[i].c2;
        dst[i].c3=GMAX-src[i].c3;
    }
}
/**********************************************************************************************************************/

void RGB2sRGB(RGB *src ,sRGB *dst,int width,int height){
    XYZ *temp=(XYZ *)malloc(sizeof(XYZ)*width*height);
    if(NULL==temp){
        exit(0);
    }
    RGB2XYZ(src, temp, width, height);
    double x,y,z;
    for(int i=0;i<width*height;i++){
        x=temp[i].c1;
        y=temp[i].c2;
        z=temp[i].c3;
        dst[i].c1=3.2410*x-1.5274*y-0.4986*z;
        dst[i].c2=-0.9692*x+1.876*y+0.0416*z;
        dst[i].c3=0.05556*x-0.204*y+1.057*z;
        
    }
    free(temp);
}
void sRGB2RGB(sRGB *src ,RGB *dst,int width,int height){
    XYZ *temp=(XYZ *)malloc(sizeof(XYZ)*width*height);
    if(NULL==temp){
        exit(0);
    }
    double sr,sg,sb;
    for(int i=0;i<width*height;i++){
        sr=src[i].c1;
        sg=src[i].c2;
        sb=src[i].c3;
        temp[i].c1=0.411508905881629*sr+0.354632552537474*sg+0.180156694689708*sb;
        temp[i].c2=0.212169935292699*sr+0.713622508110235*sg+0.071997382591820*sb;
        temp[i].c3=0.019318100273347*sr+0.119087613089409*sg+0.950499489206974*sb;
    }
    XYZ2RGB(temp, dst, width, height);
    free(temp);
}
/**********************************************************************************************************************/

void RGB2YIQ(RGB *src ,YIQ *dst,int width,int height){
    double r,g,b;
    for(int i=0;i<width*height;i++){
        r=src[i].c1;
        g=src[i].c2;
        b=src[i].c3;
        dst[i].c1=0.299*r+0.587*g+0.114*b;
        dst[i].c2=0.596*r-0.275*g-0.321*b;
        dst[i].c3=0.212*r-0.528*g+0.311*b;
    }

}
void YIQ2RGB(YIQ *src ,RGB *dst,int width,int height){
    double sy,si,sq;
    for(int i=0;i<width*height;i++){
        sy=src[i].c1;
        si=src[i].c2;
        sq=src[i].c3;
        dst[i].c1=sy+0.956*si+0.620*sq;
        dst[i].c2=sy-0.272*si-0.647*sq;
        dst[i].c3=sy-1.108*si+1.705*sq;
    }
}
/**********************************************************************************************************************/

void RGB2YUV(RGB *src ,YUV *dst,int width,int height){
    double r,g,b;
    for(int i=0;i<width*height;i++){
        r=src[i].c1;
        g=src[i].c2;
        b=src[i].c3;
        dst[i].c1=0.299*r+0.587*g+0.114*b;              //0~255
        dst[i].c2=-0.169*r-0.331*g+0.5*b+128.;          //0~255
        dst[i].c3=0.5*g-0.419*g-0.081*b+128;            //0~255
    }


}
void YUV2RGB(YUV *src ,RGB *dst,int width,int height){
    double sy,su,sv;
    for(int i=0;i<width*height;i++){
        sy=src[i].c1;
        su=src[i].c2;
        sv=src[i].c3;
        dst[i].c1=sy+1.13983*(sv-128.0);
        dst[i].c1=sy-0.39465*(su-128.0)-0.58060*(sv-128.0);
        dst[i].c1=sy+2.03211*(su-128.0);
    }


}
/**********************************************************************************************************************/

void RGB2YCbCr(RGB *src ,YCbCr *dst,int width,int height){
    double r,g,b;
    for(int i=0;i<width*height;i++){
        r=src[i].c1;
        g=src[i].c2;
        b=src[i].c3;
        dst[i].c1=0.257*r+0.564*g+0.098*b+16;
        dst[i].c2=-0.148*r-0.291*g+0.439*b+128;
        dst[i].c3=0.439*r-0.368*g-0.071*b+128;
    }

}
void YCbCr2RGB(YCbCr *src ,RGB *dst,int width,int height){
    double y,cb,cr;
    for(int i=0;i<width*height;i++){
        y =src[i].c1;
        cb=src[i].c2;
        cr=src[i].c3;
        dst[i].c1 = 1.164*(y-16)+1.596*(cr-128);
        dst[i].c2 = 1.164*(y-16)-0.392*(cb-128)-0.813*(cr-128);
        dst[i].c3 = 1.164*(y-16)+2.017*(cb-128);
    }
}
/**********************************************************************************************************************/

void RGB2YC1C2(RGB *src ,YC1C2 *dst,int width,int height){
    //todo something

}
void YC1C22RGB(YC1C2 *src ,RGB *dst,int width,int height){
    //todo something

}
/**********************************************************************************************************************/

void RGB2LAB(RGB *src ,LAB *dst,int width,int height){


}
void LAB2RGB(LAB *src ,RGB *dst,int width,int height){


}
/**********************************************************************************************************************/

void RGB2LUV(RGB *src ,LUV *dst,int width,int height){



}
void LUV2RGB(LUV *src ,RGB *dst,int width,int height){


}



/**********************************************************************************************************************/


 
void RGB2HSI(RGB *src ,HSI *dst,int width,int height){
    double r,g,b;
    for(int i=0;i<width*height;i++){
        r=src[i].c1;
        g=src[i].c2;
        b=src[i].c3;
        double delta=acos((2*r-g-b)/(2*sqrt((r-g)*(r-g)+(r-b)*(g-b))));
        dst[i].c1=b<=g?delta:2*M_PI-delta;
        dst[i].c2=1.0-3.0*(MIN3(r,g,b)/(r+g+b));
        dst[i].c3=(r+g+b)/3.0;
    }

}
void HSI2RGB(HSI *src ,RGB *dst,int width,int height){
    double src_h,src_s,src_i;
    double r,g,b;
    for(int i=0;i<width*height;i++){
        src_h=src[i].c1;
        src_s=src[i].c2;
        src_i=src[i].c3;
        if(src_h>=0.0&&src_h<(2./3.*M_PI)){
            b=src_i*(1.0-src_s);
            r=src_i*(1.0+(src_s*cos(src_h)/cos(1./3.*M_PI-src_h)));
            g=3.0*src_i-(r+b);
        }else if(src_h>=(2./3.*M_PI)&&src_h<(4./3.*M_PI)){
            src_h-=(2./3.*M_PI);
            r=src_i*(1.0-src_s);
            g=src_i*(1.0+(src_s*cos(src_h)/cos(1./3.*M_PI-src_h)));
            b=3.0*src_i-(r+g);
        }else if(src_h>=(4./3.*M_PI)&&src_h<(2.*M_PI)){
            src_h-=(4./3.*M_PI);
            g=src_i*(1.0-src_s);
            b=src_i*(1.0+(src_s*cos(src_h)/cos(1./3.*M_PI-src_h)));
            r=3.0*src_i-(g+b);
        }
        dst[i].c1=r;
        dst[i].c2=g;
        dst[i].c3=b;
    }
}
/**********************************************************************************************************************/

void RGB2HSV(RGB *src ,HSV *dst,int width,int height){
    double max=0.0,min=0.0;
    double r,g,b;
    for(int i=0;i<width*height;i++){
        r=src[i].c1;
        g=src[i].c2;
        b=src[i].c3;
        max=MAX3(r, g, b);
        min=MIN3(r, g, b);
        dst[i].c3=max/(double)(GRAY_LEVEL-1.0);//h
        if(max!=0)
            dst[i].c2=(max-min)/max;
        else{
            dst[i].c2=0.0;
            dst[i].c1=0.0;
        }
        if(dst[i].c2>0.0){
            double delta=max-min;
            double dr=(max-r)/delta;
            double dg=(max-g)/delta;
            double db=(max-b)/delta;
            if(r==max)
                dst[i].c1=(g==min?5.+db:1.-dg);
            else if(g==max)
                dst[i].c1=(b==min?1.+dr:3.-db);
            else if(r==min)
                dst[i].c1=(r==min?3.+dg:5.-dr);

            dst[i].c1*=(dst[i].c1<6.0?60.0:0.0);
           
        }
    
    }
}
void HSV2RGB(HSV *src ,RGB *dst,int width,int height){
    int *temp_int=(int *)malloc(sizeof(int)*width*height);
    double *temp_double=(double *)malloc(sizeof(double)*width*height);
    Split(src, temp_double, NULL, NULL, width, height);
    matrixDBL2INT(temp_double, temp_int, width, height);
    double h,s,v;
    double r,g,b;
    int h_int;
    for(int i=0;i<width*height;i++){
        h=src[i].c1;
        s=src[i].c2;
        v=src[i].c3;
        h_int=temp_int[i];
        int Hi=((int)h_int/60)%6;
        double f=(double)(h/60.-Hi);
        double p=v*(1.-s);
        double q=v*(1.-f*s);
        double t=v*(1.-(1.-f)*s);
        switch (Hi) {
            case 0:r=v;g=t;b=p;break;
            case 1:r=q;g=v;b=p;break;
            case 2:r=p;g=v;b=t;break;
            case 3:r=p;g=q;b=v;break;
            case 4:r=t;g=p;b=v;break;
            case 5:r=v;g=p;b=q;break;
            default:
                break;
        }
        dst[i].c1=r*(GRAY_LEVEL-1);
        dst[i].c2=g*(GRAY_LEVEL-1);
        dst[i].c3=b*(GRAY_LEVEL-1);
        
    }
    free(temp_int);
    free(temp_double);
}

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/
void Complementary_Color(C3 *src,C3 *dst,int width,int height,int color_space_type){
    switch (color_space_type) {
        case COLOR_SPACE_RGB:
            RGB2CMY(src, dst, width, height);
            break;
        case COLOR_SPACE_CMY:
            CMY2RGB(src, dst, width, height);
            break;
        case COLOR_SPACE_HSI:{
            for(int i=0;i<width*height;i++){
                double h=src[i].c1;
                if(h>=M_PI)
                    dst[i].c1=h-M_PI;
                else
                    dst[i].c1=M_PI+h;
                dst[i].c2=src[i].c2;
                dst[i].c3=255.-src[i].c3;
            }
            break;
        }
        case COLOR_SPACE_HSV:{
            for(int i=0;i<width*height;i++){
                double h=src[i].c1;
                if(h>=180.0)
                    dst[i].c1=h-180.0;
                else
                    dst[i].c1=180.0+h;
                dst[i].c2=src[i].c2;
                dst[i].c3=1.0-src[i].c3;
            }
            break;
        }
        default:
            break;
    }
}