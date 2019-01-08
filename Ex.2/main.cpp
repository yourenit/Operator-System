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

//空闲列表和分配列表初始化
void init(vector<Block> & freeLists,vector<Block> & allocedLists){
	freeLists.clear();
	allocedLists.clear();

	Block free;
	free.begin=0;
	free.end=512;
	free.length=512;
	freeLists.push_back(free);	//最开始的内存空间 
}

//首次适应算法
void FirstFit(vector<Block> &freeLists,vector<Block> &allocedLists,int size){
	Block block;
	block.length=size;
	auto pos=find_if(freeLists.begin(),freeLists.end(),[&](Block & b){				//找到第一个可以分配的空闲内存块
		return b.length>=size;
	});
	if(pos==freeLists.end())cout<<"分配失败，无可用空间！"<<endl;					  //没有找到可以分配的内存块
	else {
		block.begin=(*pos).begin;
		block.end=block.begin+size;
		(*pos).begin+=size;					//空闲内存块首地址变化
		(*pos).length-=size;				//空闲内存块长度变化
		allocedLists.push_back(block); 		//添加到已分配列表
		sort(allocedLists.begin(),allocedLists.end(),[](Block &a,Block &b){			//将以分配的内存块按照起始地址排序 
			return a.begin<b.begin;
		});
	}
}

//释放内存块
void releaseBlock(vector<Block> & freeLists,vector<Block> & allocedLists,int size){
	Block block;
	block.length=size;
	auto pos=find_if(allocedLists.begin(),allocedLists.end(),[&](Block & b){			//遍历以分配内存块 ,找需要释放的内存块
		return b.length==size;
	});
	if(pos==allocedLists.end())cout<<"未找到以分配该大小的内存块！"<<endl;
	else{
		block.begin=(*pos).begin;
		block.end=block.begin+size;
		int isMerge=false;			//用来判断是否合并成功 
					
		//合并相邻空闲内存块 
		for_each(freeLists.begin(),freeLists.end(),[&](Block & b){
			if(block.end==b.begin){
				b.begin-=block.length;
				b.length+=block.length;
				isMerge=true; 
			}
		});
		if(isMerge==false)freeLists.push_back(block);
		allocedLists.erase(pos);											//找到之后，删除该内存块 
		sort(freeLists.begin(),freeLists.end(),[](Block & a,Block & b){
			return a.begin<b.begin;
		});
	}
}

//打印内存块信息
void printMemInfo(vector<Block> & freeLists,vector<Block> & allocedLists){
	cout<<"空闲内存块的信息为：\n";
	for_each(allocedLists.begin(),allocedLists.end(),[](Block & b){
		cout<<"内存块大小："<<b.length<<"\t地址范围为：("<<b.begin<<" ~ "<<b.end<<")\n";	
	});
	cout<<"\n";
	cout<<"以分配的内存块信息为：\n";	
	for_each(freeLists.begin(),freeLists.end(),[](Block & b){
		cout<<"内存块大小："<<b.length<<"\t地址范围为：("<<b.begin<<" ~ "<<b.end<<")\n";	
	});
}
//最佳适应算法
void BestFIt(vector<Block> &freeLists,vector<Block> &allocedLists,int size){
	Block block;
	block.length=size;
	sort(freeLists.begin(),freeLists.end(),[](Block & a,Block & b){			//根据空闲空间大小进行排序（从小到大）
		return a.length<b.length;
	});
	
	auto pos=find_if(freeLists.begin(),freeLists.end(),[&](Block & b){
		return b.length>=size;
	});

	if(pos==freeLists.end())cout<<"内存分配失败，无可用空间！"<<endl;
	else{
		block.begin=(*pos).begin;
		block.end=block.begin+size;
		(*pos).begin=(*pos).begin+size;
		(*pos).length=(*pos).length-size;
		allocedLists.push_back(block);
		sort(freeLists.begin(),freeLists.end(),[](Block & a,Block & b){			//根据内存块起始位置进行排序
			return a.begin<b.begin;
		});

		sort(allocedLists.begin(),allocedLists.end(),[](Block & a,Block & b){	//根据内存块起始位置进行排序
			return a.begin<b.begin;		
		});
	}
}
int main(){
	vector<Block> freeLists;
	vector<Block> allocedLists;
	init(freeLists,allocedLists);		//初始化内存空间
	int flag;							//2为释放，1为分配 
	
	printMemInfo(freeLists,allocedLists);
	
	cout<< "请选择要进行的操作："<<endl; 
	cout<< "1. 分配内存 2.释放内存"<<endl;
	while(cin>>flag){
		switch(flag){
			case 1:{
				cout<<"请输入分配的大小：" ;
				int size;
				cin>>size;							//输入分配的大小 
				//首次适应算法
				FirstFit(freeLists,allocedLists,size);
				//最佳适应算法
				//BestFIt(freeLists,allocedLists,size);
				break;
			}
			case 2:{
				cout<<"请输入释放的大小：";
				int size;
				cin>>size;
				releaseBlock(freeLists,allocedLists,size);		//释放内存
				break;
			}
			default:{
				cout<<"无此项选，请重新输入！"<<endl; 
				break;
			}
		}

		printMemInfo(freeLists,allocedLists);
		
		cout<< "1. 分配内存 2.释放内存"<<endl;
	}
	return 0;
}
