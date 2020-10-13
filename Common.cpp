#include "stdafx.h"
#include "Indv.h"
#include "Batch.h"
#include "Job.h"
#include "Machine.h"
#include "Common.h"

nodeJ  *job=new nodeJ[JobNum];//工件结构体
vector<vector<double> > BSolutionSet;
vector<vector<double> > BSolutionSet1;
//float jobassign[JobNum][JobNum]={0.1};
float jorder[rpn][JobNum][2]={0};
float mjnum[rpn][MNum][MNum+2]={1.0/MNum};
clock_t  clockBegin, clockEnd;
clock_t  clockBegin1, clockEnd1;


void printtime(int flag,string str,clock_t clockBegin,clock_t clockEnd)
{
    if(flag==1)
        cout<<(clockEnd-clockBegin)<<endl;
}

void ReadInstance(struct nodeJ job[],int down,int up,float due,int inst)
{
    int i=0;
    char filename[10000];
    sprintf(filename,".\\instance\\%d-%d\\JN%d-MN%d-DC%1.1f-S%d~%d-%d.txt",JobNum,MNum,JobNum,MNum,due,down,down+up-1,inst+1);
    //cout<<filename<<endl;
    ifstream ifile;//读文件
    ifile.open(filename,fstream::in);
    if(!ifile)
    {
        cout<<filename<<endl;
        cout<<"File opening failed！"<<endl;
        exit(1);
    }
    while(!ifile.eof())
    {
        ifile>>job[i].JobId>>job[i].JobP>>job[i].JobSize>>job[i].JobDL>>job[i].JobEP>>job[i].JobTP;
        i++;
    }
    ifile.close();//关闭文件
    for(int j=0;j<JobNum;j++)
    {
        //cout<<job[j].JobEP<<"  "<<job[j].JobTP<<endl;
        job[j].JobId=j;
    }
}


void SortJ(nodeJ *job,int *joborder)//排序
{
	int i,j;
	int s;//当作工件结构体的中间变量

	for(i=0;i<JobNum;i++)//将工件按加工时间非增序排序
	{
		for(j=0;j<JobNum-1-i;j++)
		{
			if(job[joborder[j]].JobP<job[joborder[j+1]].JobP)
			{
				s=joborder[j];
				joborder[j]=joborder[j+1];
				joborder[j+1]=s;
			}
		}
    }
}

void SortR(nodeJ *job,int *joborder)//排序
{
	int i,j;
	int s;//当作工件结构体的中间变量

	for(i=0;i<JobNum;i++)//将工件按加工时间非增序排序
	{
		for(j=0;j<JobNum-1-i;j++)
		{
			if(job[joborder[j]].random<job[joborder[j+1]].random)
			{
				s=joborder[j];
				joborder[j]=joborder[j+1];
				joborder[j+1]=s;
			}
		}
    }
}

void SortDL(nodeJ *job,int *joborder)//排序
{
	int i,j;
	int s;//当作工件结构体的中间变量

	for(i=0;i<JobNum;i++)//将工件按加工时间非增序排序
	{
		for(j=0;j<JobNum-1-i;j++)
		{
			if(job[joborder[j]].JobDL>job[joborder[j+1]].JobDL)
			{
				s=joborder[j];
				joborder[j]=joborder[j+1];
				joborder[j+1]=s;
			}
		}
    }
}

