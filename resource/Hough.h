#ifndef HOUGH_H
#define HOUGH_H

#include <stdio.h>
#include <stdlib.h>
#include "mathematic.h"

void HoughLine(double *src,double *dst,int width,int height,int lineLength);
void HoughCircle(double *src,double *dst,int width,int height);
void HoughRectangle(double *src,double *dst,int width,int height);
void HoughSquare(double *src,double *dst,int width,int height);

void DrawLine(double *image,int width,int height,double theta,int r);


#endif