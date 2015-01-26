//
//  GrayMorphology
//  灰度图像形态学
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 14/11/28.
//  Copyright (c) 2014年 谭升. All rights reserved.
//
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include "GrayMorphology.h"
#define TOFINDMAX 0
#define TOFINDMIN 1
#define isSIZEEQU(x,y) (((x)->width)==((y)->width)&&((x)->height)==((y)->height))

//判断结构元是否平滑
int isSmooth(IplImage *src){
    int width=src->width;
    int height=src->height;
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++){
            int v=cvGetReal2D(src,j,i);
            if(v!=255.0&&v!=0.0)
                return 0;
        }
    return 1;
}
//判断两幅图像是否相等
int isEqu(IplImage *src1,IplImage *src2){
    if(!isSIZEEQU(src1, src2))
        return 0;
    for(int i=0;i<src1->width;i++)
        for(int j=0;j<src1->height;j++){
            double v1=cvGetReal2D(src1, j, i);
            double v2=cvGetReal2D(src2, j, i);
            if(v1!=v2)
                return 0;
        }
    return 1;

}
//将图像全部设置为1
void G_One(IplImage *src){
    int width=src->width;
    int height=src->height;
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++)
            cvSetReal2D(src, j, i, 255.0);


}
//位移，如果非平滑SE将加上sevalue，即对应的灰度值
void G_Translation(IplImage *src ,IplImage *dst,double SEvalue,Position *d,int istoFindMin){
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    int srcwidth=src->width;
    int srcheight=src->height;
    int dstwidth=dst->width;
    int dstheight=dst->height;
    if(istoFindMin)
        G_One(temp);
    else
        cvZero(temp);
    for(int i=0;i<srcwidth;i++){
        for(int j=0;j<srcheight;j++){
            int target_x=i+d->x;
            int target_y=j+d->y;
            if(target_x>=0&&target_y>=0&&
               target_x<dstwidth&&target_y<dstheight){
                double value=cvGetReal2D(src, j, i)+SEvalue;
                value=(value>=255.0?255.0:value);
                cvSetReal2D(temp, target_y, target_x, value);
            }
        }
    }
    cvCopy(temp, dst, NULL);
    
    cvReleaseImage(&temp);
}
//找出两幅等大图像中同一位置中相对较大的像素值
void MaxPix(IplImage *src1 ,IplImage *src2,IplImage *dst){
    if(!isSIZEEQU(src1, src2)||!isSIZEEQU(src1, dst)){
        printf("MaxPix wrong: src size not equ!\n");
        exit(1);
    }
    int width=src1->width;
    int height=src1->height;
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++){
            double value1=cvGetReal2D(src1, j,i);
            double value2=cvGetReal2D(src2, j,i);
            value1>value2?cvSetReal2D(dst, j,i,value1):cvSetReal2D(dst, j, i, value2);
        }

}
//找出两幅等大图像中同一位置中相对较小的像素值
void MinPix(IplImage *src1 ,IplImage *src2,IplImage *dst){
    if(!isSIZEEQU(src1, src2)||!isSIZEEQU(src1, dst)){
        printf("MaxPix wrong: src size not equ!\n");
        exit(1);
    }
    int width=src1->width;
    int height=src1->height;
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++){
            double value1=cvGetReal2D(src1, j,i);
            double value2=cvGetReal2D(src2, j,i);
            value1<value2?cvSetReal2D(dst, j, i, value1):cvSetReal2D(dst, j, i, value2);
        }
    
}
//灰度图像膨胀
void Dilate_Gray(IplImage *src,IplImage *dst,IplImage *se,Position *center){
    int SEissmooth=isSmooth(se);
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    IplImage *temp_last=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    Position centerde;
    centerde.x=se->width/2;
    centerde.y=se->height/2;
    if(center==NULL){
        center=&centerde;
    }
    int sewidth=se->width;
    int seheight=se->height;
    cvCopy(src,temp_last,NULL);
    for(int i=0;i<sewidth;i++)
        for(int j=0;j<seheight;j++){
            cvCopy(src,temp,NULL);
            double value=cvGetReal2D(se, j, i);
            if(value!=0.0){
                Position d;
                d.x=center->x-i;
                d.y=center->y-j;
                if(SEissmooth)
                    G_Translation(temp, temp, 0.0, &d,TOFINDMAX);
                else
                    G_Translation(temp, temp, value, &d,TOFINDMAX);
                MaxPix(temp, temp_last, temp_last);
            }
        }
    cvCopy(temp_last, dst, NULL);
    cvReleaseImage(&temp);
    cvReleaseImage(&temp_last);

}
//灰度图像腐蚀
void Erode_Gray(IplImage *src,IplImage *dst,IplImage *se,Position *center){
    int SEissmooth=isSmooth(se);
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    IplImage *temp_last=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    Position centerde;
    centerde.x=se->width/2;
    centerde.y=se->height/2;
    if(center==NULL){
        center=&centerde;
    }
    int sewidth=se->width;
    int seheight=se->height;
    cvCopy(src,temp_last,NULL);
    for(int i=0;i<sewidth;i++)
        for(int j=0;j<seheight;j++){
            cvCopy(src,temp,NULL);
            double value=cvGetReal2D(se, j, i);
            if(value!=0.0){
                Position d;
                
                d.x=i-center->x;
                d.y=j-center->y;
                if(SEissmooth)
                    G_Translation(temp, temp, 0.0, &d,TOFINDMIN);
                else
                    G_Translation(temp, temp, -1.0*value, &d,TOFINDMIN);
                MinPix(temp, temp_last, temp_last);
                
            }
        }
    cvCopy(temp_last, dst, NULL);
    cvReleaseImage(&temp);
    cvReleaseImage(&temp_last);
    
}
//开操作
void Open_Gray(IplImage *src,IplImage *dst,IplImage *se,Position *center){
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    Erode_Gray(src, temp, se, center);
    Dilate_Gray(temp, temp, se, center);
    cvCopy(temp, dst, NULL);
    cvReleaseImage(&temp);

}
//闭操作
void Close_Gray(IplImage *src,IplImage *dst,IplImage *se,Position *center){
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    Dilate_Gray(src, temp, se, center);
    Erode_Gray(temp, temp, se, center);
    cvCopy(temp, dst, NULL);
    cvReleaseImage(&temp);
    
}
//灰度梯度形态学提取
void Gard_Gray(IplImage *src,IplImage *dst,IplImage *se,Position *center){
    IplImage *temp_dilate=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    IplImage *temp_erode=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    Dilate_Gray(src, temp_dilate, se, center);
    Erode_Gray(src, temp_erode, se, center);
    cvSub(temp_dilate, temp_erode, dst, NULL);
    cvReleaseImage(&temp_erode);
    cvReleaseImage(&temp_dilate);


}
//顶帽操作
void TopHat(IplImage *src,IplImage *dst,IplImage *se,Position *center){
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    Open_Gray(src, temp, se, center);
    cvSub( src,temp, dst, NULL);
    cvReleaseImage(&temp);

}
//底帽操作
void BottomHat(IplImage *src,IplImage *dst,IplImage *se,Position *center){
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    Close_Gray(src, temp, se, center);
    cvSub(temp,src, dst, NULL);
    cvReleaseImage(&temp);
    
}
//测地腐蚀
void Erode_Gray_g(IplImage *src,IplImage *ground,IplImage *dst,IplImage *se,Position *center){
    int SEissmooth=isSmooth(se);
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    IplImage *temp_last=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    Position centerde;
    centerde.x=se->width/2;
    centerde.y=se->height/2;
    if(center==NULL){
        center=&centerde;
    }
    int sewidth=se->width;
    int seheight=se->height;
    cvCopy(src,temp_last,NULL);
    for(int i=0;i<sewidth;i++)
        for(int j=0;j<seheight;j++){
            cvCopy(src,temp,NULL);
            double value=cvGetReal2D(se, j, i);
            if(value!=0.0){
                Position d;
                
                d.x=i-center->x;
                d.y=j-center->y;
                if(SEissmooth)
                    G_Translation(temp, temp, 0.0, &d,TOFINDMIN);
                else
                    G_Translation(temp, temp, -1.0*value, &d,TOFINDMIN);
                MinPix(temp, temp_last, temp_last);
                
            }
        }
    MaxPix(temp_last,ground,temp_last);
    cvCopy(temp_last, dst, NULL);
    cvReleaseImage(&temp);
    cvReleaseImage(&temp_last);


}
//测地膨胀
void Dilate_Gray_g(IplImage *src,IplImage *ground,IplImage *dst,IplImage *se,Position *center){
    int SEissmooth=isSmooth(se);
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    IplImage *temp_last=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    Position centerde;
    centerde.x=se->width/2;
    centerde.y=se->height/2;
    if(center==NULL){
        center=&centerde;
    }
    int sewidth=se->width;
    int seheight=se->height;
    cvCopy(src,temp_last,NULL);
    for(int i=0;i<sewidth;i++)
        for(int j=0;j<seheight;j++){
            cvCopy(src,temp,NULL);
            double value=cvGetReal2D(se, j, i);
            if(value!=0.0){
                Position d;
                d.x=center->x-i;
                d.y=center->y-j;
                if(SEissmooth)
                    G_Translation(temp, temp, 0.0, &d,TOFINDMAX);
                else
                    G_Translation(temp, temp, value, &d,TOFINDMAX);
                MaxPix(temp, temp_last, temp_last);
            }
        }
    MinPix(temp_last, ground, temp_last);
    cvCopy(temp_last, dst, NULL);
    cvReleaseImage(&temp);
    cvReleaseImage(&temp_last);
}
//重建开操作
void Rebuild_Open(IplImage *src,IplImage *dst,IplImage *ground,IplImage *erodeSE,IplImage *dilateSE,int eroden){
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    IplImage *temp_last=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    cvCopy(src, temp, NULL);
    for(int i=0;i<eroden;i++){
        Erode_Gray(temp, temp, erodeSE, NULL);
    }
    
    while(!isEqu(temp, temp_last)){
        cvCopy(temp, temp_last, NULL);
        Dilate_Gray_g(temp, ground, temp, dilateSE, NULL);
        
    }
    cvCopy(temp, dst, NULL);
    cvReleaseImage(&temp);
    cvReleaseImage(&temp_last);
    
}
//重建闭操作，这段没测试
void Rebuild_Close(IplImage *src,IplImage *dst,IplImage *ground,IplImage *dilateSE,IplImage *erodeSE,int dilaten){
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    IplImage *temp_last=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    cvCopy(src, temp, NULL);
    for(int i=0;i<dilaten;i++){
        Dilate_Gray(temp, temp, dilateSE, NULL);
    }
    
    while(!isEqu(temp, temp_last)){
        cvCopy(temp, temp_last, NULL);
        Erode_Gray(temp, temp, erodeSE, NULL);
        MinPix(temp, ground, temp);
        
    }
    cvCopy(temp, dst, NULL);
    cvReleaseImage(&temp);
    cvReleaseImage(&temp_last);
    
}
//重建顶帽操作
void Rebuild_Tophat(IplImage *src,IplImage *dst,IplImage *ground,IplImage *dilateSE,IplImage *erodeSE,int eroden){
    Rebuild_Open(src,dst,ground,erodeSE,dilateSE,eroden);
    cvSub(src, dst, dst, NULL);

}
//重建底帽操作
void Rebuild_Bottomhat(IplImage *src,IplImage *dst,IplImage *ground,IplImage *dilateSE,IplImage *erodeSE,int dilaten){
    Rebuild_Close(src,dst,ground,dilateSE,erodeSE,dilaten);
    cvSub(src, dst, dst, NULL);
    
}
