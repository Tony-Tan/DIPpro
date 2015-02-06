//
//  lowpassfilter
//  低频滤波
//  Created by 谭升 on 14/12/23.
//  tony.sheng.tan@gmail.com
//  Copyright (c) 2014年 谭升. All rights reserved.
//

#include "lowpassfilter.h"



void IdealLPFilter(double *Filter,int width,int height,double cut_off_frequency){
    int center_x=width/2;
    int center_y=height/2;
    double distance=0.0;
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++){
            distance=Distance(i,j,center_x,center_y);
            if(distance<=cut_off_frequency)
                Filter[j*width+i]=1.0;
            else
                Filter[j*width+i]=0.0;
        }

}
void ButterworthLPFilter(double *Filter,int width,int height,double cut_off_frequency,int n){
    int center_x=width/2;
    int center_y=height/2;
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++){
            double value=1.0;
            for(int k=0;k<n;k++)
                value*=(Distance(i, j, center_x, center_y)/cut_off_frequency);
            Filter[j*width+i]=1/(1+value);
        }
}
void GaussianLPFilter(double *Filter,int width,int height,double cut_off_frequency){
    int center_x=width/2;
    int center_y=height/2;
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++){
            double value=Distance(i, j, center_x, center_y);
            Filter[j*width+i]=exp(-value*value/(2*cut_off_frequency*cut_off_frequency));
        }
}