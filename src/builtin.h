/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14		*/
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš, 		*/
/*                Michchal Jásenský,								*/
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00 			*/
/********************************************************************/

#ifndef _BUILTIN_H_
#define _BUILTIN_H_

#include <stdarg.h>
#include <stdio.h>
#include "symbol.h"
#include "uStack.h"

void pascal_write(uStack_t* args);
void pascal_readln(symbolVariable* var);

/**
 * Return length for string variable
 *
 * @param	var	Structure with variable for result
 * @return	String length
 */
#define pascal_length(var)  strlen(var->value.value_string);

#endif
