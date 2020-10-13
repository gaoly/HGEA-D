#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
#include "stdafx.h"
#include "Job.h"
const int JobNum=300;//工件个数
const int MNum=5;//机器
const int MSize=40;//机器容量
const int Tmax=200;//迭代次数
const int SN=200;
const int L=500;
const time_t t1 = time(0);
//const float E=0.4;
//const float R=0.5;
//const float P=0.2;
const float speed[5]={1,2,3,1.5,2.5};
const float e[5]={2,8,18,4.5,12.5};
const float evap=0.5;
const int objnum=3;//目标数
const int divnum=13;//参考点分割
const int isprintf=1;

const float randv1=0.2;

extern int COMBIN(int x,int y);
//const int rpn=COMBIN((objnum+divnum-1),divnum);//参考点数量
const int rpn=105;
extern vector<vector<double> > BSolutionSet;
//extern vector<vector<double> > BSolutionSet1;
extern int t;
extern void run(int down,int up,float due,int inst,int num);
extern bool UpdateBS(vector<vector<double> >* BS,double obj[objnum],float code[JobNum]);
//extern bool UpdateBS1(vector<vector<float> >* BS,float mc,float e,float s,float p);
extern void printtime(int flag,string str,clock_t clockBegin,clock_t clockEnd);
//extern float jobassign[JobNum][JobNum];
extern float jorder[rpn][JobNum][2];
extern float mjnum[rpn][MNum][MNum+2];
extern float GetRand();
extern float GetRand1();
extern nodeJ  *job;
extern clock_t  clockBegin, clockEnd;

#endif // COMMON_H_INCLUDED
