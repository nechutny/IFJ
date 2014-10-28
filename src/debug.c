/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14		*/
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš, 		*/
/*                Michchal Jásenský,								*/
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00 			*/
/********************************************************************/

#include "debug.h"
#include "garbage.h"
#include "uStack.h"

#ifdef print_debug
#undef print_debug
#endif

/**
 * Print properly indented message only if are debug messages enabled
 *
 * @param	lvl		Type of debug information for disable/enable in debug.h
 * @param	fmt		Formating string like in printf
 * @param	variadic	Values for %x in first string
 */
void print_debug(debug_level lvl, const char *fmt, ...)
{
	if(!DEBUG_MESSAGES)
	{
		return;
	}

	if(!DEBUG_MESSAGE_PARSER && lvl == debug_parser)
	{
		return;
	}

	if(!DEBUG_MESSAGE_PREC && lvl == debug_prec)
	{
		return;
	}

	if(!DEBUG_MESSAGE_SYMBOL && lvl == debug_symbol)
	{
		return;
	}

	if(!DEBUG_MESSAGE_INTERPRET && lvl == debug_interpret)
	{
		return;
	}

	if(!DEBUG_MESSAGE_GENERATOR && lvl == debug_generator)
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
