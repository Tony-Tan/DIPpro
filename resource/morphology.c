//
//  Morphology
//  二值图像形态学处理
//  Created by 谭升 on 14/12/23.
//  tony.sheng.tan@gmail.com
//  Copyright (c) 2014年 谭升. All rights reserved.
//
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include "morphology.h"
#define isSIZEEQU(x,y) (((x)->width)==((y)->width)&&((x)->height)==((y)->height))

//位移操作，将图像整体移动，如果超出边界舍去
int isEqual(IplImage *src1,IplImage *src2){
    if(!isSIZEEQU(src1, src2))
        return 0;
    int width=src1->width;
    int height=src1->height;
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++){
            int v0=cvGetReal2D(src1, j, i);
            int v1=cvGetReal2D(src2, j, i);
            if(v0!=v1)
                return 0;
        }
    return 1;
}
//检测图像是否为空
int isEmpty(IplImage *src){
    int width=src->width;
    int height=src->height;
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++){
            int v=cvGetReal2D(src, j, i);
            if(v!=0.0)
                return 0;
        }
    return 1;
}

void Translation(IplImage *src,IplImage *dst,MoveDirection *direction){
    int width=src->width;
    int height=src->height;
    //printf("%d,%d\n",direction->x,direction->y);
    IplImage *temp=cvCreateImage(cvSize(width, height), src->depth, src->nChannels);
    cvZero(temp);
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++){
                if(j+direction->y<height &&
                   i+direction->x<width  &&
                   j+direction->y>=0      &&
                   i+direction->x>=0        )
                cvSetReal2D(temp, j+direction->y, i+direction->x, cvGetReal2D(src, j, i));
            
        }
    cvCopy(temp, dst, NULL);
    cvReleaseImage(&temp);
}
//将小的图像弄到大的黑色图像中间，或者说是给图像加黑色边框
void Zoom(IplImage *src,IplImage *dst){
    if(dst->width<src->width         ||
       dst->height<src->height       ||
       (dst->height-src->height)%2==1||
       (dst->width-src->width)%2==1){
        if(dst->width<src->width )
            printf("Zoom wrong:dst's width too small!\n");
        if(dst->height<src->height )
            printf("Zoom wrong:dst's height too small!\n");
        if((dst->height-src->height)%2==1||(dst->width-src->width)%2==1)
            printf("Zoom wrong:dst-src not a oushu!\n");
        exit(0);
    }
    MoveDirection m;
    m.x=(dst->width-src->width)/2;
    m.y=(dst->height-src->height)/2;
    cvZero(dst);
    for(int i=m.x,j=0;j<src->width;i++,j++){
        for(int k=m.y,n=0;n<src->height;k++,n++){
            cvSetReal2D(dst, k, i, cvGetReal2D(src, n, j));
        
        }
    }

}
//逻辑与操作
int And(IplImage *src0,IplImage *src1,IplImage *dst){
    int isChanged=0;
    if(!isSIZEEQU(src0,src1)){
        printf("And wrong !\n");
        exit(0);
    }
    if(!isSIZEEQU(src0,dst)){
        printf("And wrong !\n");
        exit(0);
    }
    int width=src0->width;
    int height=src0->height;
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            if(cvGetReal2D(src0, j, i)>100.0&&
               cvGetReal2D(src1, j, i)>100.0)
                cvSetReal2D(dst, j, i, 255.0);
            else
                cvSetReal2D(dst, j, i, 0.0);
        }
    }
    return isChanged;
}
//逻辑或操作
void Or(IplImage *src0,IplImage *src1,IplImage *dst){
    if(!isSIZEEQU(src0,src1)){
        printf("And wrong !\n");
        exit(0);
    }
    if(!isSIZEEQU(src0,dst)){
        printf("And wrong !\n");
        exit(0);
    }
    int width=src0->width;
    int height=src0->height;
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            if(cvGetReal2D(src0, j, i)>100.0||
               cvGetReal2D(src1, j, i)>100.0)
                cvSetReal2D(dst, j, i, 255);
           
        }
    }
}
//取反
void Not(IplImage *src,IplImage *dst){
   
    if(!isSIZEEQU(src,dst)){
        printf("Not wrong !\n");
        exit(0);
    }
    int width=src->width;
    int height=src->height;
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            cvSetReal2D(dst, j, i, 255.0-cvGetReal2D(src, j, i));
        }
    }
}
//将所有元素设为1
void G_One(IplImage *src){
    for(int i=0;i<src->width;i++)
        for(int j=0;j<src->height;j++)
            cvSetReal2D(src, j, i, 255.0);


}

