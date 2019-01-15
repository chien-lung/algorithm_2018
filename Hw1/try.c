#include<stdio.h>
#define N 3
void recreate(int **&m){
	int **m_new;
	int dim = N+1;

	m_new = new int*[dim];
	for(int i=0;i<dim;i++){
		m_new[i] = new int[dim];
		for(int j=0;j<dim;j++)
			m_new[i][j] = 0;
	}

	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++)
			m_new[i][j] = m[i][j];
	}
	
	for(int i=0;i<N;i++)
		delete []m[i];
	delete []m;
	m = m_new;
}
int main(){
	int **m1;
	m1 = new int*[N];
	for(int i=0;i<N;i++){
		m1[i] = new int[N];
		for(int j=0;j<N;j++)
			m1[i][j] = (i+1)*(j+1);
	}
	
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++)
			printf("%d ",m1[i][j]);
		printf("\n");
	}
	printf("---------------\n");
	recreate(m1);
	for(int i=0;i<N+1;i++){
		for(int j=0;j<N+1;j++)
			printf("%d ",m1[i][j]);
		printf("\n");
	}
	

}
