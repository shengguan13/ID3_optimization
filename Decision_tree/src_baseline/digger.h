//Include libraries
#include<stdio.h> 
#include<stdlib.h> 
#include<string.h>
#include<math.h>
#include<time.h>
#include<sys/time.h>
//MACRO FOR NOISE
#define NOISE "?"

//STRUCTURE TO STORE DECISION TREE
typedef struct Node {
//NODE - ATTRIBUTE
	int column;
//NO OF BRANCHES
	int item_count;
//BRANCH VALUE
	char **item;
//CHILD NODES - RECURSIVE USE OF STRUCTURES
	struct Node *child;
//MOST PROBABLE TARGET VALUE TO USE INCASE OF UNCERTAINITY AT CHILD
	char **max_target;
}node;

//GLOBAL VARIABLE DECLARATION
extern node *root;
extern char **g_headers;
extern int g_column_count;
extern int g_target;
extern char ***g_lines;
extern int g_line_count;
extern FILE *op,*t_op;
extern int g_success;
//VARIABLES TO MEASURE PERFORMANCE
extern struct timeval start;
extern struct timeval end;

//FUNCTION DECLARATION
void load();
void load_headers(FILE *fp);
void load_records(FILE *fp);
void seed();
int grow(int *ip_subset,int *ip_column,int ip_line_count,int ip_column_count,node *ip_node);
void predict();
int navigate(char **line,node *ip_node);
void free_mem();
