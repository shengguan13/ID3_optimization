//Include libraries
#include "digger.h"
struct timeval start1;
struct timeval end1;
FILE *f_times;
float round_time;

//CREATE THE SEED ELEMENTS TO FORM THE TREE
void seed()
{
	int *subset,*column;
	int i,j;
	gettimeofday(&start,NULL);
//RECORDS TO BUILD THE ROOT ELEMENT
	subset = (int *) malloc((g_line_count) * sizeof(int));
	for (i=0;i<g_line_count;i++)
	{
		subset[i]=i;
	}
//TOTAL COLUMNS PARTICIPATING IN THE DECISION
	column = (int *) malloc((g_column_count) * sizeof(int));
	j=0;
	for (i=0;i<g_column_count;i++)
	{
		if (g_target==i)
		{
			continue;
		}
		column[j]=i;
		j++;
	}

	f_times = fopen("Timing_baseline", "w");
    op = fopen("Rules", "w");
    if (op == NULL)
	{
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
    }
	fprintf(op,"Target attribute is: %s\n",g_headers[g_target]);
	root = (node *)malloc(sizeof(node));
	root->item_count=0;
//INITIAL CALL TO START THE TREE
	grow(subset,column,g_line_count,g_column_count-1,root);
	printf("Generated Tree is stored in file Rules\n");
	gettimeofday(&end,NULL);
	fclose(op);
	free(subset);
	free(column);
}
//INCREMENTAL RECURSIVE BUILDING BLOCK TO BUILD TREE
int grow(int *ip_subset,int *ip_column,int ip_line_count,int ip_column_count,node *ip_node)
{
//STATIC VARIABLE TO TRACK THE CURRENT LEVEL IN THE TREE
	static int lvl=0;
	int i,j,k,l;
	int **subset,*column,**n_subset;
	int split_column;
	char **item,**n_item;
	int item_count,n_item_count=0;
	int *item_line_count,*n_item_line_count;
	char ***item_target,***n_item_target;
	int *item_target_count,*n_item_target_count;
	int **item_target_line_count,**n_item_target_line_count;
	float gain,item_gain,n_gain;
	int max_item,max_target,noise_flag,item_flag;
	float max_prob;
	lvl++;
	n_gain=-1.0;
	gettimeofday(&start1,NULL);
//LOOP ATTRIBUTES
	for (i=0;i<ip_column_count;i++)
	{
		item = (char **) malloc(1 * sizeof(char * ));
		item_count=0;
		item_line_count=(int *) malloc(1 * sizeof(int));
		subset = (int **) malloc(1 * sizeof(int * ));
		item_target = (char ***) malloc(1 * sizeof(char ** ));
		item_target_count=(int *) malloc(1 * sizeof(int));
		item_target_line_count=(int **) malloc(1 * sizeof(int *));
//LOOP ON ALL RECORDS AT THIS LEVEL
		for (j=0;j<ip_line_count;j++)
		{
//LOOP ON IDENTIFIED UNIQUE ITEMS
			for (k=0;k<item_count;k++)
			{
//CHECK IF ITEM ALREADY EXISTS
				if (strcmp(g_lines[ip_subset[j]][ip_column[i]],item[k])==0)
				{
//ADD RECORD AND ITEM INFO
					subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
					subset[k][item_line_count[k]]=ip_subset[j];
					item_line_count[k]++;
					break;
				}
			}
//ADD ITEM IF NOT EXISTS
			if (k==item_count)
			{
//INTIALIZE ITEM INFO
				item = (char **) realloc(item,(k+1) * sizeof(char * ));
				item[k] = (char *) malloc(strlen(g_lines[ip_subset[j]][ip_column[i]]) * sizeof(char));
				strcpy(item[k],g_lines[ip_subset[j]][ip_column[i]]);
				item_line_count=(int *) realloc(item_line_count,(k+1) * sizeof(int));
				item_count++;
//INITIALIZE RECORD INFO THAT BELONG TO THE ITEM
				item_line_count[k]=1;
				subset = (int **) realloc(subset,(k+1) * sizeof(int * ));
				subset[k] = (int *) malloc(1 * sizeof(int));
				subset[k][0]=ip_subset[j];
//INITIALIZE TARGET THAT BELONG TO THE ITEM
				item_target = (char ***) realloc(item_target,(k+1) * sizeof(char ** ));
				item_target[k] = (char **) malloc(1 * sizeof(char * ));
				item_target_count=(int *) realloc(item_target_count,(k+1) * sizeof(int));
				item_target_count[k]=0;
				item_target_line_count=(int **) realloc(item_target_line_count,(k+1) * sizeof(int *));
				item_target_line_count[k]=(int *) malloc(1 * sizeof(int));
			}
//LOOP ON IDETIFIED TARGET VALUES IN THE ITEM
			for (l=0;l<item_target_count[k];l++)
			{
//CHECK IF TARGET ALEADY EXISTS
				if (strcmp(g_lines[ip_subset[j]][g_target],item_target[k][l])==0)
				{
//ADD TARGET INFO
					item_target_line_count[k][l]++;
					break;
				}
			}
//ADD TARGET IF NOT EXISTS
			if (l==item_target_count[k])
			{
//INITIALIZE TARGET INFO
				item_target[k] = (char **) realloc(item_target[k],(l+1) * sizeof(char *));
				item_target[k][l] = (char *) malloc(strlen(g_lines[ip_subset[j]][g_target]) * sizeof(char));
				strcpy(item_target[k][l],g_lines[ip_subset[j]][g_target]);
				item_target_count[k]++;
				item_target_line_count[k]=(int *) realloc(item_target_line_count[k],(l+1) * sizeof(int));
				item_target_line_count[k][l]=1;
			}
		}
//CHECK FOR NOISE IN ATTRIBUTE
		max_item=0;
		for (j=0;j<item_count;j++)
		{
			if (strcmp(item[j],NOISE)==0)
			{
//FIND MAXIMUM OCCURING VALUE
				for (k=0;k<item_count;k++)
				{
					if (k==j)
					{
						continue;
					}
					else if (item_line_count[max_item]<item_line_count[k])
					{
						max_item=k;
					}
				}
				if (max_item==j)
				{
					continue;
				}
//REPLACING NOISE
				subset[max_item] = (int *) realloc(subset[max_item],(item_line_count[max_item]+item_line_count[j]) * sizeof(int));
				for (k=0;k<item_line_count[j];k++)
				{
					subset[max_item][item_line_count[max_item]]=subset[j][k];
					item_line_count[max_item]++;
				}
//ADDING TARGET FROM NOISE TO MAX ITEM
				for (k=0;k<item_target_count[j];k++)
				{
					for (l=0;l<item_target_count[max_item];l++)
					{
						if (strcmp(item_target[j][k],item_target[max_item][l])==0)
						{
							item_target_line_count[max_item][l]=item_target_line_count[max_item][l]+item_target_line_count[j][k];
							break;
						}
					}
//IF TARGET NOT PRESENT IN MAX ITEM, INITIALIZE IT
					if (l==item_target_count[max_item])
					{
						item_target[max_item] = (char **) realloc(item_target[max_item],(l+1) * sizeof(char *));
						item_target[max_item][l] = (char *) malloc(strlen(item_target[j][k]) * sizeof(char));
						strcpy(item_target[max_item][l],item_target[j][k]);
						item_target_count[max_item]++;
						item_target_line_count[max_item]=(int *) realloc(item_target_line_count[max_item],(l+1) * sizeof(int));
						item_target_line_count[max_item][l]=item_target_line_count[j][k];
					}
				}
			}
//CHECK FOR NOISE IN TARGET
			max_target=0;
			for (k=0;k<item_target_count[j];k++)
			{
				if (strcmp(item_target[j][k],NOISE)==0)
				{
					for (l=0;l<item_target_count[j];l++)
					{
						if (k==l)
						{
							continue;
						}
						else if (item_line_count[max_item]<item_line_count[k])
						{
							max_target=l;
						}
					}
					if (max_item==j)
					{
						break;
					}
//REPLACE NOISE
					item_target_line_count[j][max_target]=item_target_line_count[j][max_target]+item_target_line_count[j][k];
					break;
				}
			}

		}
//NOISE ENDS
		gain=0.0;
//GAIN CLACULATION
//LOOP ON ITEM
		for (j=0;j<item_count;j++)
		{
//SKIP NOISE ITEM
			if (strcmp(item[j],NOISE)==0)
			{
				continue;
			}
//ENTROPY FOR EACH ITEM
			item_gain=0.0;
			if (item_target_count[j]==1)
			{
				item_gain=0.0;
			}
			else
			{
				for (k=0;k<item_target_count[j];k++)
				{
					item_gain=item_gain+((((float)item_target_line_count[j][k])/item_line_count[j])*log2f((((float)item_target_line_count[j][k])/item_line_count[j]))/log2f((float)item_target_count[j]));
				}
			}
//GAIN
			gain=gain+(item_line_count[j]*item_gain/ip_line_count);
		}
//MAINTAIN INFO OF COLUMN WITH HIGH INFORMATION GAIN
		if (n_gain<=gain)
		{
			for (j=0;j<n_item_count;j++)
			{
				free(n_subset[j]);
				free(n_item[j]);
			}
			for (j=0;j<n_item_count;j++)
			{
				for (k=0;k<n_item_target_count[j];k++)
				{
					free(n_item_target[j][k]);
				}
				free(n_item_target[j]);
				free(n_item_target_line_count[j]);
			}
			if (n_item_count>0)
			{
				free(n_subset);
				free(n_item);
				free(n_item_line_count);
				free(n_item_target);
				free(n_item_target_count);
				free(n_item_target_line_count);
			}

			split_column=ip_column[i];
			n_subset=subset;
			n_item=item;
			n_item_count=item_count;
			n_item_line_count=item_line_count;
			n_item_target=item_target;
			n_item_target_count=item_target_count;
			n_item_target_line_count=item_target_line_count;
			n_gain=gain;
		}
//IF GAIN IS LOWER DISCARD THE COLUMN INFO
		else
		{
			for (j=0;j<item_count;j++)
			{
				free(subset[j]);
				free(item[j]);
			}
			free(subset);
			free(item);
			free(item_line_count);
			for (j=0;j<item_count;j++)
			{
				for (k=0;k<item_target_count[j];k++)
				{
					free(item_target[j][k]);
				}
				free(item_target[j]);
				free(item_target_line_count[j]);
			}
			free(item_target);
			free(item_target_count);
			free(item_target_line_count);
		}
	}
	gettimeofday(&end1,NULL);
	round_time = (float)(((end1.tv_sec-start1.tv_sec)*1000000)+(end1.tv_usec-start1.tv_usec))/1000;
	fprintf(f_times, "Round time: %f ms\n", round_time);

//HIGHEST GAIN COLUMN IS SET AS NODE AND REST OF THE COLUMNS ARE PASSED FURTHER
	column = (int *) malloc((ip_column_count-1) * sizeof(int));
	j=0;
	for (i=0;i<ip_column_count;i++)
	{
		if (split_column==ip_column[i])
		{
			continue;
		}
		column[j]=ip_column[i];
		j++;
	}
	ip_node->column=split_column;
	ip_node->child=(node *)malloc(sizeof(node));
	ip_node->item=(char **)malloc(sizeof(char *));
	ip_node->max_target=(char **)malloc(sizeof(char *));
	item_flag=0;
//BRANCHING - LOOP THROUGH ALL ITEM OF THE NODE COLUMN
	for (i=0;i<n_item_count;i++)
	{
//SKIP NOISE ITEM
		if (strcmp(n_item[i],NOISE)==0)
		{
			free(n_subset[i]);
			free(n_item[i]);
			item_flag=1;
			continue;
		}
//SET BRANCH DETAILS
    	ip_node->item_count++;
		ip_node->child=(node *)realloc(ip_node->child,(i-item_flag+1)*sizeof(node));
		ip_node->child[i-item_flag].item_count=0;
		ip_node->item=(char **)realloc(ip_node->item,(i-item_flag+1)*sizeof(char *));
		ip_node->item[i-item_flag]=(char *)malloc(strlen(n_item[i]) * sizeof(char));
		strcpy(ip_node->item[i-item_flag],n_item[i]);

		noise_flag=0;
		max_prob=0.0;
		max_target=0;
//CALCULATE HIGH OCCURING TARGET FOR EACH BRANCH
		for (j=0;j<n_item_target_count[i];j++)
		{
			if (strcmp(n_item_target[i][j],NOISE)==0)
			{
				noise_flag=1;
				continue;
			}
			if (max_prob<((float)n_item_target_line_count[i][j]*100)/n_item_line_count[i])
			{
				max_prob=((float)n_item_target_line_count[i][j]*100)/n_item_line_count[i];
				max_target=j;
			}
		}
		ip_node->max_target=(char **)realloc(ip_node->max_target,(i-item_flag+1)*sizeof(char *));
		ip_node->max_target[i-item_flag]=(char *)malloc(strlen(n_item_target[i][max_target]) * sizeof(char));
		strcpy(ip_node->max_target[i-item_flag],n_item_target[i][max_target]);
		if (n_gain==-1)
		{
			fprintf(op,"****Uncertain leaf node, please check parent node for high probable target value****");
		}
		else
		{
			for (j=0;j<lvl;j++)
			{
				fprintf(op,"\t");
			}
			fprintf(op,"if %s is %s ,then",g_headers[split_column],n_item[i]);
		}
//RECURSIVE CALL - TO GROW CHILD NODES DEPTH WISE
//CHECK IF MULTIPLE BRANCH WITH NO UNCERTAINITY AND HAS FURTHER LEVELS
		if (ip_column_count!=1 && (n_item_target_count[i]-noise_flag>1) && n_gain!=-1.0)
		{
			fprintf(op," [");
			for (j=0;j<n_item_target_count[i];j++)
			{
				if (strcmp(n_item_target[i][j],NOISE)==0)
				{
					continue;
				}
				fprintf(op,"%s-%.2f%;",n_item_target[i][j],((float)n_item_target_line_count[i][j]*100)/n_item_line_count[i]);
			}
			fprintf(op,"]\n");
			grow(n_subset[i],column,n_item_line_count[i],ip_column_count-1,&(ip_node->child[i-item_flag]));
		}
//SINGLE BRANCH, MARK AS END LEAF AND RETURN BACK
		else if (n_item_target_count[i]-noise_flag==1)
		{
			ip_node->child[i-item_flag].column=g_target;
			ip_node->child[i-item_flag].item_count=1;
			ip_node->child[i-item_flag].child=NULL;
			ip_node->child[i-item_flag].item=(char **)malloc(sizeof(char *));
			ip_node->child[i-item_flag].item[0]=(char *)malloc(strlen(n_item_target[i][0]) * sizeof(char));
			strcpy(ip_node->child[i-item_flag].item[0],n_item_target[i][0]);
			fprintf(op," %s is %s for %.2f% -\n",g_headers[g_target],n_item_target[i][0],((float)n_item_target_line_count[i][0]*100)/n_item_line_count[i]);
		}
//UNCERTAIN OR LAST POSSIBLE LEVEL
//PRINT USEFULNESS MEASURE, SET END LEAF WITH HIGH OCCURING TARGET AND RETURN BACK
//MARK UNCERTAIN IF EQUAL OCCURENCE OF TARGET
		else
		{
			fprintf(op,"\n");
			for (j=0;j<n_item_target_count[i];j++)
			{
				if (strcmp(n_item_target[i][j],NOISE)==0)
				{
					continue;
				}
				for (k=0;k<=lvl;k++)
				{
					fprintf(op,"\t");
				}
				fprintf(op," %s is %s for %.2f% \n",g_headers[g_target],n_item_target[i][j],((float)n_item_target_line_count[i][j]*100)/n_item_line_count[i]);
			}
			ip_node->child[i-item_flag].column=g_target;
			ip_node->child[i-item_flag].item_count=1;
			ip_node->child[i-item_flag].child=NULL;
			ip_node->child[i-item_flag].item=(char **)malloc(sizeof(char *));
			if (n_gain==-1.0 || n_gain==0)
			{
				ip_node->child[i-item_flag].item[0]=(char *)malloc(strlen("uncertain") * sizeof(char));
				strcpy(ip_node->child[i-item_flag].item[0],"uncertain");
			}
			else
			{
				ip_node->child[i-item_flag].item[0]=(char *)malloc(strlen(n_item_target[i][max_target]) * sizeof(char));
				strcpy(ip_node->child[i-item_flag].item[0],n_item_target[i][max_target]);
			}
		}
		if (n_gain==-1)
		{
			fprintf(op,"****Uncertainity Ends****\n");
		}
		free(n_subset[i]);
		free(n_item[i]);
	}
//RELEASE ALLOCATED MEMORY
	free(n_subset);
	free(n_item);
	free(n_item_line_count);
	for (i=0;i<n_item_count;i++)
	{
		for (j=0;j<n_item_target_count[i];j++)
		{
			free(n_item_target[i][j]);
		}
		free(n_item_target[i]);
		free(n_item_target_line_count[i]);
	}
	free(n_item_target);
	free(n_item_target_count);
	free(n_item_target_line_count);
	if (column!=NULL)
	{
		free(column);
	}
	lvl--;
}
