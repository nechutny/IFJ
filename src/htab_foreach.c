/**
 * 	@project	IFJ 2014/2015
 *	@file		htab_foreach.c
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"


/**
 * Call function on each memeber of given hash table
 *
 * @param	t		reference on hash table
 * @param	function	pointer on function
 */
void htab_foreach(htab_t *t, void (*function)(char key[], item_type type, void* ptr) )
{
	htab_listitem* item = NULL;

	for(unsigned i = 0; i < t->htab_size; i++)
	{ // Loop on list with indexes
		item = t->list[i]; 
		while(item != NULL)
		{ // loop on items
			function(item->key, item->type, item->type == type_function ? (void*)item->ptr.function : (void*)item->ptr.variable);
			item = item->next;
		}
	}
}
