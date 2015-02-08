#include "filter.h"

double getPower(Complex *src,int totalsize){
    double power=0.0;
    double real;
    double imagin;
    for(int i=0;i<totalsize;i++){
        real=src[i].real;
        imagin=src[i].imagin;
        power+=real*real+imagin*imagin;
    }
    return power;
}
void MatrixMulti_R_C(double *src1,Complex *src2,Complex *dst,int size){//m(1,1)=a(1,1)*b(1,1);
    for(int i=0;i<size;i++){
        dst[i].real=src2[i].real*src1[i];
        dst[i].imagin=src2[i].imagin*src1[i];
    }
}
int ChangtoPower2(int size){
    size--;//避免为2的幂的size会被扩大
    int i=0;
    while ((size/=2)>0) {
        i++;
    }
    return 2<<i;
    
}
//将图像伸缩到2的幂次大小，并填充
void ResizeMatrix4FFT(IplImage *src,IplImage **dst){
    int width=src->width;
    int height=src->height;
    int re_width=ChangtoPower2(width);
    int re_height=ChangtoPower2(height);
    IplImage *temp=cvCreateImage(cvSize(re_width, re_height), src->depth, src->nChannels);
    cvResize(src, temp, 0);
    *dst=cvCreateImage(cvSize(re_width*2, re_height*2), src->depth, src->nChannels);
    cvZero(*dst);
    for(int i=0;i<re_width;i++)
        for(int j=0;j<re_height;j++)
            cvSetReal2D(*dst, j, i, cvGetReal2D(temp, j, i));
    cvReleaseImage(&temp);
}
//将扩充后的图像还原为左上角的
void CutImage421(IplImage *src,IplImage *dst){
    //IplImage *temp=cvCreateImage(cvSize(src->width/2, src->height/2), src->depth, src->nChannels);
    int width=dst->width;
    int height=dst->height;
    
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++)
            cvSetReal2D(dst, j, i, cvGetReal2D(src, j, i));
}
//频域滤波
double FrequencyFiltering(IplImage *src,IplImage *dst,int filter_type,double param1,int param2,double param3,double param4,double param5,int isgetPower){
    IplImage *temp=NULL;
    double power=0.0;
    //调整至2的幂，并用黑色填充，防止周期缠绕
    ResizeMatrix4FFT(src, &temp);
    int fft_width=temp->width;
    int fft_height=temp->height;
    //产生滤波器
    double *filter=(double *)malloc(sizeof(double)*fft_height*fft_width);
    if(filter==NULL){
        printf("frequency filter malloc faile");
        exit(0);
    }
    //生成滤波器
    switch(filter_type){
        case ILPF:
            IdealLPFilter(filter, fft_width, fft_height, param1);
            break;
        case BLPF:
            if(param2<0)
                param2=2;
            ButterworthLPFilter(filter, fft_width, fft_height, param1, param2);
            break;
        case GLPF:
            GaussianLPFilter(filter, fft_width, fft_height, param1);
            break;
        case IHPF:
            IdealHPFilter(filter, fft_width, fft_height, param1);
            break;
        case BHPF:
            if(param2<0)
                param2=2;
            ButterworthHPFilter(filter, fft_width, fft_height, param1, param2);
            break;
        case GHPF:
            GaussianHPFilter(filter, fft_width, fft_height, param1);
            break;
        case HOMPF :
            HomorphicFilter(filter, fft_width, fft_height,param1 , param2/*lamda_l*/, param3/*lamda_h*/,param4/*c*/);
            break;
        case HFEPF:
            HighFrequencyEmphasisFilter(filter, fft_width, fft_height, param4, param1, param2, param3,param5);
            break;

    }
    //showfilter(filter,fft_width,fft_height);
    //FFT
    Complex *temp_complex=(Complex*)malloc(sizeof(Complex)*fft_height*fft_width);//fft结果
    if(temp_complex==NULL){
        exit(0);
    }
    ImageFFT(temp, temp_complex);
    if(isgetPower)
        power=getPower(temp_complex,fft_width*fft_height);
    //相乘
    MatrixMulti_R_C(filter,temp_complex,temp_complex,fft_width*fft_height);
    
    //IFFT
    ImageIFFT(temp_complex, temp, temp->width, temp->height);
   
    //还原图像
    IplImage *result2=cvCreateImage(cvSize(temp->width/2, temp->height/2), temp->depth, temp->nChannels);
    CutImage421(temp, result2);
    cvResize(result2, dst, 0);
    free(filter);
    free(temp_complex);
    cvReleaseImage(&temp);
    cvReleaseImage(&result2);
    if(isgetPower)
        power=getPower(temp_complex,fft_width*fft_height)/power;
    return power;

}
