#include "fr.h"
#include "Image_FFT.h"
#include "Contrast.h"
#include "convolution.h"
#define SIZE_WH 256
#include "adaboost.h"
#define HIGH_FR 1
#define LOW_FR 2
#include "lowpassfilter.h"
#include "filter.h"
#include "smooth.h"
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
    
    IplImage *src =cvLoadImage("/Users/Tony/DIPImage/lena.jpg", 0);
    IplImage *dst =cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    IplImage *cvdst =cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    //double power=FrequencyFiltering(src,dst,GHPF,20,0,0,0,0,1);
    //Complex a[512*512];
    //ImageFFT(src, a);
    //getAmplitudespectrum(a, 512, 512, dst);
    //cvSaveImage("/Users/Tony/DIPImage/testfir_ILPF_10.jpg", dst, 0);
    //MedianFilter(src, dst,13,13);
    //cvSmooth(src, cvdst, CV_MEDIAN, 13, 13, 0, 0);
    cvSmooth(src, cvdst, CV_GAUSSIAN,3,3,0,0);
    //BilateralFilter(src,dst,11,11,3,30);
    GaussianFilter(src,dst,3,3,2);
    cvNamedWindow("dst", 1);
    cvShowImage("dst", dst);
    //cvSub(cvdst,dst,cvdst,NULL);
    //HistogramEqualization(cvdst, cvdst);
    cvNamedWindow("cvdst", 1);
    cvShowImage("cvdst", cvdst);
    //HistogramEqualization(dst,dst);
    
    //printf("Power is:%lf%%\n",power*100);
    cvNamedWindow("src", 1);
    cvShowImage("src", src);
    cvWaitKey(0);
    return 0;
}





