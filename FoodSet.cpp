#include "stdafx.h"
#include "FoodSet.h"
#include "Common.h"

void FS::addsolution(float code[JobNum],double obj[objnum],float jorder1[JobNum][2],float mjnum1[MNum][MNum+2])
{
    for(int i=0;i<JobNum;i++)
    {
        this->s[this->SNum].code[i]=code[i];
    }

    for(int i=0;i<objnum;i++)
    {
        this->s[this->SNum].obj[i]=obj[i];
    }
    for(int i=0;i<JobNum;i++)
    {
        this->s[this->SNum].jorder[i][0]=jorder1[i][0];
        this->s[this->SNum].jorder[i][1]=jorder1[i][1];
    }
    for(int i=0;i<MNum;i++)
    {
        for(int j=0;j<MNum+2;j++)
        {
            this->s[this->SNum].mjnum[i][j]=mjnum1[i][j];
        }
    }
    //this->s[this->SNum].isseed=0;
    this->s[this->SNum].L=0;
    this->s[this->SNum].D=0;
    this->s[this->SNum].isshare=0;
    this->s[this->SNum].sid=this->SNum;
    this->SNum+=1;
}


void FS::updatesolution(float code[JobNum],double obj[objnum],int t,float jorder1[JobNum][2],float mjnum1[MNum][MNum+2])
{
    for(int i=0;i<JobNum;i++)
    {
        this->s[t].code[i]=code[i];
    }
    for(int i=0;i<objnum;i++)
    {
        this->s[t].obj[i]=obj[i];
    }
    for(int i=0;i<JobNum;i++)
    {
        this->s[t].jorder[i][0]=jorder1[i][0];
        this->s[t].jorder[i][1]=jorder1[i][1];
    }
    for(int i=0;i<MNum;i++)
    {
        for(int j=0;j<MNum+2;j++)
        {
            this->s[t].mjnum[i][j]=mjnum1[i][j];
        }
    }
    this->s[t].L=0;
    this->s[t].D=0;
    this->s[this->SNum].isshare=0;
}


void FS::initF()
{
    //初始化F
	for(int i=0;i<SN*3;i++)
	{
		for(int j=0;j<SN*3;j++)
		{
			this->F[i][j]=99999;
		}
	}
}


