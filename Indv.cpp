#include "stdafx.h"
#include "Indv.h"
#include "Common.h"
#include "FoodSet.h"


void Indv::InitB()//初始化批信息
{
    this->TBNum=0;
    for(int j=0;j<MNum;j++)
    {
        for(int i=0;i<JobNum;i++)//初始化批数组
        {
            this->b[j][i].BId=i;//批编号
            this->b[j][i].BW=0;//批权值
            this->b[j][i].BSize=0;//批尺寸
            this->b[j][i].BP=0;//批加工时间
            this->b[j][i].BJNum=0;//批中工件数
            this->b[j][i].BMId=0;//批所在的机器号
            for(int k=0;k<JobNum;k++)//批中工件号
                this->b[j][i].BJId[k]=99999;//初始化成一个实际达不到的值
        }
    }
}

void Indv::InitM()//初始化机器信息
{
	for(int i=0;i<MNum;i++)//初始化机器
	{
		this->m[i].MId=i;//机器编号
		this->m[i].MP=0;//机器加工时间
		this->m[i].MW=0;
		//this->m[i].MSize=MSize;//机器尺寸
		this->m[i].flat=1;//标志位表示未被局优过
		this->m[i].BNum=0;//机器中批数目
		for(int j=0;j<JobNum;j++)//初始化批号
			this->m[i].MBId[j]=99999;//初始化成一个实际达不到的值
	}
}

void Indv::InitAll()//初始化
{

    for(int i=0;i<this->List.size();i++)
    {
        this->List.erase(List.begin());
    }
    for(int i=0;i<((JobNum-1)*JobNum/2);i++)
    {
        this->JJArray[i]=0;
    }
    this->SNum=0;
    this->TBNum=0;
    InitB();
    InitM();
}



void Indv::Code2List()
{
    vector<float> temp;
    for(int i=0;i<JobNum;i++)
    {
        temp.push_back(this->code1[i]);
    }
    this->List.push_back(temp);
}


void Indv::List2Code()
{
    for(int i=0;i<JobNum;i++)
    {
        this->code[i]=this->List[0][i];
    }
}

void Indv::LocSearch1()
{
    float t=0.1;
    int temp=BSolutionSet.size();
    if(temp<=1)
    {
        for(int i=0;i<JobNum;i++)
        {
            this->code1[i]=this->code1[i]+t*GetRand();
            if(this->code1[i]<0)
            {
                this->code1[i]=0;
            }

            if(this->code1[i]>=MNum)
            {
                this->code1[i]=MNum-0.000001;
            }
        }
    }
    else
    {
        int r1=rand()%temp;
        //BSolutionSet[r1][0]+=1;
        for(int i=0;i<JobNum;i++)
        {
            this->code1[i]=this->code1[i]-(this->code1[i]-BSolutionSet[r1][i+objnum])*GetRand1();
            if(this->code1[i]<0)
            {
                this->code1[i]=0;
            }

            if(this->code1[i]>=MNum)
            {
                this->code1[i]=MNum-0.000001;
            }
        }
    }
}



