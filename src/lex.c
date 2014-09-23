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
#include "garbage.h"
#include "main.h"


TToken * token_init() {
	TToken * token;
	
	token = ( TToken * ) _malloc( sizeof( TToken ) );
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

TToken * token_buffer;

void token_return_token(TToken * token) {
	token_buffer = token;
}

void token_free( TToken * token ) {
	string_free( token->data );
	_free( token );
}

//set correct type of token (is it keyword or indentifier?)
void set_identifier(TToken *token){
 	char * keywords[]={"var","in","do","repeat","until","label","goto","case",
 						"of","to","program","id","mod","div","not","and","or",
 						"begin","end","while","for","if","then","else",
 						"porcedure","function","return","true","false","null",
 						"integer","real","char","boolean","string","array",
 						"xor","forward","find","readln","write","sort",
 						"length","copy"};
 	TToken_type tokens[]={token_var,token_in,token_do,token_repeat,token_until,
 						token_label,token_goto,token_case,token_of,token_to,
 						token_program,token_id,token_mod,token_div,token_not,
 						token_and,token_or,token_begin,token_end,token_while,
 						token_for,token_if,token_then,token_else,
 						token_procedure,token_function,token_return,token_true,
 						token_false,token_null,token_integer,token_real,
 						token_char,token_boolean,token_string_var,token_array,
 						token_xor,token_forward,token_f_find,token_readln,
 						token_write,token_f_sort,token_f_length,token_f_copy};

 	token->type = token_identifier;
 	for (int i = 0; i <= 43; i++)
 	{
 		if (strcmp(keywords[i],token->data->data)==0)
 		{
 			token->type = tokens[i];
 		}
 	}
}

//return next token from file
TToken *token_get() {
	TToken *token;
	FILE * file = global.file;
	if(token_buffer != NULL)
	{
		token = token_buffer;
		token_buffer = NULL;
		return token;
	}
	
	if((token = token_init()) == NULL){		//alokace noveho tokenu
		return NULL;
	}
	char buffer[32];
	memset(&buffer,0,32);
	unsigned int buff_i = 0;
	token_last = token;
	TState state = state_init;
	TState number_state = state_int;			//it goes from int or double to _double_sign_e?
	string_clear(token->data);
	char c;
	//char sign='+';
	int ascii=0;
	while (1)
	{
		c = tolower(fgetc(file));
		switch (state)
		{
			case state_init:
				while(isspace(c))
				{
					c = tolower(fgetc(file));
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
						token->type = token_slash;
						return token;
					case '(':
						token->type = token_parenthesis_left;
						return token;
					case '[':
						token->type = token_bracket_left;
						return token;
					case ']':
						token->type = token_bracket_right;
						return token;
					case '{':
						state = state_multiline_comment;
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
							//string_add_chr(token->data, c);
							buffer[buff_i] = c; buff_i++;
						}
						else if (isdigit(c))
						{
							state = state_int;
							number_state = state_int;
							//string_add_chr(token->data, c);
							buffer[buff_i] = c; buff_i++;
							break;
						}
						else if (c == EOF)
						{
							token->type = token_eof;
							return token;
						}
						else
						{
							token->type = token_invalid;
							return token;
						}
				}
				break;
			case state_identifier:
				if(isalnum(c) || c == '_')
				{
					//string_add_chr(token->data, c);
					buffer[buff_i] = c; buff_i++;
				}
				else
				{
					ungetc(c, file);
					string_add(token->data,buffer);
					set_identifier(token);
					return token;
				}
				break;
			case state_int:
				if(isdigit(c))
				{
					//string_add_chr(token->data, c);
					buffer[buff_i] = c; buff_i++;
				}
				else if (c=='.')
				{
					//string_add_chr(token->data, c);
					buffer[buff_i] = c; buff_i++;
					state = state__double_dot;
				}
				else if (c=='+' || c=='-')
				{
					//string_add_chr(token->data, c);
					buffer[buff_i] = c; buff_i++;
					//sign = c;
					state = state__double_sign_e;
				}
				else if (c=='e' || c=='E')
				{
					//string_add_chr(token->data, c);
					buffer[buff_i] = c; buff_i++;
					state = state__double_e;
				}
				else if (c == EOF || c == '+' || c == ',' || c == '*' ||
						c == '/' || c == '-' || c == '=' || c == ')' ||
						c == ';' || c == '>' || c == '<' || c == '[' ||
						c == ']' || c=='{' || isspace(c))	// '(' shouldnt be there)
				{
					ungetc(c, file);
					token->type=token_int;
					string_add(token->data,buffer);
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
					//string_add_chr(token->data, c);
					buffer[buff_i] = c; buff_i++;
					state = state_double;
					number_state = state_double;
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
					//string_add_chr(token->data, c);
					buffer[buff_i] = c; buff_i++;
				}
				else if (c=='+' || c=='-')
				{
					//string_add_chr(token->data, c);
					buffer[buff_i] = c; buff_i++;
					state = state__double_sign_e;
				}
				else if (c=='e' || c=='E')
				{
					//string_add_chr(token->data, c);
					buffer[buff_i] = c; buff_i++;
					state = state__double_e;
				}
				else if (c == EOF || c == '+' || c == ',' || c == '*' || 
						c == '/' || c == '-' || c == '=' || c == ')' || 
						c == ';' || c == '>' || c == '<' || c == '[' ||
						c == ']' || c=='{' || isspace(c))	// '(' shouldnt be there)
				{
					ungetc(c, file);
					token->type=token_double;
					string_add(token->data,buffer);
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
					//string_add_chr(token->data, c);
					buffer[buff_i] = c; buff_i++;
					state = state__double_e;
				}
				else
				{
					fseek(file, -2L, SEEK_CUR);
					if(number_state==state_int)
					{
						token->type=token_int;
						string_add(token->data,buffer);
						return token;
					}
					else
					{
						token->type=token_double;
						string_add(token->data,buffer);
						return token;
					}
				}
				break;
			case state__double_e:
				if (isdigit(c))
				{
					//string_add_chr(token->data, c);
					buffer[buff_i] = c; buff_i++;
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
					//string_add_chr(token->data, c);
					buffer[buff_i] = c; buff_i++;
				}
				else if (c == EOF || c == '+' || c == ',' || c == '*' || 
						c == '/' || c == '-' || c == '=' || c == ')' || 
						c == ';' || c == '>' || c == '<' || c == '[' ||
						c == ']' || c=='{' || isspace(c))	// '(' shouldnt be there)
				{
					ungetc(c, file);
					token->type=token_double;
					string_add(token->data,buffer);
					return token;
				}
				else
				{
					token->type=token_invalid;
					return token;
				}
				break;
			case state_multiline_comment:
				if (c=='}')
				{
					state = state_init;
				}
				break;
			case state_colon:
				if (c=='=')
				{
					token->type = token_assign;
					string_add(token->data,buffer);
					return token;
				}
				else
				{
					ungetc(c, file);
					token->type = token_colon;
					string_add(token->data,buffer);
					return token;
				}
			case state_greater:
				if (c=='=')
				{
					token->type = token_greater_equal;
					string_add(token->data,buffer);
					return token;
				}
				else
				{
					ungetc(c, file);
					token->type = token_greater;
					string_add(token->data,buffer);
					return token;
				}
			case state_less:
				if (c=='=')
				{
					token->type = token_less_equal;
					string_add(token->data,buffer);
					return token;
				}
				else if (c=='>')
				{
					token->type = token_not_equal;
					string_add(token->data,buffer);
					return token;
				}
				else
				{
					ungetc(c, file);
					token->type = token_less;
					string_add(token->data,buffer);
					return token;
				}
			case state_apostrophe:
				if (c=='\'')
				{
					c = tolower(fgetc(file));
					if (c == '\'')
					{
						buffer[buff_i] = c; buff_i++;
						break;
					}
					else if (c == '#')
					{
						ascii = 0;
						c = tolower(fgetc(file));
						if (c == '\'')
						{
							token->type = token_invalid;
							return token;
						}
						while (c == '0')
						{
							c = tolower(fgetc(file));
						}
						if (c == '\'')	//napr #00 
						{
							token->type = token_invalid;
							return token;
						}
						else
						{
							if (!isdigit(c))
							{
								token->type = token_invalid;
								return token;
							}
							else
							{
								ascii=(int)c;	//jednotky
								c = tolower(fgetc(file));
								if (c == '\'')
								{
									buffer[buff_i] = ascii; buff_i++;
									break;
								}
								else if (!isdigit(c))
								{
									token->type = token_invalid;
									return token;
								}
								else
								{
									ascii = (ascii * 10) + (int)c;	//desitky
									c = tolower(fgetc(file));
									if (c == '\'')
									{
										buffer[buff_i] = ascii; buff_i++;
										break;
									}
									else if (!isdigit(c))
									{
										token->type = token_invalid;
										return token;
									}
									else
									{
										ascii = (ascii * 10) + (int)c;		//stovky
										c = tolower(fgetc(file));
										if (c != '\'' || ascii > 255)
										{
											token->type = token_invalid;
											return token;
										}
										else
										{
											buffer[buff_i] = ascii; buff_i++;
											break;
										}
									}
								}
							}
						}
					}
					else
					{
						ungetc(c, file);
						token->type = token_string;
						return token;
					}
				}
				//string_add_chr(token->data, c);
				buffer[buff_i] = c; buff_i++;
				break;
			default:
				printf("Chuck Norris\n");
				break; //you shouldnt get there
		}
		
		if(buff_i == 31)
		{
			string_add(token->data,buffer);
			buff_i = 0;
			memset(&buffer, 0, 32);
		}
		
	}
}
