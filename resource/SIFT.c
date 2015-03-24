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
//  SIFT
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/03/23.
//  Copyright (c) 2015年 谭升. All rights reserved.
//

#include "SIFT.h"
//sift中降采样，最邻近取一半
double * SIFT_Resize(double *src,int width,int height){
    int d_width=width/2;
    int d_height=height/2;
    double * dst=(double *)malloc(sizeof(double)*d_width*d_height);
    Zero(dst,d_width, d_height);
    for(int j=0;j<d_height;j++){
        for(int i=0;i<d_width;i++){
            dst[j*d_width+i]=src[(j*width+i)*2];
        }
    }
    return dst;

}
int getGaussianSize(double delta){
    int size=0;
    if(delta<0.17)
        return 0;
    else{
        size=(int)(delta*6.0);
        size+=isEVEN(size)?1:2;
    }
    return size;

}
//尺度空间，返回一个指针数组，每个指针指向一层
double ScaleSpace(double *src,double * dst[],int width,int height,double delta_max,int k){
    double delta=pow(delta_max, 1./(double)k);
    double delta_thistime=delta;
    int gaussian_size=0;
    for(int l=0;l<k;l++){
        dst[l]=(double *)malloc(sizeof(double)*width*height);
        gaussian_size=getGaussianSize(delta_thistime);
        GaussianFilter(src, dst[l], width, height, gaussian_size, gaussian_size, delta_thistime);
        delta_thistime*=delta;
    }

    return delta;
}
//k是原始空间的层数，DOG为k-1层
void DOG_Scale(double *src[],double * dst[],int width,int height,int scale_level){
    for(int i=0;i<scale_level-1;i++){
        dst[i]=(double *)malloc(sizeof(double)*width*height);
        matrixSub(src[i+1], src[i], dst[i], width, height);
    }
}

void ReleaseMatArr(double * dst[],int k){
    for(int i=0;i<k;i++)
        free(dst[i]);
}
/////
///3x3x3的立方体做泰勒展开找到亚像素级的极值点，并筛选不稳定的特征点
#define HESSIAN_THRESHOLD 12.1  //(lamda+1)^2/lamda  lamda=10.0
int Accurate_Position(double *src,double *dst,double * extremum){
    double dx=src[14]-src[12];
    double dy=src[16]-src[10];
    double dd=src[22]-src[4];
    
    double dxx=src[14]+src[12]-2*src[13];
    double dyy=src[10]+src[16]-2*src[13];
    double ddd=src[4]+src[22]-2*src[13];
    double dxy=src[17]-src[15]-src[11]+src[9];
    double dxd=src[23]-src[21]-src[6]+src[4];
    double dyd=src[25]-src[19]-src[7]+src[1];
    double adj_src[9];
    double src_1[9];
    if((dxx*dyy-dxy*dxy)!=0&&(dxx+dyy)*(dxx+dyy)/(dxx*dyy-dxy*dxy)<HESSIAN_THRESHOLD){
        double matcoe=(dxx*dyy*ddd+dxy*dyd*dxd*2)-(dxd*dyy*dxd+dxy*dxy*ddd+dyd*dyd*dxx);
        matcoe=matcoe>0?matcoe:-matcoe;
        
        adj_src[0]=dyy*ddd-dyd*dyd;
        adj_src[1]=-dxy*ddd+dxd*dyd;
        adj_src[2]=dxy*dyd-dyy*dxd;
        adj_src[3]=-dxy*ddd+dyd*dxd;
        adj_src[4]=dxx*ddd-dxd*dxd;
        adj_src[5]=-dxx*dyd+dxy*dxd;
        adj_src[6]=dxy*dyd-dyy*dxd;
        adj_src[7]=-dxx*dyd+dxy*dxd;
        adj_src[8]=dxx*dyy-dxy*dxy;
        matrixMultreal(adj_src,src_1 , 1./matcoe, 3, 3);
        

        dst[0]=src_1[0]*dx+src_1[1]*dy+src_1[2]*dd;
        dst[1]=src_1[3]*dx+src_1[4]*dy+src_1[5]*dd;
        dst[2]=src_1[6]*dx+src_1[7]*dy+src_1[8]*dd;
        if( dst[0]>0.5 ||dst[1]>0.5 ||dst[2]>0.5 ||
            dst[0]<-0.5||dst[1]<-0.5||dst[2]<-0.5)
            return 0;
        else{
            *extremum=src[13]+dx*dst[0]+dy*dst[1]+dd*dst[2];
            if(fabs(*extremum)<7.65)
                return 0;
            else
                return 1;
        }
    
    }
    return 0;
}
/***********************************************************************************************************/
void findCandideat(double *DoG[],double *test,int width,int height,int DoG_level){
    int isMoreorLess=0;
    int timetoBreak=0;
    int num=0;
    double extremum=0.0;
    double delta_position[3];
    double N_temp[27];//邻域3x3x3区域
    for(int l=1;l<DoG_level-1;l++){
        for(int j=1;j<height-1;j++){
            for(int i=1;i<width-1;i++){
                isMoreorLess=0;
                timetoBreak=0;
                double value=(DoG[l])[j*width+i];
                
                
                for(int d=-1;d<2&&!timetoBreak;d++)
                    for(int m=-1;m<2&&!timetoBreak;m++)
                        for(int n=-1;n<2&&!timetoBreak;n++){
                            if(!(d==0&&m==0&&n==0)){
                                if(value>(DoG[l+d])[(j+m)*width+i+n]){
                                    if(isMoreorLess==0)
                                        isMoreorLess=1;
                                    else if(isMoreorLess==-1)
                                        timetoBreak=1;
                                }else if(value<(DoG[l+d])[(j+m)*width+i+n]){
                                    if(isMoreorLess==0)
                                        isMoreorLess=-1;
                                    else if(isMoreorLess==1)
                                        timetoBreak=1;
                                }
                            }
                        }
                if(!timetoBreak){
                    for(int d=-1;d<2;d++)
                        for(int m=-1;m<2;m++)
                            for(int n=-1;n<2;n++){
                                N_temp[(d+1)*9+(m+1)*3+n+1]=(DoG[l+d])[(j+m)*width+i+n];
                            }
                    if(Accurate_Position(N_temp, delta_position,&extremum)){
                        printf("isMore:%5d-------l:%10g  [%g,%g]  value:%10g\n",isMoreorLess,(l+delta_position[2]),j+delta_position[1],i+delta_position[0],extremum);
                        test[j*width+i]=255.0;
                        num++;
                    }
                
                }
                
            }
        }
    }
    printf("with acc total num :%d\n",num);
    
}
void SIFT(double *src,double *dst,int width,int height,int scale_k,int octave){
    double** scale=malloc(sizeof(double*)*scale_k);
    double** dog=malloc(sizeof(double*)*(scale_k-1));
    ScaleSpace(src, scale, width, height,1.7, scale_k);
    DOG_Scale(scale, dog, width,height, scale_k);
    
    findCandideat(dog,dst, width, height, scale_k-1);
    
    ReleaseMatArr(scale, scale_k);
    ReleaseMatArr(dog, scale_k-1);

}