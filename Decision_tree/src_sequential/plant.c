//Include libraries
#include "digger.h"
#include <immintrin.h>
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

	f_times = fopen("Timing_sequential", "w");
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
	lvl++;
	__m256i vec, vec1, vec2, vcmp1, vcmp2;
	int i,j,k,l;
	int **subset,*column,**n_subset;
	int split_column;
	int *item, *n_item;
	int item_count,n_item_count=0;
	int *item_line_count,*n_item_line_count;
	int **item_target, **n_item_target;
	int *item_target_count,*n_item_target_count;
	int **item_target_line_count,**n_item_target_line_count;
	float gain,item_gain,n_gain;
	int max_item,max_target,item_flag;
	float max_prob;
	n_gain = -1.0;
	round_time = 0.0;
	gettimeofday(&start1,NULL);
//LOOP ATTRIBUTES
	for (i=0;i<ip_column_count;i++)
	{
		item = (int *) malloc(25000 * sizeof(int));
		item_target_count=(int *) malloc(25000 * sizeof(int));
		item_line_count=(int *) malloc(25000 * sizeof(int));
		subset = (int **) malloc(25000 * sizeof(int * ));
		item_target = (int **) malloc(25000 * sizeof(int * ));
		item_target_line_count=(int **) malloc(25000 * sizeof(int *));
		item_count=0;
		int temp_column = ip_column[i];
		for (j=0; j<ip_line_count; j++)
		{
			int temp_glines = g_lines[ip_subset[j]][temp_column];
			int temp_subset = ip_subset[j];
			int temp_item_count = item_count - item_count % 16;
			vec = _mm256_setr_epi32(temp_glines, temp_glines, temp_glines, temp_glines, temp_glines, temp_glines, temp_glines, temp_glines);
			for (k = 0; k < temp_item_count; k+=16)
			{
				int *temp_pos = item + k;
				vec1 = _mm256_loadu_si256((__m256i*)temp_pos);
				vec2 = _mm256_loadu_si256((__m256i*)(temp_pos + 8));
				vcmp1 = _mm256_cmpeq_epi32(vec, vec1);
				vcmp2 = _mm256_cmpeq_epi32(vec, vec2);
				int test1 = _mm256_testz_si256(vcmp1, vcmp1);
				int test2 = _mm256_testz_si256(vcmp2, vcmp2);
				if(test1 & test2)
					continue;
				else{
					if(!test1){
						int check = _mm256_movemask_epi8(vcmp1);
						if(check & 1){
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
						else if((check>>4) & 1){
							k+=+1;
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
						else if((check>>8) & 1){
							k+=2;
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
						else if((check>>12) & 1){
							k+=3;
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
						else if((check>>16) & 1){
							k+=4;
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
						else if((check>>20) & 1){
							k+=5;
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
						else if((check>>24) & 1){
							k+=6;
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
						else{
							k+=7;
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
					}
					else{
						int check = _mm256_movemask_epi8(vcmp2);
						if(check & 1){
							k+=+8;
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
						else if((check>>4) & 1){
							k+=+9;
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
						else if((check>>8) & 1){
							k+=10;
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
						else if((check>>12) & 1){
							k+=11;
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
						else if((check>>16) & 1){
							k+=12;
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
						else if((check>>20) & 1){
							k+=13;
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
						else if((check>>24) & 1){
							k+=14;
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
						else{
							k+=15;
							subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
							subset[k][item_line_count[k]] = temp_subset;
							item_line_count[k]++;
							break;
						}
					}
				}
			}
			if(k >= temp_item_count){
				for(; k < item_count; k++){
					if (!(temp_glines ^ item[k]))
					{
						subset[k] = (int *) realloc(subset[k],(item_line_count[k]+1) * sizeof(int));
						subset[k][item_line_count[k]] = temp_subset;
						item_line_count[k]++;
						break;
					}
				}
			}

			if (k == item_count)
			{
				item[k] = temp_glines;
				item_count++;
				item_line_count[k] = 1;
				subset[k] = (int *) malloc(1 * sizeof(int));
				subset[k][0] = temp_subset;
				item_target[k] = (int *) malloc(25 * sizeof(int));
				item_target_count[k] = 0;
				item_target_line_count[k]=(int *) malloc(25 * sizeof(int));
			}
			int temp_glines_target = g_lines[temp_subset][g_target];
			int temp_item_target_count = item_target_count[k];
			for (l=0;l<temp_item_target_count;l++)
			{
				if (!(temp_glines_target ^ item_target[k][l]))
				{
					item_target_line_count[k][l]++;
					break;
				}
			}
			if (l == item_target_count[k])
			{
				item_target[k][l] = temp_glines_target;
				item_target_count[k]++;
				item_target_line_count[k][l]=1;
			}
		}

		gain = 0.0;

		for (j=0;j<item_count;j++)
		{
			item_gain = 1.0;
			if (item_target_count[j] != 1)
			{
				for (k=0;k<item_target_count[j];k++)
				{
					float p = (float)item_target_line_count[j][k] / item_line_count[j];
					item_gain = item_gain * pow(p, p);
				}
				item_gain = log2f(item_gain);
				item_gain = item_gain / log2f((float)item_target_count[j]);
			}
			gain = gain + item_line_count[j] * item_gain;
		}
		gain = gain / ip_line_count;

		if (n_gain<=gain)
		{
			for (j=0;j<n_item_count;j++)
			{
				free(n_subset[j]);
			}
			for (j=0;j<n_item_count;j++)
			{
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
			}
			free(subset);
			free(item);
			free(item_line_count);
			for (j=0;j<item_count;j++)
			{
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
	ip_node->item=(int *)malloc(sizeof(int));
	ip_node->max_target=(int *)malloc(sizeof(int));
	item_flag=0;

//BRANCHING - LOOP THROUGH ALL ITEM OF THE NODE COLUMN
	for (i=0;i<n_item_count;i++)
	{
//SET BRANCH DETAILS
    	ip_node->item_count++;
		ip_node->child=(node *)realloc(ip_node->child,(i-item_flag+1)*sizeof(node));
		ip_node->child[i-item_flag].item_count=0;
		ip_node->item=(int *)realloc(ip_node->item,(i-item_flag+1)*sizeof(int));
		ip_node->item[i-item_flag] = n_item[i];

		max_prob=0.0;
		max_target=0;
//CALCULATE HIGH OCCURING TARGET FOR EACH BRANCH
		for (j=0;j<n_item_target_count[i];j++)
		{
			if (max_prob<((float)n_item_target_line_count[i][j]*100)/n_item_line_count[i])
			{
				max_prob=((float)n_item_target_line_count[i][j]*100)/n_item_line_count[i];
				max_target=j;
			}
		}
		ip_node->max_target=(int *)realloc(ip_node->max_target,(i-item_flag+1)*sizeof(int));
		ip_node->max_target[i-item_flag] = n_item_target[i][max_target];
		if (n_gain==-1.0)
		{
			fprintf(op,"****Uncertain leaf node, please check parent node for high probable target value****");
		}
		else
		{
			for (j=0;j<lvl;j++)
			{
				fprintf(op,"\t");
			}
			fprintf(op,"if %s is %d ,then",g_headers[split_column],n_item[i]);
		}
//RECURSIVE CALL - TO GROW CHILD NODES DEPTH WISE
//CHECK IF MULTIPLE BRANCH WITH NO UNCERTAINITY AND HAS FURTHER LEVELS
		if (ip_column_count!=1 && (n_item_target_count[i]>1) && n_gain!=-1.0)
		{
			fprintf(op," [");
			for (j=0;j<n_item_target_count[i];j++)
			{
				fprintf(op,"%d-%.2f%;",n_item_target[i][j],((float)n_item_target_line_count[i][j]*100)/n_item_line_count[i]);
			}
			fprintf(op,"]\n");
			grow(n_subset[i],column,n_item_line_count[i],ip_column_count-1,&(ip_node->child[i-item_flag]));
		}
//SINGLE BRANCH, MARK AS END LEAF AND RETURN BACK
		else if (n_item_target_count[i]==1)
		{
			ip_node->child[i-item_flag].column=g_target;
			ip_node->child[i-item_flag].item_count=1;
			ip_node->child[i-item_flag].child=NULL;
			ip_node->child[i-item_flag].item=(int *)malloc(sizeof(int));
			ip_node->child[i-item_flag].item[0] = n_item_target[i][0];
			fprintf(op," %s is %d for %.2f% -\n",g_headers[g_target],n_item_target[i][0],((float)n_item_target_line_count[i][0]*100)/n_item_line_count[i]);
		}
		else
		{
			fprintf(op,"\n");
			for (j=0;j<n_item_target_count[i];j++)
			{
				for (k=0;k<=lvl;k++)
				{
					fprintf(op,"\t");
				}
				fprintf(op," %s is %d for %.2f% \n",g_headers[g_target],n_item_target[i][j],((float)n_item_target_line_count[i][j]*100)/n_item_line_count[i]);
			}
			ip_node->child[i-item_flag].column=g_target;
			ip_node->child[i-item_flag].item_count=1;
			ip_node->child[i-item_flag].child=NULL;
			ip_node->child[i-item_flag].item=(int *)malloc(sizeof(int));
			if (n_gain==-1.0 || n_gain==0)
			{
				ip_node->child[i-item_flag].item[0] = -1;
			}
			else
			{
				ip_node->child[i-item_flag].item[0] = n_item_target[i][max_target];
			}
		}
		if (n_gain==-1)
		{
			fprintf(op,"****Uncertainity Ends****\n");
		}
		free(n_subset[i]);
	}
//RELEASE ALLOCATED MEMORY
	free(n_subset);
	free(n_item);
	free(n_item_line_count);
	for (i=0;i<n_item_count;i++)
	{
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
