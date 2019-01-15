#include<iostream>
#include<stdio.h>
#include<limits.h>
#include<string>
#include<algorithm>
using namespace std;
 
// every block has 3 choice


struct block
{
	int x,y,z;
};
 
bool comp(const block& a,const block& b) //
{
	if( a.x == b.x)
		return a.y > b.y;
	return a.x > b.x;
}
 
block BK[200];
int index,T,ans;
int DP[200];

void DP_TR(){
	int i,j;
	for(i=0; i < index; ++i)
	{
		DP[i] = BK[i].z;
		for(j=0; j < i; ++j)
		{
			if( BK[i].x < BK[j].x && BK[i].y < BK[j].y)
				DP[i] = max(DP[i],DP[j]+BK[i].z);
		}
	}
	ans = INT_MIN;
	for(i=0; i<index; ++i)
	{
		if( DP[i] > ans )
			ans = DP[i];
	}

}

int main()
{	
	int arr[3],i=1;
	while(cin >> T && T !=0 )
	{
		//printf("Block nums: %d\n",T);
		index = 0;
		int t=T;
		while( T-- )
		{
			cin >> arr[0] >> arr[1] >> arr[2];
		
			sort(arr,arr+3);
			BK[index].x = arr[0];
			BK[index].y = arr[1];
			BK[index].z = arr[2];
			++index;
 
			BK[index].x = arr[0];
			BK[index].y = arr[2];
			BK[index].z = arr[1];
			++index;
 
			BK[index].x = arr[1];
			BK[index].y = arr[2];
			BK[index].z = arr[0];
			++index;
		}
		for(int a=0;a<3*t;a++)
			printf("BK[%d]: %d,%d,%d ",a,BK[a].x,BK[a].y,BK[a].z);
		printf("\n");
		sort(BK,BK+index,comp);
		for(int a=0;a<3*t;a++)
			printf("BK[%d]: %d,%d,%d ",a,BK[a].x,BK[a].y,BK[a].z);
		printf("\n");
		DP_TR();
		
		printf("Case %d: maximum height = %d\n",i++,ans);
	}
	return 0;
}
