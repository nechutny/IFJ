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


void pascal_write(int count, ...)
{
	symbolVariable* symbol;
	va_list args;
	va_start(args, count);
	
	for(int i = 0; i < count; i++)
	{
		symbol = va_arg(args, symbolVariable*);
		switch(symbol->type)
		{
			case variable_string:
				printf("%s",symbol->value.value_string);
				break;

			case variable_integer:
				printf("%ld",symbol->value.value_number);
				break;

			case variable_double:
				printf("%g",symbol->value.value_double);
				break;

			default:
				printf("WTF value");
		}
	}

	va_end(args);
}


int pasal_length(symbolVariable* var)
{
	return strlen(var->value.value_string);
}