void FS::makeboundaryset()
{


    //初始化F
	this->initF();
	this->Fnum=0;
	//初始化n,s
    //int m[this->SNum][this->SNum];//[i][j]记录被i支配的个体集合---i表示个体编号,s[i][j]表示被i支配的个体编号
    vector <int > temp(this->SNum,0);
    vector <vector<int > > m(this->SNum,temp);
	int n[this->SNum];//[i]记录群体中支配个体i的个体数----下标表示个体编号,n[i]表示群体中支配个体i的个体数
	for(int i=0;i<this->SNum;i++)
	{
		for(int j=0;j<this->SNum;j++)
		{
			m[i][j]=99999;
		}
		n[i]=0;
	}
	//第一部分
	int fnum=0;//下标控制
	for(int p=0;p<this->SNum;p++)
	{
		int num=0;//下标控制
		for(int q=0;q<this->SNum;q++)
		{
			//判断p支配q
			if(this->s[p].obj[0]<=this->s[q].obj[0]&&this->s[p].obj[1]<=this->s[q].obj[1]&&this->s[p].obj[2]<=this->s[q].obj[2])
			{
				if(this->s[p].obj[0]<this->s[q].obj[0]||this->s[p].obj[1]<this->s[q].obj[1]||this->s[p].obj[2]<this->s[q].obj[2])
				{
					m[p][num]=q;//保存个体
					num++;
				}
			}
			else if(this->s[p].obj[0]>=this->s[q].obj[0]&&this->s[p].obj[1]>=this->s[q].obj[1]&&this->s[p].obj[2]>=this->s[q].obj[2])//判断q支配p
			{
				if(this->s[p].obj[0]>this->s[q].obj[0]||this->s[p].obj[1]>this->s[q].obj[1]||this->s[p].obj[2]>this->s[q].obj[2])
				{
					n[p]=n[p]+1;//加1
				}
			}
		}
		if(n[p]==0)
		{
			this->F[0][fnum]=p;
            fnum++;
			this->FCnum[0]=fnum;//个体个数
		}
	}

	//第二部分
	int H[this->SNum];//中间变量
	int hnum=0;//H中元素个数
	int flat=0;//标志位
	for(int j=0;j<this->SNum;j++)
	{
		if(this->F[0][j]!=99999)
		{
			flat=1;
			break;
		}
	}
	if(flat==0)
    {
        cout<<"发现异常"<<endl;
    }
	int i=0;
	while(flat)
	{
		//初始化H
		for(int j=0;j<this->SNum;j++)
		{
			H[j]=99999;//0号位空出
		}

		int num=0;//个体个数
		//统计第i层个体个数
		for(int j=0;j<this->SNum;j++)//统计F[i]的个数即第i层的个体个数
		{
			if(this->F[i][j]!=99999)
			{
				num++;
			}
		}

        hnum=0;//初始化H[]个数
		for(int p=0;p<num;p++)
		{
			int pnum=0;
			//统计第i层第j个个体的支配个体个数
			for(int j=0;j<this->SNum;j++)//统计F[i]的个数即第i层的个体个数
			{//this->F[i][p]表示第i层中个体号为this->F[i][p]的个体，s[this->F[i][p]][j]表示被个体号为this->F[i][p]支配的个体号
				if(m[this->F[i][p]][j]!=99999)
				{
					pnum++;//被个体号为this->F[i][p]支配的个体号个数
				}
			}

			for(int q=0;q<pnum;q++)
			{
				n[m[this->F[i][p]][q]]=n[m[this->F[i][p]][q]]-1;
				if(n[m[this->F[i][p]][q]]==0)
				{
					H[hnum]=m[this->F[i][p]][q];
                    hnum++;
				}
			}
		}
		i++;//下一层
		for(int j=0;j<hnum;j++)//统计F[i]的个数即第i层的个体个数
		{
			this->F[i][j]=H[j];
		}
		this->FCnum[i]=hnum;//该层个体个数
		flat=0;//标志位
		for(int j=0;j<this->SNum;j++)
		{
			if(this->F[i][j]!=99999)
			{
				flat=1;
				break;
			}
		}
	}
	this->Fnum=i;//层次数

	for(int j=0;j<this->Fnum;j++)
    {
        for(int k=0;k<this->FCnum[j];k++)
        {
            this->s[this->F[j][k]].Fid=j;
        }
    }
}


//将个体按照cmax升序排序
void FS::sortCmax(nodeS *cc,int ccNum)
{
	int i,j;
	nodeS t;//中间变量

	for(i=0;i<ccNum;i++)
	{
		for(j=0;j<ccNum-1-i;j++)
		{
			if(cc[j].obj[0]>cc[j+1].obj[0])
			{
				t=cc[j];
				cc[j]=cc[j+1];
				cc[j+1]=t;
			}
		}
	}
}


//将个体按照W升序排序
//void FS::sortW(nodeS *cc,int ccNum)
//{
//	int i,j;
//	nodeS t;//中间变量
//
//	for(i=0;i<ccNum;i++)
//	{
//		for(j=0;j<ccNum-1-i;j++)
//		{
//			if(cc[j].rc>cc[j+1].rc)
//			{
//				t=cc[j];
//				cc[j]=cc[j+1];
//				cc[j+1]=t;
//			}
//		}
//	}
//}


