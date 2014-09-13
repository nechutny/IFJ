/**
 * 	@project	IFJ 2014/2015
 *	@file		symbol.h
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "string.h"

typedef enum {
	variable_integer,
	variable_double,
	variable_boolean,
	variable_string,
	variable_char,
	variable_array
} variableType;

typedef struct TsymbolVariable {
	struct Tstring* name;
	variableType type;
	union {
		double value_double;
		long value_number;
		char value_string[256];
		int value_boolean;
		char value_char[1];
	} value; 
} symbolVariable;

typedef struct TsymbolFunction {
	struct Tstring* name;
	variableType returnType;
	variableType *args;
} symbolFunction;

#endif
