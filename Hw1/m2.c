#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
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
void setup(int block[2][2][4], int index[]){
	/*
  	block[0][0] : block_11		|-----|
	block[0][1] : block_12		|11|12|
	block[1][0] : block_21		|-----|
	block[1][1] : block_22		|21|22|
	                     		|-----|
	
	index: st1, end1, st2, end2
	*/
	block[0][0][0] = index[0];
	block[0][0][1] = index[0]+(index[1]-index[0])/2;
	block[0][0][2] = index[2];
	block[0][0][3] = index[2]+(index[3]-index[2])/2;

	block[0][1][0] = block[0][0][0];
	block[0][1][1] = block[0][0][1];
	block[0][1][2] = block[0][0][3];
	block[0][1][3] = index[3];

	block[1][0][0] = block[0][0][1];
	block[1][0][1] = index[1];
	block[1][0][2] = block[0][0][2];
	block[1][0][3] = block[0][0][3];

	block[1][1][0] = block[1][0][0];
	block[1][1][1] = block[1][0][1];
	block[1][1][2] = block[0][1][2];
	block[1][1][3] = block[0][1][3];
	
}
void SMMR(int **m1, int **m2, int **m3, int m1_index[], int m2_index[], int m3_index[]){
	int dim = m1_index[1]-m1_index[0];
	if(dim < 8){
		for(int i=0;i<dim;i++)
			for(int j=0;j<dim;j++)
				for(int k=0;k<dim;k++)
					m3[m3_index[0]+i][m3_index[2]+j] += m1[m1_index[0]+i][m1_index[2]+k]*m2[m2_index[0]+k][m2_index[2]+j];
					
		return ;
	}
	else{
		int A[2][2][4]; //int A11[4], A12[4], A21[4], A22[4];
		int B[2][2][4]; //int B11[4], B12[4], B21[4], B22[4];
		int C[2][2][4]; //int C11[4], C12[4], C21[4], C22[4];
		setup(A,m1_index);
		setup(B,m2_index);
		setup(C,m3_index);
		
		SMMR(m1,m2,m3,A[0][0],B[0][0],C[0][0]);	
		SMMR(m1,m2,m3,A[0][1],B[1][0],C[0][0]);
	
		SMMR(m1,m2,m3,A[0][0],B[0][1],C[0][1]);	
		SMMR(m1,m2,m3,A[0][1],B[1][1],C[0][1]);	

		SMMR(m1,m2,m3,A[1][0],B[0][0],C[1][0]);	
		SMMR(m1,m2,m3,A[1][1],B[1][0],C[1][0]);

		SMMR(m1,m2,m3,A[1][0],B[0][1],C[1][1]);	
		SMMR(m1,m2,m3,A[1][1],B[1][1],C[1][1]);	
		
		return ;		
	}
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

	int index[4]={0, dim, 0, dim};
	SMMR(m1,m2,m3,index,index,index);

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
