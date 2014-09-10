/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ13		*/
/*Jména řešitelů: Michal Jásenský, Jiří Kulda ,Martin Filípek,      */
/*                Ondřej Vlk, Pet Lukeš								*/
/*Loginy řešitelů: xjasen00, xkulda00, xfilip32, xlukes06, xvlkon00 */
/********************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
//include "types.h"
#include "stack.h"
#include "main.h"
#include "garbage.h"

TStack * stack_init(){
	TStack *stack;
	stack = _malloc( sizeof ( TStack ) );
	if( stack == NULL ){
		fprintf( stderr, "%s", strerror( errno ) );
		return NULL;
	}
	stack->data = _malloc( sizeof ( void * )*10 );
	if( stack->data == NULL ){
		fprintf( stderr, "%s", strerror( errno ) );
		_free( stack );
		return NULL;
	}

	stack->allocated = 10;
	stack->count = 0;
	return stack;
}

void stack_push( TStack *stack, void *item ){
	if( stack->allocated < ( stack->count + 1 ) ){
		stack->data = _realloc( stack->data, ( sizeof( void * ) * ( stack->allocated + 10 ) ) );
		stack->allocated += 10;
	}
	stack->data[stack->count++] = item;
}

void stack_pop( TStack *stack ){
	if( stack->count != 0 ){
		stack->count--;
	}	
}

void * stack_top( TStack *stack ){
	
	if( stack->count != 0 ){
		return( stack->data[stack->count - 1] );
	}else{
		return NULL;
	}

}

int stack_count(TStack *stack){
	return stack->count;
}
