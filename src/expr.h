#ifndef EXPR_H
#define EXPR_H

#include "stack.h"
#include "uStack.h"
#include "types.h"
#include "lex.h"
#include "symbol.h"
#include "uStack.h"


typedef enum {
	operator_unary_minus,		// 0
	operator_not,				// 1
	operator_mul,				// 2
	operator_div,				// 3
	operator_sign_div,			// 4
	operator_mod,				// 5
	operator_and,				// 6
	operator_plus,				// 7
	operator_minus,				// 8
	operator_or,				// 9
	operator_xor,				// 10
	operator_equal,				// 11
	operator_diff,				// 12
	operator_less,				// 13
	operator_less_equal,		// 14
	operator_greater,			// 15
	operator_greater_equal,		// 16
	operator_in,				// 17
	operator_left_parenthesis,	// 18
	operator_right_parenthesis,	// 19
	operator_ID,				// 20
	operator_func,				// 21
	operator_array,				// 22
	operator_comma,				// 23
	operator_dolar,				// 24
	operator_non_term			// 25
} operator_number;

typedef enum{
	sign_equal = 26,			// 26
	sign_less,					// 27
	sign_greater,				// 28
	sign_fault					// 29
} precedence_number;

typedef enum{
	context_if,
	context_while,
	context_for_init,
	context_for_to,
	context_repeat,
	context_args,
	context_case,
	context_assign,
	context_index,
	context_global,
	context_function,
	context_write,
	context_readln,
} parse_context;

typedef enum
{
	rule_0,
	rule_1,
	rule_2,
	rule_3,
	rule_4,
	rule_5,
	rule_6,
	rule_7,
	rule_8,
	rule_9,
	rule_10,
	rule_11,
	rule_12,
	rule_13,
	rule_14,
	rule_15,
	rule_16,
	rule_17,
	rule_18,
	rule_19,
	rule_20,
	rule_21,
	rule_22,
	rule_23,
	rule_24
} TRule;


typedef enum 
{
	check_func,
	check_var
}seman;

int sem_check(TToken * , seman );
operator_number recon_sign(TToken * token, parse_context context, uStack_t * stack);
int precedence(FILE *filename,parse_context Func_call, symbolVariable *result);
precedence_number get_stack();
precedence_number enum_sign();
precedence_number get_sign();
int check_rule(uStack_t * stack, TRule rule, TStack *var_stack);

#endif
