/**
 * 	@project	IFJ 2014/2015
 *	@file		uStack.h
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#ifndef _USTACK_H_
#define _USTACK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* !!! count must be first element and UL for inicializing in uStack_init !!! */
typedef struct {
	unsigned long count;
	unsigned long allocated;
	void **data;
} uStack_t;

#include "garbage.h"


/**
 * Prepare variable for stack
 *
 * @param	name 	name of created variable
 */
#define uStack_init(name)	\
	uStack_t* name = memset(_malloc(sizeof(uStack_t)), 0UL ,sizeof(unsigned long));


/**
 * Push data to stack
 *
 * @param	type	data type of pushed value. Must be same for all
 * 			pushed values to stack. eg. double, long
 * @param	stack	pointer to stack for push
 * @param	_data	Constant value, or variable for push
 */
#define uStack_push(type, stack, _data)							\
do {											\
	if(stack->count == 0)								\
	{										\
		stack->data = _malloc(sizeof(type)*2);					\
		stack->allocated = 2;							\
	}										\
	else if(stack->allocated <= stack->count)					\
	{										\
		stack->allocated += 2;							\
		stack->data = _realloc(stack->data, sizeof(type)*stack->allocated);	\
	}										\
	((type *)stack->data)[stack->count] = _data;					\
	stack->count++;									\
} while(0);


/**
 * Remove data from top of stack
 * 
 * @param	stack	pointer to stack for remove data
 * @return	new number of items in stack
 **/	
#define uStack_remove(stack)								\
	--(stack->count)


/**
 * Pop data from stack
 *
 * @param	type	data type of poped value. Must be same for all
 * 			poped values to stack. eg. double, long
 * @param	stack	pointer to stack for pop
 * @return	Value from stack
 */
#define uStack_pop(type, stack)								\
	(stack->count--, ((type *)stack->data)[stack->count])


/**
 * Get item from stack by offset
 *
 * @param	type	data type of poped value. Must be same for all
 * 			poped values to stack. eg. double, long
 * @param	stack	pointer to stack for data
 * @param	offset	integer with offset for data (from 0 to n-1)
 * @return	Value from stack 
 */
#define uStack_offset(type, stack,offset)						\
	((type *)stack->data)[offset]


/**
 * Return data from top of stack
 *
 * @param	type	data type of poped value. Must be same for all
 * 			poped values to stack. eg. double, long
 * @param	stack	pointer to stack for pop
 * @return	Value from stack
 */
#define uStack_top(type, stack)								\
	((type *)stack->data)[stack->count-1]


/**
 * Return number of pushed values in stack
 *
 * @param	stack	pointer to stack
 * @return	Number of items in stack
 */
#define uStack_count(stack)								\
	(stack->count)


/**
 * Free all allocated memory for stack and data
 * 
 * @param	stack	pointer to stack
 */
#define uStack_free(stack)								\
do {											\
	_free(stack->data);								\
	_free(stack);									\
} while(0);


#endif
