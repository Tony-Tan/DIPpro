#ifndef HOUGH_H
#define HOUGH_H

#include <stdio.h>
#include <stdlib.h>
#include "mathematic.h"

void HoughLine(double *src,double *dst,int width,int height,int lineLength);


void DrawLine(double *image,int width,int height,double theta,int r);


#endif