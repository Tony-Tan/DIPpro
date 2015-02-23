//
//  Segment
//  tony.sheng.tan@gmail.com
//  Created by 谭升 on 15/2/6.
//  Copyright (c) 2014年 谭升. All rights reserved.
//
#include "Segment.h"
#define TAN67_5 2.414213562373094
#define TAN22_5 0.414213562373095
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//
void getEdgeAngle(double *src_x,double *src_y,double *edgeAngle,int width,int height){
    double x;
    double y;
    for(int i=0;i<width*height;i++){
        x=src_x[i];
        y=src_y[i];
        if(!(x==0.0&&y==0.0)){
            double v=atan2(y, x)*180.0/M_PI;
            if(v<0.0)
                v+=360.0;
            edgeAngle[i]=v;
        }else
            edgeAngle[i]=-1.0;
        
    }
    
}
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

double Robert(double *src,double *dst,double *edgedriction,int width,int height){
    double RobertMask_x[9]={0,0,0,0,-1,0,0,0,1};
    double RobertMask_y[9]={0,0,0,0,0,-1,0,1,0};
    
    double *dst_x=(double *)malloc(sizeof(double)*width*height);
    double *dst_y=(double *)malloc(sizeof(double)*width*height);
    RealConvolution(src, dst_x, RobertMask_x, width, height, ROBERT_MASK_SIZE,ROBERT_MASK_SIZE);
    RealConvolution(src, dst_y, RobertMask_y, width, height, ROBERT_MASK_SIZE,ROBERT_MASK_SIZE);
    if(edgedriction!=NULL)
        getEdgeAngle(dst_x, dst_y, edgedriction, width, height);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            dst[j*width+i]=abs(dst_x[j*width+i])+abs(dst_y[j*width+i]);
            
        }
    free(dst_x);
    free(dst_y);
    return findMatrixMax(dst,width,height);
    
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

double Sobel(double *src,double *dst,double *edgedriction,int width,int height,int sobel_size){
    //double SobelMask_x[3]={-1,-2,-1,0,0,0,1,2,1};
    double *dst_x=(double *)malloc(sizeof(double)*width*height);
    double *dst_y=(double *)malloc(sizeof(double)*width*height);
    if(sobel_size==-1){
        return Scharr(src, dst, edgedriction, width, height);
    }
    else if(sobel_size==3){
        double SobelMask1[3]={1,2,1};
        double SobelMask2[3]={1,0,-1};
        RealConvolution(src, dst_y, SobelMask2, width, height, 1, 3 );
        RealConvolution(dst_y, dst_y,SobelMask1, width, height, 3, 1);
        
        RealConvolution(src, dst_x, SobelMask1, width, height, 1, 3);
        RealConvolution(dst_x, dst_x, SobelMask2, width, height, 3, 1);
    
       
    }else if(sobel_size==5){
        double SobelMask1[5]={1,4,6,4,1};
        double SobelMask2[5]={1,2,0,-2,-1};
        RealConvolution(src, dst_x, SobelMask1, width, height, 1, 5);
        RealConvolution(dst_x, dst_x, SobelMask2, width, height, 5, 1);
        
        RealConvolution(src, dst_y, SobelMask2, width, height, 1, 5);
        RealConvolution(dst_y, dst_y, SobelMask1, width, height, 5, 1);
    
    }else if(sobel_size==7){
        double SobelMask1[7]={1,6,15,20,15,6,1};
        double SobelMask2[7]={1,0.4,5,0,-5,-4,-1};
        RealConvolution(src, dst_x, SobelMask1, width, height, 1, 7);
        RealConvolution(dst_x, dst_x, SobelMask2, width, height, 7, 1);
        
        RealConvolution(src, dst_y, SobelMask2, width, height, 1, 7);
        RealConvolution(dst_y, dst_y, SobelMask1, width, height, 7, 1);
        
    }
    
    for(int i=0;i<width*height;i++)
        dst_y[i]=-dst_y[i];
    if(edgedriction!=NULL)
        //getEdgeDirection(dst_x, dst_y, edgedriction, width, height);
        getEdgeAngle(dst_x, dst_y, edgedriction, width, height);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            dst[j*width+i]=abs(dst_x[j*width+i])+abs(dst_y[j*width+i]);
        }
    free(dst_x);
    free(dst_y);
    return findMatrixMax(dst,width,height);
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

