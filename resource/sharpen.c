//
//  Sharpen
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/1/30.
//  Copyright (c) 2014年 谭升. All rights reserved.
//

#include "sharpen.h"



void LaplaceSharpen(double *src,double *dst,int width,int height,int mask_type,double c){
    
    
    Laplace(src,dst,width,height,mask_type);
    matrixMultreal(dst,dst,c,width,height);

    switch(mask_type){
        case SHARPEN_LAP_0:
        case SHARPEN_LAP_1:
            matrixSub(src,dst,dst,width,height);
            break;
        case SHARPEN_LAP_2:
        case SHARPEN_LAP_3:
            matrixAdd(src,dst,dst,width,height);
            break;
        default:
            printf("wrong mask type\n");
            matrixCopy(src, dst, width, height);
            break;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


void UnsharpMasking(double *src,double *dst,int width,int height,int smooth_type,int smooth_mask_width,int smooth_mask_height,double gaussian_deta,double k){
    switch (smooth_type) {
        case SMOOTH_GAUSSIAN:
            GaussianFilter(src, dst,width,height, smooth_mask_width, smooth_mask_height,gaussian_deta);
            break;
        case SMOOTH_MEAN:
            MeanFilter(src, dst,width,height, smooth_mask_width, smooth_mask_height);
            break;
        default:
            break;
    }
    matrixSub(src, dst, dst, width, height);
    matrixMultreal(dst, dst, k, width, height);
    matrixAdd(src, dst, dst, width, height);


}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/*
void Sobel(double *src,double *dst,int width,int height){
    double SobelMask_y[9]={-1,-2,-1,0,0,0,1,2,1};
    double SobelMask_x[9]={-1,0,1,-2,0,2,-1,0,1};
    double *dst_x=(double *)malloc(sizeof(double)*width*height);
    double *dst_y=(double *)malloc(sizeof(double)*width*height);
    //RealRelevant
    RealConvolution(src, dst_x, SobelMask_x, width, height, SOBEL_MASK_SIZE,SOBEL_MASK_SIZE);
    RealConvolution(src, dst_y, SobelMask_y, width, height, SOBEL_MASK_SIZE,SOBEL_MASK_SIZE);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            dst[j*width+i]=abs(dst_x[j*width+i])+abs(dst_y[j*width+i]);
        
        }
    free(dst_x);
    free(dst_y);
    
}

void SobelSharpen(double *src,double *dst,int width,int height,double c){
    Sobel(src,dst,width,height);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            dst[j*width+i]=src[j*width+i]+c*dst[j*width+i];
        }


}*/
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////



void SobelSharpen(double *src,double *dst,int width,int height,double c){
    Sobel(src,dst,width,height);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            dst[j*width+i]=src[j*width+i]+c*dst[j*width+i];
        }
    
    
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////



void RobertSharpen(double *src,double *dst,int width,int height,double c){
    Robert(src,dst,width,height);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            dst[j*width+i]=src[j*width+i]+c*dst[j*width+i];
        }
    
    
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////