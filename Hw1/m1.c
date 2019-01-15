#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
int sub = 0;
FILE* f1 = fopen("b.txt","w");
int initialize(int **&m1, int **&m2, int **&m3, char* filename){
	FILE *fr;
	int dim;
	
	fr = fopen(filename,"r");
	if(fr){
		fscanf(fr,"%d ", &dim);
		m1 = new int*[dim];
		m2 = new int*[dim];
		m3 = new int*[dim];
		for(int i=0;i<dim;i++){
			m1[i] = new int[dim];
			m2[i] = new int[dim];
			m3[i] = new int[dim];
			for(int j=0;j<dim;j++)
				m3[i][j] = 0;
		}
		for(int i=0;i<dim;i++)
			for(int j=0;j<dim;j++)
				fscanf(fr,"%d ",&m1[i][j]);
		for(int i=0;i<dim;i++)
			for(int j=0;j<dim;j++)
				fscanf(fr,"%d ",&m2[i][j]);
	
		fclose(fr);
		return dim;
	}
	return 0;
}
void clear(int **&m1, int **&m2, int **&m3, int dim){
	for(int i=0;i<dim;i++){
		delete []m1[i];
		delete []m2[i];
		delete []m3[i];
	}
	delete []m1;
	delete []m2;
	delete []m3;
}
int main(int argc, char* argv[]){
	FILE *fw;
	int **m1, **m2, **m3;
	int dim, sec, usec;
	struct timeval start, end;

	fw = fopen(argv[2],"w");
	dim = initialize(m1,m2,m3,argv[1]);
	if(dim == 0){
		fprintf(stderr,"input file not exist\n");
		exit(1);
	}
	
	gettimeofday(&start,0);
	for(int i=0;i<dim;i++)
		for(int j=0;j<dim;j++)
			for(int k=0;k<dim;k++){
				m3[i][j] += m1[i][k]*m2[k][j];	
				if(sub!=m3[0][0]){
					fprintf(f1,"%d\n",m3[0][0]-sub);
					sub = m3[0][0];
				}
			}
	
	for(int i=0;i<dim;i++){
		for(int j=0;j<dim;j++)
			fprintf(fw,"%d ",m3[i][j]);
		fprintf(fw,"\n");
	}
	gettimeofday(&end,0);
	sec = end.tv_sec - start.tv_sec;
	usec = end.tv_usec - start.tv_usec;
	printf("time: %.5fsec.\n",sec+usec/1e6);		
	fclose(fw);	

	clear(m1,m2,m3,dim);
	return 0;
}