void Indv::Decode()
{
    float temp[JobNum][2];
    for(int i=0;i<JobNum;i++)//准备
    {
        temp[i][0]=i;
        temp[i][1]=this->code1[i];
    }

    float s;
    for(int i=0;i<JobNum;i++)//将编码按小数位大小升序排序
	{
		for(int j=0;j<JobNum-1-i;j++)
		{
			if(temp[j][1]>temp[j+1][1])
			{
				s=temp[j][1];
				temp[j][1]=temp[j+1][1];
				temp[j+1][1]=s;

				s=temp[j][0];
				temp[j][0]=temp[j+1][0];
				temp[j+1][0]=s;
			}
		}
    }

    int Arr_MJID[MNum][JobNum]={{99999}};
    int Arr_MJNum[MNum]={0};
//    for(int i=0;i<JobNum;i++)
//    {
//        cout<<temp[i][1]<<endl;
//    }
    for(int i=0;i<JobNum;i++)//将工件分机
    {
        //cout<<temp[i][1]<<"  "<<(int)floor(temp[i][1])<<endl;
        Arr_MJID[(int)floor(temp[i][1])][Arr_MJNum[(int)floor(temp[i][1])]]=temp[i][0];
        Arr_MJNum[(int)floor(temp[i][1])]++;
    }
//    for(int i=0;i<MNum;i++)
//    {
//        cout<<Arr_MJNum[i]<<endl;
//    }

    for(int i=0;i<Arr_MJNum[0];i++)//验证分机的正确性
    {
        if(code1[Arr_MJID[0][i]]>1)
        {
            cout<<"错误"<<code1[Arr_MJID[0][i]]<<"  "<<(int)floor(code1[Arr_MJID[0][i]])<<endl;
        }
    }

    for(int i=0;i<MNum;i++)//进行分批
    {
        int bnum=0;
        m[i].BNum=bnum;
        //顺序选取一个工件
        for(int j=0;j<Arr_MJNum[i];j++)
        {
            if((MSize-b[i][bnum].BSize)<job[Arr_MJID[i][j]].JobSize)
            {
                bnum++;
                m[i].BNum=bnum;
                //cout<<b[i][bnum].BJNum<<endl;
            }
            b[i][bnum].BSize+=job[Arr_MJID[i][j]].JobSize;
            b[i][bnum].BMId=i;
            b[i][bnum].BJId[b[i][bnum].BJNum]=job[Arr_MJID[i][j]].JobId;
            //cout<<i<<"  "<<bnum<<"  "<<b[i][bnum].BJNum<<"  "<<job[Arr_MJID[i][j]].JobId<<endl;
            b[i][bnum].BJNum++;
            if(b[i][bnum].BP<(job[Arr_MJID[i][j]].JobP*1.0/speed[i]))
            {
                b[i][bnum].BP=(job[Arr_MJID[i][j]].JobP*1.0/speed[i]);
            }
            //b[i][bnum].BW=job[Arr_MJID[i][j]].JobW;
            //int jobid=Arr_MJID[i][j];
            //Arr_MJID[i][j]=-1;
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

}



bool Indv::getFitness(FS *foodset,vector<vector<float> > *rps)
{
    //计算目标值
    float tobj[4]={0};

    for(int i=0;i<MNum;i++)
    {
        tobj[2]=tobj[2]+m[i].MP*e[i];
        if(tobj[0]<m[i].MP)
            tobj[0]=m[i].MP;
    }
    for(int j=0;j<MNum;j++)
    {
        for(int i=0;i<this->m[j].BNum;i++)
        {
            //tobj[2]=tobj[2]+(b[j][i].BP*(MSize-b[j][i].BSize));
            for(int k=0;k<b[j][i].BJNum;k++)
            {
                //tobj[2]=tobj[2]+((b[j][i].BP-job[b[j][i].BJId[k]].JobP)*job[b[j][i].BJId[k]].JobSize);
                if(job[b[j][i].BJId[k]].JobDL-b[j][i].BC>0)
                {
                    //cout<<b[i][j].BJId[k]<<endl;
                    tobj[1]=tobj[1]+(job[b[j][i].BJId[k]].JobDL-b[j][i].BC)*job[b[j][i].BJId[k]].JobEP;
                }
                else
                {
                    //cout<<b[i][j].BJId[k]<<endl;
                    tobj[1]=tobj[1]+(b[j][i].BC-job[b[j][i].BJId[k]].JobDL)*job[b[j][i].BJId[k]].JobTP;
                }

                    //tobj[1]=tobj[1]+b[j][i].BC-job[b[j][i].BJId[k]].JobDL;
            }
        }
    }
    for(int i=0;i<objnum;i++)
    {
        this->fitness[this->SNum][i]=tobj[i];
    }

    //保存非劣解
    //UpdateBS1(&BSolutionSet1,tobj[0],tobj[1],tobj[2],tobj[3]);
    //UpdateBS(&BSolutionSet,cmax,energy,storage,punish,this->code)
    //cout<<cmax<<" "<<energy<<" "<<storage<<" "<<punish<<endl;
    UpdateBS(&BSolutionSet,this->fitness[this->SNum],this->code1);

}




bool Indv::LocOpt()
{
    //按照完成时间将批排序(准备工作)
    int sum=0;
    for(int i=0;i<MNum;i++)
    {
        sum+=this->m[i].BNum;
    }
    //cout<<"sum"<<sum<<endl;
    float bct[sum][3];
    int id=0;
    for(int i=0;i<MNum;i++)
    {
        for(int j=0;j<this->m[i].BNum;j++)
        {
            bct[id][0]=this->b[i][j].BC;
            bct[id][1]=i;
            bct[id][2]=j;
            id++;
        }
    }

//    for(int i=0;i<sum;i++)
//    {
//        cout<<(int)bct[i][1]<<" "<<(int)bct[i][2]<<" "<<bct[i][0]<<endl;
//    }


    //开始排序
	float s;//排序
	for(int i=0;i<sum;i++)
	{
		for(int j=0;j<sum-1-i;j++)
		{
			if(bct[j][0]>bct[j+1][0])
			{
				s=bct[j][0];
				bct[j][0]=bct[j+1][0];
				bct[j+1][0]=s;
				s=bct[j][1];
				bct[j][1]=bct[j+1][1];
				bct[j+1][1]=s;
				s=bct[j][2];
				bct[j][2]=bct[j+1][2];
				bct[j+1][2]=s;
			}
		}
    }

    //局优第一步
    for(int i=0;i<sum;i++)//循环所有批
    {
        for(int j=0;j<this->b[(int)bct[i][1]][(int)bct[i][2]].BJNum;j++)//循环批中工件
        {
            int flag;
            if(job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobDL-bct[i][0]>0)
            {
                flag=1;
            }
            else
            {
                flag=-1;
            }
            float tup=0;
            int pid,jid;
            for(int k=i+flag;k<sum&&k>=0;k+=flag)//循环要交换的批
            {
                if(fabs(bct[k][0]-job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobDL)>fabs(bct[i][0]-job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobDL))//交换后结果变坏
                {
                    break;
                }
                for(int l=0;l<this->b[(int)bct[k][1]][(int)bct[k][2]].BJNum;l++)
                {
                    //验证是否可以交换
                    //cout<<(int)bct[i][1]<<" "<<(int)bct[i][2]<<" "<<j<<endl;
                    //cout<<(int)bct[k][1]<<" "<<(int)bct[k][2]<<" "<<l<<endl;
                    //cout<<this->b[(int)bct[k][1]][(int)bct[k][2]].BJNum<<endl;
                    if((MSize-this->b[(int)bct[i][1]][(int)bct[i][2]].BSize+job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobSize)<job[this->b[(int)bct[k][1]][(int)bct[k][2]].BJId[l]].JobSize||
                       (MSize-this->b[(int)bct[k][1]][(int)bct[k][2]].BSize+job[this->b[(int)bct[k][1]][(int)bct[k][2]].BJId[l]].JobSize)<job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobSize)
                        continue;

                    //确定交换惩罚
                    float temp1,temp2,temp3,temp4;
                    if(bct[i][0]>job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobDL)
                        temp1=(bct[i][0]-job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobDL)*job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobTP;
                    else
                        temp1=(job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobDL-bct[i][0])*job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobEP;
                    if(bct[k][0]>job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobDL)
                        temp2=(bct[k][0]-job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobDL)*job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobTP;
                    else
                        temp2=(job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobDL-bct[k][0])*job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobEP;
                    if(bct[k][0]>job[this->b[(int)bct[k][1]][(int)bct[k][2]].BJId[l]].JobDL)
                        temp3=(bct[k][0]-job[this->b[(int)bct[k][1]][(int)bct[k][2]].BJId[l]].JobDL)*job[this->b[(int)bct[k][1]][(int)bct[k][2]].BJId[l]].JobTP;
                    else
                        temp3=(job[this->b[(int)bct[k][1]][(int)bct[k][2]].BJId[l]].JobDL-bct[k][0])*job[this->b[(int)bct[k][1]][(int)bct[k][2]].BJId[l]].JobEP;
                    if(bct[i][0]>job[this->b[(int)bct[k][1]][(int)bct[k][2]].BJId[l]].JobDL)
                        temp4=(bct[i][0]-job[this->b[(int)bct[k][1]][(int)bct[k][2]].BJId[l]].JobDL)*job[this->b[(int)bct[k][1]][(int)bct[k][2]].BJId[l]].JobTP;
                    else
                        temp4=(job[this->b[(int)bct[k][1]][(int)bct[k][2]].BJId[l]].JobDL-bct[i][0])*job[this->b[(int)bct[k][1]][(int)bct[k][2]].BJId[l]].JobEP;


                    float temp=temp1-temp2+temp3-temp4;
                    //保存优势最大的交换方案
                    if(tup<temp)
                    {
                        tup=temp;
                        pid=k;
                        jid=l;
                    }
                }
            }
            if(tup!=0)//进行交换
            {
                this->b[(int)bct[i][1]][(int)bct[i][2]].BSize=this->b[(int)bct[i][1]][(int)bct[i][2]].BSize-job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobSize+job[this->b[(int)bct[pid][1]][(int)bct[pid][2]].BJId[jid]].JobSize;
                this->b[(int)bct[pid][1]][(int)bct[pid][2]].BSize=this->b[(int)bct[pid][1]][(int)bct[pid][2]].BSize-job[this->b[(int)bct[pid][1]][(int)bct[pid][2]].BJId[jid]].JobSize+job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]].JobSize;

                int s=this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j];
                this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[j]=this->b[(int)bct[pid][1]][(int)bct[pid][2]].BJId[jid];
                this->b[(int)bct[pid][1]][(int)bct[pid][2]].BJId[jid]=s;

                int temp1=0;
                int temp2=0;
                for(int p=0;p<this->b[(int)bct[i][1]][(int)bct[i][2]].BJNum;p++)
                {
                    if(temp1<job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[p]].JobP)
                        temp1=job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[p]].JobP;
                }
                for(int p=0;p<this->b[(int)bct[pid][1]][(int)bct[pid][2]].BJNum;p++)
                {
                    if(temp2<job[this->b[(int)bct[pid][1]][(int)bct[pid][2]].BJId[p]].JobP)
                        temp2=job[this->b[(int)bct[pid][1]][(int)bct[pid][2]].BJId[p]].JobP;
                }
                this->b[(int)bct[i][1]][(int)bct[i][2]].BP=temp1*1.0/speed[(int)bct[i][1]];
                this->b[(int)bct[pid][1]][(int)bct[pid][2]].BP=temp2*1.0/speed[(int)bct[pid][1]];
            }
        }
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

    if(GetRand1()>0.5)
    {


        //按照完成时间将批排序(准备工作)
        sum=0;
        for(int i=0;i<MNum;i++)
        {
            sum+=this->m[i].BNum;
        }
        id=0;
        for(int i=0;i<MNum;i++)
        {
            for(int j=0;j<this->m[i].BNum;j++)
            {
                bct[id][0]=this->b[i][j].BC;
                bct[id][1]=i;
                bct[id][2]=j;
                id++;
            }
        }
        //开始排序
        for(int i=0;i<sum;i++)
        {
            for(int j=0;j<sum-1-i;j++)
            {
                if(bct[j][0]>bct[j+1][0])
                {
                    s=bct[j][0];
                    bct[j][0]=bct[j+1][0];
                    bct[j+1][0]=s;
                    s=bct[j][1];
                    bct[j][1]=bct[j+1][1];
                    bct[j+1][1]=s;
                    s=bct[j][2];
                    bct[j][2]=bct[j+1][2];
                    bct[j+1][2]=s;
                }
            }
        }

        //局优第二步
        //cout<<"333333333333"<<endl;
        for(int i=0;i<sum;i++)//循环所有批
        {
            for(int j=i+1;j<sum;j++)//循环所有批
            {
                for(int k=0;k<this->b[(int)bct[j][1]][(int)bct[j][2]].BJNum;k++)
                {
                    if(MSize-this->b[(int)bct[i][1]][(int)bct[i][2]].BSize<job[this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[k]].JobSize)
                        continue;
                    //确定移动惩罚
                    float temp11,temp22;
                    if(bct[i][0]>job[this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[k]].JobDL)
                        temp11=(bct[i][0]-job[this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[k]].JobDL)*job[this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[k]].JobTP;
                    else
                        temp11=(job[this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[k]].JobDL-bct[i][0])*job[this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[k]].JobEP;
                    if(bct[j][0]>job[this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[k]].JobDL)
                        temp22=(bct[j][0]-job[this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[k]].JobDL)*job[this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[k]].JobTP;
                    else
                        temp22=(job[this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[k]].JobDL-bct[j][0])*job[this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[k]].JobEP;

                    if(temp11>temp22)
                        continue;
                    //开始插入
                    this->b[(int)bct[i][1]][(int)bct[i][2]].BSize+=job[this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[k]].JobSize;
                    this->b[(int)bct[j][1]][(int)bct[j][2]].BSize-=job[this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[k]].JobSize;
                    this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[this->b[(int)bct[i][1]][(int)bct[i][2]].BJNum]=this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[k];
                    for(int l=k;l<this->b[(int)bct[j][1]][(int)bct[j][2]].BJNum-1;l++)
                    {
                        this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[l]=this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[l+1];
                    }
                    k--;//少了一个工件，k减一
                    this->b[(int)bct[i][1]][(int)bct[i][2]].BJNum++;
                    this->b[(int)bct[j][1]][(int)bct[j][2]].BJNum--;
                    int temp1=0;
                    int temp2=0;
                    for(int p=0;p<this->b[(int)bct[i][1]][(int)bct[i][2]].BJNum;p++)
                    {
                        if(temp1<job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[p]].JobP)
                            temp1=job[this->b[(int)bct[i][1]][(int)bct[i][2]].BJId[p]].JobP;
                    }
                    for(int p=0;p<this->b[(int)bct[j][1]][(int)bct[j][2]].BJNum;p++)
                    {
                        if(temp2<job[this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[p]].JobP)
                            temp2=job[this->b[(int)bct[j][1]][(int)bct[j][2]].BJId[p]].JobP;
                    }
                    this->b[(int)bct[i][1]][(int)bct[i][2]].BP=temp1*1.0/speed[(int)bct[i][1]];
                    this->b[(int)bct[j][1]][(int)bct[j][2]].BP=temp2*1.0/speed[(int)bct[j][1]];
                }
            }
        }
        for(int i=0;i<sum;i++)
        {
            if(this->b[(int)bct[i][1]][(int)bct[i][2]].BJNum==0)
            {
                this->m[(int)bct[i][1]].BNum--;
            }
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
    }
}


