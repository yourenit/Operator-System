#include <iostream>

#include "PCB.h"

int main(){
    std::list<PCB> list;
    int N;
    float T,W;
    float arriveTime[1024],serverTime[1024],finishTime[1024];
    int priority[1024];
    std::cout<<"���������������";
    while(std::cin>>N){
        for(int i=0;i<N;i++){
            std::cout<<"�������������"<<i+1<<"�ĵ���ʱ�䡢����ʱ�䡢���ȼ���";
            std::cin>>arriveTime[i]>>serverTime[i]>>priority[i];
        }
        CreatePCBList(N,list,arriveTime,serverTime,priority);
        ComputerFinishTime(N,list);
        FCFS(N,list,T,W);
        //FCFS�㷨������
        std::cout<<"FCFS�㷨������"<<std::endl;
        ShowPCBList(list,std::cout);
        std::cout<<"ƽ������ʱ�䣺"<<T<<" "<<"ƽ����Ȩ��תʱ�䣺"<<W<<std::endl;
        //��̬���ȼ�������
        ClearFinishTime(list);
        staticPriorty(N,list,T,W);
        std::cout<<"��̬���ȼ��㷨������"<<std::endl;
        ShowPCBList(list,std::cout);
        std::cout<<"ƽ������ʱ�䣺"<<T<<" "<<"ƽ����Ȩ��תʱ�䣺"<<W<<std::endl;
        
        //ʱ��Ƭ�л�
        std::list<PCB> listRR;

        std::cout<<"\n\n\n"<<std::endl;
        ClearFinishTime(list);
        RR(N,listRR,arriveTime,serverTime,1,std::cout);
    }
    return 0;
}