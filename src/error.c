/**
 * 	@project	IFJ 2014/2015
 *	@file		error.c
 *
 *	@author		Stanislav Nechutny - xnechu01
 */
#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "garbage.h"
#include "lex.h"

void throw_error(errors code)
{
	int return_code = 1;
	
	fprintf(stderr,"Error: ");
	switch(code)
	{
		case error_program:
			fprintf(stderr, "Expected 'program'");
			break;
			
		case error_identifier:
			fprintf(stderr, "Expected identifier");
			break;
			
		case error_semicolon:
			fprintf(stderr, "Expected ';'");
			break;

		case error_begin:
			fprintf(stderr,"Expected 'begin'");
			break;

		case error_end:
			fprintf(stderr,"Expected 'end'");
			break;

		case error_dot:
			fprintf(stderr,"Expected '.'");
			break;

		case error_colon:
			fprintf(stderr,"Expected ':'");
			break;

		case error_expresion:
			fprintf(stderr,"In expresion");
			break;

		case error_type:
			fprintf(stderr,"Wrong datatype");
			break;

		case error_parenthesis_left:
			fprintf(stderr,"Expected '('");
			break;

		case error_parenthesis_right:
			fprintf(stderr,"Expected ')'");
			break;

		case error_then:
			fprintf(stderr,"Expected 'then'");
			break;

		case error_do:
			fprintf(stderr,"Expected 'do'");
			break;

		case error_until:
			fprintf(stderr,"Expected 'until'");
			break;

		case error_assign:
			fprintf(stderr,"Expected ':='");
			break;

		case error_to:
			fprintf(stderr,"Expected 'to'");
			break;

		case error_of:
			fprintf(stderr,"Expected 'of'");
			break;

		case error_eof:
			fprintf(stderr,"Content after 'end.'");
			break;

		case error_bracket_right:
			fprintf(stderr,"Expected ']'");
			break;

		case error_function_not_exists:
			fprintf(stderr,"Called function not exists");
			break;

		case error_function_is_var:
			fprintf(stderr,"Called function name is variable");
			break;

		case error_var_not_exists:
			fprintf(stderr,"Used variable not exists");
			break;

		default:
			fprintf(stderr, "Unkown error");
			break;
	}
	
	fprintf(stderr," on line %d. Readed '%s'.\n", file_line(), token_last->data->data);
	
	exit(return_code);
}

int file_line()
{
	long pos = ftell(global.file);
	
	long line = 1;
	long readed = 0;
	int character;

	rewind(global.file);
	while(readed < pos)
	{
		character = fgetc(global.file);
		if(character == '\n')
		{
			line++;
		}
		readed++;
		
	}
	
	return line;
}
