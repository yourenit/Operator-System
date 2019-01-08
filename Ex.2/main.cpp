#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct Block{
	int begin;
	int end;
	size_t length;
};

//�����б�ͷ����б��ʼ��
void init(vector<Block> & freeLists,vector<Block> & allocedLists){
	freeLists.clear();
	allocedLists.clear();

	Block free;
	free.begin=0;
	free.end=512;
	free.length=512;
	freeLists.push_back(free);	//�ʼ���ڴ�ռ� 
}

//�״���Ӧ�㷨
void FirstFit(vector<Block> &freeLists,vector<Block> &allocedLists,int size){
	Block block;
	block.length=size;
	auto pos=find_if(freeLists.begin(),freeLists.end(),[&](Block & b){				//�ҵ���һ�����Է���Ŀ����ڴ��
		return b.length>=size;
	});
	if(pos==freeLists.end())cout<<"����ʧ�ܣ��޿��ÿռ䣡"<<endl;					  //û���ҵ����Է�����ڴ��
	else {
		block.begin=(*pos).begin;
		block.end=block.begin+size;
		(*pos).begin+=size;					//�����ڴ���׵�ַ�仯
		(*pos).length-=size;				//�����ڴ�鳤�ȱ仯
		allocedLists.push_back(block); 		//��ӵ��ѷ����б�
		sort(allocedLists.begin(),allocedLists.end(),[](Block &a,Block &b){			//���Է�����ڴ�鰴����ʼ��ַ���� 
			return a.begin<b.begin;
		});
	}
}

//�ͷ��ڴ��
void releaseBlock(vector<Block> & freeLists,vector<Block> & allocedLists,int size){
	Block block;
	block.length=size;
	auto pos=find_if(allocedLists.begin(),allocedLists.end(),[&](Block & b){			//�����Է����ڴ�� ,����Ҫ�ͷŵ��ڴ��
		return b.length==size;
	});
	if(pos==allocedLists.end())cout<<"δ�ҵ��Է���ô�С���ڴ�飡"<<endl;
	else{
		block.begin=(*pos).begin;
		block.end=block.begin+size;
		int isMerge=false;			//�����ж��Ƿ�ϲ��ɹ� 
					
		//�ϲ����ڿ����ڴ�� 
		for_each(freeLists.begin(),freeLists.end(),[&](Block & b){
			if(block.end==b.begin){
				b.begin-=block.length;
				b.length+=block.length;
				isMerge=true; 
			}
		});
		if(isMerge==false)freeLists.push_back(block);
		allocedLists.erase(pos);											//�ҵ�֮��ɾ�����ڴ�� 
		sort(freeLists.begin(),freeLists.end(),[](Block & a,Block & b){
			return a.begin<b.begin;
		});
	}
}

//��ӡ�ڴ����Ϣ
void printMemInfo(vector<Block> & freeLists,vector<Block> & allocedLists){
	cout<<"�����ڴ�����ϢΪ��\n";
	for_each(allocedLists.begin(),allocedLists.end(),[](Block & b){
		cout<<"�ڴ���С��"<<b.length<<"\t��ַ��ΧΪ��("<<b.begin<<" ~ "<<b.end<<")\n";	
	});
	cout<<"\n";
	cout<<"�Է�����ڴ����ϢΪ��\n";	
	for_each(freeLists.begin(),freeLists.end(),[](Block & b){
		cout<<"�ڴ���С��"<<b.length<<"\t��ַ��ΧΪ��("<<b.begin<<" ~ "<<b.end<<")\n";	
	});
}
//�����Ӧ�㷨
void BestFIt(vector<Block> &freeLists,vector<Block> &allocedLists,int size){
	Block block;
	block.length=size;
	sort(freeLists.begin(),freeLists.end(),[](Block & a,Block & b){			//���ݿ��пռ��С�������򣨴�С����
		return a.length<b.length;
	});
	
	auto pos=find_if(freeLists.begin(),freeLists.end(),[&](Block & b){
		return b.length>=size;
	});

	if(pos==freeLists.end())cout<<"�ڴ����ʧ�ܣ��޿��ÿռ䣡"<<endl;
	else{
		block.begin=(*pos).begin;
		block.end=block.begin+size;
		(*pos).begin=(*pos).begin+size;
		(*pos).length=(*pos).length-size;
		allocedLists.push_back(block);
		sort(freeLists.begin(),freeLists.end(),[](Block & a,Block & b){			//�����ڴ����ʼλ�ý�������
			return a.begin<b.begin;
		});

		sort(allocedLists.begin(),allocedLists.end(),[](Block & a,Block & b){	//�����ڴ����ʼλ�ý�������
			return a.begin<b.begin;		
		});
	}
}
int main(){
	vector<Block> freeLists;
	vector<Block> allocedLists;
	init(freeLists,allocedLists);		//��ʼ���ڴ�ռ�
	int flag;							//2Ϊ�ͷţ�1Ϊ���� 
	
	printMemInfo(freeLists,allocedLists);
	
	cout<< "��ѡ��Ҫ���еĲ�����"<<endl; 
	cout<< "1. �����ڴ� 2.�ͷ��ڴ�"<<endl;
	while(cin>>flag){
		switch(flag){
			case 1:{
				cout<<"���������Ĵ�С��" ;
				int size;
				cin>>size;							//�������Ĵ�С 
				//�״���Ӧ�㷨
				FirstFit(freeLists,allocedLists,size);
				//�����Ӧ�㷨
				//BestFIt(freeLists,allocedLists,size);
				break;
			}
			case 2:{
				cout<<"�������ͷŵĴ�С��";
				int size;
				cin>>size;
				releaseBlock(freeLists,allocedLists,size);		//�ͷ��ڴ�
				break;
			}
			default:{
				cout<<"�޴���ѡ�����������룡"<<endl; 
				break;
			}
		}

		printMemInfo(freeLists,allocedLists);
		
		cout<< "1. �����ڴ� 2.�ͷ��ڴ�"<<endl;
	}
	return 0;
}