void StructB(nodeJ *job,nodeB *b,int &bnum,int f)//分批
{
	//工件按LPT规则排序
	int joborder[JobNum];
	for(int i=0;i<JobNum;i++)
    {
        joborder[i]=i;
    }


	//if(f!=0)
	if(1)
    {
        for(int i=0;i<JobNum;i++)
        {
            job[i].random=GetRand1();
        }
        SortR(job,joborder);
    }
    else
    {
        SortJ(job,joborder);
    }
	//新建一个批
	//cout<<bnum<<endl;
	b[bnum].BId=bnum;//批号
	b[bnum].BSize=0;//批尺寸
	b[bnum].BR=0;//批到达时间
	b[bnum].BP=0;//批加工时间
	b[bnum].BS=0;//批开始加工时间
	b[bnum].BC=0;//批完工时间
	b[bnum].BMId=0;//批所在机器号
	b[bnum].BJNum=0;//批中工件数
	b[bnum].BW=0;
	for(int j=0;j<JobNum;j++)//批中工件号
	{
		b[bnum].BJId[j]=99999;
	}
	bnum++;


	int Jcount=0;
	while(Jcount<JobNum)
	{
		int flat=0;
		for(int i=0;i<bnum;i++)//从第一个批开始
		{
			if(job[joborder[Jcount]].JobSize+b[i].BSize<=MSize)//容量满足
			{
				//工件加入批中
				//批加工时间
				if(job[joborder[Jcount]].JobP>b[i].BP)
				{
					b[i].BP=job[joborder[Jcount]].JobP;
				}
				//批尺寸
				b[i].BSize=job[joborder[Jcount]].JobSize+b[i].BSize;
				//批权重
				b[i].BW=b[i].BW+job[joborder[Jcount]].JobW;
				//工件入批
				b[i].BJId[b[i].BJNum]=job[joborder[Jcount]].JobId;
				b[i].BJNum++;

				Jcount++;//下一个工件
				flat=1;//表示已加入批
				break;
			}
		}
		if(flat==0)//现有的批不合适
		{
			//分配新批
			b[bnum].BId=bnum;//批号
			b[bnum].BSize=0;//批尺寸
			b[bnum].BR=0;//批到达时间
			b[bnum].BP=0;//批加工时间
			b[bnum].BS=0;//批开始加工时间
	        b[bnum].BC=0;//批完工时间
			b[bnum].BMId=0;//批所在机器号
			b[bnum].BJNum=0;//批中工件数
            b[bnum].BW=0;
			for(int j=0;j<JobNum;j++)//批中工件号
        	{
                b[bnum].BJId[j]=99999;
	        }
			//工件入批
			//批加工时间
			if(job[joborder[Jcount]].JobP>b[bnum].BP)
			{
				b[bnum].BP=job[joborder[Jcount]].JobP;
			}
			//批尺寸
			b[bnum].BSize=job[joborder[Jcount]].JobSize+b[bnum].BSize;
            //批权重
            b[bnum].BW=b[bnum].BW+job[joborder[Jcount]].JobW;
			//工件入批
			b[bnum].BJId[b[bnum].BJNum]=job[joborder[Jcount]].JobId;
            b[bnum].BJNum++;
			Jcount++;//下一个工件
            bnum++;
		}
	}
	//cout<<bnum<<endl;
}

void InitBS(vector<vector<vector<float> > >* BS,int bmun)
{
    vector<vector<float> > temp={{99999,99999}};
    for(int i=0;i<bmun+1;i++)
    {
        (*BS).push_back(temp);
    }
}

//bool UpdateBS(vector<vector<vector<float> > >* BS,int a,int mc,int rc,float code[JobNum])
//{
//    vector<vector<float> >::iterator i;
//    bool flag=true;
//
//    for(i=(*BS)[a].begin();i!=(*BS)[a].end();)
//    {
//        //cout<<(*i)[0]<<" "<<(*i)[1]<<endl;
//        if(mc>=(*i)[0]&&rc>=(*i)[1])
//        {
//            if(mc==(*i)[0]&&rc==(*i)[1])
//            {
//                i=(*BS)[a].erase(i);
//                continue;
//            }
//            flag=false;
//            break;
//        }
//        else if(mc<=(*i)[0]&&rc<=(*i)[1])
//        {
//            i=(*BS)[a].erase(i);
//        }
//        else
//        {
//            i++;
//        }
//    }
//    if(flag==true)
//    {
//        vector<float> temp={mc,rc};
//        for(int i=0;i<JobNum;i++)
//        {
//            temp.push_back(code[i]);
//        }
//        (*BS)[a].push_back(temp);
//        //cout<<"charu"<<endl;
//    }
//    return flag;
//}




