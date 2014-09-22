/**
 *	@project	IFJ 2014/2015
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
#include "uStack.h"
#include "generator.h"
#include "debug.h"

parse_context local_context;

static int isVariableType(int type);


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
	print_debug("Program name %s", token->data->data);
	token_free(token);
	
	token = token_get();
	if(token->type != token_semicolon)
	{
		throw_error(error_semicolon);
	}
	token_free(token);
	
	/* Global variables */
	local_context = context_global;
	parser_vars();
	
	/* Functions */
	local_context = context_function;
	parser_function();
	
	token = token_get();
	if(token->type != token_begin)
	{ /* Main body */
		throw_error(error_begin);
	}
	token_free(token);
	
	print_debug("Main body");
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

	print_debug("Main body end");

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
	htab_listitem* var;
	uStack_init(varStack);
	
	
	
	if(token->type == token_identifier)
	{ /* Variable identifier */
		token_return_token(token);
		
		do { /* ID [, ID] */
			token = token_get();
			if(token->type != token_identifier)
			{
				throw_error(error_identifier);
			}
			print_debug("Variable %s",token->data->data);

			if(local_context == context_global)
			{ /* Add variable to global symbol table */
				var = htab_create(global.global_symbol, token->data->data);
				symbol_variable_init(var, token->data->data);
				uStack_push(htab_listitem*, varStack, var);
			}
			else
			{ /* Add variable to local symbol table in stack */
				var = htab_create(uStack_top(htab_t*, global.local_symbols), token->data->data);
				symbol_variable_init(var, token->data->data);
				uStack_push(htab_listitem*, varStack, var);
			}
			
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
			
			/* Set variable type to global symbol table */
			while(uStack_count(varStack) > 0)
			{
				var = uStack_pop(htab_listitem*, varStack);
				symbol_variable_type_set(var->ptr.variable, token->type);
			}
			token_free(token);
			token = token_get();

			if(token->type == token_equal)
			{ /* Inicialize value */
				token_free(token);
				if(precedence(global.file, context_assign, NULL))
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
	htab_listitem* var;
	unsigned long offset = ftell(global.file);
	
	if(token->type == token_function)
	{ /* Function keywords? */
		token_free(token);
		token = token_get();
		if(token->type != token_identifier)
		{ /* Identifier */
			throw_error(error_identifier);
		}
		print_debug("function %s:",token->data->data);
		/* Add variable to global symbol table */

		/* check if is only prototpy or nothing */
		var = htab_lookup(global.global_symbol, token->data->data);
		if(var != NULL && var->type == type_function && var->ptr.function->defined)
		{
			throw_error(error_function_already_defined);
		}
		else
		{
			var = htab_create(global.global_symbol, token->data->data);
		}
		
		symbol_function_init(var, token->data->data, offset);
		token_free(token);
		
		token = token_get();
		if(token->type != token_parenthesis_left)
		{
			throw_error(error_parenthesis_left);
		}
		token_free(token);

		htab_t* table = htab_init(HASH_TABLE_SIZE);
		uStack_push(htab_t*, global.local_symbols, table);
		
		/* Function arguments */
		parser_args(var->ptr.function);
		
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
		symbol_function_type_set(var->ptr.function, token->type);
		token_free(token);
		
		token = token_get();
		if(token->type != token_semicolon)
		{
			throw_error(error_semicolon);
		}
		token_free(token);

		token = token_get();
		if(token->type != token_forward)
		{
			token_return_token(token);
			var->ptr.function->defined = 1;
			/* Function variables */
			parser_vars();

			printf("\n\n Local symbols:\n");
			htab_foreach(uStack_top(htab_t*,global.local_symbols), printData);

			token = token_get();
			if(token->type != token_begin)
			{ /* Function body */
				throw_error(error_begin);
			}
			token_free(token);
			
			do
			{
				parser_main();
				token = token_get();
			} while(token->type != token_end);

			
			
			uStack_remove(global.local_symbols);
			
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
			uStack_remove(global.local_symbols);
			token = token_get();
			if(token->type != token_semicolon)
			{
				throw_error(error_semicolon);
			}
			parser_function();
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
void parser_args(symbolFunction* func)
{
	htab_listitem* var;
	TToken * token2;
	TToken * token = token_get();

	if(token->type == token_identifier)
	{ /* ID */
		//token_free(token);
		token2 = token_get();
		if(token2->type == token_colon)
		{
			token_free(token2);
			token2 = token_get();
			if(isVariableType(token2->type))
			{ /* Datetype */
				symbol_function_arg_add(func, token->data->data, token2->type);
				
				var = htab_create(uStack_top(htab_t*, global.local_symbols), token->data->data);
				symbol_variable_init(var, token->data->data);
				symbol_variable_type_set(var->ptr.variable, token2->type);
				var->ptr.variable->inicialized = 1;
				
				token_free(token);
				token_free(token2);
				token = token_get();
				if(token->type == token_comma)
				{ /* Comma, so check for more arguments? */
					token_free(token);
					parser_args(func);
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
		func->args_count = 0;
		func->args = NULL;
		token_return_token(token);
	}
	else
	{
		throw_error(error_identifier);
	}
}


/**
 * Code block containing more code ended with 'end'
 */
void parser_main()
{
	print_debug("Code block");
	TToken * token = token_get();
	while(token->type != token_end)
	{
		token_return_token(token);
		parser_code();
		token = token_get();
	}
	print_debug("Code block end");
	token_return_token(token);
}


/**
 * Parse one command
 */
void parser_code()
{
	htab_listitem* hitem;
	print_debug("One command: ");
	
	TToken * token = token_get();
	
	switch(token->type)
	{
		case token_identifier:
			/* assign or function call */
			hitem = VariableExists(token->data->data);
			token = token_get();
			if(token->type == token_assign || token->type == token_bracket_left)
			{ /* assign */
				if(hitem == NULL || hitem->type != type_variable)
				{ /* Not global variable */
					throw_error(error_var_not_exists);
				}
				
				if(token->type == token_bracket_left)
				{ /* Array index? */
					token_return_token(token);
					
					if(precedence(global.file, context_index, NULL))
					{
						throw_error(error_expresion);
					}

					token = token_get();
					if(token->type != token_assign)
					{
						throw_error(error_assign);
					}
					token_free(token);
				}
				print_debug("assign");

				if(hitem != NULL)
				{
					if(precedence(global.file, context_assign, hitem->ptr.variable))
					{
						throw_error(error_expresion);
					}
				}
				else
				{
					throw_error(error_type);
				}
			}
			else if(token->type == token_parenthesis_left)
			{ /* function call */
				print_debug("function call");
				if(hitem == NULL)
				{
					throw_error(error_function_not_exists);
				}
				else if(hitem->type != type_function)
				{
					throw_error(error_function_is_var);
				}
				token_return_token(token);
				if(precedence(global.file, context_args, NULL))
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
			print_debug("return");
			token_free(token);
			if(precedence(global.file, context_assign, NULL))
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
	print_debug("If");
	
	symbolVariable *cond = _malloc(sizeof(symbolVariable));
	TIns *lab_else = _malloc(sizeof(TIns)), 
		 *lab_end = _malloc(sizeof(TIns));

	TNode	*n_else = _malloc(sizeof(TNode)),
			*n_end = _malloc(sizeof(TNode));

	lab_else->type = ins_lab;
	lab_else->adr1 = NULL;
	lab_else->adr2 = NULL;	
	lab_else->adr3 = NULL;

	lab_end->type = ins_lab;
	lab_end->adr1 = NULL;
	lab_end->adr2 = NULL;	
	lab_end->adr3 = NULL;
	
	n_else->data = lab_else;
	n_end->data = lab_end;

	/* Expresion */
	if(precedence(global.file, context_if, cond))
	{
		throw_error(error_expresion);
	}
	
	TToken * token = token_get();
	if(token->type != token_then)
	{ /* then? */
		throw_error(error_then);
	}
	token_free(token);

	gen_code(ins_jmp, cond, NULL, n_else);

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

	/* inser jump for skipping else block */
	gen_code(ins_jmp, NULL, NULL, n_end);
	/* Insert label for skipping then block */
	list_insert_node(global.ins_list, n_else);


	token = token_get();
	if(token->type == token_else)
	{ /* Else? */
		print_debug("else");
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
	list_insert_node(global.ins_list, n_end);
	print_debug("end if");
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
		print_debug("goto");
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
		print_debug("label");
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
	print_debug("while");
	
	symbolVariable *cond = _malloc(sizeof(symbolVariable));	

	TIns *start = _malloc(sizeof(TIns)),
		 *end = _malloc(sizeof(TIns));

	TNode	*n_start = _malloc(sizeof(TNode)),
			*n_end = _malloc(sizeof(TNode));

	start->type = ins_lab;
	start->adr1 = NULL;	
	start->adr2 = NULL;
	start->adr3 = NULL;

	end->type = ins_lab;
	end->adr1 = NULL;
	end->adr2 = NULL;
	end->adr3 = NULL;

	n_start->data = start;
	n_end->data = end;

	/* Condition */
	list_insert_node(global.ins_list, n_start);

	if(precedence(global.file, context_while, cond))
	{
		throw_error(error_expresion);
	}

	TToken *token = token_get();

	if(token->type != token_do)
	{
		throw_error(error_do);
	}
	token_free(token);

	gen_code(ins_jmp, cond, NULL, n_end);

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
	gen_code(ins_jmp, NULL, NULL, n_start);
	list_insert_node(global.ins_list, n_end);
	print_debug("end while");
}


/**
 * repeat <block> until <condition>;
 */
void parser_repeat()
{
	print_debug("repeat");

	symbolVariable *cond = _malloc(sizeof(symbolVariable));	

	TIns *start = _malloc(sizeof(TIns));

	TNode	*n_start = _malloc(sizeof(TNode));

	start->type = ins_lab;
	start->adr1 = NULL;	
	start->adr2 = NULL;
	start->adr3 = NULL;

	n_start->data = start;

	list_insert_node(global.ins_list, n_start);

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
	if(precedence(global.file, context_repeat, cond))
	{
		throw_error(error_expresion);
	}
	gen_code(ins_jmp, cond, NULL, n_start);
	print_debug("end repeat");
}


/**
 * for ID := <expr> to <expr> do <block>;
 */
void parser_for()
{
	htab_listitem* hitem;
	print_debug("for");

	TToken *token = token_get();
	if(token->type != token_identifier)
	{
		throw_error(error_identifier);
	}
	
	hitem = VariableExists(token->data->data);
	if(hitem == NULL || hitem->type != type_variable)
	{ /* Not variable */
		throw_error(error_var_not_exists);
	}
	
	token_free(token);

	token = token_get();
	if(token->type != token_assign)
	{
		throw_error(error_assign);
	}
	token_free(token);

	/* Inicial. value */
	if(precedence(global.file, context_for_init, NULL))
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
	if(precedence(global.file, context_for_to, NULL))
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

	print_debug("end for");
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
	print_debug("switch");

	TToken *token = token_get();
	if(token->type != token_identifier)
	{ /* Variable name for case */
		throw_error(error_identifier);
	}
	htab_listitem* hitem = VariableExists(token->data->data);
	if(hitem == NULL || hitem->type != type_variable)
	{
		throw_error(error_var_not_exists);
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
		print_debug("case");
		token_return_token(token);
		if(precedence(global.file, context_case, NULL))
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
		print_debug("else");
		token = token_get();
		if(token->type == token_begin)
		{ /* else block */
			token_free(token);
			parser_main();
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
