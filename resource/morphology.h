#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H
#include "mathematic.h"


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


void Translation(double *src,double *dst,int width,int height,MoveDirection *direction);
//将小的图像弄到大的黑色图像中间，或者说是给图像加黑色边框
void Zoom(double *src,int s_width,int s_height,double *dst,int d_width,int d_height);
//逻辑与操作
void And(double *src0,double *src1,double *dst,int width,int height);
//逻辑或操作
void Or(double *src0,double *src1,double *dst,int width,int height);
//取反
void Not(double *src,double *dst,int width,int height);
//将所有元素设为255
void G_One(double *src,int width,int height);

//膨胀
void Dilate(double *src,int s_width,int s_height,double *dst,int d_width,int d_height,double *se,int se_width,int se_height,Position *center);

//腐蚀
void Erode(double *src,int s_width,int s_height,double *dst,int d_width,int d_height,double *se,int se_width,int se_height,Position *center);

//开操作
void Open(double *src,int width,int height,double *dst,double *se,int se_width,int se_height,Position *center);
//关操作
void Close(double *src,int width,int height,double *dst,double *se,int se_width,int se_height,Position *center);
//击中与击不中
void HitorMiss(double *src,int width,int height,double *se1,int se1_width,int se1_height,double *se2,int se2_width,int se2_height,double *dst,Position *se1center,Position *se2center);
//二值图像，边缘检测
void BinaryEdge(double *src,int width,int height,double* dst);
//孔洞填充
void FillHole(double *src,double *dst,int width,int height,Position *seed);
//连通分量获取
void GetConCompG_Onent(double *src,double *dst,int width,int height,Position *seed);
//骨架
void FrameWork(double *src,double *dst,int width,int height,double *se,int se_width,int se_height);

//凸壳
void Convexhull(double *src,double *dst,int width,int height);

//细化操作
void Thinning(double *src,double *dst,int width,int height);
//重建开操作
void reBuildOpen(double *src,double *dst,double *ground,int width,int height,double *dilateSE,int dse_width,int dse_height,double *erodeSE,int ese_width,int ese_height,int eroden);


#endif