bool UpdateBS(vector<vector<double> >* BS,double obj[objnum],float code[JobNum])
{
    vector<vector<double> >::iterator i;
    bool flag=true;

    for(i=(*BS).begin();i!=(*BS).end();)
    {
        int temp1=1;
        int temp2=1;
        for(int j=0;j<objnum;j++)
        {
            if(obj[j]<(*i)[j])
                temp2=0;
            if(obj[j]>(*i)[j])
                temp1=0;
        }
        if(temp1)
        {
            i=(*BS).erase(i);
        }
        else if(temp2)
        {
            flag=false;
            break;
        }
        else
        {
            i++;
        }
    }
    if(flag==true)
    {
        vector<double> temp;
        for(int i=0;i<objnum;i++)
        {
            temp.push_back(obj[i]);
        }
        for(int i=0;i<JobNum;i++)
        {
            if(code[i]<0||code[i]>=200)
            {
                cout<<code[i]<<endl;
            }
            temp.push_back(code[i]);
        }
        (*BS).push_back(temp);
    }
    return flag;
}

void Assign2M(nodeB *b,int bnum,float code1[JobNum],float *fitness)//分机与拒绝
{
    int rejcost=0;
	int maxcp=0;
    int arr_bid[MNum][bnum]={99999};//存机器中的批ID
    float arr_mmc[MNum]={0};//存机器的最大完工时间
    int arr_mbn[MNum]={0};//存机器中的批数量

//    for(int i=0;i<JobNum;i++)//给工件编码
//    {
//        code1[i]=1.0*i/JobNum;
//    }



    for(int i=0;i<bnum;i++)//将批分机
    {
        int temp=99999;
        int id=99999;
        //id=rand()%MNum;
        for(int j=0;j<MNum;j++)
        {
            if(arr_mmc[j]<temp)
            {
                temp=arr_mmc[j];
                id=j;
            }
        }
        arr_bid[id][arr_mbn[id]]=i;
        arr_mmc[id]=arr_mmc[id]+b[i].BP/speed[id];
        arr_mbn[id]+=1;
        b[i].BC=arr_mmc[id];
    }

    for(int i=0;i<MNum;i++)//给工件编码
    {
        for(int j=0;j<arr_mbn[i];j++)
        {
            for(int k=0;k<b[arr_bid[i][j]].BJNum;k++)
            {
                code1[b[arr_bid[i][j]].BJId[k]]=i+(j+1.0)/(1+arr_mbn[i]);
            }
        }
    }

    //计算目标值
    float energy=0;
    float cmax=0;
    float storage=0;
    float punish=0;

    for(int i=0;i<MNum;i++)
    {
        energy=energy+arr_mmc[i]*e[i];
        if(cmax<arr_mmc[i])
            cmax=arr_mmc[i];
    }
    for(int i=0;i<bnum;i++)
    {
        for(int k=0;k<b[i].BJNum;k++)
        {
            if(job[b[i].BJId[k]].JobDL-b[i].BC>0)
                storage=storage+(job[b[i].BJId[k]].JobDL-b[i].BC)*job[b[i].BJId[k]].JobSize;
            else
                punish=punish+b[i].BC-job[b[i].BJId[k]].JobDL;
        }
    }
    //UpdateBS(&BSolutionSet,cmax,energy,storage,punish,code1);
    //UpdateBS1(&BSolutionSet1,cmax,energy,storage,punish);
    //cout<<cmax<<"  "<<energy<<"  "<<storage<<"  "<<punish<<endl;
    fitness[0]=cmax;
    fitness[1]=energy;
    fitness[2]=storage;
    fitness[3]=punish;
}

float GetRand()
{
    return 2.0*rand()/RAND_MAX-1.0;
}

