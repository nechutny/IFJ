/**
 * 	@project	IFJ 2014/2015
 *	@file		htab_remove.c
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "garbage.h"


/**
 * Remove item from hash table
 *
 * @param	t	reference on hash table
 * @param	key	string used as index for item
 */
void htab_remove(htab_t *t, const char *key)
{
	// index and pointers
	unsigned index = hash_function(key, t->htab_size);
	htab_listitem* item_ptr = t->list[index];
	htab_listitem* item = t->list[index];

	while(item != NULL)
	{ // lets search
		if(strcmp(item->key,key) == 0)
		{ // found
			if(item_ptr == item)
			{ // Found as first in list, so reset list
				t->list[index] = item->next;
			}
			item_ptr->next = item->next;

			_free(item);
			
			break;
		}
		else
		{ // Next..
			item_ptr = item;
			item = item->next;
		}
	}
	
}
