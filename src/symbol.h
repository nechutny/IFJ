/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14		*/
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš, 		*/
/*                Michchal Jásenský,								*/
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00 			*/
/********************************************************************/

#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include <stdio.h>
#include "string.h"
#include "types.h"
#include "list.h"
#include "error.h"

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
	int inicialized;		// Is inicialized
	variableType type;		// Variable type
	union {				// Variable value
		double value_double;
		int value_number;
		char value_string[256];
		int value_boolean;
		char value_char[1];
	} value;
	char name[];			// Variable name
} symbolVariable;

#include "ial.h"

typedef struct TsymbolFunction {
	TString* name;
	variableType returnType;
	unsigned int args_count;
	functionArgs *args;
	unsigned long startOffset;
	TList * ins;
	htab_t* local_symbol;
	int defined;
} symbolFunction;


void symbol_variable_init(htab_listitem* var, char* name);
symbolVariable *symbol_variable_init2(variableType type);

void copy_variable(symbolVariable *var1, symbolVariable *var2);
symbolVariable *create_const(TToken *token);

void symbol_function_init(htab_listitem* var, char* name, unsigned long offset);
void symbol_function_type_set(symbolFunction* variable, TToken_type token_type);
void symbol_function_arg_add(symbolFunction* variable, char* name, TToken_type token_type);

#define VariableExists(name)							\
	(uStack_count(global.local_symbols) == 0 ? htab_lookup(global.global_symbol,name) : ((htab_lookup(uStack_top(htab_t*, global.local_symbols), name) == NULL )  ? htab_lookup(global.global_symbol,name) : htab_lookup(uStack_top(htab_t*, global.local_symbols), name)))


/**
 * Translate token_type to variableType
 *
 * @param	token_type	type of token
 * @return	variable type
 */
variableType inline symbol_type(TToken_type token_type)
{
	switch(token_type)
	{
		case token_integer:
			return variable_integer;
			break;

		case token_char:
			return variable_char;
			break;

		case token_string_var:
			return variable_string;
			break;

		case token_real:
			return variable_double;
			break;

		case token_boolean:
			return variable_boolean;
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
#define symbol_variable_type_set(variable, token_type)				\
	variable->type = symbol_type(token_type);


#endif
