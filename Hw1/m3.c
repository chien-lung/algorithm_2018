#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
//#define N 60
int N;
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

void create_m(int **&m, int dim){
	m = new int*[dim];
	for(int i=0;i<dim;i++){
		m[i] = new int[dim];
		for(int j=0;j<dim;j++)
			m[i][j]=0;
	}
}
void remove_m(int **&m, int dim){
	for(int i=0;i<dim;i++)
		delete []m[i];
	delete []m;
}
void sub_m(int **m1, int **m2, int **ans, int m1_index[], int m2_index[]){
	int dim = m1_index[1]-m1_index[0];
	
	for(int i=0;i<dim;i++)
		for(int j=0;j<dim;j++)
			ans[i][j] = m1[m1_index[0]+i][m1_index[2]+j] - m2[m2_index[0]+i][m2_index[2]+j];
}
void add_m(int **m1, int **m2, int **ans, int m1_index[], int m2_index[]){
	int dim = m1_index[1]-m1_index[0];
	
	for(int i=0;i<dim;i++)
		for(int j=0;j<dim;j++)
			ans[i][j] = m1[m1_index[0]+i][m1_index[2]+j] + m2[m2_index[0]+i][m2_index[2]+j];
}

void recreate_even_m(int **&m_old, int **&m_new, int m_old_index[4]){
	int dim_old = m_old_index[1]-m_old_index[0];
	int dim_new = dim_old+1;

	m_new = new int*[dim_new];
	for(int i=0;i<dim_new;i++){
		m_new[i] = new int[dim_new];
		for(int j=0;j<dim_new;j++){
			if(i<dim_old && j<dim_old)
				m_new[i][j] = m_old[m_old_index[0]+i][m_old_index[2]+j];
			else
				m_new[i][j] = 0;
		}
	}
}

