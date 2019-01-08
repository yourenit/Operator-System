#include <list>
#include <algorithm>
#include <string>

struct PCB{
    int id;
    float arriveTime,serverTime,finishTime;
    // std::string status;
    int priority;
};

//����PCB�б�
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

//���PCB�б�
void ShowPCBList(std::list<PCB> &list,std::ostream& os){
    os<<"��ǰ�����б�Ϊ��"<<std::endl;
    os<<"***********************************************************"<<std::endl;
    for(auto t:list){
        
        os<<"����ID\t���̵���ʱ��\t���̷���ʱ��\t���ȼ�\t���ʱ��"<<std::endl;
        os<<t.id<<"\t"<<t.arriveTime<<"\t\t"<<t.serverTime<<"\t\t"<<t.priority<<"\t"<<t.finishTime<<std::endl;
       
    }
    os<<"***********************************************************"<<std::endl;
}

//��������̵����ʱ��
void ComputerFinishTime(int N,std::list<PCB>& list){
    list.front().finishTime=list.front().arriveTime+list.front().serverTime;    //��һ�����̵����ʱ��
   
    int before=list.front().finishTime;         //��ȡ��һ���������ʱ��
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
//�����ȷ����㷨
void FCFS(int N,std::list<PCB>& list,float &T,float &W){
    float sumTime=0;    //sumTimeΪ����תʱ�䣬avgTimeΪ�ܼ�Ȩ��תʱ��
    float d=0,avgTime=0;//dΪ��Ȩ��תʱ��
    for_each(list.begin(),list.end(),[&sumTime,&d,&avgTime](PCB &p){
        d=(p.finishTime/p.serverTime);
        sumTime+=p.finishTime;
        avgTime+=d;
    });
    T=(float)sumTime/N;
    W=(float)avgTime/N;
}

//��̬���ȼ��㷨
void staticPriorty(int N,std::list<PCB> &list,float &T,float& W){
    //ͬʱ�䵽������̶��а������ȼ���С��������
    //list.sort([](PCB &a,PCB &b){ return a.priority<b.priority;});
    std::list<PCB> execlist,finishlist,reslist;
    list.sort([](PCB &p1,PCB &p2){return p1.arriveTime<p2.arriveTime;});
    execlist.push_back(list.front());           //�����ȵ���ķ������ж���
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
    //����ƽ����תʱ���ƽ����Ȩ��תʱ��
    float r=0,sumTime=0;
    float d=0,avgTime=0;
    //sumTimeΪ����תʱ�䣬avgTimeΪ�ܼ�Ȩ��תʱ��
    for_each(reslist.begin(),reslist.end(),[&sumTime,&d,&avgTime](PCB &p){
        d=(p.finishTime/p.serverTime);
        sumTime+=p.finishTime;
        avgTime+=d;
    });
    T=(float)sumTime/N;
    W=(float)avgTime/N;
}

// void staticPriorty2(int N,std::list<PCB> &list,float &T,float & W){
//     //�Ȱ��������ȷ�������
//     list.sort([](PCB & a,PCB & b){ return a.arriveTime < b.arriveTime; });
//     for_each(list.begin()+1,list.end(),[](PCB & p){
//         if(list.front().arriveTime < p.arriveTime){         //˭�ȵ�˭��ִ��
//             if()
//         }else if(list.front().arriveTime == p.arriveTime){  //���ͬʱ��

//         }
//     });
// }

//ʱ��Ƭ�л��㷨
//const LEN=1;        //ʱ��Ƭ����Ϊ1
void RR(int N,std::list<PCB>& list,float arriveTime[],float serverTime[],float timeLen,std::ostream& os){               //time Ϊʱ��Ƭ
    int counter=1,j=1;
    //���׸�����Ľ��̼�������б�
    PCB* p=new PCB;
    p->id=1000;
    p->arriveTime=arriveTime[0];
    p->serverTime=serverTime[0];
    p->priority=0;
    list.push_back(*p);
    while(!list.empty()){
        os<<"��ǰ���̶�����ϢΪ��"<<std::endl;
        os<<"��ǰʱ��"<<counter<<"\t�������еĽ��̣�"<<list.front().id<<std::endl;
        for(int i=j;i<N;i++){                   //�������ʱ��С�ڵ�ǰʱ�䣬��������б�
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
        if(list.front().serverTime<=timeLen){    //������׵ķ���ʱ��С��ʱ��Ƭ������ת��һ���̣����Ƴ�����Ԫ��
            list.front().finishTime=counter;
            counter+=list.front().serverTime;   //����ʱ�䲻�������ʱ��Ƭ���������֮��ɾ������
            ShowPCBList(list,os);
            list.erase(list.begin());
        }else{                                  //���򣬷���ʱ��=����ʱ��-ʱ��Ƭ����ת��һ���̣�������Ԫ��������β
            list.front().serverTime-=timeLen;
            ShowPCBList(list,os);
            list.push_back(list.front());
            list.erase(list.begin());
            counter+=timeLen;
        }
    }
}