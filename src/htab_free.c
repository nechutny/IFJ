/**
 * 	@project	IFJ 2014/2015
 *	@file		htab_free.c
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"
#include "garbage.h"


/**
 * Destrohy hash table and free memory
 *
 * @param	t	reference on hash table
 */
void htab_free(htab_t *t)
{
	if(t != NULL)
	{
		htab_clear(t);
		_free(t);
	}
}
