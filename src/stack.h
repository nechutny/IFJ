#ifndef STACK_H
#define STACK_H

//include "types.h"

typedef struct _stack {
	void ** data;
	int allocated;
	int count;
} TStack;

TStack * stack_init();

void stack_push( TStack *stack, void *item );

void stack_pop( TStack *stack );

void * stack_top( TStack *stack );

void stack_free( TStack *stack );

int stack_count(TStack * stack);

#endif
