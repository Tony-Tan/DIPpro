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
#define ANGLEHISTSIZE 36
#define ANGLEZOOM (ANGLEHISTSIZE/360.0)

#include <cv.h>
#include <highgui.h>
SIFT_Feature * createNewSIFTnode(){
    SIFT_Feature* newNode=(SIFT_Feature*)malloc(sizeof(SIFT_Feature));
    return newNode;
}
void addSIFTnode(SIFT_Feature ** head,SIFT_Feature * newnode){
    SIFT_Feature * temp=*head;
    if(temp==NULL)
        *head=newnode;
    else {
        while (temp->next!=NULL) {
            temp=temp->next;
        }
        temp->next=newnode;
    }
}
void ReleaseSIFTlist(SIFT_Feature * head){
    SIFT_Feature *temp=head;
    SIFT_Feature *temp_next;
    while(temp!=NULL){
        temp_next=temp->next;
        free(temp);
        temp=temp_next;
    }

}
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
/****************************************************************/
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
/****************************************************************/
//尺度空间，返回一个指针数组，每个指针指向一层
void ScaleSpace(double *src,double * dst[],double *delta_arry,int width,int height,double delta_max,int k){
    double delta=pow(delta_max, 1./(double)k);
    double delta_thistime=delta;
    int gaussian_size=0;
    for(int l=0;l<k;l++){
        delta_arry[l]=delta_thistime;
        dst[l]=(double *)malloc(sizeof(double)*width*height);
        gaussian_size=getGaussianSize(delta_thistime);
        GaussianFilter(src, dst[l], width, height, gaussian_size, gaussian_size, delta_thistime);
        delta_thistime*=delta;
        
        
        
        
    }

}
/**************************************************************/
//k是原始空间的层数，DOG为k-1层
void DOG_Scale(double *src[],double * dst[],int width,int height,int scale_level){
    for(int i=0;i<scale_level-1;i++){
        dst[i]=(double *)malloc(sizeof(double)*width*height);
        matrixSub(src[i+1], src[i], dst[i], width, height);
        //for(int m=0;m<height;m++){
        //    for(int n=0;n<width;n++){
        //        printf("%g ",dst[i][m*width+n]);
        //    }
        //    printf("\n");
        //}
        
        
    }
}
/**************************************************************/
void ReleaseMatArr(double * dst[],int k){
    for(int i=0;i<k;i++)
        free(dst[i]);
}
/**************************************************************/
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
    double dxd=src[23]-src[21]-src[5]+src[3];
    double dyd=src[25]-src[19]-src[7]+src[1];
    double adj_src[9];
    double src_1[9];
    if((dxx*dyy-dxy*dxy)!=0&&(dxx+dyy)*(dxx+dyy)/(dxx*dyy-dxy*dxy)<HESSIAN_THRESHOLD){
        double matcoe=(dxx*dyy*ddd+dxy*dyd*dxd*2)-(dxd*dyy*dxd+dxy*dxy*ddd+dyd*dyd*dxx);
        matcoe=matcoe>0?matcoe:-matcoe;
        
        adj_src[0]=dyy*ddd-dyd*dyd;
        
        adj_src[1]=-dxy*ddd+dxd*dyd;
        
        adj_src[2]=dxy*dyd-dyy*dxd;
        
        adj_src[3]=adj_src[1];
        
        adj_src[4]=dxx*ddd-dxd*dxd;
        
        adj_src[5]=-dxx*dyd+dxy*dxd;
        
        adj_src[6]=adj_src[2];
        
        adj_src[7]=adj_src[5];
        
        adj_src[8]=dxx*dyy-dxy*dxy;
        
        matrixMultreal(adj_src,src_1 , 1./matcoe, 3, 3);
        

        dst[0]=-(src_1[0]*dx+src_1[1]*dy+src_1[2]*dd);
        dst[1]=-(src_1[3]*dx+src_1[4]*dy+src_1[5]*dd);
        dst[2]=-(src_1[6]*dx+src_1[7]*dy+src_1[8]*dd);
        if( dst[0]>0.5 ||dst[1]>0.5||
            dst[0]<-0.5||dst[1]<-0.5)
            return 0;
        else{
            *extremum=src[13]+0.5*(dx*dst[0]+dy*dst[1]+dd*dst[2]);
            if(fabs(*extremum)<7.65){
                *extremum=0.0;
                return 0;
            }else
                return 1;
        }
    
    }
    return 0;
}
/************************************************************/
int findOrientation(double *src,int width,int height,Position_DBL *position,double delta,double * orientation){
    int orientation_count=0;
    int position_x_int=(int)(position->x);
    int position_y_int=(int)(position->y);
    int w_width=(int)(delta*16.0);
    w_width+=isEVEN(w_width)?0:1;
    int w_height=w_width;
    int g_size=(int)(6.0*delta);
    g_size+=isEVEN(g_size)?0:1;

    
    if(position_x_int+w_width/2>width||position_x_int-w_width/2<0 ||
       position_y_int+w_height/2>width||position_y_int-w_height/2<0)
        return 0;
    

    double* temp_src=(double *)malloc(sizeof(double)*w_width*w_height);
    double* temp_scale=(double *)malloc(sizeof(double)*w_width*w_height);
    double* temp_angle=(double *)malloc(sizeof(double)*w_width*w_height);
    double* temp_range=(double *)malloc(sizeof(double)*w_width*w_height);
    double* gaussian_kernel=(double *)malloc(sizeof(double)*w_width*w_height);
    Position p;
    p.x=position_x_int-(int)(w_width/2);
    p.y=position_y_int-(int)(w_height/2);
    
    matrixCopyLocal(src, temp_src, width, height, w_width, w_height, &p);
    GaussianFilter(temp_src, temp_scale, w_width, w_height, g_size, g_size, delta);
    matrixOrdinaryDiff(temp_scale, temp_range, temp_angle, w_width, w_height);
    GaussianMask(gaussian_kernel, w_width, w_height, delta*1.5);
    
    matrixMul_matrix(temp_range, gaussian_kernel, temp_range, w_width, w_height);
   
    double angle_hist[ANGLEHISTSIZE];
    for(int i=0;i<ANGLEHISTSIZE;i++){
        angle_hist[i]=0.0;
    }
    for(int j=0;j<w_height;j++){
        for(int i=0;i<w_width;i++){
            double distanc=Distance(i, j, w_width/2-0.5, w_height/2-0.5);
            if(distanc<4.5*delta){
                int angle_value=(int)temp_angle[j*w_width+i];
                int num=angle_value*ANGLEZOOM-1;
                angle_hist[num]+=temp_range[j*w_width+i];
                
            }
        }
    
    }
  

    double angle=0.0;
    double angle2=0.0;
    double hist_max=-1.0;
    double hist_max2=-1.0;
    for(int i=0;i<ANGLEHISTSIZE;i++){
        if(angle_hist[i]>=hist_max){
            angle=i;
            hist_max=angle_hist[i];
        
        }else if(angle_hist[i]>=hist_max2){
            angle2=i;
            hist_max2=angle_hist[i];
            
        }
    }
    if(hist_max2/hist_max>0.8){
        orientation_count=2;
        
        orientation[0]=angle;
        orientation[1]=angle2;
    }else{
        orientation_count=1;
        orientation[0]=angle;
    }
    
    free(temp_angle);
    free(temp_range);
    free(temp_scale);
    free(temp_src);
    free(gaussian_kernel);
    
    
    return orientation_count;
}
/***************************************************************/
int  getDescriptor(double *src,int *descriptor,int width,int height,Position_DBL position,double delta,double orientation){
    if(position.x+8>width||position.y+8>height||
       position.x<8      ||position.y<8)
        return 0;
    
    
    int g_size=(int)(delta*6.0);
    g_size+=isEVEN(g_size)?0:1;
    int w_width=g_size+16;
    int w_height=w_width;
    

    
    
    double* temp_src=(double *)malloc(sizeof(double)*w_width*w_height);
    double* temp_scale=(double *)malloc(sizeof(double)*w_width*w_height);
    double* temp_angle=(double *)malloc(sizeof(double)*w_width*w_height);
    double* temp_range=(double *)malloc(sizeof(double)*w_width*w_height);
    
    
    for(int i=0;i<128;i++)
        descriptor[i]=0.0;
    
    Position lefttop;
    lefttop.x=(int)position.x-w_width/2-1;
    lefttop.y=(int)position.y-w_height/2-1;
    
    
    
    matrixCopyLocal(src, temp_src, width, height, w_width, w_height, &lefttop);
    Position_DBL  rotation_center;
    rotation_center.x=w_width/2-0.5;
    rotation_center.y=w_height/2-0.5;
    matrixRotation(temp_src, temp_src, w_width, w_height, w_width, w_height, orientation, &rotation_center);
    GaussianFilter(temp_src, temp_scale, w_width, w_height, g_size, g_size, delta);
    matrixOrdinaryDiff(temp_scale, temp_range, temp_angle, w_width,w_height);
    int up=w_height/2+9;
    int down=w_height/2-7;
    for(int j=down;j<up;j++){
        for(int i=down;i<up;i++){
            double temp_orientation=temp_angle[j*w_width+i];
            descriptor[((int)((j-down)/4)*4+(int)((i-down)/4))*8+(int)(temp_orientation/45.0)]+=(int)(temp_range[j*w_width+i]*10.0);
        }
    
    }
    
    free(temp_angle);
    free(temp_range);
    free(temp_scale);
    free(temp_src);
    
    return 1;
}

