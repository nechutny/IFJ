/**
 * 	@project	IFJ 2014/2015
 *	@file		parser.c
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lex.h"
#include "types.h"
#include "garbage.h"
#include "expr.h"
#include "error.h"

static int isVariableType(int type);

/**
 * Main function for starting parser.
 * Depend on global variable 'global' with opened file.
 */
void parser()
{
	parser_file();
}


/**
 * program programName;
 * <vars>
 * <functions>
 * begin
 * 	<main>
 * end.
 **/
void parser_file()
{
	TToken * token = token_get();
	
	/* Program name */
	if(token->type != token_program)
	{
		throw_error(error_program);
	}
	token_free(token);
	
	token = token_get();
	if(token->type != token_identifier)
	{
		throw_error(error_identifier);
	}
	printf("Program name %s\n",token->data->data);
	token_free(token);
	
	token = token_get();
	if(token->type != token_semicolon)
	{
		throw_error(error_semicolon);
	}
	token_free(token);
	
	/* Global variables */
	parser_vars();
	
	/* Functions */
	parser_function();
	
	token = token_get();
	if(token->type != token_begin)
	{ /* Main body */
		throw_error(error_begin);
	}
	token_free(token);
	
	printf("Main body\n");
	parser_main();
	
	token = token_get();
	if(token->type != token_end)
	{
		throw_error(error_end);
	}
	token_free(token);
	
	token = token_get();
	if(token->type != token_dot)
	{
		throw_error(error_dot);
	}

	printf("Main body end\n");

	token = token_get();
	if(token->type != token_eof)
	{
		throw_error(error_eof);
	}
}


/**
 * var <var>
 **/
void parser_vars()
{
	TToken * token = token_get();
	
	if(token->type == token_var)
	{ /* Found variable delcaration(s) */
		token_free(token);
		parser_var();
	}
	else
	{
		token_return_token(token);
	}
}


/**
 * ID [, ID] : TYPE <varv>;
 * <var>
 **/
void parser_var()
{
	TToken * token = token_get();
	
	if(token->type == token_identifier)
	{ /* Variable identifier */
		token_return_token(token);
		
		do { /* ID [, ID] */
			token = token_get();
			if(token->type != token_identifier)
			{
				throw_error(error_identifier);
			}
			printf("Variable %s\n",token->data->data);

			htab_listitem* var = htab_create(global.global_symbol, token->data->data);
			symbol_variable_init(var, token->data->data);
			
			token_free(token);

			token = token_get();
		} while(token->type == token_comma);

		if(token->type != token_colon)
		{
			throw_error(error_colon);
		}
		token_free(token);
		
		token = token_get();
		if(isVariableType(token->type) )
		{ /* var type */
			token_free(token);
			token = token_get();

			if(token->type == token_equal)
			{ /* Inicialize value */
				token_free(token);
				if(precedence(global.file, context_assign))
				{
					throw_error(error_expresion);
				}
				
			}
			else if(token->type == token_semicolon)
			{ /* Unicialized */
				token_free(token);
			}

			/* Check for more variables */
			parser_var();
		}
		else
		{
			throw_error(error_type);
		}
	}
	else
	{ /* No variable definition, so return token and exit */
		token_return_token(token);
	}
}


/**
 * Parse function declaration
 */
void parser_function()
{
	TToken * token = token_get();
	if(token->type == token_function)
	{ /* Function keywords? */
		token_free(token);
		token = token_get();
		if(token->type != token_identifier)
		{ /* Identifier */
			throw_error(error_identifier);
		}
		printf("function %s:\n",token->data->data);
		token_free(token);
		
		token = token_get();
		if(token->type != token_parenthesis_left)
		{
			throw_error(error_parenthesis_left);
		}
		token_free(token);
		
		/* Function arguments */
		parser_args();
		
		token = token_get();
		if(token->type != token_parenthesis_right)
		{
			throw_error(error_parenthesis_right);
		}
		token_free(token);
		
		token = token_get();
		if(token->type != token_colon)
		{
			throw_error(error_colon);
		}
		token_free(token);
		
		token = token_get();
		if(!isVariableType(token->type))
		{ /* Return type */
			throw_error(error_type);
		}
		token_free(token);
		
		token = token_get();
		if(token->type != token_semicolon)
		{
			throw_error(error_semicolon);
		}
		token_free(token);

		/* Function variables */
		parser_vars();
		
		token = token_get();
		if(token->type != token_begin)
		{ /* Function body */
			throw_error(error_begin);
		}
		token_free(token);
		
		do
		{
			parser_body();
			token = token_get();
		} while(token->type != token_end);
		
		if(token->type != token_end)
		{ /* Function code block end */
			throw_error(error_end);
		}
		token_free(token);
		
		token = token_get();
		if(token->type == token_semicolon)
		{
			token_free(token);
			parser_function();
		}
		else
		{
			throw_error(error_semicolon);
		}
	}
	else
	{
		token_return_token(token);
	}
}


