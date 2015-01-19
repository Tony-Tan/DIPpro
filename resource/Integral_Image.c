#include "Integral_Image.h"
/*
 图像积分，即一个像素点等于其左上方全部像素值之和
 */
void Inte_Image(IplImage * src,IplImage *dst){
    if(dst->depth!=IPL_DEPTH_64F||
       src->width!=dst->width   ||
       src->height!=dst->height ||
       src->nChannels!=1        ||
       dst->nChannels!=1)
        exit(0);
    int width=src->width;
    int height=src->height;
    double value;
    for(int i=1;i<width;i++)
        for(int j=1;j<height;j++){
            value=GETPIX(src, i-1, j)+GETPIX(src, i, j-1);
            SETPIXF(dst, i, j, value);
        }
    for(int i=1;i<width;i++){
        value=GETPIX(src, i, 0)+GETPIX(src, i-1, 0);
        SETPIXF(dst, i, 0, value);
    }
    for(int i=1;i<height;i++){
        value=GETPIX(src, 0, i)+GETPIX(src, 0, i-1);
        SETPIXF(dst, 0, i, value);
    }
    value=GETPIX(src, 0, 0);
    SETPIXF(dst, 0, 0, value);

}