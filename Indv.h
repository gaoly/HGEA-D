#ifndef Indv_H_INCLUDED
#define Indv_H_INCLUDED
#include "stdafx.h"
#include "Batch.h"
#include "Machine.h"
#include "Common.h"
#include "FoodSet.h"
class Indv
{
private:
    nodeB b[MNum][JobNum];//���ڻ��������
    nodeB b1[JobNum];//������
    nodeM m[MNum];
    int TBNum=0;//������
    int SNum=0;//������

    float code[JobNum];//��������
    void getJJArray();
    void InitB();
    void InitM();
    void InitAll();
    void Code2List();
    void List2Code();
    void LocSearch(FS *foodset,int s);
    void LocSearch1();
    void Decode();
    bool Reject();
    bool LocOpt();
    void Encode();
    bool getFitness(FS *foodset,vector<vector<float> > *rps);
    void ReadFS(FS *foodset,int s);
    void ChooseFS(FS *foodset,int s);
    void getdistory();

public:
    float jorder[JobNum][2];
    float mjnum[MNum][MNum+2];
    float JJMatrix[JobNum][JobNum];
    float JCT[JobNum];
    int JJArray[(JobNum-1)*JobNum/2];
    int t=0;
    float code1[JobNum];//������ı���
    vector<vector<float> > List;//ʳ��Դ��ϸ��Ϣ
    int AccCount;//�ӹ�����
    void first(FS *foodset,vector<vector<float> > *rps);
    void employ(FS *foodset,int s,vector<vector<float> > *rps);
    void onlook(FS *foodset,int s);
    double fitness[2][objnum];
    int srpn=-1;
};

#endif // Indv_H_INCLUDED