float GetRand1()
{
    return 1.0*rand()/RAND_MAX;
}

//bool UpdateBS1(vector<vector<float> >* BS,float mc,float e,float s,float p)
//{
//    vector<vector<float> >::iterator i;
//    bool flag=true;
//
//    for(i=(*BS).begin();i!=(*BS).end();)
//    {
//        //cout<<(*i)[0]<<" "<<(*i)[1]<<endl;
//        if(mc<=(*i)[0]&&e<=(*i)[1]&&s<=(*i)[2]&&p<=(*i)[3])
//        {
//            i=(*BS).erase(i);
//        }
//        else if(mc>=(*i)[0]&&e>=(*i)[1]&&s>=(*i)[2]&&p>=(*i)[3])
//        {
//            flag=false;
//            break;
//        }
//        else
//        {
//            i++;
//        }
//    }
//    if(flag==true)
//    {
//        //cout<<"charu"<<endl;
//        vector<float> temp={mc,e,s,p};
//        (*BS).push_back(temp);
//    }
//    return flag;
//}




void exportBS(vector<vector<double> > *BS,int down,int up,float due,int inst,int n)
{
    vector<vector<double> >::iterator i;
    vector<double>::iterator j;
    ofstream outfile;
    char filename[10000];
    char ch[64];
    strftime(ch, sizeof(ch), "%Y%m%d%H%M%S", localtime(&t1));
    char folderPath[10000];
    sprintf(folderPath,".\\result\\%s\\%d-%d",ch,JobNum,MNum);
    int ftyp = _access(folderPath, 0);
    if (0 != ftyp)
    {
        sprintf(folderPath,"mkdir -p .\\result\\%s\\%d-%d",ch,JobNum,MNum);
        system(folderPath);
    }
    sprintf(filename,".\\result\\%s\\%d-%d\\HGEAD-JN%d-MN%d-DC%1.1f-S%d~%d-I%d-%d.xls",ch,JobNum,MNum,JobNum,MNum,due,down,down+up-1,inst+1,n+1);
    outfile.open(filename,fstream::app);
    if(!outfile)
    {
        cout<<"File opening failed！"<<endl;
        exit(1);
    }

    for(i=(*BS).begin();i!=(*BS).end();i++)
    {
        j=(*i).begin();
        for(int k=0;k<objnum;k++,j++)
        {
            outfile<<*j<<'\t';
        }
        outfile<<endl;
    }
    outfile.close();
}

void RandomCode(float code1[JobNum],float code2[JobNum])
{
    for(int j=0;j<MNum;j++)
    {
        float max=0;
        float min=99999;
        for(int i=0;i<JobNum;i++)
        {
            if(code1[i]>j&&code1[i]<j+1)
            {
                if(code1[i]>max)
                    max=code1[i];
                if(code1[i]<min)
                    min=code1[i];
            }
        }
        float temp=max-min;
        for(int k=0;k<JobNum;k++)
        {
            if(code1[k]>j&&code1[k]<j+1)
                code2[k]=min+temp*GetRand1();
        }
    }
}

void generate_recursive(vector<vector<float> > *rps, vector<float> *pt, int num_objs,
						float left, float total, int element)
{
    for(int i=0;i<left;i++)
    {
        (*pt).push_back(-1);
    }
	if (element == num_objs-1)
	{
		(*pt)[element] = left/total;
		rps->push_back(*pt);
	}
	else
	{
		for (size_t i=0; i<=left; i+=1)
		{
			(*pt)[element] = i/total;
			generate_recursive(rps, pt, num_objs, left-i, total, element+1);
		}
	}
}

int COMBIN(int x,int y)
{
    int a=1;
    int b=1;
    int c=1;
    for(int i=0;i<x;i++)
    {
        a=a*(x-i);
    }
    for(int i=0;i<y;i++)
    {
        b=b*(y-i);
    }
    for(int i=0;i<(x-y);i++)
    {
        c=c*(x-y-i);
    }
    return a/b/c;
}

