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
//  Morphology
//  二值图像形态学处理
//  Created by 谭升 on 14/12/23.
//  tony.sheng.tan@gmail.com
//  Copyright (c) 2014年 谭升. All rights reserved.
//

#include "morphology.h"

//位移操作，将图像整体移动，如果超出边界舍去
//检测图像是否为空


void Translation(double *src,double *dst,int width,int height,MoveDirection *direction){
    double *temp=(double *)malloc(sizeof(double)*width*height);
    Zero(temp, width, height);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            int new_x=i+direction->x;
            int new_y=j+direction->y;
                if(new_y<height &&new_x<width&&new_y>=0&&new_x>=0        )
                    temp[new_y*width+new_x]=src[j*width+i];
        }
    matrixCopy(temp, dst, width, height);
    free(temp);
}
//将小的图像弄到大的黑色图像中间，或者说是给图像加黑色边框
void Zoom(double *src,int s_width,int s_height,double *dst,int d_width,int d_height){
    if(d_width<s_width         ||
       d_height<s_height       ||
       (d_height-s_height)%2==1||
       (d_width-s_width)%2==1){
        if(d_width<s_width )
            printf("Zoom wrong:dst's width too small!\n");
        if(d_height<s_height )
            printf("Zoom wrong:dst's height too small!\n");
        if((d_height-s_height)%2==1||(d_width-s_width)%2==1)
            printf("Zoom wrong:dst-src not a oushu!\n");
        exit(0);
    }
    MoveDirection m;
    m.x=(d_width-s_width)/2;
    m.y=(d_height-s_height)/2;
    Zero(dst, d_width, d_height);
    for(int k=m.y,n=0;n<s_height;k++,n++){
        for(int i=m.x,j=0;j<s_width;i++,j++){
            dst[k*d_width+i]=src[n*s_width+j];
        }
    }

}
//逻辑与操作
void And(double *src0,double *src1,double *dst,int width,int height){
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
            if(src0[j*width+i]==255.0&&
               src1[j*width+i]==255.0)
                dst[j*width+i]=255.0;
            else
                dst[j*width+i]=0.0;
        }
    }
}
//逻辑或操作
void Or(double *src0,double *src1,double *dst,int width,int height){
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
            if(src0[j*width+i]==255.0||
               src1[j*width+i]==255.0)
                dst[j*width+i]=255.0;
            else
                dst[j*width+i]=0.0;
        }
    }
}
//取反
void Not(double *src,double *dst,int width,int height){
    for(int i=0;i<width*height;i++)
        dst[i]=255.0-src[i];
    
}
//将所有元素设为1
void G_One(double *src,int width,int height){
    for(int i=0;i<width*height;i++)
        src[i]=255.0;


}

//膨胀
void Dilate(double *src,int s_width,int s_height,double *dst,int d_width,int d_height,double *se,int se_width,int se_height,Position *center){

    if(center==NULL){
        Position temp;
        temp.x=se_width/2;
        temp.y=se_height/2;
        center=&temp;
    }
    //printf("%d,%d",center->x,center->y);
    MoveDirection m;
    //IplImage *temp=cvCreateImage(cvGetSize(dst), dst->depth,dst->nChannels);
    double *temp=(double *)malloc(sizeof(double)*d_width*d_height);
    //IplImage *tempdst=cvCreateImage(cvGetSize(dst), dst->depth,dst->nChannels);
    double *tempdst=(double *)malloc(sizeof(double)*d_width*d_height);

    //IplImage *realdst=cvCreateImage(cvGetSize(dst), dst->depth,dst->nChannels);
    double *realdst=(double *)malloc(sizeof(double)*d_width*d_height);

    Zero(realdst, d_width, d_height);
    Zoom(src,s_width,s_height,temp,d_width,d_height);
    
    for(int i=0;i<se_width;i++){
        for(int j=0;j<se_height;j++){
            if(se[j*se_width+i]>100.0){
                m.x=i-center->x;
                m.y=j-center->y;
                Translation(temp,tempdst,d_width,d_height, &m);
                Or(tempdst, realdst, realdst,d_width,d_height);
            }
        }
    }
    matrixCopy(realdst, dst, d_width, d_height);
    cvCopy(realdst, dst, NULL);
    free(temp);
    free(realdst);
    free(tempdst);
}

