/**
 *	@project	IFJ 2014/2015
 *	@file		debug.c
 *
 *	@author		Stanislav Nechutny - xnechu01
 */

#include "debug.h"
#include "garbage.h"
#include "uStack.h"


/**
 * Print properly indented message only if are debug messages enabled
 *
 * @param	fmt		Formating string like in printf
 * @param	variadic	Values for %x in first string
 */
void print_debug(const char *fmt, ...)
{
	if(!DEBUG_MESSAGES)
	{
		return;
	}

	va_list args;
	va_start(args, fmt);
	
	for(int i = 0; i < uStack_count(global.local_symbols); i++)
	{
		printf("\t");
	}
	
	vprintf(fmt, args);
	printf("\n");

	va_end(args);
}