void getsolution(float code1[JobNum],double *fitness,int t)
{
    //工件按LPT规则排序

	int joborder[JobNum];

    for(int i=0;i<JobNum;i++)
    {
        joborder[i]=i;
    }

    for(int i=0;i<JobNum;i++)
    {
        job[i].random=GetRand1();
    }
    if(t==-1)
        SortDL(job,joborder);
    else
        SortR(job,joborder);
    //计算工件分布

    float mjn[MNum];
    float sum=0;
    for(int i=0;i<MNum;i++)
    {
        if(t==-1)
            mjn[i]=1;
        else
            mjn[i]=GetRand1();
        //mjn[i]=mjnum[maxn][i][0]/mjnum[maxn][i][1]*(0.5*GetRand()+1);
        sum+=mjn[i];
    }

    //cout<<mjn[0]<<" "<<mjn[1]<<" "<<sum<<endl;
    for(int i=0;i<MNum;i++)
    {
        mjn[i]/=sum;
    }

    for(int i=0;i<MNum;i++)
    {
        mjn[i]=(int)(mjn[i]*JobNum);
        if(i==MNum-1)
        {
            mjn[i]=JobNum;
            for(int j=0;j<i;j++)
            {
                mjn[i]-=mjn[j];
            }
            break;
        }
    }
    //工件装机
    vector<int > temp2;
    vector<vector<int > > jobarr(MNum,temp2);
    int k=0;
    for(int j=0;j<JobNum;j++)
    {
        while(k!=-1)
        {
            if(jobarr[k%MNum].size()<mjn[k%MNum])
            {
                jobarr[k%MNum].push_back(joborder[j]);
                k++;
                break;
            }
            else
            {
                k++;
            }
        }
    }

    //工件分批
    nodeB (*b)[JobNum]=new nodeB[MNum][JobNum];
    nodeM *m=new nodeM[MNum];
    //static nodeM m[MNum];
    //static nodeB b[MNum][JobNum];
    for(int i=0;i<MNum;i++)
    {
        int bnum=0;
        m[i].BNum=bnum;
        //顺序选取一个工件
        for(int j=0;j<mjn[i];j++)
        {
            if((MSize-b[i][bnum].BSize)<job[jobarr[i][j]].JobSize)
            {
                bnum++;
                m[i].BNum=bnum;
            }
            b[i][bnum].BSize+=job[jobarr[i][j]].JobSize;
            b[i][bnum].BMId=i;
            b[i][bnum].BJId[b[i][bnum].BJNum]=job[jobarr[i][j]].JobId;
            b[i][bnum].BJNum++;
            if(b[i][bnum].BP<(job[jobarr[i][j]].JobP*1.0/speed[i]))
            {
                b[i][bnum].BP=(job[jobarr[i][j]].JobP*1.0/speed[i]);
            }
        }
        m[i].BNum++;
    }
    //计算MP和BC
    for(int i=0;i<MNum;i++)
    {
        float cmax=0;
        for(int j=0;j<m[i].BNum;j++)
        {
            cmax=cmax+b[i][j].BP;
            b[i][j].BC=cmax;
        }
        m[i].MP=cmax;
    }
    //编码
    for(int i=0;i<MNum;i++)//给工件编码
    {
        for(int j=0;j<m[i].BNum;j++)
        {
            for(int k=0;k<b[i][j].BJNum;k++)
            {
                code1[b[i][j].BJId[k]]=i+(j+1.0)/(1+m[i].BNum);
                //cout<<code1[b[i][j].BJId[k]]<<endl;
            }
        }
    }

    //计算目标值
    float tobj[4]={0};
    for(int i=0;i<MNum;i++)
    {
        tobj[2]=tobj[2]+m[i].MP*e[i];
        if(tobj[0]<m[i].MP)
            tobj[0]=m[i].MP;
    }
    for(int i=0;i<MNum;i++)
    {
        for(int j=0;j<m[i].BNum;j++)
        {
            for(int k=0;k<b[i][j].BJNum;k++)
            {
                if(job[b[i][j].BJId[k]].JobDL-b[i][j].BC>0)
                    tobj[1]=tobj[1]+(job[b[i][j].BJId[k]].JobDL-b[i][j].BC)*job[b[i][j].BJId[k]].JobEP;
                else
                {
                    //cout<<b[i][j].BJId[k]<<endl;
                    tobj[1]=tobj[1]+(b[i][j].BC-job[b[i][j].BJId[k]].JobDL)*job[b[i][j].BJId[k]].JobTP;
                }

            }
        }
    }

    for(int i=0;i<objnum;i++)
    {
        fitness[i]=tobj[i];
    }

//    int sum1=0;
//    for(int j=0;j<MNum;j++)
//    {
//        for(int i=0;i<m[j].BNum;i++)
//        {
//            sum1+=b[j][i].BJNum;
//        }
//    }
//    cout<<sum1<<endl;
    delete []b;
    delete []m;
}



