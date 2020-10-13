#include <iostream>
#include "stdafx.h"
#include "Common.h"

int main()
{

    srand((unsigned) time(0));//设置时间种子
    int temp=rand();
    temp=rand();
    int JSDOWN[3]={1,15,1};
    int JSUP[3]={15,21,40};
    float DueDate[3]={0.5,1,1.5};
    clock_t  clockBegin1, clockEnd1;
//    run(JSDOWN[1],JSUP[1],DueDate[0],1,1);
    for(int e=0;e<sizeof(JSDOWN)/sizeof(JSDOWN[0]);e++)
    {
        for(int d=0;d<sizeof(DueDate)/sizeof(DueDate[0]);d++)
        {
            run(JSDOWN[e],JSUP[e],DueDate[d],10,10);
        }
    }
    system("pause");
    return 0;
}