//void FS::sortsolution()
//{
//    //cout<<this->Fnum<<endl;
//    for(int i=0;i<this->Fnum;i++)
//    {
//        //初始化个体距离------下标对应个体号
//        float distance[this->SNum];
//        for(int j=0;j<this->SNum;j++)
//        {
//            distance[j]=-1;//防止相减等于0的情况
//        }
//
//        nodeS *cc=new nodeS[this->FCnum[i]];
//        int ccNum=this->FCnum[i];//个数
//        //提取该层解
//        for(int j=0;j<this->FCnum[i];j++)
//        {
//            cc[j]=this->s[this->F[i][j]];
//        }
//
//        for(int j=0;j<this->FCnum[i];j++)
//        {
//            if(cc[j].sid==99999)
//            {
//                cout<<"sid不一致"<<endl;
//            }
//        }
//
//        int k=1;//目标个数
//        while(k<=2)
//        {
//            if(k==1)//第一个目标---cmax
//            {
//                //将个体按照cmax升序排序
//                this->sortCmax(cc,ccNum);
//                //计算距离
//                for(int j=0;j<ccNum;j++)
//                {
//                    //处理边界点
//                    if(j==0||j==ccNum-1)
//                    {
//                        distance[cc[j].sid]=1000000;//无穷大
//                    }
//                    else//非边界点
//                    {
//                        distance[cc[j].sid]=(float)(cc[j+1].mc-cc[j-1].mc)/(cc[0].mc-cc[ccNum-1].mc);///强转类型记得测试数值
//                    }
//                }
//                k++;//加一
//            }
//            else//第二个目标
//            {
//                //将个体按照W升序排序
//                this->sortW(cc,ccNum);
//                //计算距离
//                for(int j=0;j<ccNum;j++)
//                {
//                    //处理边界点
//                    if(j==0||j==ccNum-1)
//                    {
//                        distance[cc[j].sid]=distance[cc[j].sid]+1000000;//无穷大
//                    }
//                    else//非边界点
//                    {
//                        distance[cc[j].sid]=distance[cc[j].sid]+(float)(cc[j+1].rc-cc[j-1].rc)/(cc[0].rc-cc[ccNum-1].rc);
//                    }
//                }
//                k++;
//            }
//        }
//
//        //按照解距离排序
//        int t;//中间变量
//        for(int k=0;k<ccNum;k++)
//        {
//            for(int j=0;j<ccNum-1-i;j++)
//            {
//                if(distance[this->F[i][j]]<distance[this->F[i][j+1]])
//                {
//                    t=this->F[i][j];
//                    this->F[i][j]=this->F[i][j+1];
//                    this->F[i][j+1]=t;
//                }
//            }
//        }
//
//        delete []cc;
//    }
//}

void FS::delbadFS()
{
    int n=0;
    for(int i=0;i<this->SNum;i++)
    {
        //&&this->s[i].isseed==0
        if(this->s[i].L>=L)
        {
            for(int j=i;j<this->SNum-1;j++)
            {
                this->s[j]=this->s[j+1];
                this->s[j].sid=j;
            }
            this->SNum--;
            //cout<<this->SNum<<endl;
            i--;
            n++;
        }
    }
    this->addnum=n;
}

void FS::getexv()
{
    for(int i=0;i<objnum;i++)
    {
        this->maxobj[i]=0;
        this->minobj[i]=999999;
    }

    for(int i=0;i<this->SNum;i++)
    {
        for(int j=0;j<objnum;j++)
        {
            if(this->s[i].obj[j]>this->maxobj[j])
                this->maxobj[j]=this->s[i].obj[j];
            if(this->s[i].obj[j]<this->minobj[j])
                this->minobj[j]=this->s[i].obj[j];
        }
    }
}


//void FS::maintain()
//{
//    int tnum=SN/2;//要保留的数
//    int temp=0;
//    int lfid=0;//最后有效层
//
//    for(int i=0;i<this->Fnum;i++)
//    {
//        temp=0;
//        lfid=i;
//        for(int j=0;j<this->FCnum[i];j++)
//        {
//            temp++;
//            tnum--;
//            if(tnum==0)
//            {
//                break;
//            }
//        }
//        if(tnum==0)
//        {
//            break;
//        }
//    }
//    //删除底层无用解
//    for(int i=lfid+1;i<this->Fnum;i++)
//    {
//        for(int j=0;j<this->FCnum[i];j++)
//        {
//            this->s[this->F[i][j]].L=L+1;
//        }
//    }
//
//
//    //得到解于参考点结合的情况
//    vector<int> temp1;
//    this->sarr.clear();
//    for(int i=0;i<rpn;i++)
//    {
//        this->sarr.push_back(temp1);
//    }
//    for(int i=0;i<this->SNum;i++)
//    {
//        this->sarr[this->s[i].rpn].push_back(this->s[i].sid);
//    }
//
//    if(SN<this->SNum)
//    {
//        int delnum=this->SNum-(SN/2);
//        for(int i=0;i<delnum;i++)
//        {
//            int maxnum=0;
//            int maxid=0;
//            for(int j=0;j<rpn;j++)
//            {
//                if(this->sarr[j].size()>maxnum)
//                {
//                    maxnum=this->sarr[j].size();
//                    maxid=j;
//                }
//            }
//            this->s[this->sarr[maxid][0]].L=L+1;
//            this->sarr[maxid].erase(this->sarr[maxid].begin());
//        }
//        //删解
//        this->delbadFS();
//        cout<<"删除"<<this->addnum<<endl;
//    }
//}


