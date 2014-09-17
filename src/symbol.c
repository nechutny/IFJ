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


/**
 * Set correct value to hash table for variable
 *
 * @param	var	hash table item reference
 * @param	name	Variable name
 */
void symbol_variable_init(htab_listitem* var, char* name)
{
	var->type = type_variable;

	var->ptr.variable = _malloc(sizeof(symbolVariable));

	var->ptr.variable->name = string_new();
	string_add(var->ptr.variable->name, name);
}


/**
 * Translate token_type to variableType
 *
 * @param	token_type	type of token
 * @return	variable type
 */
variableType symbol_type(TToken_type token_type)
{
	switch(token_type)
	{
		case token_integer:
			return variable_integer;
			break;

		case token_real:
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


/**
 * Set correct variable type based on token type
 *
 * @param	variable	Pointer to variable structure
 * @param	token_type	Token type
 */
void symbol_variable_type_set(symbolVariable* variable, TToken_type token_type)
{
	variable->type = symbol_type(token_type);
}


/**
 * Create symbolVariable for constant value
 *
 * @param	token	pointer to token with value
 * @return	Pointer to new symbolVariable structure
 */
symbolVariable* create_const(TToken *token)
{
	if(token == NULL)
	{
		fprintf(stderr,"create_const got NULL\n");
		return NULL;
	}
	
	symbolVariable* var = _malloc(sizeof(symbolVariable));
	var->name = NULL;
	var->inicialized = 0;
	
	switch(token->type)
	{
		case token_int:
			var->type = var_int;
			var->value.value_number = atoi(token->data->data);
			var->inicialized = 1;
			return var;
			break;
			
		case token_double:
			var->type = var_double;
			var->value.value_double = atof(token->data->data);
			var->inicialized = 1;
			return var;
			break;
			
		default:
			return var;
	}
}


/**
 * Set correct function return type based on token type
 *
 * @param	variable	Pointer to fucntion structure
 * @param	token_type	Token type
 */
void symbol_function_type_set(symbolFunction* variable, TToken_type token_type)
{
	variable->returnType = symbol_type(token_type);
}


/**
 * Set correct value to hash table for function
 *
 * @param	var	hash table item reference
 * @param	name	function name
 */
void symbol_function_init(htab_listitem* var, char* name, unsigned long offset)
{
	var->type = type_function;

	var->ptr.function = _malloc(sizeof(symbolFunction));
	
	var->ptr.function->args_count = 0;
	var->ptr.function->args = NULL;
	var->ptr.function->startOffset = offset;
	var->ptr.function->name = string_new();
	string_add(var->ptr.function->name, name);
}


/**
 * Add one more argument to function structure
 *
 * @param	func		pointer to fucntion structure
 * @param	name		variable name
 * @param	token_type	variable type
 */
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

