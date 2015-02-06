//
//  Segment
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/2/6.
//  Copyright (c) 2014年 谭升. All rights reserved.
//
#include "Segment.h"
double findMatrixMax(double *src,int width,int height){
    double max=-1.0;
    for(int i=0;i<width*height;i++)
        max=src[i]>max?src[i]:max;
    return max;
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

double Robert(double *src,double *dst,int width,int height){
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
    return findMatrixMax(dst,width,height);
    
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

double Sobel(double *src,double *dst,int width,int height){
    //double SobelMask_x[3]={-1,-2,-1,0,0,0,1,2,1};
    double SobelMask1[3]={3,10,3};
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
    return findMatrixMax(dst,width,height);
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

double Laplace(double *src,double *dst,int width,int height){
    double LaplaceMask[9]={-1,-1,-1,-1,8,-1,-1,-1,-1};
    RealRelevant(src, dst, LaplaceMask, width, height, LAPLACE_MASK_SIZE, LAPLACE_MASK_SIZE);
    return findMatrixMax(dst,width,height);
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
double Prewitt(double *src,double *dst,int width,int height){
    double PrewittMask1[3]={1.0,1.0,1.0};
    double PrewittMask2[3]={-1.0,0.0,1.0};
    double *dst_x=(double *)malloc(sizeof(double)*width*height);
    double *dst_y=(double *)malloc(sizeof(double)*width*height);
    RealRelevant(src, dst_x, PrewittMask1, width, height, 1, 3);
    RealRelevant(dst_x, dst_x, PrewittMask2, width, height, 3, 1);

    RealRelevant(src, dst_y, PrewittMask2, width, height, 1, 3);
    RealRelevant(dst_y, dst_y, PrewittMask1, width, height, 3, 1);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            dst[j*width+i]=abs(dst_x[j*width+i])+abs(dst_y[j*width+i]);
        }
    free(dst_x);
    free(dst_y);
    return findMatrixMax(dst,width,height);

}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
double Kirsch(double *src,double *dst,int width,int height){
    double KirschMask[8][9]={{-3,-3,-3,
                          -3, 0, 5,
                          -3, 5, 5},
                         {-3,-3,-3,
                          -3, 0,-3,
                           5, 5, 5},
                         {-3,-3,-3,
                           5, 0,-3,
                           5, 5,-3},
                         {5,-3,-3,
                          5, 0,-3,
                          5,-3,-3},
                         {5, 5,-3,
                          5, 0,-3,
                          -3,-3,-3},
                         {5, 5, 5,
                          -3, 0,-3,
                          -3,-3,-3},
                         {-3, 5, 5,
                          -3, 0, 5,
                          -3,-3,-3},
                         {-3,-3, 5,
                          -3, 0, 5,
                          -3,-3, 5}};
    double *maxtemp=(double *)malloc(sizeof(double)*width*height);
    double *temp=(double*)malloc(sizeof(double)*width*height);
    Zero(maxtemp, width, height);
    for(int i=0;i<8;i++){
        RealRelevant(src, temp, KirschMask[i], width, height, 3, 3);   
        MaxPix(temp, maxtemp, maxtemp, width, height);
    }
    matrixCopy(maxtemp, dst, width, height);
    return findMatrixMax(dst,width,height);

}

void findCross(double *src,double *dst,int width,int height){
    double *dsttemp=(double *)malloc(sizeof(double)*width*height);
    Zero(dst, width, height);
    double c_value=0.0;
    int flag=1;
    for(int j=1;j<height-1;j++){
        for(int i=1;i<width-1;i++){
            c_value=src[j*width+i];
            flag=1;
            for(int m=-1;m<=1&&flag;m++)
                for(int n=-1;n<=1;n++){
                    if(c_value>0&&src[(j+m)*width+i+n]<0.0){
                        flag=0;
                        dsttemp[j*width+i]=c_value;
                        break;
                    }
                }
        }
    }
    matrixCopy(dsttemp, dst,width, height);


}
double LoG(double *src,double *dst,int width,int height,int m_width,int m_height,double deta){
    double *dsttemp=(double *)malloc(sizeof(double)*width*height);
    GaussianFilter(src,dsttemp,width,height, m_width, m_height, deta);
    Laplace(dsttemp,dsttemp, width, height);
    matrixCopy(dsttemp, dst, width, height);
    findCross(dst, dst, width, height);
    free(dsttemp);
    return findMatrixMax(dst,width,height);
    
}

void Threshold(double *src,double *dst,int width,int height,double threshold,int type){
    if(type==MORETHAN){
        for(int i=0;i<width*height;i++)
            dst[i]=src[i]>threshold?src[i]:0.0;
    }else if(type==LESSTHAN){
        for(int i=0;i<width*height;i++)
            dst[i]=src[i]<threshold?src[i]:0.0;
    }

}
void EdgeDetection(double *src,double *dst,int width,int height,int detector,double threshold,int m_width,int m_height,double deta){
    double maxvalue=0;
    switch (detector) {
        case EDGE_DETECTOR_ROBERT:
        {
            maxvalue=Robert(src, dst, width, height);
            Threshold(dst, dst, width, height, maxvalue*threshold, MORETHAN);
            break;
        }
        case EDGE_DETECTOR_PREWITT:
        {
            maxvalue=Prewitt(src, dst, width, height);
            Threshold(dst, dst, width, height, maxvalue*threshold, MORETHAN);
            break;
        }
        case EDGE_DETECTOR_SOBEL:
        {
            maxvalue=Sobel(src, dst, width, height);
            Threshold(dst, dst, width, height, maxvalue*threshold, MORETHAN);
            break;
        }
        case EDGE_DETECTOR_KIRSCH:
        {
            maxvalue=Kirsch(src, dst, width, height);
            Threshold(dst, dst, width, height, maxvalue*threshold, MORETHAN);
            break;
        }
        case EDGE_DETECTOR_LOG:
        {
            maxvalue=LoG(src, dst, width, height,m_width,m_height,deta);
            Threshold(dst, dst, width, height, maxvalue*threshold, MORETHAN);
            break;
        }
        case EDGE_DETECTOR_LAPLACE:
        {
            maxvalue=Laplace(src, dst, width, height);
            Threshold(dst, dst, width, height, maxvalue*threshold, MORETHAN);
            break;
        }
        default:
            break;
    }



}