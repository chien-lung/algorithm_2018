#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
FILE *tmp = fopen("record.txt","w");
FILE *f1 = fopen("a.txt","w");
FILE *f2 = fopen("c.txt","w");
int sub = 0;
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
	fclose(tmp);
}
void setup(int array[], int next_st1[], int next_end1[], int next_st2[], int next_end2[], int index){
	array[0] = next_st1[index];
	array[1] = next_end1[index];
	array[2] = next_st2[index];
	array[3] = next_end2[index];	
}
void mul_method1(int **&m1, int **&m2, int **&m3, int m1index[], int m2index[], int m3index[]){
	/*
 	index:                 2   3 
		0:start1   0   -----
		1:end1         |0|2|
                2:start2       -----
		3:end2         |1|3|
		           1   -----
	*/
	int m1_dim1 = m1index[1]-m1index[0];
	int m2_dim2 = m2index[3]-m2index[2];
	int co_dim = m1index[3]-m1index[2]; // =m2index[1]-m2index[0]
	
	if( m1_dim1<64 || m2_dim2<64/* size<10 */ ){
		
		fprintf(tmp,"-------1-------\n");
		fprintf(tmp,"%d %d %d\n",m1_dim1,m2_dim2,co_dim);
		fprintf(tmp,"m1:\n");
		for(int i=0;i<4;i++)	fprintf(tmp,"%d ",m1index[i]);
		fprintf(tmp,"\nm2:\n");
		for(int i=0;i<4;i++)	fprintf(tmp,"%d ",m2index[i]);
		fprintf(tmp,"\nm3:\n");
		for(int i=0;i<4;i++)	fprintf(tmp,"%d ",m3index[i]);
		fprintf(tmp,"\n");
		
		for(int i=0;i<4;i++)
			fprintf(f2,"%d ",m1index[i]);
		fprintf(f2,"\n");
		for(int i=0;i<4;i++)
			fprintf(f2,"%d ",m2index[i]);
		fprintf(f2,"\n");
		for(int i=0;i<4;i++)
			fprintf(f2,"%d ",m3index[i]);
		fprintf(f2,"\n----------------\n");

		for(int i=0;i<m1_dim1;i++)
			for(int j=0;j<m2_dim2;j++)
				for(int k=0;k<co_dim;k++)
					m3[m3index[0]+i][m3index[2]+j] += m1[m1index[0]+i][m1index[2]+k]*m2[m2index[0]+k][m2index[2]+j];	
		if(sub!=m3[0][0]){
			fprintf(f1,"%d\n",m3[0][0]-sub);
			sub = m3[0][0];
		}
		return ;
	}

	else{
		fprintf(tmp,"+++++++2+++++++\n");
		fprintf(tmp,"%d %d %d\n",m1_dim1,m2_dim2,co_dim);
		
		int next_st1[4], next_st2[4], next_end1[4], next_end2[4];
		int m1_nextindex[4], m2_nextindex[4], m3_nextindex[4];
		
		next_st1[0] = m1index[0];
		next_end1[0] = m1index[0]+(m1index[1]-m1index[0])/2; 
		next_st2[0] = m1index[2]; 
		next_end2[0] = m1index[2]+(m1index[3]-m1index[2])/2;
		
		next_st1[1] = next_end1[0];
		next_end1[1] = m1index[1];
		next_st2[1] = next_st2[0];
		next_end2[1] = next_end2[0];

		next_st1[2] = next_st1[0];
		next_end1[2] = next_end1[0];
		next_st2[2] = next_end2[0];
		next_end2[2] = m1index[3];

		next_st1[3] = next_st1[1];
		next_end1[3] = next_end1[1];
		next_st2[3] = next_st2[2];
		next_end2[3] = next_end2[2];
		
		fprintf(tmp,"st1:\n");
		for(int i=0;i<4;i++)	fprintf(tmp,"%d ",next_st1[i]);
		fprintf(tmp,"\nend1:\n");
		for(int i=0;i<4;i++)	fprintf(tmp,"%d ",next_end1[i]);
		fprintf(tmp,"\nst2:\n");
		for(int i=0;i<4;i++)	fprintf(tmp,"%d ",next_st2[i]);
		fprintf(tmp,"\nend2:\n");
		for(int i=0;i<4;i++)	fprintf(tmp,"%d ",next_end2[i]);
		fprintf(tmp,"\n");
		
		int temp1[8]={0,2,1,3,0,2,1,3};
		int temp2[8]={0,1,0,1,2,3,2,3};
		for(int i=0;i<8;i++){
			fprintf(tmp,"%d---\n",i);
			fprintf(f2,"%d---\n",i);
			setup(m1_nextindex, next_st1, next_end1, next_st2, next_end2, temp1[i]);
			setup(m2_nextindex, next_st1, next_end1, next_st2, next_end2, temp2[i]);
			setup(m3_nextindex, next_st1, next_end1, next_st2, next_end2, i/2);
			mul_method1(m1, m2, m3, m1_nextindex, m2_nextindex, m3_nextindex);
			//sleep(1);
		}
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
	int m1_index[]={0,dim,0,dim};
	int m2_index[]={0,dim,0,dim};
	int m3_index[]={0,dim,0,dim};
	mul_method1(m1,m2,m3,m1_index,m2_index,m3_index);
	
	for(int i=0;i<dim;i++){
		for(int j=0;j<dim;j++)
			fprintf(fw,"%d ",m3[i][j]);
		fprintf(fw,"\n");
	}
	gettimeofday(&end,0);
	sec = end.tv_sec - start.tv_sec;
	usec = end.tv_usec - start.tv_usec;
	printf("time: %.5fsec.\n",sec+usec/1e6);		
	

	clear(m1,m2,m3,dim);
	fclose(fw);
	return 0;
}
