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
	tmp->p = list->last;

	if(list->last != NULL)	list->last->n = tmp;
	if(list->first == NULL)	list->first = tmp;
	if(list->act == NULL)	list->act = tmp;
	
	list->last = tmp;
	return;
}

void list_insert_next(TList *list, void *data)
{
	TNode *tmp;
	tmp = _malloc(sizeof(TNode));
	if(tmp == NULL)
	{
		fprintf(stderr,"Null pointer.\n");
		exit(1);
	}

	tmp->data = data;
	tmp->p = list->act;
	
	if (list->act != NULL)
	{	
		tmp->n = list->act->n;
		list->act->n = tmp;
	}

	if(list->first == NULL)
	{
		list->first = tmp;
		list->act = list->first;
	}
	else
		list->act->n = tmp;
}

void list_insert_prev(TList *list, void *data)
{
	if( list->act != NULL)
	{
		TNode *tmp;
		tmp = _malloc(sizeof(TNode));
		if(tmp == NULL)
		{
			fprintf(stderr,"Null pointer.\n");
			exit(1);
		}

		tmp->data = data;
		tmp->n = list->act;

		if(list->act != NULL)
		{
			if( list->act->p != NULL)
				list->act->p->n = tmp;
			else
				list->first = tmp;

			tmp->p = list->act->p;
			list->act->p = tmp;
		}

		list->act->p = tmp;
	}
}

void list_next(TList *list)
{
	if( list->act != NULL)
		list->act = list->act->n;
}

void list_prev(TList *list)
{
	if (list->act != NULL)
		list->act = list->act->p;
}