void FS::Normalize()
{
    //寻找理想点以及转化目标值
    float temp[objnum]={999999};

    for(int i=0;i<SN;i++)
    {
        for(int j=0;j<objnum;j++)
        {
            this->s[i].nobj[j]=this->s[i].obj[j]-this->minobj[j];
        }
    }
    //寻找EP
    int EPid[objnum];
    for(int i=0;i<objnum;i++)
    {
        vector<float> w(objnum, 0.000001);
		w[i] = 1.0;

		float min_ASF = numeric_limits<float>::max();
		int min_indv;
		for (int j=0; j<SN; j++)  // only consider the individuals in the first front
		{

			double max_ratio = -numeric_limits<double>::max();

            float w1;
            for(int k=0;k<objnum;k++)
            {
                w1 = w[k]?w[k]:0.00001;
                max_ratio = std::max(max_ratio, this->s[j].nobj[k]/w1);
            }

			if ( max_ratio < min_ASF )
			{
				min_ASF = max_ratio;
				min_indv = j;
			}
		}
		EPid[i]=min_indv;
    }

    //计算超平面方程
    vector<float> intercepts;
    //ConstructHyperplane(&intercepts, cur, extreme_points);
    // Check whether there are duplicate extreme points.
	// This might happen but the original paper does not mention how to deal with it.
	bool duplicate = false;
	for (int i=0; !duplicate && i<objnum; i++)
	{
		for (int j=i+1; !duplicate && j<objnum; j++)
		{
			duplicate = (EPid[i] == EPid[j]);
		}
	}

	intercepts.assign(objnum, 0);

	bool negative_intercept = false;
	if (!duplicate)
	{
		// Find the equation of the hyperplane
		vector<float> b(objnum, 1.0);
		vector< vector<float> > A;
		for (int p=0; p<objnum; p+=1)
		{
		    vector<float> temp1;
		    for(int q=0;q<objnum;q++)
            {
                temp1.push_back(this->s[EPid[p]].nobj[q]);
            }
			A.push_back(temp1);
		}
		vector<float> x;
		//高斯消元
        const int N = A.size();
        for (int i=0; i<N; i+=1)
        {
            A[i].push_back(b[i]);
        }

        for (int base=0; base<N-1; base+=1)
        {
            for (int target=base+1; target<N; target+=1)
            {
                double ratio = A[target][base]/A[base][base];
                for (int term=0; term<A[base].size(); term+=1)
                {
                    A[target][term] -= A[base][term]*ratio;
                }
            }
        }
        x.resize(N);
        for (int i=N-1; i>=0; i-=1)
        {
            for (int known=i+1; known<N; known+=1)
            {
                A[i][N] -= A[i][known]*x[known];
            }
            x[i] = A[i][N]/A[i][i];
        }

		// Find intercepts
		for (int f=0; f<intercepts.size(); f+=1)
		{
			intercepts[f] = 1.0/x[f];

			if(x[f] < 0)
			{
				negative_intercept = true;
				break;
			}
		}
	}

	if (duplicate || negative_intercept) // v1.2: follow the method in Yuan et al. (GECCO 2015)
	{
        const int NumObj = objnum;
        vector<double> max_point(NumObj, -numeric_limits<double>::max());
        for (int i=0; i<SN; i+=1)
        {
            for(int j=0;j<objnum;j++)
            {
                max_point[j] = std::max(max_point[j], this->s[i].obj[j]);
            }
        }

		for (int f=0; f<intercepts.size(); f+=1)
		{
			intercepts[f] = max_point[f];
		}
	}

    //标准化
	for (int t=0; t<this->Fnum; t+=1)
	{
		for (int i=0; i<this->FCnum[t]; i+=1)
		{
			int ind = this->F[t][i];
			for(int j=0;j<objnum;j++)
            {
                if ( fabs(intercepts[j])>10e-10 ) // avoid the divide-by-zero error
                    this->s[ind].nobj[j] /= (intercepts[j]); // v1.11: fixed
                else
                    this->s[ind].nobj[j] /= 10e-10;
            }
        }
    }
}

