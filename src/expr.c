#include "expr.h"
#include "garbage.h"
#include "generator.h"
#include <stdlib.h>
#include <ctype.h>

#define table_size 21

/**
* Precedence table with priorities.
* Rows are tokens on the top of the stack and collums are actual tokens.
* Sign '<' means shift to the stack. Sign '>' means reduction.
**/
const int precedence_table[table_size][table_size]=
{
//    not    *     /    div   mod   and    +     -    or     =    <>     <    <=     >    >=    in     (     )    ID     ,    $
	{ '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // not
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // *
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // /
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // div
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // mod
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // and	
	{ '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // +
	{ '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // -
	{ '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // or
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // =
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // <>
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // <
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // <=
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // >
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // >=
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '>' , '>' }, // in
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '<' , '=' , '#' }, // (
	{ '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '>' , '#' , '>' , '>' }, // )
	{ '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '>' , '#' , '>' , '>' }, // ID
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '<' , '=' , '#' }, // ,
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '#' , '<' , '#' , '#' }, // $
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

		case token_comma:
			return operator_comma;

		case token_int:
		case token_double:
		case token_identifier:
			return operator_ID;

		case token_semicolon:
		case token_then:
		case token_do:
		case token_colon:
		case token_of:
		case token_to:
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
			//printf("blblb\n");
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
	//printf("znak %c\n", pom );
	return enum_sign(pom);
}


int check_rule(TStack * stack, TRule rule)
{
	stack_pop(stack);
	//printf("stack_top--: %d\n", (int)stack_top(stack));
	if ((int)stack_top(stack) == operator_non_term)
    {
		stack_pop(stack);
		//printf("stack_top---: %d\n", (int)stack_top(stack));
		if (rule == rule_19)
		{
			//stack_pop(stack);
	      	stack_push(stack,(void *)operator_non_term);
	       	printf("Precedence syntax used rule %d\n",rule);
	       	gen_ins(rule, global.ins_list, NULL, NULL, NULL);
	       	return 0;
		}
	    else if ((int)stack_top(stack) == sign_less)
	    {
	        stack_pop(stack);
	      	stack_push(stack,(void *)operator_non_term);
	       	printf("Precedence syntax used rule %d\n",rule);
	       	gen_ins(rule, global.ins_list, NULL, NULL, NULL);
	       	return 0;
	    }
	    else
	    {
	    fprintf(stderr,"ERROR: Excpects: < but it gets: %d \n",(int)stack_top(stack));
	    return 1;
	    }

	}
	else
	{
	fprintf(stderr,"ERROR: Excpects: E but it gets: %d \n",(int)stack_top(stack));
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
	if (token->type == token_semicolon)
	{
		fprintf(stderr,"ERROR: Wrong syntax\n");
		return 1;
	}

	do
	{
		// condition to erase function name to work with table
		if (token->type == token_identifier)
		{
			TToken * pom;
			pom = token_init();
			pom = token_get(filename);
			if (pom->type == token_parenthesis_left)
			{
				token_return_token(pom);
				token_free(token);
				token = token_get(filename);
			}
			else{
				token_return_token(pom);
			}

		}
		//if (token->type == token_colon)
		//	fprintf(stderr, "there\n" );
		//fprintf(stderr, "asdasda\n");
		//fprintf(stderr,"stack_top: %d\n", (int)stack_top(stack));
		//fprintf(stderr,"token_type: %d\n", token->type);
		//fprintf(stderr,"stack_count: %d\n", stack_count(stack));

		
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
	       				gen_ins(rule_1, global.ins_list, NULL, NULL, NULL);
					}
					else{
						fprintf(stderr,"ERROR: Excpects: < but it gets: %d \n",(int)stack_top(stack));
					}
				}
				//This condition handle rule E -> (E)
				else if((int)stack_top(stack) == operator_right_parenthesis)
				{
					stack_pop(stack);
					//fprintf(stderr, " stack %d\n",(int)stack_top(stack) );
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
	       						gen_ins(rule_2, global.ins_list, NULL, NULL, NULL);
							}
							else
							{
								fprintf(stderr,"ERROR: Excpects: < but it gets: %d \n",(int)stack_top(stack));
								return 1;
							}
						}
						else
						{
							fprintf(stderr,"ERROR: Excpects: ( but it gets: %d \n",(int)stack_top(stack));
							return 1;
						}
					}
					else if ((int)stack_top(stack) == operator_left_parenthesis)
					{
						stack_pop(stack);
						if((int)stack_top(stack) == sign_less)
						{
							stack_pop(stack);
							stack_push(stack,(void *)operator_non_term);
							printf("Precedence syntax used rule 2: E -> (E)\n");
       						gen_ins(rule_2, global.ins_list, NULL, NULL, NULL);
						}
						else
						{
							fprintf(stderr,"ERROR: Excpects: < but it gets: %d \n",(int)stack_top(stack));
							return 1;
						}
					}
					else
					{
						fprintf(stderr,"ERROR: Excpects: E or ( but it gets: %d \n",(int)stack_top(stack));
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
							if ((check_rule(stack,rule_3)) == 1 )
							{
								return 1;
							}
							break;

			  			case operator_mul:
			  				if ((check_rule(stack,rule_4)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_div:
	          				if ((check_rule(stack,rule_5)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_sign_div:
	          				if ((check_rule(stack,rule_6)) == 1 )
							{
								return 1;
							}
							break;

	         			case operator_mod:
	         				if ((check_rule(stack,rule_7)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_and:
	          				if ((check_rule(stack,rule_8)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_plus:
	          				if ((check_rule(stack,rule_9)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_minus:
	          				if ((check_rule(stack,rule_10)) == 1 )
							{
								return 1;
							}
							break;

	             		case operator_or:
	             			if ((check_rule(stack,rule_11)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_equal:
	          				if ((check_rule(stack,rule_12)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_diff:
	          				if ((check_rule(stack,rule_13)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_less:
	          				if ((check_rule(stack,rule_14)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_less_equal:
	          				if ((check_rule(stack,rule_15)) == 1 )
							{
								return 1;
							}
							break;

	                    case operator_greater:
	                    	if ((check_rule(stack,rule_16)) == 1 )
							{
								return 1;
							}
							break;

	                    case operator_greater_equal:
	                    	if ((check_rule(stack,rule_17)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_in:
	          				if ((check_rule(stack,rule_18)) == 1 )
							{
								return 1;
							}
							break;

						case operator_comma:
							if ((check_rule(stack,rule_19)) == 1 )
							{
								return 1;
							}
							break;

	          			default:
	          				fprintf(stderr,"ERROR:: Excpects: operator but it gets: %d \n",(int)stack_top(stack));
	          				return 1;
					}
				}
				else
				{
					fprintf(stderr,"ERROR Wrong syntax\n");
					return 1;
				}
				break;

			case sign_fault:
				fprintf(stderr,"ERROR: Got error sign from precedence table\n");
				return 1;
		}
		/*if ( (stack_count(stack) == 2) && ((int)stack_top(stack) == operator_non_term) && ((token->type == token_colon ) || (token->type == token_do) ||\
	 //(token->type == token_then) || (token->type == token_of)))
			//stack_pop(stack);*/

		//printf("------stack_top: %d\n", (int)stack_top(stack));
		//printf("------token_type: %d\n", token->type);
		//printf("------stack_count: %d\n", stack_count(stack));

	}while( !((stack_count(stack) == 2) && ((int)stack_top(stack) == operator_non_term ) && ((recon_sign(token)) == operator_dolar )) );

	printf("Precedence syntax analysis OK! \n");
	//fprintf(stderr, " token_type END: %d\n",token->type );
	if ((token->type == token_colon ) || (token->type == token_do) ||\
	 (token->type == token_then) || (token->type == token_of) || (token->type == token_to))
	{
		//fprintf(stderr, "Vracim %d\n",token->type );
		token_return_token(token);
		//fprintf(stderr,"asdasd\n");
	}
	else
	{
		token_free(token);
	}

	//token_free(token);
	
	return 0;

}

