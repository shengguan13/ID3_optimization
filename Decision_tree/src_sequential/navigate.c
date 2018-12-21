//Include libraries
#include "digger.h"

int navigate(int *line,node *ip_node)
{
	int i;
//REACHED FINAL NODE, UNCERTAIN END LEAF, RETURN BACK TO PARENT
	if (ip_node->column == g_target && !(ip_node->item[0] ^ -1))
	{
		return 1;
	}
//REACHED FINAL NODE, CERTAIN END LEAF, CONCLUDE AS PREDICTION
	else if (ip_node->column == g_target && (ip_node->item[0] ^ -1))
	{
		fprintf(t_op,"%d\n",ip_node->item[0]);
//CHECK PREDICTION WITH ACTUAL
		if (!(line[g_target] ^ ip_node->item[0]))
		{
			g_success++;
		}
		return 0;
	}
//NON-FINAL NODE
	for (i=0;i<ip_node->item_count;i++)
	{
//CHECK FOR THE BRANCH TO TRAVEL
		if (!(line[ip_node->column] ^ ip_node->item[i]))
		{
			break;
		}
	}
//NO BRANCH IS SUITABLE, UNCERTAINITY, RETURN TO PARENT
	if (i==ip_node->item_count)
	{
		return 1;
	}
//TRAVEL ON IDENTIFIED BRANCH
//IF RETURNED UNCERTAIN, CONCLUDE HIGH OCCURING TARGET AS PREDICTION
	if (navigate(line,&(ip_node->child[i])))
	{
		fprintf(t_op,"%d--uncertain\n",ip_node->max_target[i]);
//CHECK PREDICTION WITH ACTUAL
		if (!(line[g_target] ^ ip_node->max_target[i]))
		{
			g_success++;
		}
	}
	return 0;
}
