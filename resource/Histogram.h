#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#define TABLE_INT 0
#define TABLE_CHAR 1
#define TABLE_DOUBLE 2
#define GRAY_LEVEL 256
void HistogramEqualization(double *src,double *dst,int width,int height);
void HistogramSpecification(double *src,double *dst,int* hist,int width,int height);
#endif