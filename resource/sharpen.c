//
//  Sharpen
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/1/30.
//  Copyright (c) 2014年 谭升. All rights reserved.
//

#include "sharpen.h"
#define LAPLACE_MASK_SIZE 3
#define SOBEL_MASK_SIZE 3
#define ROBERT_MASK_SIZE 3
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void matrixAdd(double *src1,double *src2,double *dst,int width,int height){
   
    for(int i=0;i<width*height;i++)
        dst[i]=src1[i]+src2[i];
}
void matrixSub(double *src1,double *src2,double *dst,int width,int height){
    
    for(int i=0;i<width*height;i++)
        dst[i]=src1[i]-src2[i];
}
void matrixCopy(double *src,double *dst,int width,int height){
    
    for(int i=0;i<width*height;i++)
        dst[i]=src[i];
}
void matrixMultreal(double *src,double *dst,double k,int width,int height){
    for(int i=0;i<width*height;i++)
        dst[i]=src[i]*k;
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

void Laplace(double *src,double *dst,int width,int height,int mask_type){
    double LaplaceMask0[9]={0,1,0,1,-4,1,0,1,0};
    double LaplaceMask1[9]={1,1,1,1,-8,1,1,1,1};
    double LaplaceMask2[9]={0,-1,0,-1,4,-1,0,-1,0};
    double LaplaceMask3[9]={-1,-1,-1,-1,8,-1,-1,-1,-1};
    switch(mask_type){
        case SHARPEN_LAP_0:
            RealRelevant(src, dst, LaplaceMask0, width, height, LAPLACE_MASK_SIZE,LAPLACE_MASK_SIZE);
            break;
        case SHARPEN_LAP_1:
            RealRelevant(src, dst, LaplaceMask1, width, height, LAPLACE_MASK_SIZE,LAPLACE_MASK_SIZE);
            break;
        case SHARPEN_LAP_2:
            RealRelevant(src, dst, LaplaceMask2, width, height, LAPLACE_MASK_SIZE,LAPLACE_MASK_SIZE);
            break;
        case SHARPEN_LAP_3:
            RealRelevant(src, dst, LaplaceMask3, width, height, LAPLACE_MASK_SIZE,LAPLACE_MASK_SIZE);
            break;
        default:
            printf("wrong mask type\n");
            matrixCopy(src, dst, width, height);
            break;
    }



}
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

void Sobel(double *src,double *dst,int width,int height){
    double SobelMask_x[9]={-1,-2,-1,0,0,0,1,2,1};
    double SobelMask_y[9]={-1,0,1,-2,0,2,-1,0,1};
    double *dst_x=(double *)malloc(sizeof(double)*width*height);
    double *dst_y=(double *)malloc(sizeof(double)*width*height);
    RealRelevant(src, dst_x, SobelMask_x, width, height, SOBEL_MASK_SIZE,SOBEL_MASK_SIZE);
    RealRelevant(src, dst_y, SobelMask_y, width, height, SOBEL_MASK_SIZE,SOBEL_MASK_SIZE);
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


}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/*
void Sobel(double *src,double *dst,int width,int height){
    //double SobelMask_x[3]={-1,-2,-1,0,0,0,1,2,1};
    
    double SobelMask1[3]={1,2,1};
    double SobelMask2[3]={-1,0,1};
    double *dst_x=(double *)malloc(sizeof(double)*width*height);
    double *dst_y=(double *)malloc(sizeof(double)*width*height);
    //RealRelevant(src, dst_x, SobelMask_x, width, height, SOBEL_MASK_SIZE,SOBEL_MASK_SIZE);
    //RealRelevant(src, dst_y, SobelMask_y, width, height, SOBEL_MASK_SIZE,SOBEL_MASK_SIZE);
    RealConvolution(src, dst_x, SobelMask1, width, height, 1, 3);
    RealConvolution(dst_x, dst_x, SobelMask2, width, height, 3, 1);
    
    RealConvolution(src, dst_y, SobelMask2, width, height, 1, 3);
    RealConvolution(dst_y, dst_y, SobelMask1, width, height, 3, 1);
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

void Robert(double *src,double *dst,int width,int height){
    double RobertMask_x[9]={0,0,0,0,-1,0,0,0,1};
    double RobertMask_y[9]={0,0,0,0,0,-1,0,1,0};
    double *dst_x=(double *)malloc(sizeof(double)*width*height);
    double *dst_y=(double *)malloc(sizeof(double)*width*height);
    RealConvolution(src, dst_x, RobertMask_x, width, height, ROBERT_MASK_SIZE,ROBERT_MASK_SIZE);
    RealConvolution(src, dst_y, RobertMask_y, width, height, ROBERT_MASK_SIZE,ROBERT_MASK_SIZE);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            dst[j*width+i]=abs(dst_x[j*width+i])+abs(dst_y[j*width+i]);
            
        }
    free(dst_x);
    free(dst_y);
    
}

void RobertSharpen(double *src,double *dst,int width,int height,double c){
    Robert(src,dst,width,height);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            dst[j*width+i]=src[j*width+i]+c*dst[j*width+i];
        }
    
    
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////