//膨胀
void Dilate(IplImage *src,IplImage *dst,IplImage *se,Position *center){

    if(center==NULL){
        Position temp;
        temp.x=se->width/2;
        temp.y=se->height/2;
        center=&temp;
    }
    //printf("%d,%d",center->x,center->y);
    MoveDirection m;
    IplImage *temp=cvCreateImage(cvGetSize(dst), dst->depth,dst->nChannels);
    IplImage *tempdst=cvCreateImage(cvGetSize(dst), dst->depth,dst->nChannels);
    IplImage *realdst=cvCreateImage(cvGetSize(dst), dst->depth,dst->nChannels);
    cvZero(realdst);
    Zoom(src,temp);
    int width=se->width;
    int height=se->height;
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            if(cvGetReal2D(se, j, i)>100.0){
                m.x=i-center->x;
                m.y=j-center->y;
                Translation(temp,tempdst, &m);
                Or(tempdst, realdst, realdst);
            }
        }
    }
    
    cvCopy(realdst, dst, NULL);
    cvReleaseImage(&temp);
    cvReleaseImage(&realdst);
    cvReleaseImage(&tempdst);
}

//腐蚀
void Erode(IplImage *src,IplImage *dst,IplImage *se,Position *center){
    
   
    if(center==NULL){
        Position temp;
        temp.x=se->width/2;
        temp.y=se->height/2;
        center=&temp;
    }
    MoveDirection m;
    IplImage *temp=cvCreateImage(cvGetSize(dst), dst->depth,dst->nChannels);
    IplImage *tempdst=cvCreateImage(cvGetSize(dst), dst->depth,dst->nChannels);
    IplImage *realdst=cvCreateImage(cvGetSize(dst), dst->depth,dst->nChannels);
    G_One(realdst);
    Zoom(src,temp);
    int width=se->width;
    int height=se->height;
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            if(cvGetReal2D(se, j, i)>100.0){
                m.x=center->x-i;
                m.y=center->y-j;
                Translation(temp,tempdst, &m);
                And(tempdst, realdst, realdst);
            }
        }
    }
    cvCopy(realdst, dst, NULL);
    cvReleaseImage(&tempdst);
    cvReleaseImage(&temp);
    cvReleaseImage(&realdst);
}

