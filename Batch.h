#ifndef BATCH_H_INCLUDED
#define BATCH_H_INCLUDED
#include "stdafx.h"
#include "Common.h"
//批结构体
struct nodeB
{
	int BId;//批编号
	int BSize=0;//批尺寸
	float BP=0;//批加工时间
	int BR;//批到达时间
	int BS;//批开始加工时间
	float BC;//批完工时间
	int BMId;//批所在的机器号
	int BJNum=0;//批中工件数
	int BW;//批拒绝权重
	int BJId[JobNum];//批中工件号
};


#endif // BATCH_H_INCLUDED
