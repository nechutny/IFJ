/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14        */
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš,       */
/*                Michchal Jásenský,                                */
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00           */
/********************************************************************/

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

	global.file_name = argv[1];

	parser_file();

	interpret();
}
