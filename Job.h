#ifndef JOB_H_INCLUDED
#define JOB_H_INCLUDED
#include "stdafx.h"
struct nodeJ//�����ṹ��
{
	int JobId;//�������
	int JobSize;//�����ߴ�
	int JobP;//�����ӹ�ʱ��
	int JobW;// ����Ȩֵ
	float JobDL;//��������ʱ��
	float random;
	double unitJobW;
	double unitJobP;
	float JobEP=0;//�絽�ͷ�
	float JobTP=0;//�ӳٳͷ�
};

#endif // JOB_H_INCLUDED
