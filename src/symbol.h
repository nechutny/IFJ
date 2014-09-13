/**
 * 	@project	IFJ 2014/2015
 *	@file		symbol.h
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "string.h"
#include "htable.h"

typedef enum {
	variable_integer,
	variable_double,
	variable_boolean,
	variable_string,
	variable_char,
	variable_array
} variableType;

typedef struct TsymbolVariable {
	TString* name;			// Variable name
	variableType type;		// Variable type
	union {				// Variable value
		double value_double;
		long value_number;
		char value_string[256];
		int value_boolean;
		char value_char[1];
	} value;
	int inicialized;		// Is inicialized
} symbolVariable;

typedef struct TsymbolFunction {
	struct TString* name;
	variableType returnType;
	variableType *args;
} symbolFunction;

void symbol_variable_init(htab_listitem* var, char* name);

#endif
