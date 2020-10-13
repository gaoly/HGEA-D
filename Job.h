#ifndef JOB_H_INCLUDED
#define JOB_H_INCLUDED
#include "stdafx.h"
struct nodeJ//工件结构体
{
	int JobId;//工件编号
	int JobSize;//工件尺寸
	int JobP;//工件加工时间
	int JobW;// 工件权值
	float JobDL;//工件交付时间
	float random;
	double unitJobW;
	double unitJobP;
	float JobEP=0;//早到惩罚
	float JobTP=0;//延迟惩罚
};

#endif // JOB_H_INCLUDED