void Strassen(int **m1, int **m2, int **m3, int m1_index[], int m2_index[], int m3_index[]){
	int dim = m1_index[1]-m1_index[0];
	if(dim < N){
		//m3[m3_index[0]][m3_index[2]] += m1[m1_index[0]][m1_index[2]]*m2[m2_index[0]][m2_index[2]];
		
		for(int i=0;i<dim;i++)
			for(int j=0;j<dim;j++)
				for(int k=0;k<dim;k++)
					m3[m3_index[0]+i][m3_index[2]+j] += m1[m1_index[0]+i][m1_index[2]+k]*m2[m2_index[0]+k][m2_index[2]+j];
				
		return ;
	}
	else if(dim%2 == 1){ //odd dimension
		int dim_new = dim+1;
		int **m1_new, **m2_new, **m3_new;
		int m_new_index[4]={0,dim_new,0,dim_new};
		
		recreate_even_m(m1,m1_new,m1_index);
		recreate_even_m(m2,m2_new,m2_index);
		recreate_even_m(m3,m3_new,m3_index);

		int A[2][2][4]; //int A11[4], A12[4], A21[4], A22[4];
		int B[2][2][4]; //int B11[4], B12[4], B21[4], B22[4];
		int C[2][2][4]; //int C11[4], C12[4], C21[4], C22[4];
		setup(A,m_new_index);
		setup(B,m_new_index);
		setup(C,m_new_index);
		
		int **S1, **S2, **P[7];
		int sub_dim = dim_new/2;
		int sp_index[]={0,sub_dim,0,sub_dim};

		//S1=B12-B22, P1=A11*S1
		create_m(S1,sub_dim);
		create_m(P[0],sub_dim);
		sub_m(m2_new,m2_new,S1, B[0][1],B[1][1]);
		Strassen(m1_new,S1,P[0], A[0][0],sp_index,sp_index);
		remove_m(S1,sub_dim);		
		//S2=A11+A12, P2=S2*B22
		create_m(S2,sub_dim);
		create_m(P[1],sub_dim);
		add_m(m1_new,m1_new,S2, A[0][0],A[0][1]);
		Strassen(S2,m2_new,P[1], sp_index,B[1][1],sp_index);
		remove_m(S2,sub_dim);
		//S3=A21+A22, P3=S3*B11
		create_m(S1,sub_dim);
		create_m(P[2],sub_dim);
		add_m(m1_new,m1_new,S1 ,A[1][0],A[1][1]);
		Strassen(S1,m2_new,P[2], sp_index,B[0][0],sp_index);
		remove_m(S1,sub_dim);
		//S4=B21-B11, P4=A22*S4
		create_m(S2,sub_dim);
		create_m(P[3],sub_dim);
		sub_m(m2_new,m2_new,S2, B[1][0],B[0][0]);
		Strassen(m1_new,S2,P[3], A[1][1],sp_index,sp_index);
		remove_m(S2,sub_dim);
		//S5=A11+A22, S6=B11+B22, P5=S5*S6
		create_m(S1,sub_dim);
		create_m(S2,sub_dim);
		create_m(P[4],sub_dim);
		add_m(m1_new,m1_new,S1, A[0][0],A[1][1]);
		add_m(m2_new,m2_new,S2, B[0][0],B[1][1]);
		Strassen(S1,S2,P[4], sp_index,sp_index,sp_index);
		remove_m(S1,sub_dim);
		remove_m(S2,sub_dim);
		//S7=A12-A22, S8=B21+B22, P6=S7*S8
		create_m(S1,sub_dim);
		create_m(S2,sub_dim);
		create_m(P[5],sub_dim);
		sub_m(m1_new,m1_new,S1, A[0][1],A[1][1]);
		add_m(m2_new,m2_new,S2, B[1][0],B[1][1]);
		Strassen(S1,S2,P[5], sp_index,sp_index,sp_index);
		remove_m(S1,sub_dim);
		remove_m(S2,sub_dim);
		//S9=A11-A21, S10=B11+B12, P7=S9*S10
		create_m(S1,sub_dim);
		create_m(S2,sub_dim);
		create_m(P[6],sub_dim);
		sub_m(m1_new,m1_new,S1, A[0][0],A[1][0]);
		add_m(m2_new,m2_new,S2, B[0][0],B[0][1]);
		Strassen(S1,S2,P[6], sp_index,sp_index,sp_index);
		remove_m(S1,sub_dim);
		remove_m(S2,sub_dim);
		
		//C11=P5+P4-P2+P6
		for(int i=0;i<sub_dim;i++)
			for(int j=0;j<sub_dim;j++)
				m3_new[C[0][0][0]+i][C[0][0][2]+j] = P[4][i][j]+P[3][i][j]-P[1][i][j]+P[5][i][j];
		//C12=P1+P2
		for(int i=0;i<sub_dim;i++)
			for(int j=0;j<sub_dim;j++)
				m3_new[C[0][1][0]+i][C[0][1][2]+j] = P[0][i][j]+P[1][i][j];
		//C21=P3+P4
		for(int i=0;i<sub_dim;i++)
			for(int j=0;j<sub_dim;j++)
				m3_new[C[1][0][0]+i][C[1][0][2]+j] = P[2][i][j]+P[3][i][j];
		//C22=P5+P1-P3-P7
		for(int i=0;i<sub_dim;i++)
			for(int j=0;j<sub_dim;j++)
				m3_new[C[1][1][0]+i][C[1][1][2]+j] = P[4][i][j]+P[0][i][j]-P[2][i][j]-P[6][i][j];
		
		for(int i=0;i<dim;i++)
			for(int j=0;j<dim;j++)
				m3[i][j] = m3_new[i][j];
		
		clear(m1_new,m2_new,m3_new,dim_new);
	}
	else{
		int A[2][2][4]; //int A11[4], A12[4], A21[4], A22[4];
		int B[2][2][4]; //int B11[4], B12[4], B21[4], B22[4];
		int C[2][2][4]; //int C11[4], C12[4], C21[4], C22[4];
		setup(A,m1_index);
		setup(B,m2_index);
		setup(C,m3_index);
		
		int **S1, **S2, **P[7];
		int sub_dim = dim/2;
		int sp_index[]={0,sub_dim,0,sub_dim};

		//S1=B12-B22, P1=A11*S1
		create_m(S1,sub_dim);
		create_m(P[0],sub_dim);
		sub_m(m2,m2,S1, B[0][1],B[1][1]);
		Strassen(m1,S1,P[0], A[0][0],sp_index,sp_index);
		remove_m(S1,sub_dim);		
		//S2=A11+A12, P2=S2*B22
		create_m(S2,sub_dim);
		create_m(P[1],sub_dim);
		add_m(m1,m1,S2, A[0][0],A[0][1]);
		Strassen(S2,m2,P[1], sp_index,B[1][1],sp_index);
		remove_m(S2,sub_dim);
		//S3=A21+A22, P3=S3*B11
		create_m(S1,sub_dim);
		create_m(P[2],sub_dim);
		add_m(m1,m1,S1 ,A[1][0],A[1][1]);
		Strassen(S1,m2,P[2], sp_index,B[0][0],sp_index);
		remove_m(S1,sub_dim);
		//S4=B21-B11, P4=A22*S4
		create_m(S2,sub_dim);
		create_m(P[3],sub_dim);
		sub_m(m2,m2,S2, B[1][0],B[0][0]);
		Strassen(m1,S2,P[3], A[1][1],sp_index,sp_index);
		remove_m(S2,sub_dim);
		//S5=A11+A22, S6=B11+B22, P5=S5*S6
		create_m(S1,sub_dim);
		create_m(S2,sub_dim);
		create_m(P[4],sub_dim);
		add_m(m1,m1,S1, A[0][0],A[1][1]);
		add_m(m2,m2,S2, B[0][0],B[1][1]);
		Strassen(S1,S2,P[4], sp_index,sp_index,sp_index);
		remove_m(S1,sub_dim);
		remove_m(S2,sub_dim);
		//S7=A12-A22, S8=B21+B22, P6=S7*S8
		create_m(S1,sub_dim);
		create_m(S2,sub_dim);
		create_m(P[5],sub_dim);
		sub_m(m1,m1,S1, A[0][1],A[1][1]);
		add_m(m2,m2,S2, B[1][0],B[1][1]);
		Strassen(S1,S2,P[5], sp_index,sp_index,sp_index);
		remove_m(S1,sub_dim);
		remove_m(S2,sub_dim);
		//S9=A11-A21, S10=B11+B12, P7=S9*S10
		create_m(S1,sub_dim);
		create_m(S2,sub_dim);
		create_m(P[6],sub_dim);
		sub_m(m1,m1,S1, A[0][0],A[1][0]);
		add_m(m2,m2,S2, B[0][0],B[0][1]);
		Strassen(S1,S2,P[6], sp_index,sp_index,sp_index);
		remove_m(S1,sub_dim);
		remove_m(S2,sub_dim);

		//C11=P5+P4-P2+P6
		for(int i=0;i<sub_dim;i++)
			for(int j=0;j<sub_dim;j++)
				m3[C[0][0][0]+i][C[0][0][2]+j] = P[4][i][j]+P[3][i][j]-P[1][i][j]+P[5][i][j];
		//C12=P1+P2
		for(int i=0;i<sub_dim;i++)
			for(int j=0;j<sub_dim;j++)
				m3[C[0][1][0]+i][C[0][1][2]+j] = P[0][i][j]+P[1][i][j];
		//C21=P3+P4
		for(int i=0;i<sub_dim;i++)
			for(int j=0;j<sub_dim;j++)
				m3[C[1][0][0]+i][C[1][0][2]+j] = P[2][i][j]+P[3][i][j];
		//C22=P5+P1-P3-P7
		for(int i=0;i<sub_dim;i++)
			for(int j=0;j<sub_dim;j++)
				m3[C[1][1][0]+i][C[1][1][2]+j] = P[4][i][j]+P[0][i][j]-P[2][i][j]-P[6][i][j];
	}
}
int main(int argc, char* argv[]){
	FILE *fw;
	int **m1, **m2, **m3;
	int dim, sec, usec;
	struct timeval start, end;
	for(N=30; N<130	;N+=10){
	fw = fopen(argv[2],"w");
	dim = initialize(m1,m2,m3,argv[1]);
	if(dim == 0){
		fprintf(stderr,"input file not exist\n");
		exit(1);
	}
	
	gettimeofday(&start,0);
	int index[4]={0, dim, 0, dim};
	Strassen(m1,m2,m3,index,index,index);

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
	}
	return 0;
}
