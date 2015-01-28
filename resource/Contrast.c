//
//  图像增强
//  Contrast
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 14/11/25.
//  Copyright (c) 2014年 谭升. All rights reserved.
//
#include "Contrast.h"
static char GrayMappingTable[256];

unsigned char ContrastTable[MAX_PVALUE];//映射表

void ContrastStretch(IplImage *src,IplImage *dst,int method,double p0,double p1,int p2,int p3){
    /////////////////////////////////////////生成映射表////////////////////////////////////////////
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
    //for(int i=0;i<256;i++)
    //       printf("%d->%d\n",i,ContrastTable[i]);
    for(int i=0;i<src->width;i++)
        for(int j=0;j<src->height;j++)
            SETPIX(dst,i,j,ContrastTable[GETPIX(src,i,j)]);
    
}

//===========================================================================




void InitMappingTable(void * arry,int size,int Data_type){
    if(Data_type==TABLE_INT)
        for(int i=0;i<size;i++)
            ((int*)arry)[i]=0;
    else if(Data_type==TABLE_CHAR)
        for(int i=0;i<size;i++)
            ((char*)arry)[i]=0;
    
    
}
/*
 直方图均衡
 */
void HistogramEqualization(IplImage *src,IplImage *dst){
    int GrayValue[256];
    InitMappingTable(GrayMappingTable,256,TABLE_CHAR);
    InitMappingTable(GrayValue,256,TABLE_INT);
    int min=256,max=0;
    for(int i=0;i<src->width;i++)
        for(int j=0;j<src->height;j++){
            int tempv=GETPIX(src, j, i);
            GrayValue[tempv]++;
            max=tempv>=max?tempv:max;
            min=tempv<min?tempv:min;
            
        }
    
    for(int i=1;i<=max;i++)
        GrayValue[i]+=GrayValue[i-1];
    int Size=src->width*src->height;
    for(int i=min;i<=max;i++){
        //GrayMappingTable[i]=(unsigned char)255.*(GrayValue[i]-GrayValue[min])/(Size-GrayValue[min]);
        if(Size-GrayValue[min]!=0)
           GrayMappingTable[i]=(unsigned char)255.*(GrayValue[i]-GrayValue[min])/(Size-GrayValue[min]);
        //printf("%d->%d\n",i,(unsigned char)GrayMappingTable[i]);
        
    }
    
    for(int i=0;i<src->width;i++)
        for(int j=0;j<src->height;j++)
            SETPIX(dst,j,i,GrayMappingTable[GETPIX(src, j ,i)]);
    
}
