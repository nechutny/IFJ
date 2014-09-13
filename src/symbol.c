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


void symbol_variable_init(htab_listitem* var, char* name)
{
	var->type = type_function;

	var->ptr.variable = _malloc(sizeof(symbolVariable));

	var->ptr.variable->name = string_new();
	string_add(var->ptr.variable->name, name);
}
