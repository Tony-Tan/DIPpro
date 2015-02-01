#ifndef CONTRAST_H
#define CONTRAST_H
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include "fr.h"

#define CONTRASTFUNC0 -1	//翻转
#define CONTRASTFUNC1 0		//分段
#define CONTRASTFUNC2 1		//对数
#define CONTRASTFUNC3 2		//反对数
#define CONTRASTFUNC4 3		//n次幂
#define CONTRASTFUNC5 4		//n次根
#define CONTRASTGAMA  5     //gama
#define GRAYLEVEL 8
#define MAX_PVALUE (1<<GRAYLEVEL)
#define TABLE_INT 0
#define TABLE_CHAR 1

#include "Contrast.h"


unsigned char ContrastTable[MAX_PVALUE];//映射表

void ContrastStretch(IplImage *src,IplImage *dst,int method,double p0,double p1,int p2,int p3);
void InitMappingTable(void * arry,int size,int Data_type);
void HistogramEqualization(double *src,double *dst,int width,int height);




#endif