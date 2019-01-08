#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class Task{
public:
    Task(){}
    Task(int t,string n);
    ~Task(){};
    
public:
    string name;
    vector<int> Allocated;      //已分配资源数
    vector<int> Max;           //共需要的资源数
    vector<int> Need;           //还需要的资源数
    bool Finish;                //资源是否申请成功
};

Task::Task(int t,string n):Allocated(t),Max(t),Need(t),Finish(false),name(n){}

//初始化分配，返回有效资源向量
vector<int> CreateList(list<Task>& list,int task_num,int type_num,vector<int>& total){
    int index=0;                  //index用来表示创建进程数
    vector<int> available=total;
    while(index!=task_num){
        Task task(type_num,"P"+to_string(index+1));
        cout<<"请输入进程 "<<index<<" 的共需各类资源数：";
        for(int i=0;i<type_num;i++){
            cin>>task.Max[i];
        }
        cout<<"请输入进程 "<<index<<" 的已分配各类资源数：";
        for(int i=0;i<type_num;i++){
            cin>>task.Allocated[i];
        }
        //计算还需要的资源数
        cout<<"进程"<<index<<"还需要的资源数：";
        for(int i=0;i<type_num;i++){
            task.Need[i]=task.Max[i]-task.Allocated[i];
            cout<<task.Need[i]<<" ";
        }
        cout<<endl;
        //计算剩余资源数
        cout<<"系统还剩各类资源数目为：";
        for(int i=0;i<type_num;i++){
            available[i]-=task.Allocated[i];
            cout<<available[i]<<" ";
        }
        cout<<endl;
        list.push_back(task);
        index++;
    }
    return available;
}

//打印单个将进程的信息
void printTask(Task & t,vector<int> Available,int type_num){

        cout<<"|  "<<t.name<<"  |";
        for(int i=0;i<type_num;i++){
            cout<<"  "<<t.Max[i];
        }
        cout<<"|    ";
        for(int i=0;i<type_num;i++){
            cout<<" "<<t.Allocated[i];
        }
        cout<<"    |";
        for(int i=0;i<type_num;i++){
            cout<<"  "<<t.Need[i];
        }
        cout<<"|  ";
        for(int i=0;i<type_num;i++){
            cout<<"  "<<Available[i];
        }
        cout<<"   |"; 
        cout<<"\n----------------------------------------------"<<endl;
}

void printTable(list<Task> &list,vector<int> Available,int type_num){
    cout<<"----------------------------------------------"<<endl;
    cout<<"|进程名|  Max | Allocation | Need | Available |"<<endl;
    cout<<"----------------------------------------------"<<endl;
    for_each(list.begin(),list.end(),[=](Task t){
        printTask(t,Available,type_num);
    });
}

//安全性检查
bool analyseSafety(list<Task> list,vector<int> Available,vector<string> & safeList){
    int type_num=Available.size();
    int count=0;
    cout<<"安全性检查："<<endl;
    while(!list.empty()){
        int len=list.size();                            //用来判断是否有进程销毁
        for(auto it=list.begin();it!=list.end();){      //判断所有进程，如果存在安全序列，必有一个进程被销毁，即下面的满足条件
            count=0;                                    //用来判断可以分配的资源种类数 
            for(int i=0;i<type_num;i++){
                if(it->Need[i]<=Available[i]){count++;} 
            }
            
            if(count==type_num){                        //满足，则进行计算并且释放进程
                for(int i=0;i<type_num;i++){
                    Available[i]+=it->Allocated[i];     //更新有效资源数目
                }
                printTask(*it,Available,type_num);
                it->Finish=true;
                safeList.push_back(it->name);
                it=list.erase(it);                      //销毁进程
                break;
            }else it++;                                 //如果不满足，则判断下一个进程
        }
        if(len==list.size())return false;        //则判断死锁则判断是否有进程销毁，如果没有，则死锁
    }
    return true; 
}

bool request(list<Task> &list,vector<int>& Available,int type_num,string name,vector<string> & safeList){
    vector<int> Request(type_num);
    cout<<"请输入请求的各类资源数目：";
    for(int i=0;i<type_num;i++){
        cin>>Request[i];
    }
    
    for(auto it=list.begin();it!=list.end();it++){
        if(it->name==name){                         //找到请求资源的进程
            for(int i=0;i<type_num;i++){           
                if(Request[i]>it->Need[i]){         //判断Request<=Need
                    cout<<"请求的资源的大于还需要的资源数量，申请失败！"<<endl;
                    return false;
                }
            }
            for(int i=0;i<type_num;i++){           
                if(Request[i]>Available[i]){
                    cout<<it->name<<"等待"<<endl;
                    return false;                   //判断Request<=Available
                }
            }
            for(int i=0;i<type_num;i++){           //试分配
                it->Allocated[i]+=Request[i];
                it->Need[i]-=Request[i];
                Available[i]-=Request[i];
            }
        }
    }
    printTable(list,Available,type_num);
    
    //检查安全序列
    if(!analyseSafety(list,Available,safeList)){
        cout<<"可用资源Available不能满足任何进程的需要，故系统进出入不安全状态！"<<endl;
        return false;
    }
    for(auto it=list.begin();it!=list.end();){
        int count=0;
        for(int i=0;i<type_num;i++){
            if(it->Need[i]==0)count++;
        }
        if(count==type_num){
            for(int i=0;i<type_num;i++){
                Available[i]+=it->Max[i];
            }
            it=list.erase(it);
        }else{
            it++;
        }
    }
    return true;
}

