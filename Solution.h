#ifndef SOLUTION_H_INCLUDED
#define SOLUTION_H_INCLUDED
#include "stdafx.h"
#include "Common.h"
struct nodeS//��ṹ��
{
    int sid;
	int L;//δ��ø��´���
	int D=0;//ɾ����lag
	float code[JobNum];//��������
	int JJArray[(JobNum-1)*JobNum/2];
	float JJMatrix[JobNum][JobNum];
    double obj[objnum];//Ŀ��˳��mc,storage,punish,energy
	double nobj[objnum];//Ŀ��˳��mc,storage,punish,energy
    int rpn=-1;
    double cosvalue;
	int isshare=0;
	int Fid;
    float jorder[JobNum][2];
    float mjnum[MNum][MNum+2];
    double mh;//ת�����Ŀ������ģ��
};



#endif // SOLUTION_H_INCLUDED
