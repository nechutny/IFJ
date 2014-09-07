#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "garbage.h"

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

/**
 * Global variable containing pointer to allocated memory for automatic
 * cleaning when shuting down
 */
struct TGlobal global;

void sig_handler(int signo)
{
	global_free();
	exit(1);
}

/**
 * Function doing memory allocation and saving pointer for auto-free.
 * Usage is same as malloc
 *
 *  @param	size	Number of bytes to allocate
 *  @return	pointer to memory, or NULL if fail
 **/
void* _malloc(unsigned long size)
{
	if(global.allocated->ptr != NULL || global.allocated != global.allocated_last)
	{
		global.allocated_last->next = malloc(sizeof(struct TAllocItem));
		global.allocated_last = global.allocated_last->next;
	}
	global.allocated_last->ptr = malloc(size);
	global.allocated_last->next = NULL;

	return global.allocated_last->ptr;
}


/**
 * Inicialize global variable and set handler for auto-free
 **/
void global_init()
{
	global.allocated = malloc(sizeof(struct TAllocItem));
	global.allocated_last = global.allocated;
	global.allocated->ptr = NULL;

	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);
	atexit(global_free);
}


/**
 * Loop on linked list of allocated memmory and free
 **/
void global_free()
{
	struct TAllocItem* tmp;
	struct TAllocItem* ptr;

	ptr = global.allocated;

	while(ptr != NULL)
	{
		if(ptr->ptr != NULL)
		{
			free(ptr->ptr);
		}
		
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
	
}
