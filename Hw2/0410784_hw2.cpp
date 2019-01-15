#include<iostream>
#include<cstdlib>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;
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
int main(int argc, char* argv[]){
	
	//INITIAL
	vector<struct block> blocks;
	vector<int> *build;	
	int ans = 0, max_index;
	int *DP;
	ofstream out;
	ifstream in;
	string inStr;
	//INPUT
	in.open(argv[1]);
	while(getline(in, inStr)){
		inStr = inStr.substr(1,inStr.length()-2);
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
	
	sort(blocks.begin(), blocks.end(), comp);
	
	DP = new int[blocks.size()];
	build = new vector<int>[blocks.size()];
	for(int i=0;i<blocks.size();i++){
		DP[i] = blocks[i].h;
		build[i].push_back(i);
		for(int j=0;j<i;j++){ 
			if(blocks[i].l < blocks[j].l && blocks[i].w < blocks[j].w){
				if( blocks[build[j][build[j].size()-1]].color != blocks[i].color && DP[i] < DP[j]+blocks[i].h ){
					DP[i]=DP[j]+blocks[i].h;
					build[i].clear();
					build[i].assign(build[j].begin(),build[j].end());
					build[i].push_back(i);
				}
			}
		}
	}
	for(int i=0;i<blocks.size();i++){
		if(DP[i]>ans){
			ans = DP[i];
			max_index = i;
		}
	}
	
	//OUTPUT
	out.open(argv[2], ofstream::out);
	out<<ans<<endl;
	for(int i=0;i<build[max_index].size();i++){
		out<<"("<<blocks[build[max_index][i]].h<<","<<blocks[build[max_index][i]].l<<","<<blocks[build[max_index][i]].w<<")"<<endl;
	}
	out.close();

	delete []DP;
	delete []build;
	return 0;	
}
