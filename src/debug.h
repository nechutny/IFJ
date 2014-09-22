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

#define DEBUG_MESSAGES	1

void print_debug(const char *fmt, ...);

#endif