void generatesolution(FS *foodset,float code1[JobNum])
{
    //轮盘赌的前置工作
    int maxn=0;
    for(int i=0;i<rpn;i++)
    {
        //cout<<foodset->sarr[i].size()<<" ";
        if(maxn<foodset->sarr[i].size())
            maxn=foodset->sarr[i].size();
    }
    //cout<<endl;
    float assnum[rpn]={0};
    float sum=0;
    int num1=0;
    for(int i=0;i<rpn;i++)
    {

        if(jorder[i][0][0]==0)
        {
            continue;
        }
        num1++;
        assnum[i]=maxn-foodset->sarr[i].size()+0.000001;
        sum+=assnum[i];
        //cout<<assnum[i]<<" ";
    }
    for(int i=0;i<rpn;i++)
    {
        assnum[i]/=sum;
    }

    //cout<<"vvvvvvvvv"<<endl;
    //轮盘赌
    for(int j=0;j<1;)
    {
        float rand=GetRand1();
        sum=0;
        for(int i=0;i<rpn;i++)
        {
            sum+=assnum[i];
            if(rand<=sum)
            {
                maxn=i;//找到对应的参考点
                break;
            }
        }
        if(jorder[maxn][0][0]!=0)
        {
            //cout<<maxn<<endl;
            j++;
        }
    }

    //cout<<"参考点:"<<maxn<<endl;
    //cout<<"bbbbbbbbbbb"<<endl;
    //将工件排序
    float joborder[JobNum][2];
    for(int i=0;i<JobNum;i++)
    {
        //joborder[i][0]=jorder[maxn][i][0]/jorder[maxn][i][1]*GetRand1();
        joborder[i][0]=jorder[maxn][i][0]/jorder[maxn][i][1]*(randv1*1*GetRand()+1);
        joborder[i][1]=i;
    }
	float s;//排序
	for(int i=0;i<JobNum;i++)
	{
		for(int j=0;j<JobNum-1-i;j++)
		{
			if(joborder[j][0]>joborder[j+1][0])
			{
				s=joborder[j][0];
				joborder[j][0]=joborder[j+1][0];
				joborder[j+1][0]=s;
				s=joborder[j][1];
				joborder[j][1]=joborder[j+1][1];
				joborder[j+1][1]=s;
			}
		}
    }
    //计算工件分布
    float temp[MNum][2]={1};//暂存机器的工件分布
//    for(int i=0;i<MNum;i++)
//    {
//        temp[i][0]=mjnum[maxn][i][0];
//        temp[i][1]=mjnum[maxn][i][1];
//        //cout<<mid<<" ";
//    }
    //cout<<endl;

    vector<int> vec;
    for(int i=0;i<MNum;i++)
    {
        //修改i,以达到轮番取头尾位置的效果
        int p;
        if(i%2)
        {
            p=MNum-1-i/2;
        }
        else
        {
            p=i/2;
        }
        //转存
        float temp1[MNum];//暂存轮盘赌排序
        sum=0.000001;
        for(int j=0;j<MNum;j++)
        {
            int it=0;
            for(int k=0;k<vec.size();k++)
            {
                if(vec[k]==j)
                {
                    it=1;
                    break;
                }
            }
            if(it==1)
            {
                temp1[j]=0;
                //vec中存在value值
            }
            else
            {
                temp1[j]=mjnum[maxn][p][j+2]+0.000001;
                //vec中不存在value值
            }
            sum+=temp1[j];
        }
        for(int j=0;j<MNum;j++)
        {
            temp1[j]/=sum;
        }
        //轮盘赌
        float rand=GetRand1();
        int mid;
        sum=0.000001;
        for(int k=0;k<MNum;k++)
        {
            sum+=temp1[k];
            if(k==MNum-1)
                sum=2;
            if(rand<=sum)
            {
                mid=k;//找到对应的机器
                break;
            }
        }
        //保存轮盘赌结果
        vec.push_back(mid);
        temp[mid][0]=mjnum[maxn][p][0];
        temp[mid][1]=mjnum[maxn][p][1];
        //cout<<mid<<" ";
    }
    //cout<<endl;



    float mjn[MNum]={1};
    sum=0.000001;
    for(int i=0;i<MNum;i++)
    {
        mjn[i]=temp[i][0]/(0.000001+temp[i][1])*(randv1*5*GetRand()+1);
        //mjn[i]=temp[i][0]/temp[i][1]*(0.2*GetRand()+1);
        sum+=mjn[i];
        //cout<<temp[i][0]<<" "<<temp[i][1]<<" ";
//        if(temp[i][0]==0)
//        {
//            cout<<temp[i][0]<<" "<<temp[i][1];
//            cout<<endl;
//        }
    }
    //cout<<endl;
    for(int i=0;i<MNum;i++)
    {
        mjn[i]/=sum;
    }

//    for(int i=0;i<MNum;i++)
//    {
//        cout<<mjn[i]<<" ";
//    }
//    cout<<endl;

    for(int i=0;i<MNum;i++)
    {
        mjn[i]=(int)(mjn[i]*JobNum);
        if(i==MNum-1)
        {
            mjn[i]=JobNum;
            for(int j=0;j<i;j++)
            {
                mjn[i]-=mjn[j];
            }
            break;
        }
    }

    //工件分批
    nodeB (*b)[JobNum]=new nodeB[MNum][JobNum];
    nodeM *m=new nodeM[MNum];
    //static nodeM m[MNum];
    //static nodeB b[MNum][JobNum];
    int mbnum[MNum]={0};
    int mjnum[MNum]={0};
    float mbc[MNum]={0.001};
    int jn=0;

    for(int i=0;i<MNum;i++)
    {
        mbc[i]/=speed[i];
    }



    //新增部分
    int bn=0;
    for(int i=0;i<JobNum;i++)
    {
        if(MSize-b[0][bn].BSize<job[(int)joborder[i][1]].JobSize)
        {
            bn++;
        }
        b[0][bn].BSize+=job[(int)joborder[i][1]].JobSize;
        b[0][bn].BJId[b[0][bn].BJNum]=job[(int)joborder[i][1]].JobId;
        b[0][bn].BJNum++;
        if(b[0][bn].BP<job[(int)joborder[i][1]].JobP)
        {
            b[0][bn].BP=(job[(int)joborder[i][1]].JobP);
        }
    }
    for(int i=0;i<bn+1;i++)
    {
        float temp=99999999;
        int mid;
        for(int j=0;j<MNum;j++)
        {
            if(mjnum[j]>=mjn[j])
                continue;
            if((mbc[j]+b[0][i].BP*1.0/speed[j])<temp)
            {
                mid=j;
                temp=mbc[j]+b[0][i].BP*1.0/speed[j];
            }
        }
        b[mid][mbnum[mid]]=b[0][i];
        b[mid][mbnum[mid]].BMId=mid;
        b[mid][mbnum[mid]].BP=b[mid][mbnum[mid]].BP*1.0/speed[mid];

        mjnum[mid]+=b[mid][mbnum[mid]].BJNum;
        m[mid].BNum++;
        mbc[mid]+=b[mid][mbnum[mid]].BP;
        mbnum[mid]++;//必须放最后
    }

    //编码
    for(int i=0;i<MNum;i++)//给工件编码
    {
        for(int j=0;j<m[i].BNum;j++)
        {
            for(int k=0;k<b[i][j].BJNum;k++)
            {
                code1[b[i][j].BJId[k]]=i+(j+1.0)/(1+m[i].BNum);
            }
        }
    }

    delete []b;
    delete []m;
}

