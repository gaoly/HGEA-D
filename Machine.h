#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED
#include "Common.h"

struct nodeM//机器结构体
{
	int MId;//机器编号
	float MP=0;//机器加工时间
	int MW;
	int MJishu;//机器级数
	float MS;//机器速度
	int flat;//标志位，为1表示未被优化
	int BNum=0;//机器中批数目
	int M_jishuMp;
	int MBId[JobNum];//机器中批号，0位空出
};

#endif // MACHINE_H_INCLUDED
