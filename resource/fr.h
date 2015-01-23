#ifndef FR_H
#define FR_H

#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#define TABLE_INT 0
#define TABLE_CHAR 1
#define ISEVEN(x) ((x)%2==0)
#define GETPIX(image,x,y) ((unsigned char) (image->imageData)[(x)*image->width+(y)])
#define SETPIX(image,x,y,value) (((image->imageData)[(x)*image->width+y])=((unsigned char)value))
#define GETPIXF(image,x,y) ((double) ((double *)(image->imageData))[(x)*image->width+(y)])
#define SETPIXF(image,x,y,value) ((((double *)(image->imageData))[(x)*image->width+y])=((double)value))

#define ILPF 1
#define BLPF 2
#define GLPF 3
#define IHPF 4
#define BHPF 5
#define GHPF 6
#define HOMPF 7
#define HFEPF 8


#endif