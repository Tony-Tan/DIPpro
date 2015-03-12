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
    RGB * dst1arry=(RGB *)malloc(sizeof(RGB)*width*height);
    RGB * dst2arry=(RGB *)malloc(sizeof(RGB)*width*height);
    IplImage *dst1_r =cvCreateImage(cvSize(width, height), src->depth, 1);
    IplImage *dst1_g =cvCreateImage(cvSize(width, height), src->depth, 1);
    IplImage *dst1_b =cvCreateImage(cvSize(width, height), src->depth, 1);
    IplImage *dst1 =cvCreateImage(cvSize(width, height), src->depth, src->nChannels);
    IplImage *dst2_r =cvCreateImage(cvSize(width, height), src->depth, 1);
    IplImage *dst2_g =cvCreateImage(cvSize(width, height), src->depth, 1);
    IplImage *dst2_b =cvCreateImage(cvSize(width, height), src->depth, 1);
    IplImage *dst2 =cvCreateImage(cvSize(width, height), src->depth, src->nChannels);
    for (int j=0;j<height; j++) {
        for(int i=0;i<width;i++){
            CvScalar s = cvGet2D(src,j,i);
            srcarry[j*width+i].c1=s.val[0];
            srcarry[j*width+i].c2=s.val[1];
            srcarry[j*width+i].c3=s.val[2];
            
        }
    }
    //double* dst1arry_r=(double *)malloc(sizeof(double)*width*height);
    //double* dst1arry_g=(double *)malloc(sizeof(double)*width*height);
    //double* dst1arry_b=(double *)malloc(sizeof(double)*width*height);
    //double* dst2arry_r=(double *)malloc(sizeof(double)*width*height);
    //double* dst2arry_g=(double *)malloc(sizeof(double)*width*height);
    //double* dst2arry_b=(double *)malloc(sizeof(double)*width*height);
    //Split(srcarry, dst1arry_r, dst1arry_g, dst1arry_b, width, height);
    //Zero(dst1arry_b, width, height);
    RGB2sRGB(srcarry, dst1arry, width, height);
    sRGB2RGB(dst1arry, dst2arry, width, height);
    //Merge(dst1arry_r, dst1arry_g, dst1arry_b,dst1arry, width, height);
    //////////////////////gray/////////////////////////////////////
    /*
    int width=src->width, height=src->height;
    
    double* srcarry=(double *)malloc(sizeof(double)*width*height);
   
    double* dst1arry=(double *)malloc(sizeof(double)*width*height);
    double* dst2arry=(double *)malloc(sizeof(double)*width*height);
    for (int j=0;j<height; j++) {
        for(int i=0;i<width;i++)
            srcarry[j*width+i]=cvGetReal2D(src, j, i);
    }
     */
    ////////////////////////////////////////////////////////
    //Position p;
    //p.x=128;
    //p.y=128;
    //RegionGrow(srcarry, dst1arry, &p, 1, width, height, 30);
    //Threshold(dst1arry, dst1arry, width, height, 9, THRESHOLD_TYPE3);
    //Mask(srcarry, dst2arry, dst1arry, width, height);
    //GaussianFilter(srcarry, dst1arry, width, height, 15, 15, 2.4);
    //MedianFilter(srcarry, dst2arry, width, height, 3, 3);
    //MeanFilter(srcarry, dst2arry, width, height, 7, 7);
    //Sobel(srcarry, dst1arry, NULL, width, height,3);
    //HistogramEqualization(dst2arry, dst2arry, width, height);
    //MeanThreshold(srcarry, dst1arry, width, height, THRESHOLD_TYPE3);
    //PtileThreshold(srcarry, dst1arry, .3, width,height, THRESHOLD_TYPE3);
    //IterativeThreshold(srcarry, dst1arry,1, width, height, THRESHOLD_TYPE3);
    //ValleyBottomThreshold(srcarry, dst2arry, width, height, THRESHOLD_TYPE3);
    //OTSUThreshold(dst1arry, dst1arry, width, height, THRESHOLD_TYPE3);
    //GaussianFilter(srcarry, dst2arry, width, height, 15, 15, 2.4);
    //findMinimal(srcarry, dst2arry, width, height);
    //Canny(srcarry, dst1arry, width,height, 50, 600,THRESHOLD_TYPE3);
    //GaussianFilter(srcarry, dst1arry, width, height, 15, 15, 2.4);
    //MeyerWatershed(dst1arry, dst2arry, width, height);
    //matrixAdd(srcarry, dst2arry, dst2arry, width, height);
    //RegionGrow(srcarry, dst1arry, seed_arry, width, height, 50);
    //matrixCopy(seed_arry, dst2arry, width, height);
    //findMinimal(srcarry, dst1arry, width, height);
    //HistogramEqualization(dst1arry, dst2arry, width, height);
    //MeanThreshold(srcarry,dst1arry,width,height,THRESHOLD_TYPE1);
    //PtileThreshold(srcarry, dst2arry, 0.7, width, height, THRESHOLD_TYPE3);
    //IterativeThreshold(srcarry, dst1arry, 1, width, height, THRESHOLD_TYPE3);
    //ValleyBottomThreshold(srcarry,dst2arry,width,height,THRESHOLD_TYPE3);
    //MeanDoubleHumpThreshold(srcarry, dst1arry, width, height, THRESHOLD_TYPE3);
    //Canny(srcarry, dst1arry, width, height, 5, 30, 10);
    //LoGThreshold(srcarry, dst1arry, width, height, 0, THRESHOLD_TYPE3);
    //EdgeDetection(srcarry, dst2arry, width, height, EDGE_DETECTOR_SOBEL, 0.3, 3, 3, 0);
    //LocalThreshold(srcarry, dst1arry, width, height,3, 1,30);
    //OTSUThreshold(srcarry, dst2arry, width, height, THRESHOLD_TYPE3);
    //double max=Sobel(srcarry, dst2arry, NULL, width, height, 5);
    //Threshold(dst2arry, dst2arry, width, height, 0.267*max, THRESHOLD_TYPE3);
    //Mask(srcarry, dst2arry, dst2arry, width, height);
    //SobelThreshold(srcarry, dst1arry, width, height, 0.29, THRESHOLD_TYPE3);
    //GaussianFilter(srcarry, dst1arry, width, height, 10, 10, 1.6);
    //double max=Sobel(dst1arry, dst1arry, NULL, width, height, 5);
    //Threshold(dst1arry, dst1arry, width, height, max*0.7, THRESHOLD_TYPE3);
    //LoG(srcarry, dst2arry, width, height, 9, 9, 1.5, 10);
    //OTSUThreshold(dst1arry, dst2arry, width, height, THRESHOLD_TYPE3);
    /////////////////////////////////////////////////////////////////
    //RegionSplit(srcarry, dst1arry, width, height, 10, 50, 10, 25);
    //RegionSplit(srcarry, dst1arry, width, height, 10, 30, 0, 15);
    
    for (int j=0;j<height; j++) {
        for(int i=0;i<width;i++){
            cvSetReal2D(dst1_r, j, i,dst1arry[j*width+i].c1);
            cvSetReal2D(dst1_g, j, i,dst1arry[j*width+i].c2);
            cvSetReal2D(dst1_b, j, i,dst1arry[j*width+i].c3);
        }

    }
    
    for (int j=0;j<height; j++) {
        for(int i=0;i<width;i++){
            cvSetReal2D(dst2_r, j, i,dst2arry[j*width+i].c1);
            cvSetReal2D(dst2_g, j, i,dst2arry[j*width+i].c2);
            cvSetReal2D(dst2_b, j, i,dst2arry[j*width+i].c3);
        }
    }
    cvMerge(dst1_r, dst1_g, dst1_b, NULL, dst1);
    cvMerge(dst2_r, dst2_g, dst2_b, NULL, dst2);
    //cvSaveImage("/Users/Tony/DIPImage/xingyun2_dst.png", dst1, 0);
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





