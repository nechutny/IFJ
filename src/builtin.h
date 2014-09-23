/**
 *	@project	IFJ 2014/2015
 *	@file		builtin.h
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#ifndef _BUILTIN_H_
#define _BUILTIN_H_

#include <stdarg.h>
#include <stdio.h>
#include "symbol.h"
#include "uStack.h"

void pascal_write(uStack_t* args);
void pascal_readln(symbolVariable* var);
int pascal_length(symbolVariable* var);

#endif