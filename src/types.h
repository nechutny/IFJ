#ifndef TYPES_H
#define TYPES_H

#include "string.h"

/* types of tokes */
typedef enum {
	token_invalid,		//0
	token_string,		//1
	token_comma,		//2
	token_var,			//3
	token_assign,		//4
// matematic...
	token_add,			//5
	token_slash,		//6
	token_mul,			//7
	token_sub,			//8
	token_div,
	token_mod,
	
	token_in,
	token_dot,			//9
//comaring
	token_equal,		//10
	token_not_equal,	//11
	token_less,			//12
	token_less_equal,	//13
	token_greater,		//14
	token_greater_equal,//15

	token_int,			//16
	token_double,		//17
	token_eof,			//18
// ( )
	token_parenthesis_left,	//19
	token_parenthesis_right,//20
// ;
	token_semicolon,		//21
//:
	token_colon,			//22

	token_identifier,		//23
// keywords
	token_begin,			//24
	token_end,				//25
	token_while,
	token_for,
	token_if,
	token_then,
	token_else,
	token_procedure,
	token_function,
	token_return,
	token_true,
	token_false,
	token_null,
	token_or,
	token_and,
	token_not,
// integrated functions not working yet (it depands on task from teachers)
	token_f_boolval,
	token_f_doubleval,
	token_f_intval,
	token_f_strval,
	token_f_get_string,
	token_f_put_string,
	token_f_strlen,
	token_f_get_substring,
	token_f_find_string,
	token_f_sort_string
} TToken_type;

typedef struct _TToken {
	TToken_type type;
	TString * data;
} TToken;

#endif