/**
 * Parse function arguments
 * ID : type [, args]
 */
void parser_args()
{
	TToken * token = token_get();

	if(token->type == token_identifier)
	{ /* ID */
		token_free(token);
		token = token_get();
		if(token->type == token_colon)
		{
			token_free(token);
			token = token_get();
			if(isVariableType(token->type))
			{ /* Datetype */
				token_free(token);
				token = token_get();
				if(token->type == token_comma)
				{ /* Comma, so check for more arguments? */
					token_free(token);
					parser_args();
				}
				else
				{ /* No more arguments */
					token_return_token(token);
				}
			}
			else
			{
				throw_error(error_type);
			}
		}
		else
		{
			throw_error(error_identifier);
		}
	}
	else if(token->type == token_parenthesis_right)
	{ /* Got ')' so function haven't any argument */
		token_return_token(token);
	}
	else
	{
		throw_error(error_identifier);
	}
}


/**
 * Parse function body containing code
 */
void parser_body()
{
	printf("Function body\n");
	TToken * token = token_get();
	while(token->type != token_end)
	{ /* Parse content until we get end; */
		token_return_token(token);
		parser_main();
		
		token = token_get();
	}
	printf("Function body end\n");
	token_return_token(token);
}


/**
 * Code block containing more code ended with 'end'
 */
void parser_main()
{
	printf("Code block\n");
	TToken * token = token_get();
	while(token->type != token_end)
	{
		token_return_token(token);
		parser_code();
		token = token_get();
	}
	printf("Code block end\n");
	token_return_token(token);
}


/**
 * Parse one command
 */
void parser_code()
{
	printf("One command: ");
	
	TToken * token = token_get();
	
	switch(token->type)
	{
		case token_identifier:
			/* assign or function call */
			token_free(token);
			token = token_get();
			if(token->type == token_assign || token->type == token_bracket_left)
			{ /* assign */
				if(token->type == token_bracket_left)
				{ /* Array index? */
					token_return_token(token);
					
					if(precedence(global.file, context_index))
					{
						throw_error(error_expresion);
					}
					
					token = token_get();
					if(token->type != token_bracket_right)
					{ /* then? */
						throw_error(error_bracket_right);
					}
					token_free(token);

					token = token_get();
					if(token->type != token_assign)
					{
						throw_error(error_assign);
					}
					token_free(token);
				}
				printf("assign\n");
				if(precedence(global.file, context_assign))
				{
					throw_error(error_expresion);
				}
			}
			else if(token->type == token_parenthesis_left)
			{ /* function call */
				printf("function call\n");
				token_return_token(token);
				if(precedence(global.file, context_args))
				{
					throw_error(error_expresion);
				}
			}
			else
			{
				throw_error(error_unkown);
			}
			break;
			
		case token_if:
			/* if */
			token_free(token);
			parser_if();
			break;
			
		case token_while:
			/* while */
			token_free(token);
			parser_while();
			break;
			
		case token_repeat:
			/* repeat */
			token_free(token);
			parser_repeat();
			break;
			
		case token_for:
			/* for */
			token_free(token);
			parser_for();
			break;
			
		case token_label:
			/* label */
			token_free(token);
			parser_label();
			break;
			
		case token_goto:
			/* goto */
			token_free(token);
			parser_goto();
			break;
			
		case token_case:
			/* switch */
			token_free(token);
			parser_switch();
			break;
			
		case token_return:
			/* return */
			printf("return \n");
			token_free(token);
			if(precedence(global.file, context_assign))
			{
				throw_error(error_expresion);
			}
			break;
			
		case token_semicolon:
			/* empty command */
			break;
			
		default:
			/* Unkown command */
			throw_error(error_unkown);
			break;
	}
}


/**
 * Parse IF
 * 
 * if <expr> then <block> <else>
 *
 * <block> can be one command or more in begin end
 * else is optionable: else <block>
 */
void parser_if()
{
	printf("If\n");
	
	/* Expresion */
	if(precedence(global.file, context_if))
	{
		throw_error(error_expresion);
	}
	
	TToken * token = token_get();
	if(token->type != token_then)
	{ /* then? */
		throw_error(error_then);
	}
	token_free(token);

	token = token_get();
	if(token->type == token_begin)
	{ /* Code block */
		token_free(token);
		parser_main();
		token = token_get();
	}
	else
	{ /* Only one command without begin/end */
		token_return_token(token);
		parser_code();
	}
	
	token = token_get();
	if(token->type == token_else)
	{ /* Else? */
		printf("else\n");
		token = token_get();
		if(token->type == token_begin)
		{ /* else block */
			token_free(token);
			parser_main();
			token = token_get();
		}
		else
		{ /* only one command */
			token_return_token(token);
			parser_code();
		}
	}
	else
	{
		token_return_token(token);
	}
	printf("end if %d\n",token->type);
}


