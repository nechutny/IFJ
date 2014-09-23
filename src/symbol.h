/**
 *	@project	IFJ 2014/2015
 *	@file		symbol.h
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "string.h"
#include "types.h"
#include "list.h"
#include "htable.h"


#define VariableExists(name)							\
	(uStack_count(global.local_symbols) == 0 ? htab_lookup(global.global_symbol,name) : ((htab_lookup(uStack_top(htab_t*, global.local_symbols), name) == NULL )  ? htab_lookup(global.global_symbol,name) : htab_lookup(uStack_top(htab_t*, global.local_symbols), name)))


typedef enum {
	variable_integer,
	variable_double,
	variable_boolean,
	variable_string,
	variable_char,
	variable_array
} variableType;

typedef struct {
	variableType type;
	TString* name;
} functionArgs;

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

//#include "htable.h"

typedef struct TsymbolFunction {
	TString* name;
	variableType returnType;
	unsigned int args_count;
	functionArgs *args;
	int defined;
	unsigned long startOffset;
	TList * ins;
	htab_t* local_symbol;
} symbolFunction;


void symbol_variable_init(htab_listitem* var, char* name);
void symbol_variable_type_set(symbolVariable* variable, TToken_type token_type);

symbolVariable *create_const(TToken *token);

void symbol_function_init(htab_listitem* var, char* name, unsigned long offset);
void symbol_function_type_set(symbolFunction* variable, TToken_type token_type);
void symbol_function_arg_add(symbolFunction* variable, char* name, TToken_type token_type);

#endif
