#include <stdio.h>
#include "lex.h"
#include "types.h"
#include "expr.h"
#include "garbage.h"
#include "parser.h"
#include "generator.h"
#include "uStack.h"
#include "interpret.h"

int main(int argc, char* argv[])
{
	if(argc  != 2)
	{
		return 99;
	}
	
	global_init();

	if( (global.file = fopen(argv[1], "r")) == NULL)
		return 99;

	parser_file();

	interpret();

	//printf("***********\nvysledek: %lu\n***********\n", VariableExists("ret")->ptr.variable->value.value_number);

	return 0;
}
