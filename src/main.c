#include <stdio.h>
#include "lex.h"
#include "types.h"
#include "expr.h"
#include "garbage.h"
#include "parser.h"

int main(int argc, char* argv[])
{
	global_init();
	
	TToken * token;
	global.file=fopen(argv[1],"r");
	if (global.file==NULL)
		return 0;
	parser(global.file);
	precedence(global.file);
	token = token_get(global.file);
	while (token->type!=token_eof)
	{
		//printf("%s ",token->data->data);
		//printf("%d\n",token->type);
		token_free(token);
		token = token_get(global.file);
	}
	return 0;
}
