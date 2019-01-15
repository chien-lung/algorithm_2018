#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main(int argc, char* argv[]){
	srand(time(NULL));
	FILE *fp = fopen(argv[1],"w");
	int dim = 600;
	fprintf(fp,"%d\n\n",dim);
	for(int i=0;i<dim;i++){
		for(int j=0;j<dim;j++)
			fprintf(fp,"%d ",rand()%1000);
		fprintf(fp,"\n");
	}
	fprintf(fp,"\n");
	for(int i=0;i<dim;i++){
		for(int j=0;j<dim;j++)
			fprintf(fp,"%d ",rand()%1000);
		fprintf(fp,"\n");
	}
	fclose(fp);
	return 0;
}