//开操作
void Open(IplImage *src,IplImage *dst,IplImage *se,Position *center){
    Erode(src, dst, se, center);
    Dilate(dst, dst, se, center);

}
//关操作
void Close(IplImage *src,IplImage *dst,IplImage *se,Position *center){
    Dilate(src, dst, se, center);
    Erode(dst, dst, se, center);
    
}
//击中与击不中
void HitorMiss(IplImage *src,IplImage *se1,IplImage *se2,IplImage *dst,Position *se1center,Position *se2center){
    IplImage *temp1=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    IplImage *temp2=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    Erode(src, temp1, se1, se1center);
    Not(src, temp2);
    Erode(temp2, temp2, se2, se2center);
    And(temp1, temp2, dst);
    cvReleaseImage(&temp1);
    cvReleaseImage(&temp2);
}
//二值图像，边缘检测
void BinaryEdge(IplImage *src,IplImage* dst){
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    Erode(src, temp, NULL, NULL);
    cvSub(src, temp, dst, NULL);
    cvReleaseImage(&temp);
}
//孔洞填充
void FillHole(IplImage *src,IplImage *dst,IplImage *se,Position *seed){
    IplImage * temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    cvZero(temp);
    IplImage * lasttemp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    IplImage * nsrc=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    Not(src, nsrc);
    cvSetReal2D(temp, seed->y, seed->x, 255.0);
    while(!isEqual(lasttemp, temp)){
        cvCopy(temp, lasttemp, NULL);
        Dilate(temp, temp, se, NULL);
        And(temp, nsrc, temp);
        

        
    }
    Or(temp, src, dst);
    cvReleaseImage(&temp);
    cvReleaseImage(&lasttemp);
    cvReleaseImage(&nsrc);
}
//连通分量获取
void GetConCompG_Onent(IplImage *src,IplImage *dst,IplImage *se,Position *seed){
    IplImage * temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    cvZero(temp);
    IplImage * lasttemp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    
    cvSetReal2D(temp, seed->y, seed->x, 255.0);
    while(!isEqual(lasttemp, temp)){
        cvCopy(temp, lasttemp, NULL);
        Dilate(temp, temp, se, NULL);
        And(temp, src, temp);
        
    }
    cvCopy(temp, dst, NULL);
    cvReleaseImage(&temp);
    cvReleaseImage(&lasttemp);
}
//骨架
void FrameWork(IplImage *src,IplImage *dst,IplImage *se){
    cvZero(dst);
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth,src->nChannels);
    IplImage *temp_open=cvCreateImage(cvGetSize(src), src->depth,src->nChannels);
    cvCopy(src, temp, NULL);
    while(!isEmpty(temp)){
        Erode(temp, temp, se, NULL);
        cvCopy(temp, temp_open, NULL);
        Open(temp_open, temp_open, se, NULL);
        cvSub(temp, temp_open, temp_open,NULL);
        Or(temp_open, dst, dst);
        
    }
    cvReleaseImage(&temp);
    cvReleaseImage(&temp_open);
}
//凸壳生成结构元
IplImage* CreateConvexhullSE(int num){
    IplImage *se=cvCreateImage(cvSize(3, 3), 8, 1);
    cvZero(se);
    switch (num) {
        case 0:
        {
            cvSetReal2D(se, 0, 0, 255.0);
            cvSetReal2D(se, 1, 0, 255.0);
            cvSetReal2D(se, 2, 0, 255.0);
        }
            break;
        case 1:
        {
            cvSetReal2D(se, 0, 0, 255.0);
            cvSetReal2D(se, 0, 1, 255.0);
            cvSetReal2D(se, 0, 2, 255.0);
        
        }
            break;
        case 2:
        {
            cvSetReal2D(se, 0, 2, 255.0);
            cvSetReal2D(se, 1, 2, 255.0);
            cvSetReal2D(se, 2, 2, 255.0);
        }
            break;
        case 3:
        {
            cvSetReal2D(se, 2, 0, 255.0);
            cvSetReal2D(se, 2, 1, 255.0);
            cvSetReal2D(se, 2, 2, 255.0);
        }
            break;
        default:
            break;
    }
    return se;
}
//凸壳
void Convexhull(IplImage *src,IplImage *dst){
    cvCopy(src, dst, NULL);
    IplImage * se[4];
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    IplImage *temp_last=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    //cvCopy(src, temp, NULL);
    for(int i=0;i<4;i++){
        cvCopy(src, temp, NULL);
        se[i]=CreateConvexhullSE(i);
        while (!isEqual(temp, temp_last)) {
            cvCopy(temp, temp_last, NULL);
            Erode(temp, temp, se[i],NULL);
            Or(temp, dst, temp);
            
          
        }
        cvCopy(temp, dst, NULL);
        cvReleaseImage(&se[i]);
    }
    cvReleaseImage(&temp);
    cvReleaseImage(&temp_last);

}
//生成细化结构元
IplImage* CreateThinningSE(int num){
    IplImage *se=cvCreateImage(cvSize(3, 3), 8, 1);
    cvZero(se);
    switch (num) {
        case 0:
        {
            cvSetReal2D(se, 2, 0, 255.0);
            cvSetReal2D(se, 2, 1, 255.0);
            cvSetReal2D(se, 2, 2, 255.0);
            cvSetReal2D(se, 1, 1, 255.0);
        }
            break;
        case 1:
        {
            cvSetReal2D(se, 1, 1, 255.0);
            cvSetReal2D(se, 1, 0, 255.0);
            cvSetReal2D(se, 2, 0, 255.0);
            cvSetReal2D(se, 2, 1, 255.0);
            
        }
            break;
        case 2:
        {
            cvSetReal2D(se, 1, 1, 255.0);
            cvSetReal2D(se, 1, 0, 255.0);
            cvSetReal2D(se, 2, 0, 255.0);
            cvSetReal2D(se, 0, 0, 255.0);
        }
            break;
        case 3:
        {
            cvSetReal2D(se, 1, 1, 255.0);
            cvSetReal2D(se, 0, 0, 255.0);
            cvSetReal2D(se, 0, 1, 255.0);
            cvSetReal2D(se, 1, 0, 255.0);
        }
            break;
        case 4:
        {
            cvSetReal2D(se, 1, 1, 255.0);
            cvSetReal2D(se, 0, 0, 255.0);
            cvSetReal2D(se, 0, 1, 255.0);
            cvSetReal2D(se, 0, 2, 255.0);
        }
            break;
        case 5:
        {
            cvSetReal2D(se, 0, 1, 255.0);
            cvSetReal2D(se, 0, 2, 255.0);
            cvSetReal2D(se, 1, 1, 255.0);
            cvSetReal2D(se, 1, 2, 255.0);
            
        }
            break;
        case 6:
        {
            cvSetReal2D(se, 1, 1, 255.0);
            cvSetReal2D(se, 0, 2, 255.0);
            cvSetReal2D(se, 1, 2, 255.0);
            cvSetReal2D(se, 2, 2, 255.0);
        }
            break;
        case 7:
        {
            cvSetReal2D(se, 1, 1, 255.0);
            cvSetReal2D(se, 1, 2, 255.0);
            cvSetReal2D(se, 2, 1, 255.0);
            cvSetReal2D(se, 2, 2, 255.0);
        }
            break;
        default:
            break;
    }
    return se;
}
IplImage* CreateThinningUSE(int num){
    IplImage *se=cvCreateImage(cvSize(3, 3), 8, 1);
    cvZero(se);
    switch (num) {
        case 0:
        {
            
            cvSetReal2D(se, 0, 1, 255.0);
            cvSetReal2D(se, 0, 2, 255.0);
            cvSetReal2D(se, 0, 0, 255.0);
        }
            break;
        case 1:
        {
            cvSetReal2D(se, 0, 1, 255.0);
            cvSetReal2D(se, 0, 2, 255.0);
            cvSetReal2D(se, 1, 2, 255.0);
            
        }
            break;
        case 2:
        {
            
            cvSetReal2D(se, 0, 2, 255.0);
            cvSetReal2D(se, 1, 2, 255.0);
            cvSetReal2D(se, 2, 2, 255.0);
        }
            break;
        case 3:
        {
            cvSetReal2D(se, 1, 2, 255.0);
            cvSetReal2D(se, 2, 1, 255.0);
            cvSetReal2D(se, 2, 2, 255.0);
        }
            break;
        case 4:
        {
            
            cvSetReal2D(se, 2, 0, 255.0);
            cvSetReal2D(se, 2, 1, 255.0);
            cvSetReal2D(se, 2, 2, 255.0);
        }
            break;
        case 5:
        {
            
            cvSetReal2D(se, 1, 0, 255.0);
            cvSetReal2D(se, 2, 0, 255.0);
            cvSetReal2D(se, 2, 1, 255.0);
            
        }
            break;
        case 6:
        {
           
            cvSetReal2D(se, 0, 0, 255.0);
            cvSetReal2D(se, 1, 0, 255.0);
            cvSetReal2D(se, 2, 0, 255.0);
        }
            break;
        case 7:
        {
            cvSetReal2D(se, 0, 0, 255.0);
            cvSetReal2D(se, 0, 1, 255.0);
            cvSetReal2D(se, 1, 0, 255.0);
        }
            break;
        default:
            break;
    }
    return se;
}

