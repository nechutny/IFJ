#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lex.h"
#include "types.h"
#include "garbage.h"
#include "expr.h"

static int isVariableType(int type);

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
				token_free(token);
				parser_vars();
				parser_function();
			}
			else
			{
				token_free(token);
				printf("Error: Program name should end with ';'.");
			}
			
		}
		else
		{
			token_free(token);
			printf("Error: Expected program name.\n");
		}
	}
	else
	{
		token_free(token);
		printf("Error: Expected 'program'.\n");
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

			if(isVariableType(token->type) )
			{
				token_free(token);
				token = token_get(global.file);

				if(token->type == token_equal)
				{
					token_free(token);
					precedence(global.file);
					
				}
				else if(token->type == token_semicolon)
				{
					token_free(token);
				}

				parser_var();
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
			printf("Error: Expected ':'.\n");
		}
	}
	else
	{
		token_return_token(token);
	}
}

void parser_function()
{
	TToken * token = token_get(global.file);
	if(token->type == token_function)
	{
		token_free(token);
		token = token_get(global.file);
		if(token->type == token_identifier)
		{
			printf("function %s:\n",token->data->data);
			token_free(token);
			token = token_get(global.file);
			if(token->type == token_parenthesis_left)
			{
				token_free(token);
				parser_args();
				
				token = token_get(global.file);
				if(token->type == token_parenthesis_right)
				{
					token_free(token);
					token = token_get(global.file);
					if(token->type == token_colon)
					{
						token_free(token);
						token = token_get(global.file);
						if(isVariableType(token->type))
						{
							token_free(token);
							token = token_get(global.file);
							if(token->type == token_semicolon)
							{
								token_free(token);
								parser_vars();
								token = token_get(global.file);
								if(token->type == token_begin)
								{
									token_free(token);
									parser_body();
									token = token_get(global.file);
									if(token->type == token_end)
									{
										token_free(token);
										token = token_get(global.file);
										if(token->type == token_semicolon)
										{
											token_free(token);
											parser_function();
										}
										else
										{
											token_free(token);
											printf("Error: Expected ';'.\n");
										}
									}
									else
									{
										token_free(token);
										printf("Error: Function block should ended with 'end'.\n");
									}
								}
								else
								{
									token_free(token);
									printf("Error: Function block should start with 'begin'.\n");
								}
							}
							else
							{
								token_free(token);
								printf("Error: Function return type should be ended with ';'.\n");
							}
						}
						else
						{
							token_free(token);
							printf("Error: Expected function return type.\n");
						}
					}
					else
					{
						token_free(token);
						printf("Error: Expected ':'.\n");
					}
				}
				else
				{
					printf("Error: Expected ')' %d.\n",token->type);
					token_free(token);
				}
			}
			else
			{
				printf("Error: Expected '(' %d.\n",token->type);
				token_free(token);
			}
		}
		else
		{
			token_free(token);
			printf("Error: Expected ':'.\n");
		}
	}
	else
	{
		token_return_token(token);
	}
}

void parser_args()
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
			if(isVariableType(token->type))
			{
				token_free(token);
				token = token_get(global.file);
				if(token->type == token_comma)
				{
					token_free(token);
					parser_args();
				}
				else
				{
					token_return_token(token);
				}
			}
			else
			{
				token_free(token);
				printf("Error: Expected function argument type.\n");
			}
		}
		else
		{
			token_free(token);
			printf("Error: Expected identifier.\n");
		}
	}
	else if(token->type == token_parenthesis_right)
	{
		token_return_token(token);
	}
	else
	{
		printf("Error: Expected identifier %d.\n",token->type);
		token_free(token);
	}
}

void parser_body()
{
	TToken * token = token_get(global.file);
	while(token->type != token_end)
	{
		if(token->type == token_return)
		{
			token_free(token);
			precedence(global.file);
		}
		else
		{
			token_return_token(token);
			parser_main();
		}
		token = token_get(global.file);
	}
	token_return_token(token);
}

void parser_main()
{
	TToken * token = token_get(global.file);
	while(token->type != token_end && token->type != token_return)
	{
		token_return_token(token);
		parser_code();
		token = token_get(global.file);
	}
	token_return_token(token);
}

void parser_code()
{
	TToken * token = token_get(global.file);
	if(token->type == token_identifier)
	{
		token_free(token);
		token = token_get(global.file);
		if(token->type == token_assign)
		{ // asign
			precedence(global.file);
		}
		else if(token->type == token_parenthesis_left)
		{ // function call
			token_free(token);
			token = token_get(global.file);
			while(token->type != token_parenthesis_right && token->type != token_semicolon)
			{
				token_return_token(token);
				precedence(global.file);
				token = token_get(global.file);
			}
		}
		else
		{
			printf("Error: Unkown variable operation %d.\n",token->type);
			token_free(token);
		}
		
	}
	else if(token->type == token_if)
	{ // if
		token_free(token);
		precedence(global.file);
		token = token_get(global.file);
		printf("If %d\n",token->type);
	}
	else if(token->type == token_while)
	{// while
		token_free(token);
		precedence(global.file);
		token = token_get(global.file);
		printf("While %d\n",token->type);
	}
	else if(token->type == token_repeat)
	{ // repeat
		
		token_free(token);
	}
	else if(token->type == token_for)
	{ // for
		token_free(token);
	}
	else if(token->type == token_label)
	{ // label
		token_free(token);
		token = token_get(global.file);
		if(token->type == token_identifier)
		{
			token_free(token);
			token = token_get(global.file);
			if(token->type != token_semicolon)
			{
				printf("Error: Label name should end with ';'.");
			}
			token_free(token);
		}
		else
		{
			printf("Error: Label has bad name.");
			token_free(token);
		}
	}
	else if(token->type == token_goto)
	{ // goto
		token_free(token);
		token = token_get(global.file);
		if(token->type == token_identifier)
		{
			token_free(token);
			token = token_get(global.file);
			if(token->type != token_semicolon)
			{
				printf("Error: Goto target should end with ';'.");
			}
			token_free(token);
		}
		else
		{
			printf("Error: Goto target has bad value.");
			token_free(token);
		}
	}
	else
	{
		printf("Error: Unkown command %d.\n",token->type);
		token_free(token);
	}

	
}

static int isVariableType(int type)
{
	return (	type == token_real	||
			type == token_integer	||
			type == token_char	||
			type == token_boolean	);
}
