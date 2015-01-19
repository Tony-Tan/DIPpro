#include "fr.h"
#include "Image_FFT.h"
#include "Contrast.h"
#define SIZE_WH 256
#include "adaboost.h"
#define HIGH_FR 1
#define LOW_FR 2


int main(int argc, const char * argv[]) {

    IplImage * src=cvLoadImage("/Users/Tony/DIPImage/lena.tif", 0);
    IplImage * dst_show=cvCreateImage(cvGetSize(src), 8, 1);
    IplImage * dst_test=cvCreateImage(cvGetSize(src), 8, 1);
    
    
    int width=src->width;
    int height=src->height;
    //printf("width:%d       height:%d\n",width,height);
    Complex dst[src->width*src->height];
    ImageFFT(src, dst);
    //dst[(width+1)*height/2].real=0.0;
    //dst[(width+1)*height/2].imagin=0.0;
    //printf("%d  %d\n",(width+1)*height/2,(width+1)*height/2);
  
    
    getAmplitudespectrum(dst,src->width,src->height,dst_show);
    ImageIFFT(dst, dst_test, width, height);
    cvNamedWindow("Image",1);
    cvNamedWindow("Image-FFT",1);
    cvNamedWindow("Image-IFFT",1);
    cvShowImage("Image",src);
    cvShowImage("Image-FFT", dst_show);
    cvShowImage("Image-IFFT", dst_test);
    cvSaveImage("/Users/Tony/DIPImage/lenafudu`.jpg", dst_test, 0);
    //cvSaveImage("/Users/Tony/DIPImage/lena.jpg", src, 0);
    cvWaitKey(0);
    cvReleaseImage(&src);
    cvReleaseImage(&dst_show);
    cvReleaseImage(&dst_test);
    cvDestroyWindow("Image-FFT");
    cvDestroyWindow("Image-IFFT");
    return 0;
}





