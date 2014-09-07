#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "lex.h"
#include "string.h"
#include "main.h"


TToken * token_init() {
	TToken * token;
	
	token = ( TToken * ) malloc( sizeof( TToken ) );
	if( token == NULL ) {
		fprintf( stderr, "%s", strerror( errno ) );
		return NULL;
	}
	
	token->data = string_new();
	if( !token->data )
		return NULL;
		
	token->type = token_invalid;
	return token;
}

void token_free( TToken * token ) {
	string_free( token->data );
	free( token );
}

//set correct type of token (is it keyword or indentifier?)
void set_identifier(TToken *token){
 	if (strcmp("var",token->data->data)==0)
 		token->type = token_var;
 	else if (strcmp("in",token->data->data)==0)
 		token->type = token_in;
 	else if (strcmp("mod",token->data->data)==0)
 		token->type = token_mod;
 	else if (strcmp("div",token->data->data)==0)
 		token->type = token_div;
 	else if (strcmp("not",token->data->data)==0)
 		token->type = token_not;
 	else if (strcmp("and",token->data->data)==0)
 		token->type = token_and;
 	else if (strcmp("or",token->data->data)==0)
 		token->type = token_or;
 	else if (strcmp("begin",token->data->data)==0)
 		token->type = token_begin;
 	else if (strcmp("end",token->data->data)==0)
 		token->type = token_end;
 	else if (strcmp("while",token->data->data)==0)
 		token->type = token_while;
 	else if (strcmp("for",token->data->data)==0)
 		token->type = token_for;
 	else if (strcmp("end",token->data->data)==0)
 		token->type = token_end;
 	else if (strcmp("if",token->data->data)==0)
 		token->type = token_if;
 	else if (strcmp("then",token->data->data)==0)
 		token->type = token_then;
 	else if (strcmp("else",token->data->data)==0)
 		token->type = token_else;
 	else if (strcmp("procedure",token->data->data)==0)
 		token->type = token_procedure;
 	else if (strcmp("function",token->data->data)==0)
 		token->type = token_function;
 	else if (strcmp("return",token->data->data)==0)
 		token->type = token_return;
 	else if (strcmp("true",token->data->data)==0)
 		token->type = token_true;
 	else if (strcmp("false",token->data->data)==0)
 		token->type = token_false;
 	else if (strcmp("null",token->data->data)==0)
 		token->type = token_null;
 	else
 		token->type = token_identifier;
}

