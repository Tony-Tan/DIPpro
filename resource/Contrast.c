//
//  图像增强
//  Contrast
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 14/11/25.
//  Copyright (c) 2014年 谭升. All rights reserved.
//
#include "Contrast.h"




void ContrastStretch(double *src,double *dst,int width,int height,int method,double p0,double p1,int p2,int p3){
    /////////////////////////////////////////生成映射表////////////////////////////////////////////
    unsigned char ContrastTable[MAX_PVALUE];//映射表
    if(method==CONTRASTFUNC0){//图像翻转
        for(int i=0;i<MAX_PVALUE;i++)
            ContrastTable[i]=MAX_PVALUE-1-i;
        
    }
    else if(method==CONTRASTFUNC1){//分段拉伸
        for(int i=0;i<MAX_PVALUE;i++)
            ContrastTable[i]=i<=p0?i*p1/p0                 :
            i<=p2?(i-p0)*(p3-p1)/(p2-p0)+p1:
            (i-p2)*(MAX_PVALUE-1-p3)/(MAX_PVALUE-1-p2)+p3;
        
    }
    else if(method==CONTRASTFUNC2){//对数
        for(int i=0;i<MAX_PVALUE;i++)
            ContrastTable[i]=46*log((double)(1+i));//46*log(256)近似于256
        
    }else if(method==CONTRASTFUNC3){//反对数
        for(int i=0;i<MAX_PVALUE;i++)
            ContrastTable[(int)(46*log((double)(1+i)))]=i;
        for(int i=0;i<MAX_PVALUE;i++)
            if(ContrastTable[i]==0)
                ContrastTable[i]=ContrastTable[i-1];
        
    }else if(method==CONTRASTFUNC4){//N次方
        double coef=255/pow(255.,(double) p0);//coef为系数，即255要映射到255
        for(int i=0;i<MAX_PVALUE;i++)
            ContrastTable[i]=coef*pow((double)i,(double)p0);
        
    }else if(method==CONTRASTFUNC5){//N次根
        double coef=255/pow(255.,(double) p0);//coef为系数，即255要映射到255
        for(int i=0;i<MAX_PVALUE;i++)
            ContrastTable[(int)(coef*pow((double)i,(double)p0))]=i;
        for(int i=0;i<MAX_PVALUE;i++)
            if(ContrastTable[i]==0)
                ContrastTable[i]=ContrastTable[i-1];
        
    }else if(method==CONTRASTGAMA){//gama
        double gama=p0;
        double coef=255/pow(255.,gama);//coef为系数，即255的gama次幂要映射到255
        coef=(p1<=coef&&p1>0.0)?p1:coef;
        for(int i=0;i<MAX_PVALUE;i++)
            ContrastTable[i]=coef*pow((double)i,gama);
        
    }
    
    ///////////////////////////////重新映射/////////////////////////////////////////////
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++)
            dst[j*width+i]= ContrastTable[(int)src[j*width+i]];
    
}

//===========================================================================





