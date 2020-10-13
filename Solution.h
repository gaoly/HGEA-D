#ifndef SOLUTION_H_INCLUDED
#define SOLUTION_H_INCLUDED
#include "stdafx.h"
#include "Common.h"
struct nodeS//解结构体
{
    int sid;
	int L;//未获得更新次数
	int D=0;//删除的lag
	float code[JobNum];//工件编码
	int JJArray[(JobNum-1)*JobNum/2];
	float JJMatrix[JobNum][JobNum];
    double obj[objnum];//目标顺序mc,storage,punish,energy
	double nobj[objnum];//目标顺序mc,storage,punish,energy
    int rpn=-1;
    double cosvalue;
	int isshare=0;
	int Fid;
    float jorder[JobNum][2];
    float mjnum[MNum][MNum+2];
    double mh;//转换后的目标向量模长
};



#endif // SOLUTION_H_INCLUDED
