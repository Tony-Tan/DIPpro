#include "Homomorphicfilter.h"

static double Distance(int x,int y,int c_x,int c_y){
    
    return sqrt((x-c_x)*(x-c_x)+(y-c_y)*(y-c_y));
}



void HomorphicFilter(double *filter,int width,int height,double cut_off_frequency,double lambda_l,double lambda_h,double c){
    int center_x=width/2;
    int center_y=height/2;
    double distance;
    double distance_2;
    double cut_off_frequency_2=cut_off_frequency*cut_off_frequency;
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++){
            distance=Distance(j, i, center_x, center_y);
            distance_2=distance*distance;
            filter[i*width+j]=(lambda_h-lambda_l)*(1.0-exp(-c*distance_2/cut_off_frequency_2))+lambda_l;
            
        }





}