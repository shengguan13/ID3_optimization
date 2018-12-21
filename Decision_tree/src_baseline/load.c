//Include libraries
#include "digger.h"

//INPUT FILE READING
void load()
{
	char ip_file[200];
	FILE *fp;
	int i;
//TRAINING DATA SET FILE
	printf("Enter the input file name:");
    scanf("%s",ip_file);
	gettimeofday(&start,NULL);
	fp = fopen(ip_file, "r");
    if (fp == NULL)
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
    }
	
	load_headers(fp);
	load_records(fp);
	fclose(fp);
	gettimeofday(&end,NULL);
//TARGET SELECTION
	printf("Please choose an attribute (by number):\n");
	for (i=0;i<g_column_count;i++)
	{
		printf("\t%d. %s\n",i+1,g_headers[i]);
	}
	printf("Target:");
	scanf("%d",&g_target);
	g_target--;
	if (g_target<0 || g_target>=g_column_count)
	{
        perror("Invalid target.\n");
        exit(EXIT_FAILURE);
	}
}
//READ THE COLUMN NAMES
void load_headers(FILE *fp)
{
	char c;
	int i,flag1;
	flag1=0;
	i=0;
    g_headers = (char **) malloc(1 * sizeof(char * ));
	g_headers[g_column_count] = (char * ) malloc(sizeof(char));
//READ ATTRIBUTES
	while ((c = getc(fp)) != EOF)
	{
//END OF LINE BREAK
		if (c=='\n' && flag1==1)
		{
			i++;
			g_headers[g_column_count] = (char * ) realloc(g_headers[g_column_count],(i+1) * sizeof(char));
			g_headers[g_column_count][i]='\0';
			g_column_count++;
			break;
		}
		else if (c=='\n' && flag1==0)
		{
			break;
		}
		else if (c==' ' && flag1==1)
		{
			i++;
			g_headers[g_column_count] = (char * ) realloc(g_headers[g_column_count],(i+1) * sizeof(char));
			g_headers[g_column_count][i]='\0';
			flag1=0;
			i=0;
			g_column_count++;
		}
//END OF WORD
		else if (c!=' ' && flag1==0)
		{
			flag1=1;
			g_headers = (char **) realloc(g_headers, (g_column_count + 1) * sizeof(char * ));
			g_headers[g_column_count] = (char * ) malloc(sizeof(char));
			g_headers[g_column_count][i]=c;
		}
//WORD
		else if (c!=' ' && c!='\n')
		{
			i++;
			g_headers[g_column_count] = (char * ) realloc(g_headers[g_column_count],(i+1) * sizeof(char));
			g_headers[g_column_count][i]=c;
		}
	}

}

//READ ALL RECORDS
void load_records(FILE *fp)
{
	char str[100];
    int i=0;
    g_lines = (char ***) malloc(1 * sizeof(char ** ));
    if (g_lines == NULL)
	{
		perror("Error in memory allocation.\n");
		exit(EXIT_FAILURE);
    }
//READ WORD BY WORD
    while (fscanf(fp, "%s", str) != EOF) 
	{
		if (i%g_column_count==0)
		{
			g_line_count++;
			g_lines = (char ***) realloc(g_lines,(g_line_count) * sizeof(char ** ));
			if (g_lines == NULL) 
			{
				perror("Error in memory allocation.\n");
				exit(EXIT_FAILURE);
			}
			g_lines[g_line_count-1] = (char **) malloc(g_column_count * sizeof(char * ));
			if (g_lines[g_line_count-1] == NULL) 
			{
				perror("Error in memory allocation.\n");
				exit(EXIT_FAILURE);
			}
		}
		g_lines[g_line_count-1][i%g_column_count]= (char *) malloc(strlen(str) * sizeof(char));
		if (g_lines[g_line_count-1][i%g_column_count] == NULL)
		{
			perror("Error in memory allocation.\n");
			exit(EXIT_FAILURE);
		}
		strcpy(g_lines[g_line_count-1][i%g_column_count], str);
		i++;
	}
}
