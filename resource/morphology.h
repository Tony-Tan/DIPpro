#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#define isSIZEEQU(x,y) (((x)->width)==((y)->width)&&((x)->height)==((y)->height))
typedef int DataType;
struct Position_{
    int x;
    int y;
};
typedef struct Position_ Position;
typedef struct Position_ MoveDirection;
//位移操作，将图像整体移动，如果超出边界舍去
int isEqual(IplImage *src1,IplImage *src2);
//检测图像是否为空
int isEmpty(IplImage *src);
//移动图像
void Translation(IplImage *src,IplImage *dst,MoveDirection *direction);
//将小的图像弄到大的黑色图像中间，或者说是给图像加黑色边框
void Zoom(IplImage *src,IplImage *dst);
//逻辑与操作
int And(IplImage *src0,IplImage *src1,IplImage *dst);
//逻辑或操作
void Or(IplImage *src0,IplImage *src1,IplImage *dst);
//取反
void Not(IplImage *src,IplImage *dst);
//将所有元素设为1
void One(IplImage *src);
//膨胀
void Dilate(IplImage *src,IplImage *dst,IplImage *se,Position *center);

//腐蚀
void Erode(IplImage *src,IplImage *dst,IplImage *se,Position *center);
//开操作
void Open(IplImage *src,IplImage *dst,IplImage *se,Position *center);
//关操作
void Close(IplImage *src,IplImage *dst,IplImage *se,Position *center);
//击中与击不中
void HitorMiss(IplImage *src,IplImage *se1,IplImage *se2,IplImage *dst,Position *se1center,Position *se2center);
//二值图像，边缘检测
void BinaryEdge(IplImage *src,IplImage* dst);
//孔洞填充
void FillHole(IplImage *src,IplImage *dst,IplImage *se,Position *seed);
//连通分量获取
void GetConComponent(IplImage *src,IplImage *dst,IplImage *se,Position *seed);
//骨架
void FrameWork(IplImage *src,IplImage *dst,IplImage *se);
//凸壳生成结构元
IplImage* CreateConvexhullSE(int num);
//凸壳
void Convexhull(IplImage *src,IplImage *dst);
//生成细化结构元
IplImage* CreateThinningSE(int num);
IplImage* CreateThinningUSE(int num);
//细化操作
void Thinning(IplImage *src,IplImage *dst);
//重建开操作
void reBuildOpen(IplImage *src,IplImage *dst,IplImage *ground,IplImage *dilateSE,IplImage *erodeSE,int eroden);
#endif