#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lex.h"
#include "types.h"
#include "garbage.h"

void parser(FILE* file)
{
	parser_file();
}

/**
 * program programName; <vars> <functions> begin <main> end.
 **/
void parser_file()
{
	TToken * token = token_get(global.file);
	
	if(token->type == token_program)
	{
		token_free(token);
		token = token_get(global.file);
		if(token->type == token_identifier)
		{
			token_free(token);
			token = token_get(global.file);
			if(token->type == token_semicolon)
			{
				parser_vars();
			}
			else
			{
				token_free(token);
				printf("Error: Program name should end with ';'.");
			}
			
		}
		else
		{
			printf("Error: Expected program name.\n");
		}
	}
	else
	{
		printf("Error: expected program\n");
	}
}

/**
 * var <var>
 **/
void parser_vars()
{
	TToken * token = token_get(global.file);
	
	if(token->type == token_var)
	{
		token_free(token);
		parser_var();
	}
	else
	{
		token_return_token(token);
	}
	
}

/**
 * ID : TYPE <varv>; <var>
 **/
void parser_var()
{
	TToken * token = token_get(global.file);
	
	if(token->type == token_identifier)
	{
		token_free(token);
		token = token_get(global.file);
		
		if(token->type == token_colon)
		{
			token_free(token);
			token = token_get(global.file);

			if(	token->type == token_real	||
				token->type == token_integer	||
				token->type == token_char	||
				token->type == token_boolean	)
			{
				token_free(token);
				token = token_get(global.file);

				if(token->type == token_equal)
				{
					token_free(token);
					token = token_get(global.file);

					if(token->type == token_int || token->type == token_double)
					{
						token_free(token);
						token = token_get(global.file);

						if(token->type == token_semicolon)
						{
							token_free(token);
							parser_var();
						}
						else
						{
							token_free(token);
							printf("Error: Expected ';'.\n");
						}
					}
					else
					{
						
						printf("Error: Bad inicialization value %d.\n",token->type);
						token_free(token);
					}
				}
				else if(token->type == token_semicolon)
				{
					token_free(token);
					parser_var();
				}
				else
				{
					token_free(token);
					printf("Error: Expected ';' or '='.\n");
				}
			}
			else
			{
				token_free(token);
				printf("Error: Bad variable type.\n");
			}
		}
		else
		{
			token_free(token);
			printf("Error: expected ':'.\n");
		}
	}
	else
	{
		token_return_token(token);
	}
	
}
