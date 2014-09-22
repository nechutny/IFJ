#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "list.h"
#include "garbage.h"

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

void list_insert_node(TList *list, TNode *node){
	if(list->last != NULL)	list->last->n = node;
	if(list->first == NULL)	list->first = node;
	list->last = node;
	return;
}

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

