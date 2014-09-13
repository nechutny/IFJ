/**
 * 	@project	IFJ 2014/2015
 *	@file		htab_lookup.c
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "garbage.h"


/**
 * Helpfull function for creating item in hash table
 *
 * @param	key	string used for key
 * @return	pointer on structure or NULL if error
 */
htab_listitem* htab_create(htab_t *t, const char *key)
{
	// Get index and pointer
	unsigned index = hash_function(key, t->htab_size);
	htab_listitem* item_ptr = NULL;
	htab_listitem* item = t->list[index];
	htab_listitem* nitem;

	if(item == NULL)
	{ // Pointer is null (item not found), so create new item

		// allocate memory for structure size + size of key (length + 1 byte for null-terminating)
		nitem = _malloc(sizeof(htab_listitem) + sizeof(char)*(strlen(key)+1));
		if(nitem == NULL)
		{ // Check malloc
			return NULL;
		}

		// Set values
		memcpy(nitem->key,key,strlen(key)+1);
		nitem->next = NULL;

		item = t->list[index] = nitem;
		if(item == NULL)
		{
			return NULL;
		}
	}
	else
	{ // Possible result
		while(item != NULL)
		{ // Loop on table and find if item exists
			if(strcmp(item->key,key) == 0)
			{ // Found
				item_ptr = item;
				break;
			}
			else
			{ // Next 
				item_ptr = item;
				item = item->next;
			}
			
		}

		if(item_ptr != NULL && item != item_ptr)
		{ // Not found, so create
			// allocate memory for structure size + size of key (length + 1 byte for null-terminating)
			nitem = _malloc(sizeof(htab_listitem) + sizeof(char)*(strlen(key)+1));
			if(nitem == NULL)
			{ // Check malloc
				return NULL;
			}

			// Set values
			memcpy(nitem->key,key,strlen(key)+1);
			nitem->next = NULL;
			item = nitem;
			if(item == NULL)
			{
				return NULL;
			}
			item_ptr->next = item; // link in list
		}
	}

	return item;
}


/**
 * Find or create new item in hash table
 *
 * @param	t	reference on hash table
 * @param	key	string used as index for item
 * @return	pointer on structure or NULL if don't exist
 */
htab_listitem* htab_lookup(htab_t *t, const char *key)
{
	// Get index and pointer
	unsigned index = hash_function(key, t->htab_size);
	htab_listitem* item_ptr = NULL;
	htab_listitem* item = t->list[index];

	if(item == NULL)
	{ // Pointer is null (item not found)
		return NULL;
	}
	else
	{ // Possible result
		while(item != NULL)
		{ // Loop on table and find if item exists
			if(strcmp(item->key,key) == 0)
			{ // Found
				item_ptr = item;
				break;
			}
			else
			{ // Next 
				item_ptr = item;
				item = item->next;
			}
		}

		if(item_ptr != NULL && item != item_ptr)
		{ // Not found
			return NULL;
		}
	}

	return item;
}