void Indv::Encode()
{
    for(int i=0;i<MNum;i++)//给工件编码
    {
        for(int j=0;j<this->m[i].BNum;j++)
        {
            for(int k=0;k<this->b[i][j].BJNum;k++)
            {
                code1[b[i][j].BJId[k]]=i+(j+1.0)/(1+m[i].BNum);
            }
        }
    }
}

void Indv::ReadFS(FS *foodset,int s)
{
    for(int i=0;i<JobNum;i++)
    {
        this->code1[i]=foodset->s[s].code[i];
    }
    for(int i=0;i<objnum;i++)
    {
        this->fitness[this->SNum][i]=foodset->s[s].obj[i];
    }
    this->SNum+=1;
    this->Code2List();
}

void Indv::ChooseFS(FS *foodset,int s)
{
    if(this->fitness[0][0]>=this->fitness[1][0]&&this->fitness[0][1]>=this->fitness[1][1]&&this->fitness[0][2]>=this->fitness[1][2])
    {
        foodset->updatesolution(this->code1,this->fitness[1],s,this->jorder,this->mjnum);
    }
    else if(this->fitness[0][0]<=this->fitness[1][0]&&this->fitness[0][1]<=this->fitness[1][1]&&this->fitness[0][2]<=this->fitness[1][2])
    {
        foodset->s[s].L+=1;
    }
    else
    {
        foodset->s[s].L=0;
        foodset->addsolution(this->code1,this->fitness[1],this->jorder,this->mjnum);
    }
}

