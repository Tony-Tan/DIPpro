//
//  highpassfilter
//  高频滤波
//  Created by 谭升 on 14/12/23.
//  tony.sheng.tan@gmail.com
//  Copyright (c) 2014年 谭升. All rights reserved.
//

#include "highpassfilter.h"


void IdealHPFilter(double *Filter,int width,int height,double cut_off_frequency){
    int center_x=width/2;
    int center_y=height/2;
    double distance=0.0;
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++){
            distance=Distance(i,j,center_x,center_y);
            if(distance<=cut_off_frequency)
                Filter[j*width+i]=0.0;
            else
                Filter[j*width+i]=1.0;
        }
    Filter[width*(height+1)/2]+=1.0;
}
void ButterworthHPFilter(double *Filter,int width,int height,double cut_off_frequency,int n){
    int center_x=width/2;
    int center_y=height/2;
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++){
            double value=1.0;
            for(int k=0;k<n;k++)
                value*=(Distance(i, j, center_x, center_y)/cut_off_frequency);
            Filter[j*width+i]=1.0-1.0/(1.0+value);
        }
    Filter[width*(height+1)/2]+=1.0;
}
void GaussianHPFilter(double *Filter,int width,int height,double cut_off_frequency){
    int center_x=width/2;
    int center_y=height/2;
    for(int i=0;i<width;i++)
        for(int j=0;j<height;j++){
            double value=Distance(i, j, center_x, center_y);
            Filter[j*width+i]=1.0-exp(-value*value/(2*cut_off_frequency*cut_off_frequency));
        }
    Filter[width*(height+1)/2]+=1.0;
}
void HighFrequencyEmphasisFilter(double *Filter,int width,int height,int filter_type,double cut_off_frequency,double k1,double k2,double param1){
    switch(filter_type){
        case IHPF:
            IdealHPFilter(Filter, width, height, cut_off_frequency);
        break;
        case BHPF:
            ButterworthHPFilter(Filter, width, height, cut_off_frequency,param1);
        break;
        case GHPF:
            GaussianHPFilter(Filter, width, height, cut_off_frequency);
        break;
    
    
    
    }

    for(int i=0;i<width*height;i++)
        Filter[i]=k1+k2*Filter[i];

}