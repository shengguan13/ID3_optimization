//Include libraries
#include "digger.h"

void predict()
{
    char ip_file[200];
    char str[100];
	char **line;
    FILE *fp;
	int i,j;
	printf("Enter 1 to test or any other key to exit:");
	scanf("%d",&i);
	if (i!=1)
	{
		gettimeofday(&start,NULL);
		gettimeofday(&end,NULL);
		return;
	}
    printf("PREDICTION\n----------\n");
    printf("***Input file should follow the same structure of training data and should not have new values in dataset***\n");
    printf("Enter the input file name:");
    scanf("%s",ip_file);
	gettimeofday(&start,NULL);
    fp = fopen(ip_file, "r");
	t_op=fopen("Test", "w");

    if (fp == NULL || t_op==NULL)
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
    }
//SKIP HEADER
	line=(char **)malloc(g_column_count*sizeof(char *));
	for (i=0;i<g_column_count;i++)
	{
		line[i]=(char *)malloc(100*sizeof(char));
		fscanf(fp, "%s", str);
		fprintf(t_op,"%s ",str);
	}
	fprintf(t_op,"PREDICTION\n");
	j=0;
//LOOP EVERY RECORD
    while (fscanf(fp, "%s", str) != EOF)
	{
		fprintf(t_op,"%s ",str);
		strcpy(line[0],str);
//LOOP ON COLUMNS
		for (i=1;i<g_column_count;i++)
		{
			fscanf(fp, "%s", str);
			fprintf(t_op,"%s ",str);
			strcpy(line[i],str);
		}
//START TRAVERSAL FROM ROOT
		if (navigate(line,root))
		{
			fprintf(t_op,"--uncertain\n");
		}
		j++;
	}
//PRINT ACCURACY
	printf("Accuracy:%f\n",((float)g_success)*100.0/j);
	fprintf(t_op,"Accuracy:%f\n",((float)g_success)*100.0/j);
	printf("Test output is stored in file Test\n");
	fclose(fp);
	fclose(t_op);
	gettimeofday(&end,NULL);
}
