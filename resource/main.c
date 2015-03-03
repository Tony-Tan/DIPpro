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
#include "watershed.h"
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
    
    //IplImage *src =cvLoadImage("/Users/Tony/DIPImage/threshold_l3.jpg", 0);
    IplImage *src =cvLoadImage("/Users/Tony/DIPImage/watershed.png", 0);
    int width=src->width, height=src->height;
    //printf("%d,%d",width,height);
    double* srcarry=(double *)malloc(sizeof(double)*width*height);
    double* dst1arry=(double *)malloc(sizeof(double)*width*height);
    double* dst2arry=(double *)malloc(sizeof(double)*width*height);
    for (int j=0;j<height; j++) {
        for(int i=0;i<width;i++)
            srcarry[j*width+i]=cvGetReal2D(src, j, i);
    }
   

    
    findMinimal(srcarry, dst1arry, width, height);
    
    //MeanThreshold(srcarry,dst1arry,width,height,THRESHOLD_TYPE1);
    //PtileThreshold(srcarry, dst2arry, 0.7, width, height, THRESHOLD_TYPE3);
    //IterativeThreshold(srcarry, dst1arry, 1, width, height, THRESHOLD_TYPE3);
    //ValleyBottomThreshold(srcarry,dst2arry,width,height,THRESHOLD_TYPE3);
    //MeanDoubleHumpThreshold(srcarry, dst1arry, width, height, THRESHOLD_TYPE3);
    //Canny(srcarry, dst1arry, width, height, 5, 30, 10);
    //LoGThreshold(srcarry, dst1arry, width, height, 0, THRESHOLD_TYPE3);
    //EdgeDetection(srcarry, dst2arry, width, height, EDGE_DETECTOR_SOBEL, 0.3, 3, 3, 0);
    //LocalThreshold(srcarry, dst1arry, width, height, 3, 1.1 ,30);
    //SobelThreshold(srcarry, dst1arry, width, height, 0.25, THRESHOLD_TYPE3);
    //GaussianFilter(srcarry, dst1arry, width, height, 10, 10, 1.6);
    //double max=Sobel(dst1arry, dst1arry, NULL, width, height, 5);
    //Threshold(dst1arry, dst1arry, width, height, max*0.7, THRESHOLD_TYPE3);
    //LoG(srcarry, dst2arry, width, height, 9, 9, 1.5, 10);
    //OTSUThreshold(srcarry, dst2arry, width, height, THRESHOLD_TYPE3);
    /////////////////////////////////////////////////////////////////
    IplImage *dst1 =cvCreateImage(cvSize(width, height), src->depth, src->nChannels);
    
    for (int j=0;j<height; j++) {
        for(int i=0;i<width;i++)
            cvSetReal2D(dst1, j, i,dst1arry[j*width+i]);
    }
    //cvSaveImage("/Users/Tony/DIPImage/scharr_t0_490291.jpg", dst1, 0);
    IplImage *dst2 =cvCreateImage(cvSize(width, height), src->depth, src->nChannels);
    for (int j=0;j<height; j++) {
        for(int i=0;i<width;i++)
            cvSetReal2D(dst2, j, i,dst2arry[j*width+i]);
    }
    //cvSaveImage("/Users/Tony/DIPImage/hough_canny_edge3.jpg", dst1, 0);
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
    free(dst1arry);
    free(dst2arry);
    return 0;
}