//return next token from file
TToken *token_get(FILE * file) {
	TToken *token;
	if((token = token_init()) == NULL){		//alokace noveho tokenu
		return NULL;
	}
	TState state = state_init;
	string_clear(token->data);
	char c;
	/*token->type = token_block_begin;
	string_add_chr(token->data, 'a');
	string_add_chr(token->data, 'c');
	return token;*/
	while (1)
	{
		c = fgetc(file);
		switch (state)
		{
			case state_init:
				while(isspace(c))
				{
					c = fgetc(file);
				}
				switch (c)
				{
					case '.':
						token->type = token_dot;
						return token;
					case ',':
						token->type = token_comma;
						return token;
					case '+':
						token->type = token_add;
						return token;
					case '-':
						token->type = token_sub;
						return token;
					case '*':
						token->type = token_mul;
						return token;
					case '=':
						token->type = token_equal;
						return token;
					case ';':
						token->type = token_semicolon;
						return token;
					case ')':
						token->type = token_parenthesis_right;
						return token;
					case '/':
						state = state_slash;
						break;
					case '(':
						state = state_parenthesis_left;
						break;
					case ':':
						state = state_colon;
						break;
					case '>':
						state = state_greater;
						break;
					case '<':
						state = state_less;
						break;
					case '\'':
						state = state_apostrophe;
						break;
					default:
						if (isalpha(c) || c == '_')
						{	//identifikator nebo klicove slovo;
							state = state_identifier;
							string_add_chr(token->data, c);
						}
						else if (isdigit(c))
						{
							state = state_int;
							string_add_chr(token->data, c);
							break;
						}
						else
						{
							token->type = token_eof;
							return token;
						}
				}
				break;
			case state_identifier:
				if(isalnum(c) || c == '_')
				{
					string_add_chr(token->data, c);
				}
				else
				{
					ungetc(c, file);
					set_identifier(token);
					return token;
				}
				break;
			case state_int:
				if(isdigit(c))
				{
					string_add_chr(token->data, c);
				}
				else if (c=='.')
				{
					string_add_chr(token->data, c);
					state = state__double_dot;
				}
				else if (c=='+' || c=='-')
				{
					string_add_chr(token->data, c);
					state = state__double_sign_e;
				}
				else if (c=='e' || c=='E')
				{
					string_add_chr(token->data, c);
					state = state__double_e;
				}
				else if (c == EOF || c == '+' || c == ',' || c == '*' || 
						c == '/' || c == '-' || c == '=' || c == ')' || 
						c == ';' || c == '>' || c == '<' ||
					 	isspace(c))	// '(' shouldnt be there)
				{
					ungetc(c, file);
					token->type=token_int;
					return token;
				}
				else
				{
					token->type=token_invalid;
					return token;
				}
				break;
			case state__double_dot:
				if (isdigit(c))
				{
					string_add_chr(token->data, c);
					state = state_double;
				}
				else
				{
					token->type=token_invalid;
					return token;
				}
				break;
			case state_double:
				if(isdigit(c))
				{
					string_add_chr(token->data, c);
				}
				else if (c=='+' || c=='-')
				{
					string_add_chr(token->data, c);
					state = state__double_sign_e;
				}
				else if (c=='e' || c=='E')
				{
					string_add_chr(token->data, c);
					state = state__double_e;
				}
				else if (c == EOF || c == '+' || c == ',' || c == '*' || 
						c == '/' || c == '-' || c == '=' || c == ')' || 
						c == ';' || c == '>' || c == '<' ||
					 	isspace(c))	// '(' shouldnt be there)
				{
					ungetc(c, file);
					token->type=token_double;
					return token;
				}
				else
				{
					token->type=token_invalid;
					return token;
				}
				break;
			case state__double_sign_e:
				if (c=='e' || c=='E')
				{
					string_add_chr(token->data, c);
					state = state__double_e;
				}
				else
				{
					token->type=token_invalid;
					return token;
				}
				break;
			case state__double_e:
				if (isdigit(c))
				{
					string_add_chr(token->data, c);
					state = state_double_e;
				}
				else
				{
					token->type=token_invalid;
					return token;
				}
				break;
			case state_double_e:
				if (isdigit(c))
				{
					string_add_chr(token->data, c);
				}
				else if (c == EOF || c == '+' || c == ',' || c == '*' || 
						c == '/' || c == '-' || c == '=' || c == ')' || 
						c == ';' || c == '>' || c == '<' ||
					 	isspace(c))	// '(' shouldnt be there)
				{
					ungetc(c, file);
					token->type=token_double;
					return token;
				}
				else
				{
					token->type=token_invalid;
					return token;
				}
				break;
			case state_slash:
				if (c=='/')
				{
					state = state_line_comment;
				}
				else if (isalnum(c) || isspace(c) || c=='_')
				{
					ungetc(c, file);
					token->type=token_slash;
					return token;
				}
				else
				{
					token->type=token_invalid;
					return token;
				}
				break;
			case  state_line_comment:
				if (c=='\n')
				{
					state = state_init;
				}
				break;
			case state_parenthesis_left:
				if (c=='*')
				{
					state = state_multiline_comment;
				}
				else
				{
					token->type = token_parenthesis_left;
					return token;
				}
			case state_multiline_comment:
				if (c=='*')
				{
					state = state_star;
				}
				break;
			case state_star:
				if (c==')')
				{
					state = state_init;
				}
				else if (c!='*')
				{
					state = state_multiline_comment;
				}
				break;
			case state_colon:
				if (c=='=')
				{
					token->type = token_assign;
					return token;
				}
				else
				{
					token->type = token_colon;
					return token;
				}
			case state_greater:
				if (c=='=')
				{
					token->type = token_greater_equal;;
					return token;
				}
				else
				{
					ungetc(c, file);
					token->type = token_greater;
					return token;
				}
			case state_less:
				if (c=='=')
				{
					token->type = token_less_equal;
					return token;
				}
				else if (c=='>')
				{
					token->type = token_not_equal;
					return token;
				}
				else
				{
					ungetc(c, file);
					token->type = token_less;
					return token;
				}
			case state_apostrophe:
				if (c=='\'')
				{
					token->type = token_string;
					return token;
				}
				string_add_chr(token->data, c);
				break;
			default:
				printf("chuck\n");
				break; //you shouldnt get there
		}
	}
}
