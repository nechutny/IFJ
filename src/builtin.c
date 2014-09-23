/**
 *	@project	IFJ 2014/2015
 *	@file		builtin.c
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "builtin.h"
#include "symbol.h"
#include "uStack.h"
#include "error.h"


/**
 * Write arguments to stdout
 *
 * @param	args	uStack_t with arguments (symbolVariable* type)
 */
void pascal_write(uStack_t* args)
{
	symbolVariable* symbol;
	
	for(int i = 0; i < uStack_count(args); i++)
	{
		symbol = uStack_offset(symbolVariable*, args, i);
		
		switch(symbol->type)
		{
			case variable_string:
				printf("%s", symbol->value.value_string);
				break;

			case variable_integer:
				printf("%ld", symbol->value.value_number);
				break;

			case variable_double:
				printf("%g", symbol->value.value_double);
				break;

			default:
				fprintf(stderr, "WTF value");
		}
	}
}


/**
 * Read from stdin and set value to variable
 *
 * @param	var	Structure with variable for result
 */
void pascal_readln(symbolVariable* var)
{
	int readed = 0;
	switch(var->type)
	{
		case variable_string:
			readed = scanf("%255s", (var->value.value_string));
			break;

		case variable_integer:
			readed = scanf("%ld", &(var->value.value_number));
			break;

		case variable_double:
			readed = scanf("%lf", &(var->value.value_double));
			break;

		case variable_boolean:
			throw_error(error_read_to_bool);
			break;

		default:
			fprintf(stderr, "WTF type");
	}
	if(readed != 1)
	{
		throw_error(error_read_invalid);
	}
}


/**
 * Return length for string variable
 *
 * @param	var	Structure with variable for result
 * @return	String length
 */
int pascal_length(symbolVariable* var)
{
	return strlen(var->value.value_string);
}