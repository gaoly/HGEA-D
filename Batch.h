#ifndef BATCH_H_INCLUDED
#define BATCH_H_INCLUDED
#include "stdafx.h"
#include "Common.h"
//���ṹ��
struct nodeB
{
	int BId;//�����
	int BSize=0;//���ߴ�
	float BP=0;//���ӹ�ʱ��
	int BR;//������ʱ��
	int BS;//����ʼ�ӹ�ʱ��
	float BC;//���깤ʱ��
	int BMId;//�����ڵĻ�����
	int BJNum=0;//���й�����
	int BW;//���ܾ�Ȩ��
	int BJId[JobNum];//���й�����
};


#endif // BATCH_H_INCLUDED
