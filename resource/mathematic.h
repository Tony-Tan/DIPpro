#ifndef MATHEMATIC_H
#define MATHEMATIC_H
#ifndef M_PI
#define M_PI 3.14159265358979323846
#define W_PI 57.29577951308233
#endif
#define GRAY_LEVEL 256
#define isEVEN(x) (!(x%2))
#define GETINT(x) (((x)-((double)((int)(x)))>=0.5)?((int)(x)+1):((int)(x)))
#include <math.h>
#include <stdlib.h> 
#include <limits.h>
typedef struct Position_DBL_ Position_DBL;
typedef struct Position_ Position;
typedef struct Position_ MoveDirection;
typedef int DataType;
struct Position_{
    int x;
    int y;
};
struct Position_DBL_{
    double x;
    double y;
};

////////////////////////////////////////////////////////////////////
//复数数据结构
///////////////////////////////////////////////////////////////////
struct Complex_{
    double real;
    double imagin;
};
typedef struct Complex_ Complex;
////////////////////////////////////////////////////////////////////

int isBase2(int size_n);
////////////////////////////////////////////////////////////////////
void Add_Complex(Complex * src1,Complex *src2,Complex *dst);
void Sub_Complex(Complex * src1,Complex *src2,Complex *dst);
void Multy_Complex(Complex * src1,Complex *src2,Complex *dst);
void Copy_Complex(Complex * src,Complex *dst);
void Show_Complex(Complex * src,int size_n);

double Distance(double x,double y,double c_x,double c_y);
void matrixAdd(double *src1,double *src2,double *dst,int width,int height);
void matrixSub(double *src1,double *src2,double *dst,int width,int height);
void matrixCopy(double *src,double *dst,int width,int height);
int matrixCopyLocal(double *src,double *dst,int width,int height,int w_width,int w_height,Position *lefttoppoint);
void matrixMultreal(double *src,double *dst,double k,int width,int height);
void matrixMul_matrix(double *src1,double *src2,double *dst,int width,int height);
void matrixDBL2INT(double *src,int *dst,int width,int height);
double matrixMean(double *mat,int width,int height);




double findMatrixMax(double *src,int width,int height);
double findMatrixMin(double *src,int width,int height);
//找出两幅等大图像中同一位置中相对较大的像素值
void MaxPix(double *src1 ,double *src2,double *dst,int width,int height);
//找出两幅等大图像中同一位置中相对较小的像素值
void MinPix(double *src1 ,double *src2,double *dst,int width,int height);

//将图像全部设置为1
//将图像全部设置为1
void One(double *src,int width,int height);
void Zero(double *src,int width,int height);
void Mask(double *src,double *dst,double *mask,int width,int height);

//一般差分，返回幅度值和方向矩阵
void matrixOrdinaryDiff(double *src,double *range,double* angle,int width,int height);
#endif