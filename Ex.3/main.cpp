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
    vector<int> Allocated;      //�ѷ�����Դ��
    vector<int> Max;           //����Ҫ����Դ��
    vector<int> Need;           //����Ҫ����Դ��
    bool Finish;                //��Դ�Ƿ�����ɹ�
};

Task::Task(int t,string n):Allocated(t),Max(t),Need(t),Finish(false),name(n){}

//��ʼ�����䣬������Ч��Դ����
vector<int> CreateList(list<Task>& list,int task_num,int type_num,vector<int>& total){
    int index=0;                  //index������ʾ����������
    vector<int> available=total;
    while(index!=task_num){
        Task task(type_num,"P"+to_string(index+1));
        cout<<"��������� "<<index<<" �Ĺ��������Դ����";
        for(int i=0;i<type_num;i++){
            cin>>task.Max[i];
        }
        cout<<"��������� "<<index<<" ���ѷ��������Դ����";
        for(int i=0;i<type_num;i++){
            cin>>task.Allocated[i];
        }
        //���㻹��Ҫ����Դ��
        cout<<"����"<<index<<"����Ҫ����Դ����";
        for(int i=0;i<type_num;i++){
            task.Need[i]=task.Max[i]-task.Allocated[i];
            cout<<task.Need[i]<<" ";
        }
        cout<<endl;
        //����ʣ����Դ��
        cout<<"ϵͳ��ʣ������Դ��ĿΪ��";
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

//��ӡ���������̵���Ϣ
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
    cout<<"|������|  Max | Allocation | Need | Available |"<<endl;
    cout<<"----------------------------------------------"<<endl;
    for_each(list.begin(),list.end(),[=](Task t){
        printTask(t,Available,type_num);
    });
}

//��ȫ�Լ��
bool analyseSafety(list<Task> list,vector<int> Available,vector<string> & safeList){
    int type_num=Available.size();
    int count=0;
    cout<<"��ȫ�Լ�飺"<<endl;
    while(!list.empty()){
        int len=list.size();                            //�����ж��Ƿ��н�������
        for(auto it=list.begin();it!=list.end();){      //�ж����н��̣�������ڰ�ȫ���У�����һ�����̱����٣����������������
            count=0;                                    //�����жϿ��Է������Դ������ 
            for(int i=0;i<type_num;i++){
                if(it->Need[i]<=Available[i]){count++;} 
            }
            
            if(count==type_num){                        //���㣬����м��㲢���ͷŽ���
                for(int i=0;i<type_num;i++){
                    Available[i]+=it->Allocated[i];     //������Ч��Դ��Ŀ
                }
                printTask(*it,Available,type_num);
                it->Finish=true;
                safeList.push_back(it->name);
                it=list.erase(it);                      //���ٽ���
                break;
            }else it++;                                 //��������㣬���ж���һ������
        }
        if(len==list.size())return false;        //���ж��������ж��Ƿ��н������٣����û�У�������
    }
    return true; 
}

bool request(list<Task> &list,vector<int>& Available,int type_num,string name,vector<string> & safeList){
    vector<int> Request(type_num);
    cout<<"����������ĸ�����Դ��Ŀ��";
    for(int i=0;i<type_num;i++){
        cin>>Request[i];
    }
    
    for(auto it=list.begin();it!=list.end();it++){
        if(it->name==name){                         //�ҵ�������Դ�Ľ���
            for(int i=0;i<type_num;i++){           
                if(Request[i]>it->Need[i]){         //�ж�Request<=Need
                    cout<<"�������Դ�Ĵ��ڻ���Ҫ����Դ����������ʧ�ܣ�"<<endl;
                    return false;
                }
            }
            for(int i=0;i<type_num;i++){           
                if(Request[i]>Available[i]){
                    cout<<it->name<<"�ȴ�"<<endl;
                    return false;                   //�ж�Request<=Available
                }
            }
            for(int i=0;i<type_num;i++){           //�Է���
                it->Allocated[i]+=Request[i];
                it->Need[i]-=Request[i];
                Available[i]-=Request[i];
            }
        }
    }
    printTable(list,Available,type_num);
    
    //��鰲ȫ����
    if(!analyseSafety(list,Available,safeList)){
        cout<<"������ԴAvailable���������κν��̵���Ҫ����ϵͳ�����벻��ȫ״̬��"<<endl;
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
    cout<<"�������ͷŸ�����Դ����Ŀ��";
    vector<int> Release(type_num);
    for(int i=0;i<type_num;i++){
        cin>>Release[i];
    }
    for(auto it=list.begin();it!=list.end();it++){
        if(it->name==name){                         //�ҵ�������Դ�Ľ���
            for(int i=0;i<type_num;i++){           
                if(Release[i]>it->Allocated[i]){    //�ж�Release<=Allocated
                    cout<<"�ͷŵ���Դ��Ŀ�����ѷ������Դ��Ŀ���ͷ�ʧ�ܣ�"<<endl;
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
    cout<<"*                 1. ������Դ����������Դ����           *"<<endl;
    cout<<"*                 2. ���ý�������������Դ               *"<<endl;
    cout<<"*                 3. ��鵱ǰʱ�̵�����״̬             *"<<endl;
    cout<<"*                 4. ������Դ                           *"<<endl;
    cout<<"*                 5. �ͷ���Դ                           *"<<endl;
    cout<<"*********************************************************"<<endl;
    list<Task> list;
    int type_num=0,task_num=0;
    vector<string> safelist;            //��Ϊ��ȫ���е��������
    vector<int> Available,Total;
    cout<<"��ѡ�������";
    while(cin>>select){
        switch(select){
            case 1:{
                if(!Total.empty())Total.clear();
                cout<<"��������Դ����������";
                cin>>type_num;
                //vector<int> Total(type_num);
                cout<<"�����������Դ�������Ŀ��";
                for(int i=0;i<type_num;i++){
                    int t;
                    cin>>t;
                    Total.push_back(t);
                }
                cout<<"������ϣ�"<<endl;
                break;
            }
            case 2:{
                if(type_num==0){
                    cout<<"����������ѡ��1��\n";break;
                }
                if(!list.empty())list.clear();
                cout<<"��������̵���Ŀ��";
                cin>>task_num;
                Available=CreateList(list,task_num,type_num,Total);
                cout<<"������ϣ�"<<endl;
                cout<<"��ǰʱ�̵���Դ�����"<<endl;
                printTable(list,Available,type_num);
                break;
            }
            case 3:{
                if(type_num==0 || task_num==0){
                    cout<<"����������ǰ����ѡ�\n";break;
                }
                if(analyseSafety(list,Available,safelist)){
                    cout<<"��ʱ�̵İ�ȫ����Ϊ��";
                    for_each(safelist.begin(),safelist.end(),[](string s){
                        cout<<s<<" ";
                    });
                    cout<<endl;
                }
                else cout<<"����ȫ��"<<endl;
                break;
            }
            case 4:{
                if(type_num==0 || task_num==0){
                    cout<<"����������ǰ����ѡ�\n";break;
                }
                string name;
                safelist.clear();
                cout<<"����������Ľ�������";
                cin>>name;
                if(request(list,Available,type_num,name,safelist)){
                    cout<<"��ʱ�̵İ�ȫ����Ϊ��";
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
                cout<<"����������Ľ�������";
                cin>>name;
                if(!release(list,Available,type_num,name)){
                    cout<<"�ͷ�ʧ�ܣ����������Ƿ���ȷ��"<<endl;
                }
                printTable(list,Available,type_num);
                break;
            }
            default:cout<<"�޴�ѡ����������룡\n";break;
        }
        cout<<"*********************************************************"<<endl;
        cout<<"*                 1. ������Դ����������Դ����           *"<<endl;
        cout<<"*                 2. ���ý�������������Դ               *"<<endl;
        cout<<"*                 3. ��鵱ǰʱ�̵�����״̬             *"<<endl;
        cout<<"*                 4. ������Դ                           *"<<endl;
        cout<<"*                 5. �ͷ���Դ                           *"<<endl;
        cout<<"*********************************************************"<<endl;
        cout<<"��ѡ�������";
    }
    return 0;
}