void Indv::getJJArray()
{
    for(int l=0;l<MNum;l++)
    {
        for(int i=0;i<this->m[l].BNum;i++)//批控制
        {
            for(int j=0;j<this->b[l][i].BJNum;j++)//批中工件数控制
            {
                this->JCT[this->b[l][i].BJId[j]]=this->b[l][i].BC;
                for(int k=j+1;k<this->b[l][i].BJNum;k++)
                {
                    this->JJMatrix[this->b[l][i].BJId[j]][this->b[l][i].BJId[k]]=1;
                    this->JJMatrix[this->b[l][i].BJId[k]][this->b[l][i].BJId[j]]=1;
                }
            }
        }
    }
}

//void Indv::onlook(FS *foodset,int s)
//{
//    if(s<foodset->FCnum[0])
//    {
//        this->employ(foodset,foodset->F[0][s]);
//    }
//    else if(GetRand1()<(2*(foodset->SNum+1-s)/(foodset->SNum*(foodset->SNum+1))))
//    {
//        int i;
//        for(i=0;i<foodset->Fnum;i++)
//        {
//            if(s>=foodset->FCnum[i])
//            {
//                s=s-foodset->FCnum[i];
//            }
//            else
//            {
//                break;
//            }
//        }
//        this->employ(foodset,foodset->F[i][s]);
//    }
//}

