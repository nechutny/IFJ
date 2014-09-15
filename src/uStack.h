/**
 * 	@project	IFJ 2014/2015
 *	@file		uStack.h
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#ifndef _USTACK_H_
#define _USTACK_H_

#include "garbage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
	unsigned int allocated;
	unsigned int count;
	void **data;
} uStack_t;


/**
 * Prepare variable for stack
 *
 * @param	name 	name of created variable
 */
#define uStack_init(name)	\
	uStack_t* name = _malloc(sizeof(uStack_t));


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
	if(stack->data == NULL)								\
	{										\
		stack->data = _malloc(sizeof(type));					\
		stack->allocated = 1;							\
	}										\
	else if(stack->allocated <= stack->count)					\
	{										\
		stack->allocated++;							\
		stack->data = _realloc(stack->data, sizeof(type)*stack->allocated);	\
	}										\
	((type *)stack->data)[stack->count] = _data;					\
	stack->count++;									\
} while(0);


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
 * Return number of pushed values in stack
 *
 * @param	stack	pointer to stack
 * @return	Number of items in stack
 */
#define uStack_count(stack)								\
	stack->count;


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