//腐蚀
void Erode(double *src,int s_width,int s_height,double *dst,int d_width,int d_height,double *se,int se_width,int se_height,Position *center){
    
    if(center==NULL){
        Position temp;
        temp.x=se_width/2;
        temp.y=se_height/2;
        center=&temp;
    }
    //printf("%d,%d",center->x,center->y);
    MoveDirection m;
    //IplImage *temp=cvCreateImage(cvGetSize(dst), dst->depth,dst->nChannels);
    double *temp=(double *)malloc(sizeof(double)*d_width*d_height);
    //IplImage *tempdst=cvCreateImage(cvGetSize(dst), dst->depth,dst->nChannels);
    double *tempdst=(double *)malloc(sizeof(double)*d_width*d_height);
    
    //IplImage *realdst=cvCreateImage(cvGetSize(dst), dst->depth,dst->nChannels);
    double *realdst=(double *)malloc(sizeof(double)*d_width*d_height);
    
    Zero(realdst, d_width, d_height);
    Zoom(src,s_width,s_height,temp,d_width,d_height);
    
    for(int i=0;i<se_width;i++){
        for(int j=0;j<se_height;j++){
            if(se[j*se_width+i]>100.0){
                m.x=center->x-i;
                m.y=center->y-j;
                Translation(temp,tempdst,d_width,d_height, &m);
                And(tempdst, realdst, realdst,d_width,d_height);
            }
        }
    }
    matrixCopy(realdst, dst, d_width, d_height);
    cvCopy(realdst, dst, NULL);
    free(temp);
    free(realdst);
    free(tempdst);
}

