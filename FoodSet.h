#ifndef FOODSET_H_INCLUDED
#define FOODSET_H_INCLUDED
#include "stdafx.h"
#include "Solution.h"
using namespace std;

class FS
{
	public:
    nodeS s[SN*3];//解信息
    int SNum=0;//当前解的数量
    int BNum;//批数目
    int F[SN*3][SN*3];//[层级][个体号]从[1][1]开始
	int Fnum;//层次数
    int FCnum[SN*3];//该层个体数
    int addnum;//需要补充的解个数
    double maxobj[objnum]={0};
    double minobj[objnum]={999999};
    vector<vector<int> > sarr;
    vector<vector<int> > subp;//子种群成员集合
	//构造函数

	//操作
	void calccosvalue(vector<vector<float> > *rps);
	void setseed();
	void addsolution(float code[JobNum],double obj[objnum],float jorder[JobNum][2],float mjnum[MNum][MNum+2]);//添加解
	void updatesolution(float code[JobNum],double obj[objnum],int t,float jorder[JobNum][2],float mjnum[MNum][MNum+2]);//更新解
	void delsolution(int num);//删除解
	void makeboundaryset();
	void initF();
    void sortsolution();
    void sortCmax(nodeS *cc,int ccNum);//按cmax升序排序
	void sortW(nodeS *cc,int ccNum);//按成本升序排序
    void delbadFS();
    void maintain(vector<vector<float> > *rps);
    void getexv();
    void Normalize();
    void translation();
};

#endif // FOODSET_H_INCLUDED
