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
	TToken * token = token_get(global.file);
	
	/* Program name */
	if(token->type != token_program)
	{
		token_free(token);
		fprintf(stderr,"Error: Expected 'program'.\n");
		return;
	}
	token_free(token);
	
	token = token_get(global.file);
	if(token->type != token_identifier)
	{
		token_free(token);
		fprintf(stderr,"Error: Expected program name.\n");
		return;
	}
	printf("Program name %s\n",token->data->data);
	token_free(token);
	
	token = token_get(global.file);
	if(token->type != token_semicolon)
	{
		token_free(token);
		fprintf(stderr,"Error: Program name should end with ';'.");
		return;
	}
	token_free(token);
	
	/* Global variables */
	parser_vars();
	
	/* Functions */
	parser_function();
	
	token = token_get(global.file);
	if(token->type != token_begin)
	{ /* Main body */
		fprintf(stderr,"Error: Expected 'begin' %d.",token->type);
		token_free(token);
		return;
	}
	token_free(token);
	
	printf("Main body\n");
	parser_main();
	
	token = token_get(global.file);
	if(token->type != token_end)
	{
		fprintf(stderr,"Error: Expected 'end.'");
		token_free(token);
		return;
	}
	token_free(token);
	
	token = token_get(global.file);
	if(token->type != token_dot)
	{
		fprintf(stderr,"Error: Expected '.' after 'end'.");
		token_free(token);
		return;
	}

	printf("Main body end\n");
}


/**
 * var <var>
 **/