void Indv::getdistory()
{
    //更新工件历史信息
    for(int i=0;i<MNum;i++)
    {
        for(int j=0;j<this->m[i].BNum;j++)
        {
            for(int k=0;k<this->b[i][j].BJNum;k++)
            {
                this->jorder[this->b[i][j].BJId[k]][0]=this->b[i][j].BC;
                this->jorder[this->b[i][j].BJId[k]][1]=1;
            }
        }
    }
    //更新机器历史信息
    float temp[MNum][2];
    for(int i=0;i<MNum;i++)
    {
        float sum=0;
        for(int j=0;j<this->m[i].BNum;j++)
        {
            sum+=this->b[i][j].BJNum;
        }
        temp[i][0]=sum;
        temp[i][1]=i;
    }
    float s;
    for(int i=0;i<MNum;i++)
    {
        for(int j=0;j<MNum-1-i;j++)//降序排列
		{
			if(temp[j][0]<temp[j+1][0])
			{
				s=temp[j][0];
				temp[j][0]=temp[j+1][0];
				temp[j+1][0]=s;

                s=temp[j][1];
				temp[j][1]=temp[j+1][1];
				temp[j+1][1]=s;
			}
		}
    }
    for(int i=0;i<MNum;i++)
    {
        this->mjnum[i][0]=temp[i][0];
        this->mjnum[i][1]=1;
        for(int j=0;j<MNum;j++)
        {
            this->mjnum[i][j+2]=0;
        }
        this->mjnum[i][(int)temp[i][1]+2]=1;
    }
}


void Indv::employ(FS *foodset,int s,vector<vector<float> > *rps)
{
    InitAll();
    this->ReadFS(foodset,s);
    //this->LocSearch1();
    this->Decode();///O(J^2)
    //this->LocSearch(foodset,s);///O(J)
    this->LocOpt();
    //this->getJJArray();///O(J^2)
    this->Encode();///O(J)
    this->getFitness(foodset,rps);///O(J^2)
    this->SNum+=1;
    this->getdistory();
    //this->Reject();
    this->ChooseFS(foodset,s);
}

void Indv::first(FS *foodset,vector<vector<float> > *rps)
{
    InitAll();
    this->SNum+=1;
    this->Decode();///O(J^2)
    //this->LocOpt();
    //this->Encode();
    //this->getJJArray();///O(J^2)
    this->getdistory();
    this->getFitness(foodset,rps);///O(J^2)
    //this->Code2List();
}
