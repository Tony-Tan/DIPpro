#ifndef GRAYMORPHOLOGY_H
#define GRAYMORPHOLOGY_H
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#define TOFINDMAX 0
#define TOFINDMIN 1
#define isSIZEEQU(x,y) (((x)->width)==((y)->width)&&((x)->height)==((y)->height))
struct position{
    int x;
    int y;
};
typedef struct position Position;
//判断结构元是否平滑
int isSmooth(IplImage *src);
//判断两幅图像是否相等
int isEqu(IplImage *src1,IplImage *src2);
//将图像全部设置为1
void G_One(IplImage *src);
//位移，如果非平滑SE将加上sevalue，即对应的灰度值
void G_Translation(IplImage *src ,IplImage *dst,double SEvalue,Position *d,int istoFindMin);
//找出两幅等大图像中同一位置中相对较大的像素值
void MaxPix(IplImage *src1 ,IplImage *src2,IplImage *dst);
//找出两幅等大图像中同一位置中相对较小的像素值
void MinPix(IplImage *src1 ,IplImage *src2,IplImage *dst);
//灰度图像膨胀
void Dilate_Gray(IplImage *src,IplImage *dst,IplImage *se,Position *center);
//灰度图像腐蚀
void Erode_Gray(IplImage *src,IplImage *dst,IplImage *se,Position *center);
//开操作
void Open_Gray(IplImage *src,IplImage *dst,IplImage *se,Position *center);
//闭操作
void Close_Gray(IplImage *src,IplImage *dst,IplImage *se,Position *center);
//灰度梯度形态学提取
void Gard_Gray(IplImage *src,IplImage *dst,IplImage *se,Position *center);
//顶帽操作
void TopHat(IplImage *src,IplImage *dst,IplImage *se,Position *center);
//底帽操作
void BottomHat(IplImage *src,IplImage *dst,IplImage *se,Position *center);
//测地腐蚀
void Erode_Gray_g(IplImage *src,IplImage *ground,IplImage *dst,IplImage *se,Position *center);
//测地膨胀
void Dilate_Gray_g(IplImage *src,IplImage *ground,IplImage *dst,IplImage *se,Position *center);
//重建开操作
void Rebuild_Open(IplImage *src,IplImage *dst,IplImage *ground,IplImage *erodeSE,IplImage *dilateSE,int eroden);
//重建闭操作，这段没测试
void Rebuild_Close(IplImage *src,IplImage *dst,IplImage *ground,IplImage *dilateSE,IplImage *erodeSE,int dilaten);
//重建顶帽操作
void Rebuild_Tophat(IplImage *src,IplImage *dst,IplImage *ground,IplImage *dilateSE,IplImage *erodeSE,int eroden);
//重建底帽操作
void Rebuild_Bottomhat(IplImage *src,IplImage *dst,IplImage *ground,IplImage *dilateSE,IplImage *erodeSE,int dilaten);

#endif
