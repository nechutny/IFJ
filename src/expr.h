#include "stack.h"
#include "types.h"
#include "lex.h"

typedef enum {operator_not,
			  operator_mul,
	          operator_div,
	          operator_sign_div,
	          operator_mod,
	          operator_and,
	          operator_plus,
	          operator_minus,
	          operator_or,
	          operator_equal,
	          operator_diff,
	          operator_less,
	          operator_less_equal,
	          operator_greater,
	          operator_greater_equal,
	          operator_in,
	          operator_left_parenthesis,
	          operator_right_parenthesis,
	          operator_ID,
	          operator_dolar,
	          operator_non_term
}operator_number;

typedef enum{
			sign_equal = 21,
	        sign_less,
	        sign_greater,
	        sign_fault
}precedence_number;

int precedence();
precedence_number get_stack();
precedence_number enum_sign();
