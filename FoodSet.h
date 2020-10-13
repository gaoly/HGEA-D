#ifndef FOODSET_H_INCLUDED
#define FOODSET_H_INCLUDED
#include "stdafx.h"
#include "Solution.h"
using namespace std;

class FS
{
	public:
    nodeS s[SN*3];//����Ϣ
    int SNum=0;//��ǰ�������
    int BNum;//����Ŀ
    int F[SN*3][SN*3];//[�㼶][�����]��[1][1]��ʼ
	int Fnum;//�����
    int FCnum[SN*3];//�ò������
    int addnum;//��Ҫ����Ľ����
    double maxobj[objnum]={0};
    double minobj[objnum]={999999};
    vector<vector<int> > sarr;
    vector<vector<int> > subp;//����Ⱥ��Ա����
	//���캯��

	//����
	void calccosvalue(vector<vector<float> > *rps);
	void setseed();
	void addsolution(float code[JobNum],double obj[objnum],float jorder[JobNum][2],float mjnum[MNum][MNum+2]);//��ӽ�
	void updatesolution(float code[JobNum],double obj[objnum],int t,float jorder[JobNum][2],float mjnum[MNum][MNum+2]);//���½�
	void delsolution(int num);//ɾ����
	void makeboundaryset();
	void initF();
    void sortsolution();
    void sortCmax(nodeS *cc,int ccNum);//��cmax��������
	void sortW(nodeS *cc,int ccNum);//���ɱ���������
    void delbadFS();
    void maintain(vector<vector<float> > *rps);
    void getexv();
    void Normalize();
    void translation();
};

#endif // FOODSET_H_INCLUDED
