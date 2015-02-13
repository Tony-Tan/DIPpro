
#include "Image_FFT.h"
#include "Contrast.h"
#include "convolution.h"
#define SIZE_WH 256
#include "adaboost.h"
#define HIGH_FR 1
#define LOW_FR 2
#include "lowpassfilter.h"
#include "morphology.h"
#include "filter.h"
#include "smooth.h"
#include "sharpen.h"
#include "Histogram.h"
#include "Segment.h"
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
    
    IplImage *src =cvLoadImage("/Users/Tony/DIPImage/edge_b.jpg", 0);
    
    int width=src->width, height=src->height;
    //printf("%d,%d",width,height);
    double* srcarry=(double *)malloc(sizeof(double)*src->width*src->height);
    double* dst1arry=(double *)malloc(sizeof(double)*src->width*src->height);
    double* dst2arry=(double *)malloc(sizeof(double)*src->width*src->height);
    for (int j=0;j<height; j++) {
        for(int i=0;i<width;i++)
            srcarry[j*width+i]=cvGetReal2D(src, j, i);
    }
    

    //Canny(srcarry, dst1arry, src->width, src->height, 1.3, 200,60);
    //cvCanny(src, dst2, 200, 60, 3);
    //Sobel(srcarry, dst2arry, NULL, src->width, src->height);
    //double power=FrequencyFiltering(src,dst,GHPF,20,0,0,0,0,1);
    //Complex a[512*512];
    //ImageFFT(src, a);
    //getAmplitudespectrum(a, 512, 512, dst);
    //cvSaveImage("/Users/Tony/DIPImage/testfir_ILPF_10.jpg", dst, 0);
    //MedianFilter(srcarry,dstarry,src->width,src->height,7,7);
    //GaussianFilter(srcarry,dstarry,src->width,src->height, 3, 3, 1);
    //BilateralFilter(srcarry, dstarry, src->width, src->height, 15, 15, 5, 75);
    //cvSmooth(src, dstcv, CV_BILATERAL, 15, 15, 75,5);
    //MeanFilter(src, dst,  7, 7);
    //NoLinearMeanFilter(src, dst, 3, 3, NLMF_MASK_G, NLMF_FUN_G, 1.0);
    //RobustSmoothFilter(src,dst,3,3);
    
    //cvSmooth(src, cvdst, CV_MEDIAN, 13, 13, 0, 0);
    //cvSmooth(src, cvdst, CV_BLUR,15,15,0,0);
    //UnsharpMasking(srcarry,dstarry,src->width,src->height,SMOOTH_GAUSSIAN,7,7,2.0,2.0);
    //SobelSharpen(srcarry,dstarry,src->width,src->height,1);
    //Sobel(srcarry, dstarry, src->width, src->height);
    //LaplaceSharpen(srcarry, dstarry, src->width, src->height,0,0.5);
    //GaussianFilter(srcarry,dst1arry,src->width, src->height,3,3,1);
    //MeanFilter(src, dst,  5, 5);
    //Laplace(srcarry,dstarry,src->width,src->height,3);
    ////////////////////////////////////////////////////////
    //int hist[GRAY_LEVEL];
    //for(int i=1;i<GRAY_LEVEL;i++){
    //    hist[i]=exp(-((double)(i)*(i))/(12800))*10000.0;
    //    printf("%d ",hist[i]);
    //}
    //HistogramEqualization(srcarry,dst1arry,src->width,src->height);
    //HistogramSpecification(srcarry,dst2arry,hist,src->width,src->height);
    //SobelSharpen(srcarry, dst1arry, src->width, src->height, 1);
    //GaussianFilter(srcarry, srcarry, src->width, src->height, 3, 3, 0.5);
    //Sobel(srcarry, dst1arry, src->width,src->height);
    //RobertSharpen(srcarry, dst1arry, src->width, src->height, 1);
    //Robert(srcarry, dst1arry, width,height);
    //EdgeDetection(srcarry ,dst1arry,width,height,EDGE_DETECTOR_SOBEL,0.490291,0,0,-1);
    
    LoG(srcarry,dst1arry,width,height,11,11,1.5,0);
    //Scharr(srcarry, dst1arry, NULL, width, height);
    //Canny(srcarry, dst2arry,width,height,3,30,10);
    //Sobel(srcarry, dst2arry, NULL,width,height , 3);
    //Canny(srcarry, dst1arry, width, height,3,200, 150);
    //Prewitt(srcarry, dst1arry, width, height);
    //Scharr(srcarry, dst2arry, NULL, width, height);
    //getV_HBoundary(srcarry,dst2arry,width,height,10,20,1);
    //DoG(srcarry,dst2arry,width,height,5,5,0.7,10);
    //HistogramEqualization(dst2arry, dst2arry, width, height);
    IplImage *dst1 =cvCreateImage(cvSize(width, height), src->depth, src->nChannels);
    
    for (int j=0;j<height; j++) {
        for(int i=0;i<width;i++)
            cvSetReal2D(dst1, j, i,dst1arry[j*src->width+i]);
    }
    //cvSaveImage("/Users/Tony/DIPImage/scharr_t0_490291.jpg", dst1, 0);
    IplImage *dst2 =cvCreateImage(cvSize(width, height), src->depth, src->nChannels);
    for (int j=0;j<height; j++) {
        for(int i=0;i<width;i++)
            cvSetReal2D(dst2, j, i,dst2arry[j*src->width+i]);
    }
    cvSaveImage("/Users/Tony/DIPImage/log_1111150.jpg", dst1, 0);
    
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





