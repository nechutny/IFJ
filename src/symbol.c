/**
 * 	@project	IFJ 2014/2015
 *	@file		symbol.c
 *
 *	@author		Stanislav Nechutny - xnechu01
 */

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "symbol.h"
#include "htable.h"
#include "garbage.h"
#include "error.h"


void symbol_variable_init(htab_listitem* var, char* name)
{
	var->type = type_variable;

	var->ptr.variable = _malloc(sizeof(symbolVariable));

	var->ptr.variable->name = string_new();
	string_add(var->ptr.variable->name, name);
}

variableType symbol_type(TToken_type token_type)
{
	switch(token_type)
	{
		case token_integer:
			return variable_integer;
			break;

		case token_double:
			return variable_double;
			break;

		case token_boolean:
			return variable_boolean;
			break;

		case token_string_var:
			return variable_string;
			break;

		case token_char:
			return variable_char;
			break;

		case token_array:
			return variable_array;
			break;
			
		default:
			fprintf(stderr, "Error: Chuck Norris? No way how to get there!\n");
	}

	throw_error(error_type);
	return token_integer;
}

void symbol_variable_type_set(symbolVariable* variable, TToken_type token_type)
{
	variable->type = symbol_type(token_type);
}


void symbol_function_type_set(symbolFunction* variable, TToken_type token_type)
{
	variable->returnType = symbol_type(token_type);
}


void symbol_function_init(htab_listitem* var, char* name)
{
	var->type = type_function;

	var->ptr.function = _malloc(sizeof(symbolFunction));
	
	var->ptr.function->args_count = 0;
	var->ptr.function->args = NULL;
	var->ptr.function->name = string_new();
	string_add(var->ptr.function->name, name);
}

void symbol_function_arg_add(symbolFunction* func, char* name, TToken_type token_type)
{
	(func->args_count)++;
	
	if(func->args_count == 1)
	{
		func->args = _malloc(sizeof(functionArgs));
	}
	else
	{
		func->args = _realloc(func->args, sizeof(functionArgs)*func->args_count);
	}
	
	if(func->args == NULL)
	{
		fprintf(stderr,"Error: NUll pointer in symbol_function_arg_add\n");
	}
	
	func->args[ func->args_count-1 ].type = symbol_type(token_type);
	
	func->args[ func->args_count-1 ].name = string_new();
	string_add(func->args[func->args_count-1].name, name);
}
