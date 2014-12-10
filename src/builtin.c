/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14		*/
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš, 		*/
/*                Michchal Jásenský,								*/
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00 			*/
/********************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
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
				printf("%d", symbol->value.value_number);
				break;

			case variable_double:
				printf("%g", symbol->value.value_double);
				break;

			case variable_boolean:
				printf("%s", symbol->value.value_boolean ? "TRUE" : "FALSE");
				//printf("%d", symbol->value.value_boolean);
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
	char c;
	char buffer[255];
	char *ptr = buffer;

	// Can't read bool
	if(var->type == variable_boolean)
	{
		throw_error(error_read_to_bool);
	}

	// Ignore spaces at begin
	do
	{
		c = getchar();
	}
	while(isspace(c));

	// Integer/double
	if(var->type == variable_integer || var->type == variable_double)
	{
		if(c == EOF)
		{
			throw_error(error_read_invalid);
		}

		(*ptr) = c;

		for(unsigned int i = 0; i < 255 && !isspace(c) && c != EOF; i++)
		{ // Read until space, EOF or buffer end
			ptr++;
			c = getchar();
			(*ptr) = c;
		}

		*(ptr) = '\0';
		ptr = NULL;

		if(var->type == variable_integer)
		{ // Integer
			var->value.value_number = (int)strtol(buffer, &ptr, 10);
		}
		else
		{ // Double
			var->value.value_double = strtod(buffer, &ptr);
		}

		if(*ptr != '\0')
		{ // Check if is readed valid input
			throw_error(error_read_invalid);
		}

		var->inicialized = 1;

		if(c != EOF && c != '\n')
		{
			do
			{
				c = getchar();
			}
			while(c != EOF && c != '\n');
		}
	}
	else
	{ // String
		ptr = var->value.value_string;

		(*ptr) = c;

		for(unsigned int i = 0; i < 255 && c != EOF && c != '\n'; i++)
		{ // Read until new line, EOF or buffer end
			ptr++;
			c = getchar();
			(*ptr) = c;
		}

		*(ptr) = '\0';

		var->inicialized = 1;
	}
}

