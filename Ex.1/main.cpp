#include <iostream>

#include "PCB.h"

int main(){
    std::list<PCB> list;
    int N;
    float T,W;
    float arriveTime[1024],serverTime[1024],finishTime[1024];
    int priority[1024];
    std::cout<<"请输入进程数量：";
    while(std::cin>>N){
        for(int i=0;i<N;i++){
            std::cout<<"请依次输入进程"<<i+1<<"的到达时间、服务时间、优先级：";
            std::cin>>arriveTime[i]>>serverTime[i]>>priority[i];
        }
        CreatePCBList(N,list,arriveTime,serverTime,priority);
        ComputerFinishTime(N,list);
        FCFS(N,list,T,W);
        //FCFS算法结果输出
        std::cout<<"FCFS算法结果输出"<<std::endl;
        ShowPCBList(list,std::cout);
        std::cout<<"平均周期时间："<<T<<" "<<"平均带权周转时间："<<W<<std::endl;
        //静态优先级结果输出
        ClearFinishTime(list);
        staticPriorty(N,list,T,W);
        std::cout<<"静态优先级算法结果输出"<<std::endl;
        ShowPCBList(list,std::cout);
        std::cout<<"平均周期时间："<<T<<" "<<"平均带权周转时间："<<W<<std::endl;
        
        //时间片切换
        std::list<PCB> listRR;

        std::cout<<"\n\n\n"<<std::endl;
        ClearFinishTime(list);
        RR(N,listRR,arriveTime,serverTime,1,std::cout);
    }
    return 0;
}