//开操作
void Open(double *src,int width,int height,double *dst,double *se,int se_width,int se_height,Position *center){
    Erode(src,width,height, dst,width,height,se,se_width,se_height, center);
    Dilate(dst,width,height, dst,width,height,se,se_width,se_height, center);

}
//关操作
void Close(double *src,int width,int height,double *dst,double *se,int se_width,int se_height,Position *center){
    Dilate(src,width,height, dst,width,height,se,se_width,se_height, center);
    Erode(dst,width,height, dst,width,height,se,se_width,se_height, center);
    
}
//击中与击不中
void HitorMiss(double *src,int width,int height,double *se1,int se1_width,int se1_height,double *se2,int se2_width,int se2_height,double *dst,Position *se1center,Position *se2center){
    //IplImage *temp1=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    //IplImage *temp2=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    double *temp1=(double *)malloc(sizeof(double)*width*height);
    double *temp2=(double *)malloc(sizeof(double)*width*height);
    //Erode(src, temp1, se1, se1center);
    Erode(src, width, height, temp1, width, height, se1, se1_width, se1_height, se1center);
    //Not(src, temp2);
    Not(src, temp2, width, height);
    //Erode(temp2, temp2, se2, se2center);
    Erode(temp2, width, height, temp2, width, height, se2, se2_width, se2_height, se2center);
    //And(temp1, temp2, dst);
    And(temp1, temp2, dst, width, height);
    free(temp1);
    free(temp2);
}
//二值图像，边缘检测
void BinaryEdge(double *src,int width,int height,double* dst){
    //IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    double *temp=(double *)malloc(sizeof(double)*width*height);
    //Erode(src, temp, NULL, NULL);
    double se[9]={255.,255.,255.,255.,255.,255.,255.,255.,255.};
    Erode(src, width, height, temp, width, height,se ,3, 3, NULL);
    matrixSub(src, temp, dst, width, height);
    free(temp);
}
//孔洞填充
void FillHole(double *src,double *dst,int width,int height,Position *seed){
    //IplImage * temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    double *temp=(double *)malloc(sizeof(double)*width*height);
    //cvZero(temp);
    Zero(temp, width, height);
    //IplImage * lasttemp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    double *lasttemp=(double *)malloc(sizeof(double)*width*height);
    //IplImage * nsrc=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    double *nsrc=(double *)malloc(sizeof(double)*width*height);
    //Not(src, nsrc);
    Not(src, nsrc, width, height);
    //cvSetReal2D(temp, seed->y, seed->x, 255.0);
    temp[seed->y*width+seed->x]=255.;
    double se[9]={255.,255.,255.,255.,255.,255.,255.,255.,255.};
    while(!matrixisEqu(lasttemp, temp, width, height)){
        //cvCopy(temp, lasttemp, NULL);
        matrixCopy(temp, lasttemp, width, height);
        //Dilate(temp, temp, se, NULL);
        Dilate(temp, width, height, temp, width, height, se, 3, 3, NULL);
        //And(temp, nsrc, temp);
        And(temp, nsrc, temp, width, height);
    }
    Or(temp, src, dst, width, height);
    free(temp);
    free(lasttemp);
    free(nsrc);
}
//连通分量获取
void GetConCompG_Onent(double *src,double *dst,int width,int height,Position *seed){
    //IplImage * temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    double *temp=(double *)malloc(sizeof(double)*width*height);
    
    //cvZero(temp);
    Zero(temp, width, height);
    //IplImage * lasttemp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    double *lasttemp=(double*)malloc(sizeof(double)*width*height);
    //cvSetReal2D(temp, seed->y, seed->x, 255.0);
    temp[seed->y*width+seed->x]=255.0;
    double se[9]={255.,255.,255.,255.,255.,255.,255.,255.,255.};
    while(!matrixisEqu(lasttemp, temp, width, height)){
        //cvCopy(temp, lasttemp, NULL);
        matrixCopy(temp, lasttemp, width, height);
        //Dilate(temp, temp, se, NULL);
        Dilate(temp, width, height, temp, width, height, se, 3, 3, NULL);
        //And(temp, src, temp);
        And(temp, src, temp, width, height);
        
    }
    //cvCopy(temp, dst, NULL);
    matrixCopy(temp, dst, width, height);
    free(temp);
    free(lasttemp);
}
//骨架
void FrameWork(double *src,double *dst,int width,int height,double *se,int se_width,int se_height){
    //cvZero(dst);
    double *temp_dst=(double*)malloc(sizeof(double)*width*height);
    Zero(temp_dst, width, height);
    //IplImage *temp=cvCreateImage(cvGetSize(src), src->depth,src->nChannels);
    //IplImage *temp_open=cvCreateImage(cvGetSize(src), src->depth,src->nChannels);
    double *temp=(double *)malloc(sizeof(double)*width*height);
    double *temp_open=(double *)malloc(sizeof(double)*width*height);
    //cvCopy(src, temp, NULL);
    matrixCopy(src, temp, width, height);
    while(!matrixisEmpty(temp,width,height)){
        //Erode(temp, temp, se, NULL);
        Erode(temp, width, height, temp, width, height, se, se_width, se_height, NULL);
        //cvCopy(temp, temp_open, NULL);
        matrixCopy(temp, temp_open, width, height);
        //Open(temp_open, temp_open, se, NULL);
        Open(temp_open, width, height, temp_open, se, se_width, se_height, NULL);
        //cvSub(temp, temp_open, temp_open,NULL);
        matrixSub(temp, temp_open,temp_open, width, height);
        //Or(temp_open, dst, dst);
        Or(temp_open, temp_dst, temp_dst, width, height);
    }
    matrixCopy(temp_dst, dst, width, height);
    free(temp);
    free(temp_open);
    free(temp_dst);
}
//凸壳生成结构元
double* CreateConvexhullSE(int num){
    //IplImage *se=cvCreateImage(cvSize(3, 3), 8, 1);
    double *se=(double*)malloc(sizeof(double)*9);
    //cvZero(se);
    Zero(se, 3, 3);
    switch (num) {
        case 0:
        {
            //cvSetReal2D(se, 0, 0, 255.0);
            //cvSetReal2D(se, 1, 0, 255.0);
            //cvSetReal2D(se, 2, 0, 255.0);
            se[0]=255.0;
            se[3]=255.0;
            se[6]=255.0;
        }
            break;
        case 1:
        {
            //cvSetReal2D(se, 0, 0, 255.0);
            //cvSetReal2D(se, 0, 1, 255.0);
            //cvSetReal2D(se, 0, 2, 255.0);
            se[0]=255.0;
            se[1]=255.0;
            se[2]=255.0;
        
        }
            break;
        case 2:
        {
            //cvSetReal2D(se, 0, 2, 255.0);
            //cvSetReal2D(se, 1, 2, 255.0);
            //cvSetReal2D(se, 2, 2, 255.0);
            se[2]=255.0;
            se[5]=255.0;
            se[8]=255.0;
        }
            break;
        case 3:
        {
            //cvSetReal2D(se, 2, 0, 255.0);
            //cvSetReal2D(se, 2, 1, 255.0);
            //cvSetReal2D(se, 2, 2, 255.0);
            se[6]=255.0;
            se[7]=255.0;
            se[8]=255.0;
        }
            break;
        default:
            break;
    }
    return se;
}
//凸壳
void Convexhull(double *src,double *dst,int width,int height){
    //cvCopy(src, dst, NULL);
    double * temp_dst=(double *)malloc(sizeof(double)*width*height);
    matrixCopy(src, temp_dst, width, height);
    double * se[4];
    //IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    //IplImage *temp_last=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    double * temp=(double *)malloc(sizeof(double)*width*height);
    double * temp_last=(double *)malloc(sizeof(double)*width*height);
    //cvCopy(src, temp, NULL);
    matrixCopy(src, temp, width,height);
    for(int i=0;i<4;i++){
        //cvCopy(src, temp, NULL);
        se[i]=CreateConvexhullSE(i);
        while (!matrixisEqu(temp, temp_last, width, height)) {
            //cvCopy(temp, temp_last, NULL);
            matrixCopy(temp, temp_last, width, height);
            //Erode(temp, temp, se[i],NULL);
            Erode(temp, width,height, temp, width, height, se[i], 3, 3, NULL);
            //Or(temp, dst, temp);
            Or(temp, temp_dst, temp, width, height);
          
        }
        //cvCopy(temp, dst, NULL);
        matrixCopy(temp, temp_dst, width, height);
        free(se[i]);
    }
    matrixCopy(temp_dst, dst, width, height);
    free(temp);
    free(temp_last);

}
//生成细化结构元
double* CreateThinningSE(int num){
    //IplImage *se=cvCreateImage(cvSize(3, 3), 8, 1);
    double *se=(double *)malloc(sizeof(double)*9);
    //cvZero(se);
    Zero(se, 3, 3);
    switch (num) {
        case 0:
        {
            //cvSetReal2D(se, 2, 0, 255.0);
            //cvSetReal2D(se, 2, 1, 255.0);
            //cvSetReal2D(se, 2, 2, 255.0);
            //cvSetReal2D(se, 1, 1, 255.0);
            se[6]=255.0;
            se[7]=255.0;
            se[8]=255.0;
            se[4]=255.0;
        }
            break;
        case 1:
        {
            //cvSetReal2D(se, 1, 1, 255.0);
            //cvSetReal2D(se, 1, 0, 255.0);
            //cvSetReal2D(se, 2, 0, 255.0);
            //cvSetReal2D(se, 2, 1, 255.0);
            se[3]=255.0;
            se[4]=255.0;
            se[6]=255.0;
            se[7]=255.0;
            
        }
            break;
        case 2:
        {
            //cvSetReal2D(se, 1, 1, 255.0);
            //cvSetReal2D(se, 1, 0, 255.0);
            //cvSetReal2D(se, 2, 0, 255.0);
            //cvSetReal2D(se, 0, 0, 255.0);
            se[3]=255.0;
            se[4]=255.0;
            se[6]=255.0;
            se[0]=255.0;
        }
            break;
        case 3:
        {
            //cvSetReal2D(se, 1, 1, 255.0);
            //cvSetReal2D(se, 0, 0, 255.0);
            //cvSetReal2D(se, 0, 1, 255.0);
            //cvSetReal2D(se, 1, 0, 255.0);
            se[4]=255.0;
            se[0]=255.0;
            se[1]=255.0;
            se[3]=255.0;
        }
            break;
        case 4:
        {
            //cvSetReal2D(se, 1, 1, 255.0);
            //cvSetReal2D(se, 0, 0, 255.0);
            //cvSetReal2D(se, 0, 1, 255.0);
            //cvSetReal2D(se, 0, 2, 255.0);
            se[4]=255.0;
            se[0]=255.0;
            se[1]=255.0;
            se[2]=255.0;
        }
            break;
        case 5:
        {
            //cvSetReal2D(se, 0, 1, 255.0);
            //cvSetReal2D(se, 0, 2, 255.0);
            //cvSetReal2D(se, 1, 1, 255.0);
            //cvSetReal2D(se, 1, 2, 255.0);
            se[1]=255.0;
            se[2]=255.0;
            se[4]=255.0;
            se[5]=255.0;
            
        }
            break;
        case 6:
        {
            //cvSetReal2D(se, 1, 1, 255.0);
            //cvSetReal2D(se, 0, 2, 255.0);
            //cvSetReal2D(se, 1, 2, 255.0);
            //cvSetReal2D(se, 2, 2, 255.0);
            se[4]=255.0;
            se[2]=255.0;
            se[5]=255.0;
            se[8]=255.0;
        }
            break;
        case 7:
        {
            //cvSetReal2D(se, 1, 1, 255.0);
            //cvSetReal2D(se, 1, 2, 255.0);
            //cvSetReal2D(se, 2, 1, 255.0);
            //cvSetReal2D(se, 2, 2, 255.0);
            se[4]=255.0;
            se[5]=255.0;
            se[7]=255.0;
            se[8]=255.0;
        }
            break;
        default:
            break;
    }
    return se;
}
double* CreateThinningUSE(int num){
    //IplImage *se=cvCreateImage(cvSize(3, 3), 8, 1);
    double *se=(double *)malloc(sizeof(double)*9);
    //cvZero(se);
    Zero(se, 3, 3);
    switch (num) {
        case 0:
        {
            
            //cvSetReal2D(se, 0, 1, 255.0);
            //cvSetReal2D(se, 0, 2, 255.0);
            //cvSetReal2D(se, 0, 0, 255.0);
            se[1]=255.0;
            se[2]=255.0;
            se[0]=255.0;
        }
            break;
        case 1:
        {
            //cvSetReal2D(se, 0, 1, 255.0);
            //cvSetReal2D(se, 0, 2, 255.0);
            //cvSetReal2D(se, 1, 2, 255.0);
            se[1]=255.0;
            se[2]=255.0;
            se[5]=255.0;
            
        }
            break;
        case 2:
        {
            
            //cvSetReal2D(se, 0, 2, 255.0);
            //cvSetReal2D(se, 1, 2, 255.0);
            //cvSetReal2D(se, 2, 2, 255.0);
            se[2]=255.0;
            se[5]=255.0;
            se[8]=255.0;
        }
            break;
        case 3:
        {
            //cvSetReal2D(se, 1, 2, 255.0);
            //cvSetReal2D(se, 2, 1, 255.0);
            //cvSetReal2D(se, 2, 2, 255.0);
            se[5]=255.0;
            se[7]=255.0;
            se[8]=255.0;
        }
            break;
        case 4:
        {
            
            //cvSetReal2D(se, 2, 0, 255.0);
            //cvSetReal2D(se, 2, 1, 255.0);
            //cvSetReal2D(se, 2, 2, 255.0);
            se[6]=255.0;
            se[7]=255.0;
            se[8]=255.0;
        }
            break;
        case 5:
        {
            
            //cvSetReal2D(se, 1, 0, 255.0);
            //cvSetReal2D(se, 2, 0, 255.0);
            //cvSetReal2D(se, 2, 1, 255.0);
            se[3]=255.0;
            se[6]=255.0;
            se[7]=255.0;
            
        }
            break;
        case 6:
        {
           
            //cvSetReal2D(se, 0, 0, 255.0);
            //cvSetReal2D(se, 1, 0, 255.0);
            //cvSetReal2D(se, 2, 0, 255.0);
            se[0]=255.0;
            se[3]=255.0;
            se[6]=255.0;
        }
            break;
        case 7:
        {
            //cvSetReal2D(se, 0, 0, 255.0);
            //cvSetReal2D(se, 0, 1, 255.0);
            //cvSetReal2D(se, 1, 0, 255.0);
            se[0]=255.0;
            se[1]=255.0;
            se[3]=255.0;
        }
            break;
        default:
            break;
    }
    return se;
}

