#include "Image_FFT.h"
#include "Contrast.h"
#include "convolution.h"
#include "lowpassfilter.h"
#include "morphology.h"
#include "filter.h"
#include "smooth.h"
#include "sharpen.h"
#include "Histogram.h"
#include "Segment.h"
#include "Hough.h"
#include "adaboost.h"
#include "Threshold.h"
#include "regionSegment.h"
#include "watershed.h"
#include "color.h"
#include "Pseudo_Color.h"
#define HIGH_FR 1
#define LOW_FR 2
#define SIZE_WH 256

void showfilter(double *filter,int width,int height){
    IplImage *show=cvCreateImage(cvSize(width, height),8,1);
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++){
            cvSetReal2D(show, j, i, filter[i*width+j]*255.0);
        }
    cvNamedWindow("Filter", 1);
    cvShowImage("Filter", show);
    cvWaitKey(0);
    cvReleaseImage(&show);

}

int main(int argc, const char * argv[]) {
    
    
    IplImage *src =cvLoadImage("/Users/Tony/DIPImage/lena.png", 1);
    
    int width=src->width, height=src->height;
    RGB * srcarry=(RGB *)malloc(sizeof(RGB)*width*height);
    double * srcarry_dbl=(double *)malloc(sizeof(double)*width*height);
    RGB * dst1arry=(RGB *)malloc(sizeof(RGB)*width*height);
    RGB * dst2arry=(RGB *)malloc(sizeof(RGB)*width*height);
    double * dst3arry=(double *)malloc(sizeof(double)*width*height);
    double * dst4arry=(double *)malloc(sizeof(double)*width*height);
    double * dst5arry=(double *)malloc(sizeof(double)*width*height);
    IplImage *dst1_r =cvCreateImage(cvSize(width, height), src->depth, 1);
    IplImage *dst1_g =cvCreateImage(cvSize(width, height), src->depth, 1);
    IplImage *dst1_b =cvCreateImage(cvSize(width, height), src->depth, 1);
    IplImage *dst1   =cvCreateImage(cvSize(width, height), src->depth, 3);
    IplImage *dst2_r =cvCreateImage(cvSize(width, height), src->depth, 1);
    IplImage *dst2_g =cvCreateImage(cvSize(width, height), src->depth, 1);
    IplImage *dst2_b =cvCreateImage(cvSize(width, height), src->depth, 1);
    IplImage *dst2 =cvCreateImage(cvSize(width, height), src->depth, 3);
    IplImage *dst3 =cvCreateImage(cvSize(width, height), src->depth, 1);
    IplImage *dst4 =cvCreateImage(cvSize(width, height), src->depth, 1);
    IplImage *dst5 =cvCreateImage(cvSize(width, height), src->depth, 1);
    if(src->nChannels==3){
        for (int j=0;j<height; j++) {
            for(int i=0;i<width;i++){
                CvScalar s = cvGet2D(src,j,i);
                srcarry[j*width+i].c1=s.val[2];
                srcarry[j*width+i].c2=s.val[1];
                srcarry[j*width+i].c3=s.val[0];
            
            }
        }
    }else {
        for (int j=0;j<height; j++) {
            for(int i=0;i<width;i++){
                srcarry_dbl[j*width+i]=cvGetReal2D(src,j,i);
             
            }
        }
    }
    //double* dst1arry_r=(double *)malloc(sizeof(double)*width*height);
    //double* dst1arry_g=(double *)malloc(sizeof(double)*width*height);
    //double* dst1arry_b=(double *)malloc(sizeof(double)*width*height);
    //double* dst2arry_r=(double *)malloc(sizeof(double)*width*height);
    //double* dst2arry_g=(double *)malloc(sizeof(double)*width*height);
    //double* dst2arry_b=(double *)malloc(sizeof(double)*width*height);
    
    //Zero(dst1arry_b, width, height);
    //RGB2sRGB(srcarry, dst1arry, width, height);
    //sRGB2RGB(dst1arry, dst2arry, width, height);
    //RGB2YIQ(srcarry , dst1arry, width, height);
    //Split(dst1arry, dst3arry, dst4arry, dst5arry, width, height);
    //Zero(dst4arry, width, height);
    //Gray2Color(srcarry_dbl, dst1arry, width, height,1);
    RGB2HSV(srcarry, dst2arry, width, height);
    Complementary_Color(dst2arry, dst2arry, width, height, COLOR_SPACE_HSV);
    HSV2RGB(dst2arry, dst1arry, width, height);
    for (int j=0;j<height; j++) {
        for(int i=0;i<width;i++){
            cvSetReal2D(dst1_r, j, i,dst1arry[j*width+i].c3);
            cvSetReal2D(dst1_g, j, i,dst1arry[j*width+i].c2);
            cvSetReal2D(dst1_b, j, i,dst1arry[j*width+i].c1);
            
            
            
            cvSetReal2D(dst2_r, j, i,dst2arry[j*width+i].c3);
            cvSetReal2D(dst2_g, j, i,dst2arry[j*width+i].c2);
            cvSetReal2D(dst2_b, j, i,dst2arry[j*width+i].c1);
            
            
            cvSetReal2D(dst3, j, i,dst3arry[j*width+i]);
            cvSetReal2D(dst4, j, i,dst4arry[j*width+i]);
            cvSetReal2D(dst5, j, i,dst5arry[j*width+i]);
        }

    }
    
 
    cvMerge(dst1_r, dst1_g, dst1_b, NULL, dst1);
    cvMerge(dst2_r, dst2_g, dst2_b, NULL, dst2);
    cvSaveImage("/Users/Tony/DIPImage/lena_complementary_dst_HSV.png", dst1, 0);
    //cvSaveImage("/Users/Tony/DIPImage/water_min.png", dst2, 0);
    //cvSaveImage("/Users/Tony/DIPImage/hough_edge3.jpg", dst2, 0);
    //printf("%lf",M_PI_2);
    //cvCanny(src, dst2, 200, 150,3);
    //Thinning(dst2, dst2);
    cvNamedWindow("src", 1);
    cvShowImage("src", src);
    cvNamedWindow("dst1", 1);
    cvShowImage("dst1", dst1);
    cvNamedWindow("dst2", 1);
    cvShowImage("dst2", dst2);
    cvNamedWindow("dst3", 1);
    cvShowImage("dst3", dst3);
    cvNamedWindow("dst4", 1);
    cvShowImage("dst4", dst4);
    cvNamedWindow("dst5", 1);
    cvShowImage("dst5", dst5);
    //cvSaveImage("/Users/Tony/DIPImage/log_cross_0.225.jpg",dst2, 0);
    //cvSaveImage("/Users/Tony/DIPImage/sample_sobel_edge.jpg",dst2, 0);
    //cvSub(dst2,dst1,dst2,NULL);

    //cvSub( dstcv,dst, dstcv, NULL);
    //HistogramEqualization(dstcv,dstcv);
    //cvNamedWindow("dstsub", 1);
    //cvShowImage("dstsub", dstcv);
    
    //HistogramEqualization(cvdst, cvdst);
    //cvNamedWindow("dst2", 1);
    //cvShowImage("dst2", dst2);
    //HistogramEqualization(dst,dst);
    //printf("work done\n");
    
    cvWaitKey(0);
    free(srcarry);

    return 0;
}





