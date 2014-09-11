#ifndef TYPES_H
#define TYPES_H

#include "string.h"

typedef enum {
	var_bool,
	var_int,
	var_double,
	var_string
}TVarType;

typedef struct _TVar {
	TVarType	type;
	union {
		int i;
		double d;
		TString *s;
	}data;
}TVar;

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
	token_div,			//9
	token_mod,			//10
	
	token_in,			//11
	token_dot,			//12
//comaring
	token_equal,		//13
	token_not_equal,	//14
	token_less,			//15
	token_less_equal,	//16
	token_greater,		//17
	token_greater_equal,//18

	token_int,			//19
	token_double,		//20
	token_eof,			//21
// ( )
	token_parenthesis_left,	//22
	token_parenthesis_right,//23
// ;
	token_semicolon,		//24
//:
	token_colon,			//25

	token_identifier,		//26
// keywords
	token_begin,			//27
	token_end,				//28
	token_while,			//29
	token_for,				//30
	token_if,				//31
	token_then,				//32
	token_else,				//33
	token_procedure,		//34
	token_function,			//35
	token_return,			//36
	token_true,				//37
	token_false,			//38
	token_null,				//39
	token_or,				//40
	token_and,				//41
	token_not,				//42
	token_do,
	token_repeat,
	token_until,
	token_label,
	token_goto,
	token_case,
	token_of,
	token_to,
	token_program,
	token_id,
	token_real,
	token_integer,
	token_boolean,
	token_char,

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