double Scharr(double *src,double *dst,double *edgedriction,int width,int height){
    double ScharrMask1[3]={3,10,3};
    double ScharrMask2[3]={1,0,-1};
    double *dst_x=(double *)malloc(sizeof(double)*width*height);
    double *dst_y=(double *)malloc(sizeof(double)*width*height);
    RealConvolution(src, dst_x, ScharrMask1, width, height, 1, 3);
    RealConvolution(dst_x, dst_x, ScharrMask2, width, height, 3, 1);
    
    RealConvolution(src, dst_y, ScharrMask2, width, height, 1, 3);
    RealConvolution(dst_y, dst_y, ScharrMask1, width, height, 3, 1);
    for(int i=0;i<width*height;i++)
        dst_y[i]=-dst_y[i];
    if(edgedriction!=NULL)
        getEdgeAngle(dst_x, dst_y, edgedriction, width, height);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            dst[j*width+i]=abs(dst_x[j*width+i])+abs(dst_y[j*width+i]);
        }
    free(dst_x);
    free(dst_y);
    //matrixMultreal(dst, dst, 1.0/16.0, width, height);
    return findMatrixMax(dst,width,height);
}


/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//拉普拉斯算子
double Laplace(double *src,double *dst,int width,int height){
    double LaplaceMask[9]={-1,-1,-1,-1,8,-1,-1,-1,-1};
    RealRelevant(src, dst, LaplaceMask, width, height, LAPLACE_MASK_SIZE, LAPLACE_MASK_SIZE);
    return findMatrixMax(dst,width,height);
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//Prewitt算子
double Prewitt(double *src,double *dst,int width,int height){
    double PrewittMask1[3]={1.0,1.0,1.0};
    double PrewittMask2[3]={-1.0,0.0,1.0};
    double *dst_x=(double *)malloc(sizeof(double)*width*height);
    double *dst_y=(double *)malloc(sizeof(double)*width*height);
    RealRelevant(src, dst_x, PrewittMask1, width, height, 1, 3);
    RealRelevant(dst_x, dst_x, PrewittMask2, width, height, 3, 1);

    RealRelevant(src, dst_y, PrewittMask2, width, height, 1, 3);
    RealRelevant(dst_y, dst_y, PrewittMask1, width, height, 3, 1);
    for(int j=0;j<height;j++)
        for(int i=0;i<width;i++){
            dst[j*width+i]=abs(dst_x[j*width+i])+abs(dst_y[j*width+i]);
        }
    free(dst_x);
    free(dst_y);
    return findMatrixMax(dst,width,height);

}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//Kirsch算子
double Kirsch(double *src,double *dst,int width,int height){
    double KirschMask[8][9]={{-3,-3,-3,
                          -3, 0, 5,
                          -3, 5, 5},
                         {-3,-3,-3,
                          -3, 0,-3,
                           5, 5, 5},
                         {-3,-3,-3,
                           5, 0,-3,
                           5, 5,-3},
                         {5,-3,-3,
                          5, 0,-3,
                          5,-3,-3},
                         {5, 5,-3,
                          5, 0,-3,
                          -3,-3,-3},
                         {5, 5, 5,
                          -3, 0,-3,
                          -3,-3,-3},
                         {-3, 5, 5,
                          -3, 0, 5,
                          -3,-3,-3},
                         {-3,-3, 5,
                          -3, 0, 5,
                          -3,-3, 5}};
    double *maxtemp=(double *)malloc(sizeof(double)*width*height);
    double *temp=(double*)malloc(sizeof(double)*width*height);
    Zero(maxtemp, width, height);
    for(int i=0;i<8;i++){
        RealRelevant(src, temp, KirschMask[i], width, height, 3, 3);   
        MaxPix(temp, maxtemp, maxtemp, width, height);
    }
    matrixCopy(maxtemp, dst, width, height);
    return findMatrixMax(dst,width,height);

}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//3x3的邻域内，如果中心像素大于阈值，切周围存在负像素值，则此点为0交叉点
void findCross(double *src,double *dst,int width,int height,double threshold){
    double *dsttemp=(double *)malloc(sizeof(double)*width*height);
    Zero(dst, width, height);
    double c_value=0.0;
    int flag=1;
    Zero(dsttemp, width, height);
    for(int j=1;j<height-1;j++){
        for(int i=1;i<width-1;i++){
            c_value=src[j*width+i];
            flag=1;
            for(int m=-1;m<=1&&flag;m++)
                for(int n=-1;n<=1;n++){
                    if(c_value>threshold&&src[(j+m)*width+i+n]<-0.1){
                        flag=0;
                        dsttemp[j*width+i]=255.0;
                        break;
                    }
                    
                }
        }
    }
    matrixCopy(dsttemp, dst,width, height);
    free(dsttemp);


}
//得到LoG模板，根据公式得出double型模板，然后调整整体位置，使模板内系数和为0
void getLoGMask(double *mask,int width,int height,double delta){
    int center_x=width/2;
    int center_y=height/2;
    double x,y;
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
            x=i-center_x;
            y=j-center_y;
            mask[j*width+i]=-((x*x+y*y-2*delta*delta)/pow(delta,4))*(exp(-(x*x+y*y)/(2*delta*delta)));
            
        }
    
    }

    double sum=0.0;
    for(int i=0;i<width*height;i++){
        sum+=mask[i];
    }
    double di=sum/(double)(width*height);
    for(int i=0;i<width*height;i++){
        mask[i]-=di;
    }

}

