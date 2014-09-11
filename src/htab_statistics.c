/**
 * 	@project	IFJ 2014/2015
 *	@file		htab_statistics.c
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"

/**
 * Print hash table statistics on stdout
 *
 * @param	t	reference on hash table
 */
void htab_statistics(htab_t *t)
{
	htab_listitem* item = NULL;
	unsigned min, max, sum, count;
	max = sum = 0;
	min = -1;
	
	for(unsigned i = 0; i < t->htab_size; i++)
	{ // Loop on items
		count = 0;
		item = t->list[i];
		while(item != NULL)
		{
			item = item->next;
			sum++;
			count++;
		}

		if(count < min || min == -1)
		{ // found new minimum
			min = count;
		}
		if(count > max)
		{ // found new maximum
			max = count;
		}
		
	}

	printf("\nHash table size:\t%d",t->htab_size);
	printf("\nTotal items:\t\t%d",sum);
	printf("\nMaximum items:\t%d",max);
	printf("\nMinimum items:\t%d",min);
	printf("\nAverage items:\t%f\n",((double)sum/(double)t->htab_size));
}