//细化操作
void Thinning(IplImage *src,IplImage *dst){
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    IplImage *temp_last=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    IplImage *temp_com=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    cvZero(temp_last);
    cvCopy(src, temp, NULL);

    while(!isEqual(temp, temp_com)){
        cvCopy(temp, temp_com, NULL);
        for(int i=0;i<8;i++){
            cvCopy(temp, temp_last, NULL);
            IplImage *se1=CreateThinningSE(i);
            IplImage *se2=CreateThinningUSE(i);
            HitorMiss(temp, se1, se2, temp, NULL, NULL);
            
            cvSub(temp_last, temp, temp, NULL);
                        cvReleaseImage(&se1);
            cvReleaseImage(&se2);
        }
        
    }
    cvCopy(temp, dst, NULL);
    cvReleaseImage(&temp);
    cvReleaseImage(&temp_com);
    cvReleaseImage(&temp_last);
}
//重建开操作
void reBuildOpen(IplImage *src,IplImage *dst,IplImage *ground,IplImage *dilateSE,IplImage *erodeSE,int eroden){
    IplImage *temp=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    IplImage *temp_last=cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    cvCopy(src, temp, NULL);
    for(int i=0;i<eroden;i++){
        Erode(temp, temp, erodeSE, NULL);
    }
   
    while(!isEqual(temp, temp_last)){
        cvCopy(temp, temp_last, NULL);
        Dilate(temp, temp, dilateSE, NULL);
        And(temp, ground, temp);
        
    }
    cvCopy(temp, dst, NULL);
    cvReleaseImage(&temp);
    cvReleaseImage(&temp_last);

}

