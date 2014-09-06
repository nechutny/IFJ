#include <stdio.h>

#include "lex.h"
#include "types.h"

int main(int argc, char* argv[])
{
	TToken * token;
	FILE * file;
	file=fopen("sourcetest.pas","r");
	if (file==NULL)
		return 0;
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
