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
