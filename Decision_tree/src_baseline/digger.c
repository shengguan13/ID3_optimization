//Include libraries
#include "digger.h"

//GLOBAL VARIABLE DEFINITION
node *root;
char **g_headers;
int g_column_count = 0;
int g_target;
char ***g_lines;
int g_line_count = 0;
FILE *op,*t_op;
int g_success=0;
struct timeval start;
struct timeval end;

int main ()
{
    int i,j,flag1;
	float load_time,train_time,test_time;
//INPUT & FILE READING
	load();
	load_time=(float)(((end.tv_sec-start.tv_sec)*1000000)+(end.tv_usec-start.tv_usec))/1000;
//DERIVE TREE FROM INPUT
	seed();
	train_time=(float)(((end.tv_sec-start.tv_sec)*1000000)+(end.tv_usec-start.tv_usec))/1000;
//APPLY TREE ON TEST DATASET
	predict();
	test_time=(float)(((end.tv_sec-start.tv_sec)*1000000)+(end.tv_usec-start.tv_usec))/1000;
//RELEASE ALLOCATED MEMORY
	free_mem();
	printf("----------------------------------------------------------------\n");
	printf("Statistics\n");
	printf("----------------------------------------------------------------\n");
	printf("Loading Time Taken:%.3f ms\n",load_time);
	printf("Training Time Taken:%.3f ms\n",train_time);
	printf("Testing Time Taken:%.3f ms\n",test_time);
	return 0;
}

void free_mem()
{
	int i,j;
	for (i=0;i<g_column_count;i++)
	{
		free(g_headers[i]);
	}
	free(g_headers);
	//printf("LINES:\n");
    for (i=0;i<g_line_count;i++) {
		for (j=0;j<g_column_count;j++)
		{
			//printf("%s\t",g_lines[i][j]);
			free(g_lines[i][j]);
		}
		//printf("\n");
		free(g_lines[i]);
    }
    free(g_lines);
}