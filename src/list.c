/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14        */
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš,       */
/*                Michchal Jásenský,                                */
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00           */
/********************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "list.h"
#include "garbage.h"

/**
 * crete new list
 *
 * @return			pointer to list
 */
TList* list_init()
{
	TList *list = _malloc(sizeof(TList));
	if(list == NULL)
	{
		fprintf(stderr,"Null pointer.\n");
		exit(1);
	}
	list->first = NULL;
	list->last = NULL;
	list->act = NULL;
	
	return list;
}

/**
 * insert new element into list
 *
 * @param	list	list intto insert
 * @param	node	node to insert
 */
void list_insert_node(TList *list, TNode *node){
	if(list->last != NULL)	list->last->n = node;
	if(list->first == NULL)	list->first = node;
	list->last = node;
	node->n = NULL;
	return;
}

/**
 * insert new element into list
 *
 * @param	list	list into insert
 * @param	data	data to insert
 */
void list_insert(TList *list, void *data)
{
	TNode *tmp  = _malloc(sizeof(TNode));
	if(tmp == NULL || list == NULL)
	{
		fprintf(stderr,"Null pointer.\n");
		exit(1);
	}

	tmp->data = data;
	tmp->n = NULL;

	if(list->last != NULL)	list->last->n = tmp;
	if(list->first == NULL)	list->first = tmp;
	
	list->last = tmp;
	return;
}

