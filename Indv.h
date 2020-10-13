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
    nodeB b[MNum][JobNum];//批在机器的情况
    nodeB b1[JobNum];//批序列
    nodeM m[MNum];
    int TBNum=0;//批数量
    int SNum=0;//解数量

    float code[JobNum];//工件编码
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
    float code1[JobNum];//搜索后的编码
    vector<vector<float> > List;//食物源详细信息
    int AccCount;//加工批数
    void first(FS *foodset,vector<vector<float> > *rps);
    void employ(FS *foodset,int s,vector<vector<float> > *rps);
    void onlook(FS *foodset,int s);
    double fitness[2][objnum];
    int srpn=-1;
};

#endif // Indv_H_INCLUDED
