#ifndef WATERSHED_H
#define WATERSHED_H
#include <stdlib.h>
#include "mathematic.h"
#define INIT      0
#define MINIMAL   1
#define CATCHMENT 2
#define WATERSHED 3

typedef struct PriQueueNode_ PriQueueHead;
typedef struct NLevelPriQueueNode_ * NLevelPriQueue;
typedef struct NLevelPriQueueNode_ * NLevelPriNode;
typedef struct ExPix_ Pix_Label;
/*
 *          ___    ____________________
 *         | P |->|   NLevelPriQueue   |
 *         |---|  |--------------------|
 *         | r |->|   NLevelPriQueue   |
 *         |---|  |--------------------|
 *         | i |->|   NLevelPriQueue   |
 *         |---|  |--------------------|
 *         | Q |->|   NLevelPriQueue   |
 *         |---|  |--------------------|
 *         | u |->|   NLevelPriQueue   |
 *         |---|  |--------------------|
 *         | e |->|   NLevelPriQueue   |
 *         |---|  |--------------------|
 *         | u |->|   NLevelPriQueue   |
 *         |---|  |--------------------|
 *         | e |->|   NLevelPriQueue   |
 *         |___|  |____________________|
 */
struct GrayLevelPriQueueNode_{
    int x;
    int y;
    NLevelPriQueue * next;
};
struct PriQueueNode_{
    int nodeNum;
    NLevelPriQueue * head;
    NLevelPriQueue * tail;
};
struct ExPix_{
    int grayvalue;
    int label;
};
Pix_Label * ExImageArray(double *src,int width,int height);
void MakeNewNode(int x,int y);
void inQueue(int gray_level);
NLevelPriNode outQueue(int gray_level);
void releaseNode(NLevelPriNode node);
void releasePix_Label(Pix_Label * src);
void findMinimal(double *src,double *dst,int width,int height);
#endif