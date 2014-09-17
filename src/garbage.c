/**
 * 	@project	IFJ 2014/2015
 *	@file		garbage.c
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "garbage.h"
#include "list.h"
#include "uStack.h"

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

/**
 * Handle SIGINT and SIGTERM, clean memory and exit(1);
 *
 * @param	signo	Code of received signal
 **/
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
inline void* _malloc(unsigned long size)
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
 * Function doing memory reallocation, removing old pointer and saving pointer
 * for auto-free.
 * Usage is same as realloc
 *
 *  @param	ptr	Pointer to current allocated memory
 *  @param	size	Number of bytes to allocate
 *  @return	pointer to memory, or NULL if fail
 **/
inline void* _realloc(void* ptr, unsigned long size)
{
	struct TAllocItem* tmp = global.allocated;
	while(tmp != NULL)
	{
		if(tmp->ptr == ptr)
		{
			tmp->ptr = realloc(tmp->ptr, size);
			return tmp->ptr;
		}
		tmp = tmp->next;
	}
	
	return NULL;
}


/**
 * Inicialize global variable and set handler for auto-free
 **/
void global_init()
{
	global.allocated = malloc(sizeof(struct TAllocItem));
	global.allocated_last = global.allocated;
	global.allocated->ptr = NULL;

	global.local_symbols = _malloc(sizeof(uStack_t));
	global.local_symbols->count = 0;

	global.ins_list = list_init();

	// 42 is answer for anything!
	global.global_symbol = htab_init(42);
	
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);
	atexit(global_free);
}

void printDataFunction(struct TsymbolFunction* data)
{
	printf("\t return type: %d\n",data->returnType);
	printf("\t arguments: %d\n",data->args_count);
	for(int i = 0; i < data->args_count; i++)
	{
		printf("\t\targ type: %d, name: %s\n",data->args[i].type, data->args[i].name->data);
	}
}

void printDataVariable(struct TsymbolVariable* data)
{
	printf("\t type: %d\n",data->type);
}

void printData(char key[], item_type type, htab_listitem* item)
{
	printf("Name: %s",key);
	if(type  == type_function)
	{
		printf(" - function\n");
		printDataFunction(item->ptr.function);
	}
	else
	{
		printf(" - variable\n");
		printDataVariable(item->ptr.variable);
	}
}

/**
 * Loop on linked list of allocated memory and free
 **/
void global_free()
{
	struct TAllocItem* tmp;
	struct TAllocItem* ptr;

	printf("\n\n Global symbols:\n");
	htab_foreach(global.global_symbol, printData);

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
	

	fclose(global.file);
	
	//htab_free(global.global_symbol);
}
