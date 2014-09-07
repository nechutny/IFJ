#include <stdio.h>
#include <stdlib.h>

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

struct TAllocItem
{
	void* ptr;
	struct TAllocItem* next;
};

struct TGlobal
{
	struct TAllocItem* allocated;
	struct TAllocItem* allocated_last;
};

extern struct TGlobal global;

/**
 * Inicialize global variable with correct values
 */
void global_init();

/**
 * Free all allocated memory by _malloc
 */
void global_free();

void* _malloc(unsigned long size);

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
