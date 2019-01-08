#include <list>
#include <algorithm>
#include <string>

struct PCB{
    int id;
    float arriveTime,serverTime,finishTime;
    // std::string status;
    int priority;
};

//创建PCB列表
void CreatePCBList(int N,std::list<PCB> &list,float arriveTime[],float serverTime[],int priority[]){
    int id=1000;
    for(int i=0;i<N;i++){
        PCB* p=new PCB;
        p->id=id++;
        p->arriveTime=arriveTime[i];
        p->serverTime=serverTime[i];
        p->priority=priority[i];

        list.push_back(*p);
    }
}

//输出PCB列表
void ShowPCBList(std::list<PCB> &list,std::ostream& os){
    os<<"当前进程列表为："<<std::endl;
    os<<"***********************************************************"<<std::endl;
    for(auto t:list){
        
        os<<"进程ID\t进程到达时间\t进程服务时间\t优先级\t完成时间"<<std::endl;
        os<<t.id<<"\t"<<t.arriveTime<<"\t\t"<<t.serverTime<<"\t\t"<<t.priority<<"\t"<<t.finishTime<<std::endl;
       
    }
    os<<"***********************************************************"<<std::endl;
}

//计算各进程的完成时间
void ComputerFinishTime(int N,std::list<PCB>& list){
    list.front().finishTime=list.front().arriveTime+list.front().serverTime;    //第一个进程的完成时间
   
    int before=list.front().finishTime;         //获取上一个进程完成时间
    std::for_each(++(list.begin()),list.end(),[&before](PCB & pcb){
        pcb.finishTime=before+pcb.serverTime;
        before=pcb.finishTime;
    });
}

void ClearFinishTime(std::list<PCB>& list){
    for(auto& t:list){
        t.finishTime=0;
    }
}
//先来先服务算法
void FCFS(int N,std::list<PCB>& list,float &T,float &W){
    float sumTime=0;    //sumTime为总周转时间，avgTime为总加权周转时间
    float d=0,avgTime=0;//d为加权周转时间
    for_each(list.begin(),list.end(),[&sumTime,&d,&avgTime](PCB &p){
        d=(p.finishTime/p.serverTime);
        sumTime+=p.finishTime;
        avgTime+=d;
    });
    T=(float)sumTime/N;
    W=(float)avgTime/N;
}

//静态优先级算法
void staticPriorty(int N,std::list<PCB> &list,float &T,float& W){
    //同时间到达，将进程队列按照优先级大小进行排列
    //list.sort([](PCB &a,PCB &b){ return a.priority<b.priority;});
    std::list<PCB> execlist,finishlist,reslist;
    list.sort([](PCB &p1,PCB &p2){return p1.arriveTime<p2.arriveTime;});
    execlist.push_back(list.front());           //将最先到达的放入运行队列
    list.erase(list.begin());
    //auto begin=execlist.begin();
    int index=1;
    ComputerFinishTime(index,execlist);
    while(!list.empty() && !execlist.empty()){
        index=0;
        auto begin=execlist.begin();
        for(auto it=list.begin();it!=list.end();){
            if(it->arriveTime<=begin->finishTime){
                execlist.push_back(*it);index++;
                it=list.erase(it);
            }else it++;
        }
        finishlist.push_back(execlist.front());
        execlist.erase(execlist.begin());
        execlist.sort([](PCB & p1,PCB & p2){return p1.priority<p2.priority;});
        for_each(execlist.begin(),execlist.end(),[&](PCB p){finishlist.push_back(p);});
        ComputerFinishTime(index+1,finishlist);
        for_each(finishlist.begin(),finishlist.end()--,[&](PCB p){reslist.push_back(p);});
        finishlist.clear();
    }
    list=reslist;
    //list.push_back(finishlist.back());
    //计算平均周转时间和平均加权周转时间
    float r=0,sumTime=0;
    float d=0,avgTime=0;
    //sumTime为总周转时间，avgTime为总加权周转时间
    for_each(reslist.begin(),reslist.end(),[&sumTime,&d,&avgTime](PCB &p){
        d=(p.finishTime/p.serverTime);
        sumTime+=p.finishTime;
        avgTime+=d;
    });
    T=(float)sumTime/N;
    W=(float)avgTime/N;
}

// void staticPriorty2(int N,std::list<PCB> &list,float &T,float & W){
//     //先按照先来先服务排序
//     list.sort([](PCB & a,PCB & b){ return a.arriveTime < b.arriveTime; });
//     for_each(list.begin()+1,list.end(),[](PCB & p){
//         if(list.front().arriveTime < p.arriveTime){         //谁先到谁先执行
//             if()
//         }else if(list.front().arriveTime == p.arriveTime){  //如果同时到

//         }
//     });
// }

//时间片切换算法
//const LEN=1;        //时间片长度为1
void RR(int N,std::list<PCB>& list,float arriveTime[],float serverTime[],float timeLen,std::ostream& os){               //time 为时间片
    int counter=1,j=1;
    //将首个到达的进程加入进程列表
    PCB* p=new PCB;
    p->id=1000;
    p->arriveTime=arriveTime[0];
    p->serverTime=serverTime[0];
    p->priority=0;
    list.push_back(*p);
    while(!list.empty()){
        os<<"当前进程队列信息为："<<std::endl;
        os<<"当前时刻"<<counter<<"\t正在运行的进程："<<list.front().id<<std::endl;
        for(int i=j;i<N;i++){                   //如果到达时间小于当前时间，加入进程列表
            if(counter>=arriveTime[i]){
                p=new PCB;
                p->id=1000+j;
                p->arriveTime=arriveTime[i];
                p->serverTime=serverTime[i];
                p->finishTime=0;
                p->priority=0;
                list.push_back(*p);
                j++;
            }
        }
        //ShowPCBList(list,os);   
        if(list.front().serverTime<=timeLen){    //如果队首的服务时间小于时间片，则跳转下一进程，并移除队首元素
            list.front().finishTime=counter;
            counter+=list.front().serverTime;   //服务时间不足以填充时间片，运行完成之后删除进程
            ShowPCBList(list,os);
            list.erase(list.begin());
        }else{                                  //否则，服务时间=服务时间-时间片，跳转下一进程，并将首元素移至队尾
            list.front().serverTime-=timeLen;
            ShowPCBList(list,os);
            list.push_back(list.front());
            list.erase(list.begin());
            counter+=timeLen;
        }
    }
}