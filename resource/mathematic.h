#ifndef MATHEMATIC_H
#define MATHEMATIC_H
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <math.h>

typedef int DataType;
struct Position_{
    int x;
    int y;
};
typedef struct Position_ Position;
typedef struct Position_ MoveDirection;

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

double Distance(int x,int y,int c_x,int c_y);
void matrixAdd(double *src1,double *src2,double *dst,int width,int height);
void matrixSub(double *src1,double *src2,double *dst,int width,int height);
void matrixCopy(double *src,double *dst,int width,int height);
void matrixMultreal(double *src,double *dst,double k,int width,int height);
double findMatrixMax(double *src,int width,int height);

//找出两幅等大图像中同一位置中相对较大的像素值
void MaxPix(double *src1 ,double *src2,double *dst,int width,int height);
//找出两幅等大图像中同一位置中相对较小的像素值
void MinPix(double *src1 ,double *src2,double *dst,int width,int height);

//将图像全部设置为1
//将图像全部设置为1
void One(double *src,int width,int height);
void Zero(double *src,int width,int height);

#endif