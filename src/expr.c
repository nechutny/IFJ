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


/**
*Fucntion to recognize sign from token and revert it to operator_number made in expr.h
* @param token token
* @return operator_number
**/
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

		case token_semicolon:
			return operator_dolar;

		default:
			return sign_fault;
	}
}


/**
* Function that revert sing in precedence table to operator_number
* @param sing sign from precedence table
* @return operator_number
**/
precedence_number enum_sign(int sign)
{
	switch(sign){
		case '=':
			return sign_equal;
		
		case '<':
			return sign_less;

		case '>':
			return sign_greater;

		case '#': 
			return sign_fault;

		default:
			return sign_fault;
			//default fault, but it's from table where isn't anything else
	}
}


/**
* Function that get sign from table ... revert to operator_number
* @param token token from file
* @param stack stack with operation_number
* @return operator_number
**/
precedence_number get_sign(TToken * token, TStack * stack)
{
	int pom = 0;
	if ((int)stack_top(stack) != operator_non_term)
	{
		pom = precedence_table[(int)stack_top(stack)][recon_sign(token)];
	}
	else
	{
		stack_pop(stack);
		pom = precedence_table[(int)stack_top(stack)][recon_sign(token)];
		stack_push(stack,(void *)operator_non_term);
	}
	return enum_sign(pom);
}


int check_rule(TStack * stack, char * rule)
{
	stack_pop(stack);
	//printf("stack_top--: %d\n", (int)stack_top(stack));
	if ((int)stack_top(stack) == operator_non_term)
    {
		stack_pop(stack);
		//printf("stack_top---: %d\n", (int)stack_top(stack));
	    if ((int)stack_top(stack) == sign_less)
	    {
	        stack_pop(stack);
	      	stack_push(stack,(void *)operator_non_term);
	       	printf("Precedence syntax used rule %s\n",rule);
	       	return 0;
	    }
	    else
	    {
	    printf("ERROR: Excpects: < but it gets: %d \n",(int)stack_top(stack));
	    return 1;
	    }

	}
	else
	{
	printf("ERROR: Excpects: E but it gets: %d \n",(int)stack_top(stack));
	return 1;
	}
}


/**
* Function precedence making syntax analysition of expressions.
* @param filename is file to translate
* @return number of failiure or correct
**/
int precedence(FILE *filename)
{
	TToken * token;
	token = token_init();
	token = token_get(filename);

	TStack *stack;
	stack = stack_init();
	stack_push(stack , (void *)operator_dolar);

	//printf("stack_top: %d\n", (int)stack_top(stack));
	//printf("dolar: %d\n",operator_dolar );

	do
	{
		//printf("stack_top: %d\n", (int)stack_top(stack));
		//printf("token_type: %d\n", token->type);
		//printf("stack_count: %d\n", stack_count(stack));
		switch(get_sign(token,stack)){
			case sign_equal:
				stack_push(stack,(void *)recon_sign(token));
				token_free(token);
				token = token_get(filename);
				break;

			case sign_less:
				if ((int)stack_top(stack) != operator_non_term)
				{
					stack_push(stack,(void *)sign_less);
					stack_push(stack,(void *)recon_sign(token));
				}
				else{
					stack_pop(stack);
					stack_push(stack,(void *)sign_less);
					stack_push(stack,(void *)operator_non_term);
					stack_push(stack,(void *)recon_sign(token));
				}

				token_free(token);
				token = token_get(filename);
				break;	

			case sign_greater:
				//This condition handle rule E -> ID
				if ((int)stack_top(stack) == operator_ID){
					stack_pop(stack);
					if((int)stack_top(stack) == sign_less)
					{
						stack_pop(stack);
						stack_push(stack,(void *)operator_non_term);
						printf("Precedence syntax used rule 1: E -> ID\n");
					}
					else{
						printf("ERROR: Excpects: < but it gets: %d \n",(int)stack_top(stack));
					}
				}
				//This condition handle rule E -> (E)
				else if((int)stack_top(stack) == operator_right_parenthesis)
				{
					stack_pop(stack);
					if ((int)stack_top(stack) == operator_non_term)
					{
						stack_pop(stack);
						if ((int)stack_top(stack) == operator_left_parenthesis)
						{
							stack_pop(stack);
							//stack_push(stack,(void *)operator_non_term);
							if((int)stack_top(stack) == sign_less)
							{
								stack_pop(stack);
								stack_push(stack,(void *)operator_non_term);
								printf("Precedence syntax used rule 2: E -> (E)\n");
							}
							else
							{
								printf("ERROR: Excpects: < but it gets: %d \n",(int)stack_top(stack));
								return 1;
							}
						}
						else
						{
							printf("ERROR: Excpects: ( but it gets: %d \n",(int)stack_top(stack));
							return 1;
						}
					}
					else
					{
						printf("ERROR: Excpects: E but it gets: %d \n",(int)stack_top(stack));
						return 1;
					}
				}
				//This condition should handle with others rules, ended with E
				else if ((int)stack_top(stack) == operator_non_term)
				{
					//printf("stack_top-: %d\n", (int)stack_top(stack));
					stack_pop(stack);
					switch((int)stack_top(stack)){
						case operator_not:
							if ((check_rule(stack,"3: E -> E not E")) == 1 )
							{
								return 1;
							}
							break;

			  			case operator_mul:
			  				if ((check_rule(stack,"4: E -> E * E")) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_div:
	          				if ((check_rule(stack,"5: E -> E / E")) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_sign_div:
	          				if ((check_rule(stack,"6: E -> E div E")) == 1 )
							{
								return 1;
							}
							break;

	         			case operator_mod:
	         				if ((check_rule(stack,"7: E -> E mod E")) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_and:
	          				if ((check_rule(stack,"8: E -> E and E")) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_plus:
	          				if ((check_rule(stack,"9: E -> E + E")) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_minus:
	          				if ((check_rule(stack,"10: E -> E - E")) == 1 )
							{
								return 1;
							}
							break;

	             		case operator_or:
	             			if ((check_rule(stack,"11: E -> E or E")) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_equal:
	          				if ((check_rule(stack,"12: E -> E = E")) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_diff:
	          				if ((check_rule(stack,"13: E -> E <> E")) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_less:
	          				if ((check_rule(stack,"14: E -> E < E")) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_less_equal:
	          				if ((check_rule(stack,"15: E -> E <= E")) == 1 )
							{
								return 1;
							}
							break;

	                    case operator_greater:
	                    	if ((check_rule(stack,"16: E -> E > E")) == 1 )
							{
								return 1;
							}
							break;

	                    case operator_greater_equal:
	                    	if ((check_rule(stack,"17: E -> E >= E")) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_in:
	          				if ((check_rule(stack,"18: E -> E in E")) == 1 )
							{
								return 1;
							}
							break;

	          			default:
	          				printf("ERROR:: Excpects: operator but it gets: %d \n",(int)stack_top(stack));
					}
				}
				else
				{
					printf("ERROR Wrong syntax\n");
					return 0;
				}
				break;

			case sign_fault:
				printf("ERROR: Got error sign from precedence table\n");
				return 0;
		}
		if ((token->type == token_semicolon) && (stack_count(stack) == 2) && ((int)stack_top(stack) == operator_non_term))
			stack_pop(stack);

	}while( !((stack_count(stack) == 1) && ((operator_number)stack_top(stack) == operator_dolar )) );

	//printf("hmmm%d\n",get_sign(token,stack) );
	//printf("aaaaa%d\n",(int)stack_top(stack) );
	//printf("token%d\n",token->type );
	token_free(token);
	
	return 1;

}

