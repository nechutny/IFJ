#ifndef EXPR_H
#define EXPR_H

#include "stack.h"
#include "uStack.h"
#include "types.h"
#include "lex.h"
#include "symbol.h"
#include "uStack.h"

typedef enum {
	operator_not,
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
	operator_func,
	operator_comma,
	operator_dolar,
	operator_non_term,
	operator_array
} operator_number;

typedef enum{
	sign_equal = 23,
	sign_less,
	sign_greater,
	sign_fault
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
	rule_19
} TRule;

int precedence(FILE *filename,parse_context Func_call, symbolVariable *result);
precedence_number get_stack();
precedence_number enum_sign();
precedence_number get_sign();
int check_rule(uStack_t * stack, TRule rule, TStack *var_stack);

#endif
