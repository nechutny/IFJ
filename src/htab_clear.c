/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14		*/
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš, 		*/
/*                Michchal Jásenský,								*/
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00 			*/
/********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"


/**
 * Remove all items from hash table
 *
 * @param	t	reference on hash table
 */
void htab_clear(htab_t *t)
{
	htab_listitem* item = NULL;

	for(unsigned i = 0; i < t->htab_size; i++)
	{ // Loop on list with indexes
		while((item = t->list[i]) != NULL)
		{ // loop on items
			htab_remove(t,item->key);
		}
	}
}