/**************************************************************/
void findCandideat(double *DoG[],double *delta_arry,double *src,SIFT_Feature ** dst,int width,int height,int DoG_level,double sizeRatio){
    double orientation[2];
    int isMoreorLess=0;
    int timetoBreak=0;
    double extremum=0.0;
    double theta_position[3];
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
                                else
                                    timetoBreak=1;
                            }
                        }
                if(!timetoBreak&&isMoreorLess){
                    for(int d=-1;d<2;d++){
                        for(int m=-1;m<2;m++){
                            for(int n=-1;n<2;n++){
                                N_temp[(d+1)*9+(m+1)*3+n+1]=(DoG[l+d])[(j+m)*width+i+n];
                               
                            }
                        }
                    }
                    if(Accurate_Position(N_temp, theta_position,&extremum)){
                        
                        Position_DBL p_d;
                        p_d.x=i+theta_position[0];
                        p_d.y=j+theta_position[1];
                        
                        int o_num=findOrientation(src, width, height, &p_d, delta_arry[l]+theta_position[2],orientation );
                        
                        while(o_num--){
                            SIFT_Feature *new=createNewSIFTnode();
                            new->x=p_d.x/sizeRatio;
                            new->y=p_d.y/sizeRatio;
                            new->next=NULL;
                            new->orientation=orientation[o_num]/ANGLEZOOM;
                            new->scale=delta_arry[l]+theta_position[2];
                            getDescriptor(src,new->des_vector, width, height, p_d,new->scale,new->orientation);
                            addSIFTnode(dst,new);
                        }
                        
                    }
                }
            }
        }
    }
}
/**************************************************************/
void SIFT(double *src,SIFT_Feature **dst,int width,int height,int scale_k,int octave){
    if((*dst)!=NULL){
        printf("sift dst must be NULL!\n");
        exit(0);
    }
    double sizeRatio=2.0;
    double delta=1.6;
    double *src_temp;
    int realtime_width=(int)(((double)width)*sizeRatio);
    int realtime_height=(int)(((double)height)*sizeRatio);
    src_temp=(double *)malloc(sizeof(double)*realtime_width*realtime_height);
    Zero(src_temp, realtime_width,realtime_height);
    Resize(src, width, height, src_temp, realtime_width, realtime_height);
    for(int i=0;i<octave;i++){
        
        double *delta_arry=(double *)malloc(sizeof(double)*scale_k);
        double** scale=malloc(sizeof(double*)*scale_k);
        double** dog=malloc(sizeof(double*)*(scale_k-1));
        ScaleSpace(src_temp, scale,delta_arry,realtime_width, realtime_height,delta*(1<<i), scale_k);
        DOG_Scale(scale, dog, realtime_width,realtime_height, scale_k);
        findCandideat(dog,delta_arry,src,dst , realtime_width, realtime_height, scale_k-1,sizeRatio);
        
        free(delta_arry);
        free(src_temp);
        sizeRatio/=2.0;
        int temp_width=realtime_width;
        int temp_height=realtime_height;
        realtime_width=(int)(((double)width)*sizeRatio);
        realtime_height=(int)(((double)height)*sizeRatio);
        src_temp=(double *)malloc(sizeof(double)*realtime_width*realtime_height);
        Resize(scale[scale_k-2], temp_width, temp_height, src_temp, realtime_width, realtime_height);
        ReleaseMatArr(scale, scale_k);
        ReleaseMatArr(dog, scale_k-1);
    }
}