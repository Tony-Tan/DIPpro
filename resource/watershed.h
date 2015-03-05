#ifndef WATERSHED_H
#define WATERSHED_H
#include <stdlib.h>
#include "mathematic.h"
#define INIT      1
#define MINIMAL   2
#define INQUEUE   3
#define WATERSHED 4

typedef struct PriQueueNode_ PriQueueHead;
typedef struct NLevelPriNode_ * NLevelPriQueue;
typedef struct NLevelPriNode_  NLevelPriNode;
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
struct NLevelPriNode_{
    int x;
    int y;
    NLevelPriQueue next;
};
struct PriQueueNode_{
    int nodeNum;
    NLevelPriQueue head;
    NLevelPriQueue tail;
};
struct ExPix_{
    int grayvalue;
    int label;
};
void MakeNewNode(int x,int y);
void inQueue(PriQueueHead* priQueue,int gray_level,int x,int y);
NLevelPriNode* outQueue(PriQueueHead* priqueue);
void releaseNode(NLevelPriNode node);
void releasePix_Label(Pix_Label * src);
void findMinimal(double *src,double *dst,int width,int height);
void InitPriQueue(PriQueueHead* priQueue,Pix_Label *srclabel,int width,int height);



void MeyerWatershed(double *src,double *dst,int width,int height);
#endif