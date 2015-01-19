//
//  main.c
//  Adaboost
//
//  Created by 谭升 on 14/11/28.
//  Copyright (c) 2014年 谭升. All rights reserved.
//

#include "adaboost.h"
//define data type of train data


int Data_Size_G=0;

//get data and label from input
TrainData* getTrainData(){
    
    printf("Input your datasize:");
    scanf("%d",&Data_Size_G);
    TrainData *data=(TrainData *)malloc(sizeof(TrainData)*Data_Size_G);
    if(data!=NULL)
        printf("Memory allocation succeeds\n");
    else
        printf("Memory allocation failed\n");
    printf("input property and label(positive 1 and nagitive 0):\n");
    for(int i=0;i<Data_Size_G;i++){
        scanf("%lf,%d",&data[i].property,&data[i].label);
        
    }
    return data;
}
//free memory
void freeTrainData(TrainData* data){
    free(data);
}
//showdata
void showTrainData(TrainData* data){
    printf("property1         label\n");
    for(int i=0;i<Data_Size_G;i++)
        printf("%9lf           %5d\n",data[i].property,data[i].label);

}
/*计算beta值*/
double getBeta(double erro){
    return erro/(1.0-erro);
}
/*计算alpha的值*/
double getAlpha(double beta){
    return log(1.0/beta);
}
/*
 更新权重
 */
void updataWi(TrainData *data,double beta){
    for(int i=0;i<Data_Size_G;i++){
        if(data[i].status==HIT)
            data[i].w=data[i].w*beta;
        else if(data[i].status==MISS)
            data[i].w=data[i].w;
        
    }

}
/*
 归一化权重，使所有权重之和为1
 */

void nomalization(TrainData *data){
    double sum=0.0;
    for(int i=0;i<Data_Size_G;i++){
        sum+=data[i].w;
    }
    //printf("Sum of w:%lf\n",sum);
    for(int i=0;i<Data_Size_G;i++){
        data[i].w=data[i].w/sum;
    }
}
/*
 初始化权重
 */
void InitWi(TrainData *data){
    double positive=0.0;
    for(int i=0;i<Data_Size_G;i++)
        if(data[i].label==1)
            positive++;
    for(int i=0;i<Data_Size_G;i++){
        if(data[i].label==1)
            data[i].w=1.0/(2.0*positive);
        else
            data[i].w=1.0/(2.0*(Data_Size_G-positive));
        
    }
}
/*
 初始化状态
 */
void InitStatus(TrainData *data){
    for(int i=0;i<Data_Size_G;i++)
        data[i].status=HIT;
}
/*
 计算错误值
 */
double getError(TrainData *data){
    double error=0.0;
    for(int i=0;i<Data_Size_G;i++)
        if(data[i].status==MISS)
            error+=data[i].w;
    return error;
}
void Adaboost(TrainData *data,int T){
    InitWi(data);
    double temptheta=0.0,theta1=0.0;
    double error,beta;
    int p=0;                            //p=0 <=> '<' p=0 <=> '>'
    double min;
//////////////left is positive & right is nagitive//////////////
    for(int i=0;i<T;i++){
        //get theta first
        p=0;
        min=DBL_MAX;//////Be careful
        nomalization(data);
        for(int j=0;j<Data_Size_G;j++){
            InitStatus(data);
            temptheta=data[j].property;
            for(int k=0;k<Data_Size_G;k++){
                if((data[k].property<=temptheta)&&(data[k].label==0))
                    data[k].status=MISS;
                if((data[k].property>temptheta)&&(data[k].label))
                    data[k].status=MISS;
            }
            error=getError(data);
            if(error<=min&&error<0.5){
                theta1=temptheta;
                min=error;
            
            }
        }
        
//////////////right is positive & left is nagitive//////////////
        temptheta=0.0;
        double theta2=0.0;
        for(int j=0;j<Data_Size_G;j++){
            InitStatus(data);
            temptheta=data[j].property;
            for(int k=0;k<Data_Size_G;k++){
                if((data[k].property>=temptheta)&&(data[k].label==0))
                    data[k].status=MISS;
                if((data[k].property<temptheta)&&(data[k].label))
                    data[k].status=MISS;
            }
            error=getError(data);
            if(error<=min){
                theta2=temptheta;
                min=error;
                p=1;
                
            }
        }
//////////////////////////////////////////////////////////////////////////
        InitStatus(data);
        double theta=p?theta2:theta1;
        if(p)
            for(int k=0;k<Data_Size_G;k++){
                if((data[k].property>=theta)&&(data[k].label==0))
                    data[k].status=MISS;
                if((data[k].property<theta)&&(data[k].label))
                    data[k].status=MISS;
            }
        else
            for(int k=0;k<Data_Size_G;k++){
                if((data[k].property<=theta)&&(data[k].label==0))
                    data[k].status=MISS;
                if((data[k].property>theta)&&(data[k].label))
                    data[k].status=MISS;
            }
        
        
        
        error=getError(data);
        beta=getBeta(error);
        updataWi(data, beta);
        
        if(p)
            printf("|>=|   |Threshold:%9lf|error:%9lf |Alpha:%9lf|\n",theta,error,getAlpha(beta));
        else
            printf("|<=|   |Threshold:%9lf|error:%9lf |Alpha:%9lf|\n",theta,error,getAlpha(beta));
        
        //if(error>=0.5)
          //  break;
        
    }

}