bool release(list<Task> &list,vector<int> & Available,int type_num,string name){
    cout<<"请输入释放各类资源的数目：";
    vector<int> Release(type_num);
    for(int i=0;i<type_num;i++){
        cin>>Release[i];
    }
    for(auto it=list.begin();it!=list.end();it++){
        if(it->name==name){                         //找到请求资源的进程
            for(int i=0;i<type_num;i++){           
                if(Release[i]>it->Allocated[i]){    //判断Release<=Allocated
                    cout<<"释放的资源数目多余已分配的资源数目，释放失败！"<<endl;
                    return false;
                }
            }
            for(int i=0;i<type_num;i++){           
                it->Allocated[i]-=Release[i];
                it->Need[i]+=Release[i];
                Available[i]+=Release[i];
            }
        }
    }
    return true;
}

int main(){
    int select=1;
    cout<<"*********************************************************"<<endl;
    cout<<"*                 1. 设置资源种类数及资源总数           *"<<endl;
    cout<<"*                 2. 设置进程数及进程资源               *"<<endl;
    cout<<"*                 3. 检查当前时刻的死锁状态             *"<<endl;
    cout<<"*                 4. 申请资源                           *"<<endl;
    cout<<"*                 5. 释放资源                           *"<<endl;
    cout<<"*********************************************************"<<endl;
    list<Task> list;
    int type_num=0,task_num=0;
    vector<string> safelist;            //作为安全序列的输出容器
    vector<int> Available,Total;
    cout<<"请选择操作：";
    while(cin>>select){
        switch(select){
            case 1:{
                if(!Total.empty())Total.clear();
                cout<<"请输入资源的种类数：";
                cin>>type_num;
                //vector<int> Total(type_num);
                cout<<"请输入各类资源的最大数目：";
                for(int i=0;i<type_num;i++){
                    int t;
                    cin>>t;
                    Total.push_back(t);
                }
                cout<<"设置完毕！"<<endl;
                break;
            }
            case 2:{
                if(type_num==0){
                    cout<<"必须先设置选项1！\n";break;
                }
                if(!list.empty())list.clear();
                cout<<"请输入进程的数目：";
                cin>>task_num;
                Available=CreateList(list,task_num,type_num,Total);
                cout<<"设置完毕！"<<endl;
                cout<<"当前时刻的资源分配表："<<endl;
                printTable(list,Available,type_num);
                break;
            }
            case 3:{
                if(type_num==0 || task_num==0){
                    cout<<"必须先设置前两个选项！\n";break;
                }
                if(analyseSafety(list,Available,safelist)){
                    cout<<"此时刻的安全序列为：";
                    for_each(safelist.begin(),safelist.end(),[](string s){
                        cout<<s<<" ";
                    });
                    cout<<endl;
                }
                else cout<<"不安全！"<<endl;
                break;
            }
            case 4:{
                if(type_num==0 || task_num==0){
                    cout<<"必须先设置前两个选项！\n";break;
                }
                string name;
                safelist.clear();
                cout<<"请输入请求的进程名：";
                cin>>name;
                if(request(list,Available,type_num,name,safelist)){
                    cout<<"此时刻的安全序列为：";
                    for_each(safelist.begin(),safelist.end(),[](string s){
                        cout<<s<<" ";
                    });
                    cout<<endl;
                }
                safelist.clear();
                break;
            }
            case 5:{
                string name;
                cout<<"请输入请求的进程名：";
                cin>>name;
                if(!release(list,Available,type_num,name)){
                    cout<<"释放失败，请检查数据是否正确！"<<endl;
                }
                printTable(list,Available,type_num);
                break;
            }
            default:cout<<"无此选项，请重新输入！\n";break;
        }
        cout<<"*********************************************************"<<endl;
        cout<<"*                 1. 设置资源种类数及资源总数           *"<<endl;
        cout<<"*                 2. 设置进程数及进程资源               *"<<endl;
        cout<<"*                 3. 检查当前时刻的死锁状态             *"<<endl;
        cout<<"*                 4. 申请资源                           *"<<endl;
        cout<<"*                 5. 释放资源                           *"<<endl;
        cout<<"*********************************************************"<<endl;
        cout<<"请选择操作：";
    }
    return 0;
}