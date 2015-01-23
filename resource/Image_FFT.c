#include "Image_FFT.h"

/*
 中心化，根据傅里叶性质的平移性质
 */
void FFT_Shift(double * src,int size_w,int size_h){
    for(int j=0;j<size_h;j++)
        for(int i=0;i<size_w;i++){
            if((i+j)%2)
                src[j*size_w+i]=-src[j*size_w+i];
        
    }
    
}
/*
 图像快速傅里叶变换，图像大小为2的N次幂
 */
void ImageFFT(IplImage * src,Complex * dst){
    //FFT_Shift(src, src);
    if(src->depth!=IPL_DEPTH_8U)
        exit(0);
    int width=src->width;
    int height=src->height;
    double *image_data=(double*)malloc(sizeof(double)*width*height);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            image_data[j*width+i]=GETPIX(src, j, i);
        
    }
    FFT_Shift(image_data,width, height);//图像中心化
    FFT2D(image_data, dst, width, height);
    free(image_data);
    
}
/*
 将幅度值伸缩到0到255，用于频谱显示
 */
void Nomalsize(double *src,double *dst,int size_w,int size_h){
    double  max=0.0,min=DBL_MAX;
    for(int i=0;i<size_w*size_h;i++){
        max=src[i]>max?src[i]:max;
        min=src[i]<min?src[i]:min;
    }
    double step=255.0/(max-min);
    //printf("%d",test);
    //printf("max:%lf   min:%lf\n",max,min);
    for(int i=0;i<size_w*size_h;i++){
        dst[i]=(src[i]-min)*step;
        dst[i]*=45.9*log((double)(1+dst[i]));
    }
    
}
/*
 获得频谱
 */
void getAmplitudespectrum(Complex * src,int size_w,int size_h,IplImage *dst){
    double *despe=(double *)malloc(sizeof(double)*size_w*size_h);
    if(despe==NULL)
        exit(0);
    double real=0.0;
    double imagin=0.0;
    
    for(int j=0;j<size_h;j++)
        for(int i=0;i<size_w;i++){
            real=src[j*size_w+i].real;
            imagin=src[j*size_w+i].imagin;
            despe[j*size_w+i]=sqrt(real*real+imagin*imagin);
            
        }
    Nomalsize(despe, despe, size_w, size_h);
    for(int j=0;j<size_h;j++)
        for(int i=0;i<size_w;i++){
            cvSetReal2D(dst, j, i, despe[j*size_w+i]);
            
        }
    free(despe);
    
}
/*
 图像傅里叶反变换
 */
void ImageIFFT(Complex *src,IplImage *dst,int size_w,int size_h){
    Complex *temp_c=(Complex*)malloc(sizeof(Complex)*size_w*size_h);
    if(temp_c==NULL)
        exit(0);
    for(int i=0;i<size_w*size_h;i++)
        Copy_Complex(&src[i],&temp_c[i]);
    Complex *temp=(Complex*)malloc(sizeof(Complex)*size_w*size_h);
    if(temp==NULL)
        exit(0);
    double *temp_d=(double *)malloc(sizeof(double)*size_w*size_h);
    if(temp_d==NULL)
        exit(0);
    IFFT2D(temp_c,temp,size_w,size_h);
    for(int j=0;j<size_h;j++)
        for(int i=0;i<size_w;i++){
            temp_d[j*size_w+i]=temp[j*size_w+i].real;
        }
    FFT_Shift(temp_d, size_w, size_h);
    for(int j=0;j<size_h;j++)
        for(int i=0;i<size_w;i++){
            cvSetReal2D(dst, j, i, temp_d[j*size_w+i]);
        }
    free(temp);
    free(temp_c);
    free(temp_d);

}