//细化操作
void Thinning(double *src,double *dst,int width,int height){
    //IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    //IplImage *temp_last=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    //IplImage *temp_com=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    //cvZero(temp_last);
    //cvCopy(src, temp, NULL);
    double *temp=(double *)malloc(sizeof(double)*width*height);
    double *temp_last=(double *)malloc(sizeof(double)*width*height);
    double *temp_com=(double *)malloc(sizeof(double)*width*height);
    matrixCopy(src, temp, width, height);
    while(!matrixisEqu(temp, temp_com,width,height)){
        //cvCopy(temp, temp_com, NULL);
        matrixCopy(temp, temp_com, width, height);
        for(int i=0;i<8;i++){
            //cvCopy(temp, temp_last, NULL);
            //IplImage *se1=CreateThinningSE(i);
            //IplImage *se2=CreateThinningUSE(i);
            matrixCopy(temp, temp_last, width, height);
            double *se1=CreateThinningSE(i);
            double *se2=CreateThinningUSE(i);
            //HitorMiss(temp, se1, se2, temp, NULL, NULL);
            HitorMiss(temp, width, height, se1, 3, 3, se2, 3, 3, temp, NULL, NULL);
            //cvSub(temp_last, temp, temp, NULL);
            matrixSub(temp_last, temp, temp, width, height);
            //cvReleaseImage(&se1);
            //cvReleaseImage(&se2);
            free(se1);
            free(se2);
        }
        
    }
    //cvCopy(temp, dst, NULL);
    matrixCopy(temp, dst, width, height);
    //cvReleaseImage(&temp);
    //cvReleaseImage(&temp_com);
    //cvReleaseImage(&temp_last);
    free(temp);
    free(temp_com);
    free(temp_last);
}
//重建开操作
void reBuildOpen(double *src,double *dst,double *ground,int width,int height,double *dilateSE,int dse_width,int dse_height,double *erodeSE,int ese_width,int ese_height,int eroden){
    //IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    //IplImage *temp_last=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    double *temp=(double*)malloc(sizeof(double)*width*height);
    double *temp_last=(double*)malloc(sizeof(double)*width*height);
    
    //cvCopy(src, temp, NULL);
    matrixCopy(src, temp, width, height);
    for(int i=0;i<eroden;i++){
        //Erode(temp, temp, erodeSE, NULL);
        Erode(temp, width, height, temp,width, height, erodeSE, ese_height, ese_height, NULL);
    }
   
    while(!matrixisEqu(temp, temp_last,width,height)){
        //cvCopy(temp, temp_last, NULL);
        matrixCopy(temp, temp_last, width, height);
        //Dilate(temp, temp, dilateSE, NULL);
        Dilate(temp, width, height, temp, width, height, dilateSE, dse_width, dse_height, NULL);
        //And(temp, ground, temp);
        And(temp, ground, temp, width, height);
        
    }
    //cvCopy(temp, dst, NULL);
    matrixCopy(temp, dst, width, height);
    free(temp);
    free(temp_last);

}

