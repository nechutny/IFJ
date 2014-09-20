#include "debug.h"
#include "garbage.h"
#include "uStack.h"

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