//LoG边缘检测完整过程，
//生成模板
//与图像卷积
//寻找0交叉
double LoG(double *src,double *dst,int width,int height,int m_width,int m_height,double delta,double threshold){
    double *dsttemp=(double *)malloc(sizeof(double)*width*height);
    double * mask=(double *)malloc(sizeof(double)*m_width*m_height);
    getLoGMask(mask, m_width,m_height,delta);
    RealConvolution(src, dsttemp, mask, width, height, m_width, m_height);
    findCross(dsttemp,dst,width,height,threshold);
    free(dsttemp);
    free(mask);
    return findMatrixMax(dst,width,height);
    
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//生成DoG模板
//使用两个高斯模板做差
void getDoGMask(double *mask,int width,int height,double delta){
    double *mask1=(double *)malloc(sizeof(double)*width*height);
    double *mask2=(double *)malloc(sizeof(double)*width*height);
    GaussianMask(mask1, width, height, delta);
    GaussianMask(mask2, width, height, delta*1.6);
    matrixMultreal(mask1, mask1, delta*2*M_PI, width, height);
    matrixMultreal(mask2, mask2, delta*3.2*M_PI, width, height);
    matrixSub( mask2,mask1, mask1, width, height);
    double sum=0.0;
    for(int i=0;i<width*height;i++){
        sum+=mask1[i];
       
    }
    double di=sum/(double)(width*height);
    for(int i=0;i<width*height;i++){
        mask1[i]-=di;
    }
    matrixCopy(mask1, mask, width, height);
    //matrixMultreal(mask, mask, 10, width, height);
    free(mask1);
    free(mask2);
    
}
//DoG完整过程
//生成DoG模板
//卷积
//寻找零交叉
double DoG(double *src,double *dst,int width,int height,int m_width,int m_height,double delta,double threshold){
    double *dsttemp=(double *)malloc(sizeof(double)*width*height);
    double * mask=(double *)malloc(sizeof(double)*m_width*m_height);
    getDoGMask(mask, m_width,m_height,delta);
    RealConvolution(src, dsttemp, mask, width, height, m_width, m_height);
    findCross(dsttemp, dst, width, height,threshold);
    free(dsttemp);
    free(mask);
    return findMatrixMax(dst,width,height);
    
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
void EdgeDetection(double *src,double *dst,int width,int height,int detector,double threshold,int m_width,int m_height,double deta){
    double maxvalue=0;
    switch (detector) {
        case EDGE_DETECTOR_ROBERT:
        {
            maxvalue=Robert(src, dst,NULL, width, height);
            Threshold(dst, dst, width, height, maxvalue*threshold, THRESHOLD_TYPE1);
            break;
        }
        case EDGE_DETECTOR_PREWITT:
        {
            maxvalue=Prewitt(src, dst, width, height);
            Threshold(dst, dst, width, height, maxvalue*threshold, THRESHOLD_TYPE1);
            break;
        }
        case EDGE_DETECTOR_SOBEL:
        {
            maxvalue=Sobel(src, dst,NULL, width, height,(int)deta);
            Threshold(dst, dst, width, height, maxvalue*threshold, THRESHOLD_TYPE1);
            break;
        }
        case EDGE_DETECTOR_KIRSCH:
        {
            maxvalue=Kirsch(src, dst, width, height);
            Threshold(dst, dst, width, height, maxvalue*threshold, THRESHOLD_TYPE1);
            break;
        }
        case EDGE_DETECTOR_LOG:
        {
            maxvalue=LoG(src, dst, width, height,m_width,m_height,deta,threshold);
            Threshold(dst, dst, width, height, maxvalue*threshold, THRESHOLD_TYPE1);
            break;
        }
        case EDGE_DETECTOR_LAPLACE:
        {
            maxvalue=Laplace(src, dst, width, height);
            Threshold(dst, dst, width, height, maxvalue*threshold, THRESHOLD_TYPE1);
            break;
        }
        default:
            break;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/*
 *   四个角度对应编号
 *   0 1 2
 *   3 * 5
 *   6 7 8
 *   edgedirction
 */
void getEdgeDirection(double *edgedirection,double *sample_direction,int width,int height){
    
    
    double angle=0.0;
    for(int i=0;i<width*height;i++){
        angle=edgedirection[i];
        if(angle<22.5||angle>=337.5)
            sample_direction[i]=5.0;
        else if(angle<67.5&&angle>=22.5)
            sample_direction[i]=2.0;
        else if(angle<112.5&&angle>=67.5)
            sample_direction[i]=1.0;
        else if(angle<157.5&&angle>=112.5)
            sample_direction[i]=0.0;
        else if(angle<202.5&&angle>=157.5)
            sample_direction[i]=3.0;
        else if(angle<247.5&&angle>=202.5)
            sample_direction[i]=6.0;
        else if(angle<292.5&&angle>=247.5)
            sample_direction[i]=7.0;
        else if(angle<337.5&&angle>=292.5)
            sample_direction[i]=8.0;
        else if(angle==-1.0)
            sample_direction[i]=-1.0;
        }
    
}

/*
 *   四个角度对应编号
 *   0 1 2
 *   3 * 5
 *   6 7 8
 *
 */
void Non_MaxSuppression(double *src,double *dst,double *dirction,int width,int height){
    double *temp=(double*)malloc(sizeof(double)*width*height);
    int dir;
    int y;
    int x;
    double value_c;
    Zero(temp, width, height);
    for(int j=1;j<height-1;j++)
        for(int i=1;i<width-1;i++){
            if(dirction[j*width+i]!=-1.0){
                dir=(int)dirction[j*width+i];
                y=dir/3-1;
                x=dir%3-1;
                value_c=src[j*width+i];
                if(value_c<=src[(j+y)*width+i+x]||value_c<src[(j-y)*width+i-x])
                    temp[j*width+i]=0.0;
                else
                    temp[j*width+i]=value_c;
            }
        }
    matrixCopy(temp, dst, width, height);
    free(temp);

}
void EdgeTrack(double *src,int width,int height,Position *seed){
    int x=seed->x;
    int y=seed->y;
    if(x>=0&&x<width&&y>=0&&y<height&&src[y*width+x]==1.0){
        src[y*width+x]=2;
        for(int j=-1;j<2;j++)
            for(int i=-1;i<2;i++){
                if(!(j==0&&i==0)){
                    Position seed_next;
                    seed_next.x=x+i;
                    seed_next.y=y+j;
                    EdgeTrack(src,width,height,&seed_next);
                }
            }
        
    }
}
void NonZeroSetOne(double *src,double *dst,int width,int height){
    for(int i=0;i<width*height;i++)
        dst[i]=src[i]!=0.0?1.0:0.0;
}
void Canny(double *src,double *dst,int width,int height,int sobel_size,double threshold1,double threshold2){
    double *temp=(double *)malloc(sizeof(double)*width*height);
    double *edge_a=(double *)malloc(sizeof(double)*width*height);//边缘幅度
    double *edge_d=(double *)malloc(sizeof(double)*width*height);//边缘方向
    double *threshold_max=(double *)malloc(sizeof(double)*width*height);
    double *threshold_min=(double *)malloc(sizeof(double)*width*height);
/*********************************************************************
 *step1:gaussian smooth
 *********************************************************************/
    double gaussianmask[25]={ 2, 4, 5, 4, 2,
                              4, 9,12, 9, 4,
                              5,12,15,12, 5,
                              4, 9,12, 9, 4,
                              2, 4, 5, 4, 2};
    RealConvolution(src, temp, gaussianmask, width, height, 5, 5);
    matrixMultreal(temp, temp, 1.0/159.0, width, height);
/*********************************************************************
 *step2:sobel
 *********************************************************************/
    if(sobel_size==3)
        Scharr(temp, edge_a, edge_d, width, height);
    else if(sobel_size==5||sobel_size==7)
        Sobel(temp, edge_a, edge_d, width, height,sobel_size);
/*********************************************************************
 *step3:Non_MaxSuppression
 *********************************************************************/
    getEdgeDirection(edge_d, edge_d, width, height);
    Non_MaxSuppression(edge_a, temp, edge_d, width, height);
 /*********************************************************************
 *step4:double threshold
 *********************************************************************/
    Threshold(temp, threshold_max, width, height, threshold1, THRESHOLD_TYPE1);
    Threshold(temp, threshold_min, width, height, threshold2, THRESHOLD_TYPE1);
    NonZeroSetOne(threshold_max,threshold_max,width,height);
    NonZeroSetOne(threshold_min,threshold_min,width,height);
    
    for(int j=0;j<height;j++){
        for(int i=0;i<width;i++){
            if(threshold_max[j*width+i]==1.0&&threshold_min[j*width+i]!=2.0){
                Position p;
                p.x=i;
                p.y=j;
                EdgeTrack(threshold_min, width, height, &p);
            }
        
        }
    }
/*********************************************************************
 *step5:result
*********************************************************************/
    Zero(dst, width, height);
    for(int i=0;i<width*height;i++)
        if(threshold_min[i]==2.0)
            dst[i]=255.0;
    free(temp);
    free(threshold_max);
    free(threshold_min);
    free(edge_d);
    free(edge_a);

}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//垂直边界或水平边界
void BoundaryDetection(double *src,double *dst,double *angle,int width,int height,double v_threshold,double a_threshold,int isVertical){
    double angle_value=0.0;
    double pix_value=0.0;
    for(int i=0;i<width*height;i++){
        pix_value=src[i];
        angle_value=angle[i];
        if(pix_value>=v_threshold){
            if(!isVertical){
                if((angle_value>90.0- a_threshold && angle_value<90+a_threshold)||
                   (angle_value>270.0-a_threshold && angle_value<270+a_threshold))
                dst[i]=255.0;
            }else{
                if((angle_value>360.0 - a_threshold || angle_value<a_threshold)||
                   (angle_value>180.0-a_threshold && angle_value<180.0+a_threshold))
                dst[i]=255.0;
            }
        }
    }
}
void getV_HBoundary(double *src,double *dst,int width,int height,double v_threshold,double a_threshold,int isVertical){
    double *dst_v=(double *)malloc(sizeof(double)*width*height);
    double *dst_a=(double *)malloc(sizeof(double)*width*height);
    double *dst_d=(double *)malloc(sizeof(double)*width*height);
    double *temp=(double *)malloc(sizeof(double)*width*height);
    Scharr(src, dst_v, dst_a, width, height);
    getEdgeDirection(dst_a, dst_d, width, height);
    Non_MaxSuppression(dst_v, temp, dst_d, width, height);
    BoundaryDetection(temp, dst, dst_a, width, height, v_threshold, a_threshold,isVertical);
    
    free(dst_a);
    free(dst_v);
    free(dst_d);
    free(temp);
}









