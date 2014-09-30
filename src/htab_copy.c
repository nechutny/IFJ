/**
 *	@project	IFJ 2014/2015
 *	@file		htab_copy.c
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"
#include "htable.h"
#include "garbage.h"

// Used for access to hash table in foreach
htab_t* target;


/**
 * Function for usage in htab_copy creating copy of all components of hash table item
 *
 * @param	key	Key of item for copy
 * @param	type	Type of item
 * @param	item	Pointer to hash table item for copy
 */
void htab_copy_foreach(char key[], item_type type, htab_listitem* item)
{
	htab_listitem* new = htab_create(target, key);

	// Copy symbolVariable
	symbolVariable* sym_v = memcpy(_malloc(sizeof(symbolVariable)), item->ptr.variable, sizeof(symbolVariable));

	// Copy name
	sym_v->name = string_new();
	sym_v->name = string_add(sym_v->name, item->ptr.variable->name->data);

	// Copy htable item
	memcpy(new, item, sizeof(htab_listitem));
	new->next = NULL;

	// Set new pointer
	new->ptr.variable = sym_v;
}


/**
 * Copy hash table of local symbols
 *
 * @param	source	pointer to structure for copy
 * @return	pointer to new copy
 *
 */
htab_t *htab_copy(htab_t * source)
{
	target = htab_init(source->htab_size);
	htab_foreach(source, htab_copy_foreach);

	return target;
}


