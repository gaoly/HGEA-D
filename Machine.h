#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED
#include "Common.h"

struct nodeM//�����ṹ��
{
	int MId;//�������
	float MP=0;//�����ӹ�ʱ��
	int MW;
	int MJishu;//��������
	float MS;//�����ٶ�
	int flat;//��־λ��Ϊ1��ʾδ���Ż�
	int BNum=0;//����������Ŀ
	int M_jishuMp;
	int MBId[JobNum];//���������ţ�0λ�ճ�
};

#endif // MACHINE_H_INCLUDED
