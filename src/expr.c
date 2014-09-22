#include "expr.h"
#include "garbage.h"
#include "generator.h"
#include "types.h"
#include "uStack.h"
#include "symbol.h"
#include "error.h"
#include <stdlib.h>
#include <ctype.h>

#define table_size 25

/**
* Precedence table with priorities.
* Rows are tokens on the top of the stack and collums are actual tokens.
* Sign '<' means shift to the stack. Sign '>' means reduction.
**/
const int precedence_table[table_size][table_size]=
{
//   unary- not    *     /    div   mod   and    +     -    or    xor   =     <>     <    <=     >    >=    in     (     )    ID    func  array  ,    $
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '#' , '>' }, //unary -
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // not
	{ '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // *
	{ '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // /
	{ '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // div
	{ '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // mod
	{ '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // and	
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // +
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // -
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // or
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // xor
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // =
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // <>
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // <
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // <=
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // >
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // >=
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // in
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '<' , '<' , '<' , '=' , '#' }, // (
	{ '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '>' , '#' , '#' , '#' , '>' , '>' }, // )
	{ '#' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '>' , '#' , '#' , '#' , '>' , '>' }, // ID
	{ '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '=' , '#' , '#' , '#' , '#' , '#' , '#' }, // func
	{ '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '=' , '#' , '#' , '#' , '#' , '#' , '#' }, // array
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '<' , '<' , '<' , '=' , '#' }, // ,
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '#' , '<' , '<' , '<' , '#' , '#' }, // $
};


int sem_check(TToken * token, seman check)
{
	htab_listitem * item;
	item = VariableExists(token->data->data);

	if (item == NULL)
	{
		throw_error(error_var_not_exists);
	}
	else if ((item->type == type_function) && (check == check_var))
	{
		throw_error(error_type);
	}
	else if ((item->type == type_variable) && (check == check_func))
	{
		throw_error(error_type);
	}

	return 0;
}


/**
*Fucntion to recognize sign from token and revert it to operator_number made in expr.h
* @param token token
* @return operator_number
**/
operator_number recon_sign(TToken * token, parse_context context, uStack_t * stack)
{
	TToken * pom;
	pom = token_init();
	int pom_operand = 0;

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
			//printf("stack_top in ser %d \n", uStack_top(int,stack));
			if (((uStack_count(stack)) == 1) && (uStack_top(int,stack) == operator_dolar ))
			{
				return operator_unary_minus;
			}
			else if (((uStack_top(int,stack)) == operator_left_parenthesis) || (uStack_top(int,stack) == operator_comma) )
			{
				return operator_unary_minus;
			}
			else if (uStack_top(int, stack) >= sign_equal)
			{
				pom_operand = uStack_top(int, stack);
				uStack_remove(stack);
				if (((uStack_top(int,stack)) <= operator_left_parenthesis) || (uStack_top(int,stack) == operator_comma) || (uStack_top(int,stack) == operator_dolar )) 
				{
					uStack_push(int,stack ,pom_operand );
					return operator_unary_minus;
				}
				else
				{
					uStack_push(int,stack ,pom_operand );
					return operator_minus;
				}
				
			}
			else if (((uStack_top(int,stack)) <= operator_left_parenthesis))
			{
				return operator_unary_minus;
			}
			else
			{	
				return operator_minus;
			}
			
		case token_xor:
			return operator_xor;

		case token_or:
			return operator_or;

		case token_equal:
			if(context ==  context_index)
			{
				return operator_dolar;
			}
			else{
				return operator_equal;
			}

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
		case token_bracket_left:
			return operator_left_parenthesis;

		case token_parenthesis_right:
		case token_bracket_right:
			return operator_right_parenthesis;

		case token_comma:
			return operator_comma;

		case token_assign:
			if (context == context_index)
				return operator_dolar;

			return sign_fault;

		case token_int:
		case token_double:
		case token_string:
		case token_false:
		case token_true:
			return operator_ID;

		case token_identifier:
			pom = token_get(global.file);
			if (pom->type == token_parenthesis_left)
			{
				token_return_token(pom);
				return operator_func;
			}
			else if (pom->type == token_bracket_left)
			{
				token_return_token(pom);
				return operator_array;
			}
			else{
				token_return_token(pom);
				return operator_ID;
			}


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
precedence_number get_sign(TToken * token, uStack_t * stack, parse_context context)
{
	int pom = 0;
	//fprintf(stderr,"Stack_top %d \n", uStack_top(int,stack));
	//fprintf(stderr,"token %d \n", recon_sign(token,context));

	if (uStack_top(int, stack) != operator_non_term)
	{
		pom = precedence_table[uStack_top(int, stack)][recon_sign(token,context,stack)];
	}
	else
	{
		uStack_remove(stack);
		pom = precedence_table[uStack_top(int, stack)][recon_sign(token,context,stack)];
		uStack_push(int, stack,operator_non_term);
	}
	//printf("recon %d \n", recon_sign(token,context,stack));
	if ((token->type == token_identifier) && ((recon_sign(token,context,stack)) == operator_ID ))
	{
		
		sem_check(token, check_var);
	}
	else if ((token->type == token_identifier) && ((recon_sign(token,context,stack)) == operator_func ))
	{
		sem_check(token, check_func);
	}
	
	//printf("znak %c\n", pom );
	return enum_sign(pom);
}


/**
*Function that check rule
*@param stack is stack of term and non-term operators
*@param rule is rule which is now expected
**/
int check_rule(uStack_t * stack, TRule rule, TStack *var_stack)
{
	symbolVariable *tmp;
	symbolVariable *new_var;
	uStack_remove(stack);
	if (uStack_top(int, stack) == operator_non_term)
    {
		uStack_remove(stack);
		if (rule == rule_19)
		{
	      	uStack_push(int, stack,operator_non_term);
	       	printf("Precedence syntax used rule %d\n",rule);
	    // 	gen_ins(rule, global.ins_list, NULL, NULL, NULL);
	       	return 0;
		}
	    else if (uStack_top(int, stack) == sign_less)
	    {
	        uStack_remove(stack);
	      	uStack_push(int, stack,operator_non_term);
	       	printf("Precedence syntax used rule %d\n",rule);
	       	tmp = stack_top(var_stack);
	       	stack_pop(var_stack);
	       	new_var = create_const(NULL);
	       	gen_expr(rule, stack_top(var_stack), tmp, new_var);
	       	stack_pop(var_stack);
	       	stack_push(var_stack, new_var);
	       	return 0;
	    }
	    else
	    {
			throw_error(error_sign_less_precedence);
	    }

	}
	else if (rule == rule_24)
	{	// 2 + - 4
	
		if (uStack_top(int, stack) == sign_less)
	    {
	        uStack_remove(stack);
	      	uStack_push(int, stack,operator_non_term);
	       	printf("Precedence syntax used rule %d\n",rule);
	       	gen_expr(rule, stack_top(var_stack), NULL, stack_top(var_stack));
	       	return 0;
	    }
	    else
	    {
			throw_error(error_sign_less_precedence);
	    }		
	}	
	else
	{
		fprintf(stderr,"ERROR: Excpectss: E but it gets: %d \n",uStack_top(int, stack));
		return 1;
	}
	return 1;
}


/**
* Function precedence making syntax analysition of expressions.
* @param filename is file to translate
* @return number of failiure or correct
**/
int precedence(FILE *filename,parse_context Func_call, symbolVariable *result)
{
	TToken * token;
	token = token_init();
	token = token_get();

	TStack *var_stack;
	var_stack = stack_init();

	uStack_init(stack);
	uStack_push(int, stack, operator_dolar);
	
	if (Func_call == context_args)
	{
		uStack_push(int, stack, sign_less);
		uStack_push(int, stack, operator_func);

	}
	else if (Func_call == context_index)
	{
		uStack_push(int, stack, sign_less);
		uStack_push(int, stack, operator_array);
	
	}

	if (token->type == token_semicolon)
	{
		throw_error(error_syntax_in_precedence);
	}

	do
	{
		
		//if (token->type == token_colon)
		//	fprintf(stderr, "there\n" );
		//fprintf(stderr, "asdasda\n");
		//fprintf(stderr,"stack_top: %d\n", uStack_top(int,stack));
		//fprintf(stderr,"token_type: %d\n", token->type);
		//fprintf(stderr,"uStack_count: %ld\n", uStack_count(stack));
		
		switch(get_sign(token,stack,Func_call)){
			case sign_equal:
				uStack_push(int, stack , recon_sign(token,Func_call,stack));
				
				if(token->type == token_int || token->type == token_double || token->type == token_string)
					stack_push(var_stack,create_const(token));
				else if(token->type == token_identifier)
				{
					htab_listitem* hitem = VariableExists(token->data->data);
					if(hitem->type == type_variable)
						stack_push(var_stack,hitem->ptr.variable);
					else
						printf("volani funkce\n");
				}

				token_free(token);
				token = token_get();
				break;

			case sign_less:
				if (uStack_top(int , stack) != operator_non_term)
				{
					uStack_push(int, stack, sign_less);
					uStack_push(int,stack,recon_sign(token,Func_call,stack));
				}
				else{
					uStack_remove(stack);
					uStack_push(int,stack,sign_less);
					uStack_push(int,stack,operator_non_term);
					uStack_push(int, stack,recon_sign(token,Func_call,stack));
				}

				if(token->type == token_int || token->type == token_double || token->type == token_string)
					stack_push(var_stack,create_const(token));
				else if(token->type == token_identifier)
				{
					htab_listitem* hitem = VariableExists(token->data->data);
					if(hitem->type == type_variable)
						stack_push(var_stack,hitem->ptr.variable);
					else
						printf("volani funkce\n");
				}

				token_free(token);
				token = token_get();
				break;	

			case sign_greater:
				//printf("aasdasd");
				//This condition handle rule E -> ID
				if (uStack_top(int,stack) == operator_ID){
					uStack_remove(stack);
					if(uStack_top(int,stack) == sign_less)
					{
						uStack_remove(stack);
						uStack_push(int,stack,operator_non_term);
						printf("Precedence syntax used rule 1: E -> ID\n");
	       				//gen_ins(rule_1, global.ins_list, NULL, NULL, NULL);
					}
					else{
						throw_error(error_sign_less_precedence);
					}
				}
				//This condition handle rule E -> (E)
				else if(uStack_top(int,stack) == operator_right_parenthesis)
				{
					uStack_remove(stack);
			
					switch (uStack_top(int, stack))
					{

						case operator_non_term:
						{
							uStack_remove(stack);
							//This handle rule E -> (E);
							if (uStack_top(int, stack) == operator_left_parenthesis)
							{
								uStack_remove(stack);
								if(uStack_top(int, stack) == sign_less)
								{
									uStack_remove(stack);
									uStack_push(int, stack,operator_non_term);
									printf("Precedence syntax used rule 2: E -> (E)\n");
	       						//	gen_ins(rule_2, global.ins_list, NULL, NULL, NULL);
								}
								else if (uStack_top(int, stack) == operator_func)
								{
									uStack_remove(stack);
									if (uStack_top(int, stack) == sign_less)
									{
										uStack_remove(stack);
										uStack_push(int, stack,operator_non_term);
										printf("Precedence syntax used rule 20: E -> func(E)\n");
									}
									else
									{
										throw_error(error_sign_less_precedence);
									}
								}
								else if (uStack_top(int, stack) == operator_array)
								{
									uStack_remove(stack);
									if (uStack_top(int, stack) == sign_less)
									{
										uStack_remove(stack);
										uStack_push(int, stack,operator_non_term);
										printf("Precedence syntax used rule 22: E -> array[E]\n");
									}
									else
									{
										throw_error(error_sign_less_precedence);
									}
								}
								else
								{
									throw_error(error_sign_less_precedence);
								}
							}
							//This handle rule E -> func(E,E...)
							else if (uStack_top(int, stack) == operator_comma)
							{
								int number_param = 1;
								int check_E = 1;
								uStack_remove(stack);
								while (!(uStack_top(int, stack) == operator_left_parenthesis))
								{
									if ((uStack_top(int, stack) == operator_non_term) && check_E)
									{
										check_E = 0;
										number_param ++;
										uStack_remove(stack);
									}
									else if ((uStack_top(int, stack) == operator_comma) && !(check_E))
									{
										check_E = 1;
										uStack_remove(stack);
									}
									else
									{
										fprintf(stderr,"ERROR: Wrong sign in function parametr \n");
										return 1;
									}
								}
								uStack_remove(stack);
								if (uStack_top(int, stack) == operator_func)
								{
									uStack_remove(stack);
									if (uStack_top(int, stack) == sign_less)
									{
										uStack_remove(stack);
										uStack_push(int, stack,operator_non_term);
										printf("Precedence syntax used rule 21: E -> func(E,E..) with %d parametrs\n",number_param);
									}
									else
									{
										throw_error(error_sign_less_precedence);
									}
								}	
								else
								{
									fprintf(stderr,"ERROR: Excpects: func but it gets: %d \n",uStack_top(int, stack));
									return 1;
								}
							}
							else
							{
								fprintf(stderr,"ERROR: Excpects: ( but it gets: %d \n",uStack_top(int, stack));
								return 1;
							}
							break;
						}

						case operator_left_parenthesis:
							uStack_remove(stack);
							//this handle empty parenthesis
							if (uStack_top(int, stack) == sign_less)
							{
								uStack_remove(stack);
							}
							else if (uStack_top(int, stack) == operator_func)
							{
								uStack_remove(stack);
								if (uStack_top(int, stack) == sign_less)
								{
									uStack_remove(stack);
									uStack_push(int, stack,operator_non_term);
									printf("Precedence syntax used rule 19: E -> func() \n");

								}
								else
								{
									throw_error(error_sign_less_precedence);
								}
							}
							else
							{
								throw_error(error_syntax_in_precedence);
							}
							break;


					}
				}
				//This condition should handle with others rules, ended with E
				else if (uStack_top(int, stack) == operator_non_term)
				{
					uStack_remove(stack);
					switch(uStack_top(int,stack)){
						case operator_not:
							if ((check_rule(stack, rule_3, var_stack)) == 1 )
							{
								return 1;
							}
							break;

			  			case operator_mul:
			  				if ((check_rule(stack,rule_4, var_stack)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_div:
	          				if ((check_rule(stack,rule_5, var_stack)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_sign_div:
	          				if ((check_rule(stack,rule_6, var_stack)) == 1 )
							{
								return 1;
							}
							break;

	         			case operator_mod:
	         				if ((check_rule(stack,rule_7, var_stack)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_and:
	          				if ((check_rule(stack,rule_8, var_stack)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_plus:
	          				if ((check_rule(stack,rule_9, var_stack)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_minus:
	          				if ((check_rule(stack,rule_10, var_stack)) == 1 )
							{
								return 1;
							}
							break;

	             		case operator_or:
	             			if ((check_rule(stack,rule_11, var_stack)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_equal:
	          				if ((check_rule(stack,rule_12, var_stack)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_diff:
	          				if ((check_rule(stack,rule_13, var_stack)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_less:
	          				if ((check_rule(stack,rule_14, var_stack)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_less_equal:
	          				if ((check_rule(stack,rule_15, var_stack)) == 1 )
							{
								return 1;
							}
							break;

	                    case operator_greater:
	                    	if ((check_rule(stack,rule_16, var_stack)) == 1 )
							{
								return 1;
							}
							break;

	                    case operator_greater_equal:
	                    	if ((check_rule(stack,rule_17, var_stack)) == 1 )
							{
								return 1;
							}
							break;

	          			case operator_in:
	          				if ((check_rule(stack,rule_18, var_stack)) == 1 )
							{
								return 1;
							}
							break;
							
						case operator_xor:
	          				if ((check_rule(stack,rule_23, var_stack)) == 1 )
							{
								return 1;
							}
							break;
							
						case operator_unary_minus:
							if ((check_rule(stack,rule_24, var_stack)) == 1 )
							{
								return 1;
							}
							break;

	          			default:
	          				fprintf(stderr,"ERROR:: Excpects: operator but it gets: %d \n",uStack_top(int,stack));
	          				return 1;
					}
				}
				else
				{
					throw_error(error_syntax_in_precedence);
				}
				break;

			case sign_fault:
				throw_error(error_sign_fault);
		}

	}while( !((uStack_count(stack) == 2) && (uStack_top(int,stack) == operator_non_term ) && ((recon_sign(token,Func_call,stack)) == operator_dolar )) );

	printf("Precedence syntax analysis OK! \n");
	//fprintf(stderr, " token_type END: %d\n",token->type );
	if ((token->type == token_colon ) || (token->type == token_do) ||\
	 (token->type == token_then) || (token->type == token_of) || (token->type == token_to)\
	 || (Func_call == context_index))
	{
		//fprintf(stderr, "Vracim %d\n",token->type );
		token_return_token(token);
		//fprintf(stderr,"asdasd\n");
	}
	else
	{
		token_free(token);
	}

	if(result != NULL){
		if(global.ins_list->last != NULL)
		{
			TIns *ins = global.ins_list->last->data;
		//	_free(ins->adr3);
			ins->adr3 = result;
		}
		else
		{
			symbolVariable *var = stack_top(var_stack);
			result->type = var->type;
			switch(var->type){
				case variable_integer:
					result->value.value_number = var->value.value_number;
					break;
				case variable_double:
					result->value.value_double = var->value.value_double;
					break;
				case variable_boolean:
					result->value.value_boolean = var->value.value_boolean;
					break;
				//case variable_string:
				//	result->value.value_string = var->value.value_string;
				//	break;
				//case variable_char:
				//	result->value.value_char = var->value.value_char;
				//	break;
				default:
					printf("array or string or char\n");				
			}
		}
		//((TIns *)global.ins_list->act->data)->adr3 = result;
		// _free(((TIns *)global.ins_list->act->data)->adr3);
	}
	//token_free(token);
	
	return 0;
}
