#include "expr.h"
#include <stdlib.h>
#include <ctype.h>

#define table_size 20

/**
* Precedence table with priorities.
* Rows are tokens on the top of the stack and collums are actual tokens.
* Sign '<' means shift to the stack. Sign '>' means reduction.
**/
const int precedence_table[table_size][table_size]=
{
//    not    *     /    div   mod   and    +     -    or     =    <>     <    <=     >    >=    in     (     )    ID     $
	{ '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // not
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // *
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // /
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // div
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // mod
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // and	
	{ '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // +
	{ '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // -
	{ '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // or
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // =
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // <>
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // <
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // <=
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // >
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // >=
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' }, // in
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '<' , '#' }, // (
	{ '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '>' , '#' , '>' }, // )
	{ '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '>' , '#' , '>' }, // ID
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '#' , '<' , '#' }, // $
};

operator_number recon_sign(TToken * token)
{
	switch(token->type){
		case token_not:
			return operator_not;

		case token_mul:
			return operator_mul;

		case token_slash:
			return operator_div;

		case token_div:
			return operator_sign_div;

		case token_mod:
			return operator_mod;

		case token_and:
			return operator_and;

		case token_add:
			return operator_plus;

		case token_sub:
			return operator_minus;

		case token_or:
			return operator_or;

		case token_equal:
			return operator_equal;

		case token_not_equal:
			return operator_diff;

		case token_less:
			return operator_less;

		case token_less_equal:
			return operator_less_equal;

		case token_greater:
			return operator_greater;

		case token_greater_equal:
			return operator_greater_equal;

		case token_parenthesis_left:
			return operator_left_parenthesis;

		case token_parenthesis_right:
			return operator_right_parenthesis;

		case token_int:
		case token_double:
		case token_identifier:
			return operator_ID;

		default:
			return operator_dolar;
	}
}


/**
*Function precedence making syntax analysition of expressions.
* @param filename is file to translate
* @return number of failiure or correct
**/
int precedence(FILE *filename)
{
	TStack *stack;
	stack = stack_init();
	operator_number end;
	end = operator_dolar;
	stack_push( stack , (void *)end);
	//printf("Ahoooj\n");
	//printf("%d\n",(operator_number)stack_top(stack) );
	TToken * token;
	token = token_init();
	token = token_get(filename);
	token_free(token);
	
	return 1;

}