/**
 * Parse goto statement
 */
void parser_goto()
{
	TToken *token = token_get();
	if(token->type == token_identifier)
	{ /* Identifier ? */
		token_free(token);
		token = token_get();
		if(token->type != token_semicolon)
		{ /* End with ';' ? */
			throw_error(error_semicolon);
		}
		printf("goto\n");
		token_free(token);
	}
	else
	{
		throw_error(error_identifier);
	}
}


/** 
 * Parse label - goto jump target
 */
void parser_label()
{
	TToken *token = token_get();
	if(token->type == token_identifier)
	{
		token_free(token);
		token = token_get();
		if(token->type != token_semicolon)
		{
			throw_error(error_semicolon);
		}
		printf("label\n");
		token_free(token);
	}
	else
	{
		throw_error(error_identifier);
	}
}


/**
 * while <condition> do <block>
 */
void parser_while()
{
	printf("while\n");
	
	/* Condition */
	if(precedence(global.file, context_while))
	{
		throw_error(error_expresion);
	}

	TToken *token = token_get();

	if(token->type != token_do)
	{
		throw_error(error_do);
	}
	token_free(token);
	
	token = token_get();
	if(token->type == token_begin)
	{ /* Code block */
		token_free(token);
		parser_main();
		token = token_get();
	}
	else
	{ /* Only one command */
		token_return_token(token);
		parser_code();
	}
	
	printf("end while\n");
}


/**
 * repeat <block> until <condition>;
 */
void parser_repeat()
{
	printf("repeat\n");

	TToken *token = token_get();
	if(token->type == token_begin)
	{ /* Code block */
		token_free(token);
		parser_main();
		token = token_get();
	}
	else
	{ /* Only one command */
		token_return_token(token);
		parser_code();
	}

	token = token_get();
	if(token->type != token_until)
	{
		throw_error(error_until);
	}
	
	/* Condition */
	if(precedence(global.file, context_repeat))
	{
		throw_error(error_expresion);
	}
	
	printf("end repeat\n");
}


/**
 * for ID := <expr> to <expr> do <block>;
 */
void parser_for()
{
	printf("for\n");

	TToken *token = token_get();
	if(token->type != token_identifier)
	{
		throw_error(error_identifier);
	}
	token_free(token);

	token = token_get();
	if(token->type != token_assign)
	{
		throw_error(error_assign);
	}
	token_free(token);

	/* Inicial. value */
	if(precedence(global.file, context_for_init))
	{
		throw_error(error_expresion);
	}

	token = token_get();
	if(token->type != token_to)
	{
		throw_error(error_to);
	}
	token_free(token);

	/* Target value */
	if(precedence(global.file, context_for_to))
	{
		throw_error(error_expresion);
	}

	token = token_get();
	if(token->type != token_do)
	{
		throw_error(error_do);
	}
	token_free(token);

	token = token_get();
	if(token->type == token_begin)
	{ /* Code block */
		token_free(token);
		parser_main();
		token = token_get();
	}
	else
	{ /* Only one command */
		token_return_token(token);
		parser_code();
	}

	printf("end for\n");
}

/**
 * Switch statement
 * 
 * case ID of <cases> <else> end;
 *
 * <cases>	... <expr> : <block>
 * <else>	... else <block> 
 */
void parser_switch()
{
	printf("switch\n");

	TToken *token = token_get();
	if(token->type != token_identifier)
	{ /* Variable name for case */
		throw_error(error_identifier);
	}
	token_free(token);

	token = token_get();
	if(token->type != token_of)
	{
		throw_error(error_of);
	}
	token_free(token);

	token = token_get();
	while(token->type != token_end && token->type != token_else)
	{ /* Cases */
		printf("case\n");
		token_return_token(token);
		if(precedence(global.file, context_case))
		{
			throw_error(error_expresion);
		}
		token = token_get();
		if(token->type != token_colon)
		{
			throw_error(error_colon);
		}
		token_free(token);

		token = token_get();
		if(token->type == token_begin)
		{ /* Code block */
			token_free(token);
			parser_main();
			token = token_get();
			token_free(token);
			token = token_get();
		}
		else
		{ /* Only one command without begin/end */
			token_return_token(token);
			parser_code();
		}

		token = token_get();
	}

	if(token->type == token_else)
	{
		printf("else\n");
		token = token_get();
		if(token->type == token_begin)
		{ /* else block */
			token_free(token);
			parser_body();
		}
		else
		{ /* only one command */
			token_return_token(token);
			parser_code();
		}
	}
	token = token_get();
	if(token->type != token_end)
	{
		throw_error(error_end);
	}
}

/**
 * Check, if is token variable type
 *
 * @param	type	enum of type
 * @return	true if is variable type, false if not
 */
static inline int isVariableType(int type)
{
	return (	type == token_real		||
			type == token_integer		||
			type == token_char		||
			type == token_string_var	||
			type == token_boolean		);
}
