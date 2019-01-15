#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<sys/time.h>
using namespace std;
vector<struct block> blocks;
vector<int> *build;	
int *DP;
int *sub;
int counts=0;
struct block{
	int l,w; //short, long 
	int h;
	char color; // R, G, B
};
bool comp(const struct block& b1, const struct block& b2){
	if(b1.l==b2.l)
		return (b1.w>b2.w);
	return (b1.l>b2.l);
}
int find_max(int block_now_index){
	//cout<<block_now_index<<endl;
	int max = blocks[block_now_index].h;
	
	if(sub[block_now_index] != 0) 
		return DP[block_now_index];
	counts++;	
	for(int i=block_now_index-1;i>=0;i--){
		max = find_max(i);
		if(blocks[block_now_index].l < blocks[i].l && blocks[block_now_index].w < blocks[i].w){
			if( blocks[build[i][build[i].size()-1]].color != blocks[block_now_index].color && DP[block_now_index] < max+blocks[block_now_index].h ){
				DP[block_now_index]=DP[i]+blocks[block_now_index].h;
				build[block_now_index].clear();
				build[block_now_index].assign(build[i].begin(),build[i].end());
				build[block_now_index].push_back(block_now_index);
			}
		}
	}
	/*
	cout<<"build[block_now_index]: "<<block_now_index<<" // ";
	for(int j=0;j<build[block_now_index].size();j++)
		cout<<build[block_now_index][j]<<" ";
	cout<<endl;
	*/
	sub[block_now_index]=1;
	return max;
}
int main(int argc, char* argv[]){
	
	//INITIAL
	int ans = 0, max_index;
	ofstream out;
	ifstream in;
	string inStr;
	struct timeval start, end;
	int sec, usec;

	//INPUT
	in.open(argv[1]);
	while(getline(in, inStr)){
		inStr = inStr.substr(1,inStr.length()-2);
		//cout<<inStr<<endl;
		int pos = 0, count = 0;
		string delim = ",";
		int sides[3];
		char faces[3];

		while((pos = inStr.find(delim)) != string::npos){
			string token = inStr.substr(0,pos);	
			if(count<3){
				sides[count++] = atoi(token.c_str()); 
			}
			else{//Preprocess Faces for height<-->color
				faces[(count+2)%3] = inStr.c_str()[0];
				count++;
			}
			inStr.erase(0,pos+delim.length());
		}
		faces[(count+2)%3] = inStr.c_str()[0];
		
		//Generate blocks
		for(int i=0;i<3;i++){
			struct block* ptr = new struct block;
			ptr->h = sides[i];
			ptr->color = faces[i];
			int l=0,w;
			for(int j=0;j<3;j++){
				if(j == i)	continue;
				if(l == 0){
					l=sides[j];
					continue;
				}
				if(sides[j] < l){
					w = l;
					l = sides[j];
				}
				else
					w=sides[j];
			}
			ptr->l = l;
			ptr->w = w;
			blocks.push_back(*ptr);
		}
	}
	in.close();
	/*
 	cout<<blocks.size()<<endl;
	cout<<"length, width, height, color: "<<endl;
	for(int i=0;i<blocks.size();i++){
		cout<<blocks[i].l<<", "<<blocks[i].w<<", "<<blocks[i].h<<", "<<blocks[i].color<<endl;
	}
	*/
	
	//Dymanic Programing
	/*
	for(int i=0;i<blocks.size();i++){
		cout<<"blocks["<<i<<"]: "<<blocks[i].l<<", "<<blocks[i].w<<", "<<blocks[i].h<<", "<<blocks[i].color<<endl;
	}
	cout<<endl;
	*/
	gettimeofday(&start,0);
	sort(blocks.begin(), blocks.end(), comp);
	/*
	for(int i=0;i<blocks.size();i++){
		cout<<"blocks["<<i<<"]: "<<blocks[i].h<<", "<<blocks[i].l<<", "<<blocks[i].w<<", "<<blocks[i].color<<endl;
	}
	cout<<endl;
	*/
	DP = new int[blocks.size()];
	sub = new int[blocks.size()];
	build = new vector<int>[blocks.size()];
	//recursive
	for(int i=0;i<blocks.size();i++){
		DP[i] = blocks[i].h;
		build[i].push_back(i);
		sub[i]=0;
	}
	find_max(blocks.size()-1);
	
	//DP
	
	for(int i=0;i<blocks.size();i++){
		if(DP[i]>ans){
			ans = DP[i];
			max_index = i;
		}
	}
	
	cout<<ans<<endl;
	for(int i=0;i<build[max_index].size();i++){
		cout<<"("<<blocks[build[max_index][i]].h<<","<<blocks[build[max_index][i]].l<<","<<blocks[build[max_index][i]].w<<")"<<blocks[build[max_index][i]].color<<endl;
	}
	cout<<"recursive count: "<<counts<<endl;
	//OUTPUT
	out.open(argv[2], ofstream::out);
	out<<ans<<endl;
	for(int i=0;i<build[max_index].size();i++){
		out<<"("<<blocks[build[max_index][i]].h<<","<<blocks[build[max_index][i]].l<<","<<blocks[build[max_index][i]].w<<")"<<endl;
	}
	out.close();
	
	gettimeofday(&end,0);
	sec = end.tv_sec - start.tv_sec;
	usec = end.tv_usec - start.tv_usec;
	cout<<"time: "<<sec+usec/1e6<<"sec."<<endl;
	
	delete []sub;
	delete []DP;
	delete []build;
	return 0;	
}