void FS::translation()
{
    for(int i=0;i<this->SNum;i++)
    {
        for(int j=0;j<objnum;j++)
        {
            this->s[i].nobj[j]=(this->s[i].obj[j]-this->minobj[j])/(1.0*(this->maxobj[j]-this->minobj[j]));
        }
    }

    //计算模长
    for(int i=0;i<this->SNum;i++)
    {
        double sum=0;
        for(int j=0;j<objnum;j++)
        {
            sum+=pow(this->s[i].nobj[j],2);
        }
        this->s[i].mh=pow(sum,0.5);
    }
}

void FS::calccosvalue(vector<vector<float> > *rps)
{
    this->subp.clear();
    vector <int > temp;
    for(int j=0;j<(*rps).size();j++)
    {

        this->subp.push_back(temp);
    }

    for(int i=0;i<this->SNum;i++)
    {
        this->s[i].cosvalue=0;
    }
    for(int i=0;i<this->SNum;i++)
    {
        for(int j=0;j<(*rps).size();j++)
        {
            double sum=0;
            for(int k=0;k<objnum;k++)
            {
                sum+=this->s[i].nobj[k]*(*rps)[j][k];
            }
            sum/=this->s[i].mh;
            if(this->s[i].cosvalue<sum)
            {
                this->s[i].cosvalue=sum;
                this->s[i].rpn=j;
            }
        }
        this->subp[this->s[i].rpn].push_back(i);
    }
}

void FS::maintain(vector<vector<float> > *rps)
{
    for(int i=0;i<this->subp.size();i++)
    {
        int s;
        for(int j=0;j<this->subp[i].size();j++)//增序排序
        {
            for(int k=0;k<this->subp[i].size()-1-j;k++)
            {
                if(this->s[this->subp[i][j]].mh>this->s[this->subp[i][j+1]].mh)
                {
                    s=this->subp[i][j];
                    this->subp[i][j]=this->subp[i][j+1];
                    this->subp[i][j+1]=s;
                }
            }
        }
    }

    int num=0;
    for(int i=0;i>=0;i++)
    {
        int j=i%this->subp.size();
        if(this->subp[j].size()==0)
            continue;
        this->s[this->subp[j][0]].D=1;
        this->subp[j].erase(this->subp[j].begin());
        num++;
        if(num==(SN/2))
            break;
    }


    for(int i=0;i<this->SNum;i++)
    {
        if(this->s[i].D==1)
        {
            this->s[i].D=0;
            //num++;
        }
        else
            this->s[i].D=1;
    }

    //手动删解
    static nodeS stemp[SN];
    int stempnum=0;
    for(int i=0;i<this->SNum;i++)
    {
        if(this->s[i].L<L&&this->s[i].D==0)
        {
            //cout<<stempnum<<" "<<i<<endl;
            stemp[stempnum]=this->s[i];
            stempnum++;
        }
    }
    for(int i=0;i<stempnum;i++)
    {
        this->s[i]=stemp[i];
        this->s[i].sid=i;
    }
    this->SNum=stempnum;

    //cout<<"SN"<<stempnum<<endl;

    vector<int> temp2;
    this->sarr.clear();
    for(int i=0;i<rpn;i++)
    {
        this->sarr.push_back(temp2);
    }
    //cout<<this->SNum<<endl;
    for(int i=0;i<this->SNum;i++)
    {
        //cout<<"rpn"<<this->s[i].rpn<<" "<<this->s[i].sid<<" "<<i<<endl;
        this->sarr[this->s[i].rpn].push_back(i);
        if(this->s[i].isshare==0)
        {
            //更新历史信息
            for(int j=0;j<JobNum;j++)
            {
                jorder[this->s[i].rpn][j][0]+=this->s[i].jorder[j][0];
                jorder[this->s[i].rpn][j][1]+=this->s[i].jorder[j][1];
            }
            for(int j=0;j<MNum;j++)
            {
                mjnum[this->s[i].rpn][j][0]+=this->s[i].mjnum[j][0];
                mjnum[this->s[i].rpn][j][1]+=this->s[i].mjnum[j][1];
                for(int k=0;k<MNum;k++)
                {
                    mjnum[this->s[i].rpn][j][k+2]=mjnum[this->s[i].rpn][j][k+2]*0.5+this->s[i].mjnum[j][k+2]*0.5;
                }
            }
            this->s[i].isshare=0;
        }
    }
}