void normalize(vector<vector<float> > *rps)
{
    for(int i=0;i<rps->size();i++)
    {
        float sum=0;
        for(int j=0;j<objnum;j++)
        {
            sum+=pow((*rps)[i][j],2);
        }
        sum=pow(sum,0.5);
        for(int j=0;j<objnum;j++)
        {
            (*rps)[i][j]/=sum;
        }
    }
}

void run(int down,int up,float due,int inst,int num)
{

    vector<float> pt;
    vector<vector<float> > rps;
    generate_recursive(&rps, &pt, objnum, divnum, divnum, 0);
    normalize(&rps);

    for(int ins=0;ins<inst;ins++)
    {
        for(int n=0;n<num;n++)
        {
            //初始化矩阵
            for(int i=0;i<rpn;i++)///O(RJ)
            {
                for(int j=0;j<JobNum;j++)
                {
                    jorder[i][j][0]=0;
                    jorder[i][j][1]=0;
                }

                for(int j=0;j<MNum;j++)
                {
                    mjnum[i][j][0]=0;
                    mjnum[i][j][1]=0;
                    for(int k=0;k<MNum;k++)
                    {
                        mjnum[i][j][k+2]=1.0/MNum;
                    }
                }

            }

            BSolutionSet.clear();

            float code1[JobNum];//工件编码
            nodeB *b=new nodeB[JobNum];//批结构体
            ReadInstance(job,down,up,due,ins);//将工件数据读入数组



            FS *foodset=new FS;
            Indv *Indvs=new Indv[SN];

            //初始化生成第一轮

            for(int i=0;i<SN;i++)///O(SN*J^2)
            {
                int bnum=0;
                double fitness[objnum];
                getsolution(code1,fitness,i);///O(J^2)

                for(int j=0;j<JobNum;j++)
                {
                    Indvs[i].code1[j]=code1[j];
                }
                Indvs[i].first(foodset,&rps);///O(J^2)
                foodset->addsolution(code1,fitness,Indvs[i].jorder,Indvs[i].mjnum);
            }


            //主迭代循环
            for(int t=0;t<Tmax;t++)
            {
                cout<<"iteration"<<t<<endl;

                int temp=foodset->SNum;

                for(int i=0;i<temp;i++)///O(SN*J^2)
                {
                    Indvs[i].employ(foodset,i,&rps);
                }

                foodset->getexv();


                foodset->translation();
                foodset->calccosvalue(&rps);


                foodset->maintain(&rps);
                foodset->getexv();

                temp=SN-foodset->SNum;
                for(int i=0;i<temp;i++)///O(SN*J^2)
                {
                    generatesolution(foodset,code1);

                    for(int j=0;j<JobNum;j++)
                    {
                        Indvs[i].code1[j]=code1[j];
                    }
                    Indvs[i].first(foodset,&rps);
                    foodset->addsolution(code1,Indvs[i].fitness[1],Indvs[i].jorder,Indvs[i].mjnum);
                }

            }
            exportBS(&BSolutionSet,down,up,due,ins,n);
            delete foodset;
            delete []Indvs;
            delete []b;
        }
        //BSolutionSet1.clear();
    }
}
