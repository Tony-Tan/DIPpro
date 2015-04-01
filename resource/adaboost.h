#ifndef ADABOOST_H
#define ADABOOST_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#define HIT 1
#define MISS 0
//define data type of train data
typedef struct TrainData_  TrainData;
struct TrainData_{
    double property;
    double w;
    char status;
    int label;
    
};

//int Data_Size_G=0;


TrainData * getTrainData();//get data and label from input
void freeTrainData(TrainData* data);//free memory

void showTrainData(TrainData* data);//showdata//
double getBeta(double erro);
double getAlpha(double beta);
void updataWi(TrainData *data,double beta);

void nomalization(TrainData *data);
void InitWi(TrainData *data);
void InitStatus(TrainData *data);
double getError(TrainData *data);
void Adaboost(TrainData *data,int T);





#endif