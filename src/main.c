#include <stdio.h>
#include "lex.h"
#include "types.h"
#include "expr.h"

int main(int argc, char* argv[])
{
	TToken * token;
	FILE * file;
	file=fopen(argv[1],"r");
	if (file==NULL)
		return 0;
	precedence(file);
	token = token_get(file);
	while (token->type!=token_eof)
	{
		printf("%s ",token->data->data);
		printf("%d\n",token->type);
		token_free(token);
		token = token_get(file);
	}
	fclose(file);
	return 0;
}
