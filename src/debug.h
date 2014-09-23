/**
 *	@project	IFJ 2014/2015
 *	@file		debug.h
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <stdarg.h>

#define DEBUG_MESSAGES		1
#define DEBUG_MESSAGE_PARSER	1
#define DEBUG_MESSAGE_PREC	1
#define DEBUG_MESSAGE_SYMBOL	1

typedef enum {
	debug_parser,
	debug_prec,
	debug_symbol,
} debug_level;

void print_debug(debug_level lvl, const char *fmt, ...);



#endif
