//
//  convolution
//  卷积相关
//  Created by 谭升 on 14/12/23.
//  tony.sheng.tan@gmail.com
//  Copyright (c) 2014年 谭升. All rights reserved.
//

#include "convolution.h"
/*
 实数矩阵的180度旋转
 */
void RotateRealMatrix(double *matrix,int width,int height){
    double *temp=(double*)malloc(sizeof(double)*width*height);
    if(temp==NULL){
        printf("malloc wrong!\n");
        exit(0);
    }
    for(int i=0;i<width*height;i++){
        temp[width*height-1-i]=matrix[i];
    }
    for(int i=0;i<width*height;i++)
        matrix[i]=temp[i];
    free(temp);

}
/*
 复数矩阵的180度旋转
 */
void RotateComplexMatrix(Complex *matrix,int width,int height){
    Complex *temp=(Complex*)malloc(sizeof(Complex)*width*height);
    if(temp==NULL){
        printf("malloc wrong!\n");
        exit(0);
    }
    for(int i=0;i<width*height;i++){
        Copy_Complex(&matrix[i],&temp[width*height-1-i]);
    }
    for(int i=0;i<width*height;i++)
        Copy_Complex(&temp[i],&matrix[i]);
    free(temp);
}

/*
 0 0 0 0
 0 0 0 0
 0 0 1 0
 0 0 0 0
 偶数中心设置为(width/2,height/2)
 输入输出矩阵大小相同
 */
void RealRelevant(double *src,double *dst,double *mask,
                  int width,int height,int m_width,int m_height){
    double *temp=(double *)malloc(sizeof(double)*width*height);
    if(temp==NULL){
        printf("realrelecant:malloc wrong\n");
        exit(0);
    }
    int mask_center_x=m_width/2;
    int mask_center_y=m_height/2;
    
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            double value=0.0;
            for(int n=0;n<m_height;n++)
                for(int m=0;m<m_width;m++){
                    if((i+m-mask_center_x)<width&&(j+n-mask_center_y)<height&&
                       (i+m-mask_center_x)>=0&&(j+n-mask_center_y)>=0){
                        value+=src[(j+n-mask_center_y)*width+(i+m-mask_center_x)]*mask[n*m_width+m];
                    }
                }
            temp[j*width+i]=value;
        }
    for(int i=0;i<width*height;i++)
        dst[i]=temp[i];
    free(temp);
}



///////////////////////////////////////////////////////////////////////////////////////////////
void ComplexRelevant(Complex* src,Complex *dst,Complex *mask,
                     int width,int height,int m_width,int m_height){
    Complex *temp=(Complex *)malloc(sizeof(Complex)*width*height);
    if(temp==NULL){
        printf("Complexrelecant:malloc wrong\n");
        exit(0);
    }
    int mask_center_x=m_width/2;
    int mask_center_y=m_height/2;
    Complex value;//用于存储乘法结果
    Complex value2;//用于存储加法结果
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++){
            value2.imagin=0.0;
            value2.real=0.0;
            for(int m=0;m<m_width;m++)
                for(int n=0;n<m_height;n++){
                    if((i+m-mask_center_x)<width&&(j+n-mask_center_y)<height&&
                       (i+m-mask_center_x)>=0&&(j+n-mask_center_y)>=0){
                        Multy_Complex(&src[(i+(m-mask_center_x))*width+j+(n-mask_center_y)],&mask[m*m_width+n],&value);
                        Add_Complex(&value, &value2, &value2);
                    }
                }
            Copy_Complex(&value2, &temp[i*width+j]);
        }
    for(int i=0;i<width*height;i++)
        Copy_Complex(&temp[i],&dst[i]);
    free(temp);

}
void RealConvolution(double *src,double *dst,double *mask,
                     int width,int height,int m_width,int m_height){
    RotateRealMatrix(mask,m_width,m_height);
    RealRelevant(src, dst, mask, width, height, m_width, m_height);


}
void ComplexCovolution(Complex* src,Complex *dst,Complex *mask,
                       int width,int height,int m_width,int m_height){
    RotateComplexMatrix(mask,m_width,m_height);
    ComplexRelevant(src, dst, mask, width, height, m_width,m_height);


}