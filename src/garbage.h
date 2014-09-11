/**
 * 	@project	IFJ 2014/2015
 *	@file		garbage.h
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#ifndef _GARBAGE_H_
#define _GARBAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/*
 *   __      __  _____    _____   _______    ____    _____  
 *   \ \    / / |_   _|  / ____| |__   __|  / __ \  |  __ \ 
 *    \ \  / /    | |   | |         | |    | |  | | | |__) |
 *     \ \/ /     | |   | |         | |    | |  | | |  _  / 
 *      \  /     _| |_  | |____     | |    | |__| | | | \ \ 
 *       \/     |_____|  \_____|    |_|     \____/  |_|  \_\
 * 
 *            The Cleaner v1.0
 */


/*
 * Structure representing one allocated pointer
 */
struct TAllocItem
{
	void* ptr;
	struct TAllocItem* next;
};


/*
 * Global variable accessable as "global" after including garbage.h
 */
struct TGlobal
{
	struct TAllocItem* allocated;
	struct TAllocItem* allocated_last;

	FILE* file;
	TList* ins_list;
};
extern struct TGlobal global;


void global_init();
void global_free();
void* _malloc(unsigned long size);
void* _realloc(void* ptr, unsigned long size);


/**
 * Free allocated memory by _malloc
 *
 * Find item in list, free it and remove from list. normal free() can't be used
 * on memory allocated via _malloc.
 *
 * @param	_ptr	pointer to memory
 **/
#define _free(_ptr)								\
do {										\
	struct TAllocItem* tmp = global.allocated;				\
	struct TAllocItem* prev = tmp;						\
	while(tmp != NULL)							\
	{									\
		if(tmp->ptr == _ptr)						\
		{								\
			if(prev != tmp && tmp->next != NULL)			\
			{							\
				prev->next = tmp->next;				\
			}							\
			else if(prev == tmp && tmp->next != NULL)		\
			{							\
				global.allocated = tmp->next;			\
			}							\
										\
			if(tmp->next == NULL)					\
			{							\
				global.allocated_last = prev;			\
				prev->next = NULL;				\
			}							\
										\
			if(prev == tmp && tmp->next == NULL)			\
			{							\
				tmp->ptr = NULL;				\
			}							\
			else							\
			{							\
				free(tmp);					\
			}							\
										\
			break;							\
		}								\
										\
		prev = tmp;							\
		tmp = tmp->next;						\
	}									\
										\
	if(_ptr != NULL)							\
	{									\
		free(_ptr);							\
		_ptr = NULL;							\
	}									\
} while(0);

#endif