void parser_vars()
{
	TToken * token = token_get(global.file);
	
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
	TToken * token = token_get(global.file);
	
	if(token->type == token_identifier)
	{ /* Variable identifier */
		token_return_token(token);
		
		do { /* ID [, ID] */
			token = token_get(global.file);
			if(token->type != token_identifier)
			{
				token_free(token);
				fprintf(stderr,"Error: Expected variable name.\n");
			}
			printf("Variable %s\n",token->data->data);
			token_free(token);

			token = token_get(global.file);
		} while(token->type == token_comma);

		if(token->type != token_colon)
		{
			token_free(token);
			fprintf(stderr,"Error: Expected ':', or ','.\n");
			return;
		}
		token_free(token);
		
		token = token_get(global.file);
		if(isVariableType(token->type) )
		{ /* var type */
			token_free(token);
			token = token_get(global.file);

			if(token->type == token_equal)
			{ /* Inicialize value */
				token_free(token);
				precedence(global.file,false);
				
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
			token_free(token);
			fprintf(stderr,"Error: Bad variable type.\n");
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
	TToken * token = token_get(global.file);
	if(token->type == token_function)
	{ /* Function keywords? */
		token_free(token);
		token = token_get(global.file);
		if(token->type != token_identifier)
		{ /* Identifier */
			token_free(token);
			fprintf(stderr,"Error: Expected function name.\n");
			return;
		}
		printf("function %s:\n",token->data->data);
		token_free(token);
		
		token = token_get(global.file);
		if(token->type != token_parenthesis_left)
		{
			fprintf(stderr,"Error: Expected '(' %d.\n",token->type);
			token_free(token);
			return;
		}
		token_free(token);
		
		/* Function arguments */
		parser_args();
		
		token = token_get(global.file);
		if(token->type != token_parenthesis_right)
		{
			fprintf(stderr,"Error: Expected ')' %d.\n",token->type);
			token_free(token);
			return;
		}
		token_free(token);
		
		token = token_get(global.file);
		if(token->type != token_colon)
		{
			token_free(token);
			fprintf(stderr,"Error: Expected ':'.\n");
			return;
		}
		token_free(token);
		
		token = token_get(global.file);
		if(!isVariableType(token->type))
		{ /* Return type */
			token_free(token);
			fprintf(stderr,"Error: Expected function return type.\n");
			return;
		}
		token_free(token);
		
		token = token_get(global.file);
		if(token->type != token_semicolon)
		{
			token_free(token);
			fprintf(stderr,"Error: Function return type should be ended with ';'.\n");
			return;
		}
		token_free(token);

		/* Function variables */
		parser_vars();
		
		token = token_get(global.file);
		if(token->type != token_begin)
		{ /* Function body */
			token_free(token);
			fprintf(stderr,"Error: Function block should start with 'begin'.\n");
			return;
		}
		token_free(token);
		
		do
		{
			parser_body();
			token = token_get(global.file);
		} while(token->type != token_end);
		
		if(token->type != token_end)
		{ /* Function code block end */
			token_free(token);
			fprintf(stderr,"Error: Function block should ended with 'end;'.\n");
			return;
		}
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
			fprintf(stderr,"Error: Expected ';'.\n");
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
	TToken * token = token_get(global.file);

	if(token->type == token_identifier)
	{ /* ID */
		token_free(token);
		token = token_get(global.file);
		if(token->type == token_colon)
		{
			token_free(token);
			token = token_get(global.file);
			if(isVariableType(token->type))
			{ /* Datetype */
				token_free(token);
				token = token_get(global.file);
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
				token_free(token);
				fprintf(stderr,"Error: Expected function argument type.\n");
			}
		}
		else
		{
			token_free(token);
			fprintf(stderr,"Error: Expected identifier.\n");
		}
	}
	else if(token->type == token_parenthesis_right)
	{ /* Got ')' so function haven't any argument */
		token_return_token(token);
	}
	else
	{
		fprintf(stderr,"Error: Expected identifier %d.\n",token->type);
		token_free(token);
	}
}


/**
 * Parse function body containing code
 */
void parser_body()
{
	printf("Function body\n");
	TToken * token = token_get(global.file);
	while(token->type != token_end)
	{ /* Parse content until we get end; */
		token_return_token(token);
		parser_main();
		
		token = token_get(global.file);
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
	TToken * token = token_get(global.file);
	while(token->type != token_end)
	{
		token_return_token(token);
		parser_code();
		token = token_get(global.file);
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
	
	TToken * token = token_get(global.file);
	
	switch(token->type)
	{
		case token_identifier:
			/* assign or function call */
			token_free(token);
			token = token_get(global.file);
			if(token->type == token_assign)
			{ /* assign */
				printf("assign\n");
				precedence(global.file,false);
			}
			else if(token->type == token_parenthesis_left)
			{ /* function call */
				printf("function call\n");
				token_return_token(token);
				precedence(global.file,true);
			}
			else
			{
				fprintf(stderr,"Error: Unkown variable operation %d.\n",token->type);
				token_free(token);
				return;
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
			precedence(global.file,false);
			break;
			
		case token_semicolon:
			/* empty command */
			break;
			
		default:
			/* Unkown command */
			fprintf(stderr,"Error: Unkown command %d.\n",token->type);
			token_free(token);
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
	precedence(global.file,false);
	
	TToken * token = token_get(global.file);
	if(token->type != token_then)
	{ /* then? */
		token_free(token);
		fprintf(stderr,"Error: Expected 'then'.");
		return;
	}
	token_free(token);

	token = token_get(global.file);
	if(token->type == token_begin)
	{ /* Code block */
		token_free(token);
		parser_main();
		token = token_get(global.file);
	}
	else
	{ /* Only one command without begin/end */
		token_return_token(token);
		parser_code();
	}
	
	token = token_get(global.file);
	if(token->type == token_else)
	{ /* Else? */
		printf("else\n");
		token = token_get(global.file);
		if(token->type == token_begin)
		{ /* else block */
			token_free(token);
			parser_main();
			token = token_get(global.file);
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
	precedence(global.file,false);
	TToken *token = token_get(global.file);
	if(token->type == token_identifier)
	{ /* Identifier ? */
		token_free(token);
		token = token_get(global.file);
		if(token->type != token_semicolon)
		{ /* End with ';' ? */
			fprintf(stderr,"Error: Goto target should end with ';'.");
		}
		printf("goto\n");
		token_free(token);
	}
	else
	{
		fprintf(stderr,"Error: Goto target has bad value.");
		token_free(token);
	}
}


/** 
 * Parse label - goto jump target
 */
void parser_label()
{
	TToken *token = token_get(global.file);
	if(token->type == token_identifier)
	{
		token_free(token);
		token = token_get(global.file);
		if(token->type != token_semicolon)
		{
			fprintf(stderr,"Error: Label name should end with ';'.");
		}
		printf("label\n");
		token_free(token);
	}
	else
	{
		fprintf(stderr,"Error: Label has bad name.");
		token_free(token);
	}
}


/**
 * while <condition> do <block>
 */
void parser_while()
{
	printf("while\n");
	
	/* Condition */
	precedence(global.file,false);

	TToken *token = token_get(global.file);

	if(token->type != token_do)
	{
		fprintf(stderr,"Error: Expected 'do'.");
		token_free(token);
		return;
	}
	token_free(token);
	
	token = token_get(global.file);
	if(token->type == token_begin)
	{ /* Code block */
		token_free(token);
		parser_main();
		token = token_get(global.file);
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

	TToken *token = token_get(global.file);
	if(token->type == token_begin)
	{ /* Code block */
		token_free(token);
		parser_main();
		token = token_get(global.file);
	}
	else
	{ /* Only one command */
		token_return_token(token);
		parser_code();
	}

	token = token_get(global.file);
	if(token->type != token_until)
	{
		token_free(token);
		fprintf(stderr,"Error: Expected 'until'.\n");
		return;
	}
	
	/* Condition */
	precedence(global.file,false);
	
	
	printf("end repeat\n");
}


/**
 * for ID := <expr> to <expr> do <block>;
 */
void parser_for()
{
	printf("for\n");

	TToken *token = token_get(global.file);
	if(token->type != token_identifier)
	{
		token_free(token);
		fprintf(stderr,"Error: Expected identifier.\n");
		return;
	}
	token_free(token);

	token = token_get(global.file);
	if(token->type != token_assign)
	{
		token_free(token);
		fprintf(stderr,"Error: Expected ':='.\n");
		return;
	}
	token_free(token);

	/* Inicial. value */
	precedence(global.file,false);

	token = token_get(global.file);
	if(token->type != token_to)
	{
		token_free(token);
		fprintf(stderr,"Error: Expected 'to'.\n");
		return;
	}
	token_free(token);

	/* Target value */
	precedence(global.file,false);

	token = token_get(global.file);
	if(token->type != token_do)
	{
		token_free(token);
		fprintf(stderr,"Error: Expected 'do'.\n");
		return;
	}
	token_free(token);

	token = token_get(global.file);
	if(token->type == token_begin)
	{ /* Code block */
		token_free(token);
		parser_main();
		token = token_get(global.file);
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

	TToken *token = token_get(global.file);
	if(token->type != token_identifier)
	{ /* Variable name for case */
		fprintf(stderr, "Error: Expected variable.");
		token_free(token);
		return;
	}
	token_free(token);

	token = token_get(global.file);
	if(token->type != token_of)
	{
		fprintf(stderr, "Error: Expected 'of'.");
		token_free(token);
		return;
	}
	token_free(token);

	token = token_get(global.file);
	while(token->type != token_end && token->type != token_else)
	{ /* Cases */
		printf("case %d\n",token->type);
		token_return_token(token);
		precedence(global.file,false);
		token = token_get(global.file);
		if(token->type != token_colon)
		{
			fprintf(stderr, "Error: Expected ':' %d.\n",token->type);
			token_free(token);
			return;
		}
		token_free(token);

		token = token_get(global.file);
		if(token->type == token_begin)
		{ /* Code block */
			token_free(token);
			parser_main();
			token = token_get(global.file);
			token_free(token);
			token = token_get(global.file);
		}
		else
		{ /* Only one command without begin/end */
			token_return_token(token);
			parser_code();
		}

		token = token_get(global.file);
	}

	if(token->type == token_else)
	{
		printf("else\n");
		token = token_get(global.file);
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
	token = token_get(global.file);
	if(token->type != token_end)
	{
		fprintf(stderr, "Error: Expected 'end;'.");
		token_free(token);
		return;
	}
}

/**
 * Check, if is token variable type
 *
 * @param	type	enum of type
 * @return	true if is variable type, false if not
 */
static int isVariableType(int type)
{
	return (	type == token_real	||
			type == token_integer	||
			type == token_char	||
			type == token_string	||
			type == token_boolean	);
}
