#include "expr.h"
#include "garbage.h"
#include "generator.h"
#include "types.h"
#include "uStack.h"
#include "symbol.h"
#include "error.h"
#include <stdlib.h>
#include <ctype.h>

#define table_size 23

/**
* Precedence table with priorities.
* Rows are tokens on the top of the stack and collums are actual tokens.
* Sign '<' means shift to the stack. Sign '>' means reduction.
**/
const int precedence_table[table_size][table_size]=
{
//    not    *     /    div   mod   and    +     -    or     =    <>     <    <=     >    >=    in     (     )    ID    func  array  ,    $
	{ '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // not
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // *
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // /
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // div
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // mod
	{ '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // and	
	{ '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // +
	{ '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // -
	{ '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // or
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // =
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // <>
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // <
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // <=
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // >
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // >=
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '<' , '>' , '<' , '<' , '<' , '>' , '>' }, // in
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '<' , '<' , '<' , '=' , '#' }, // (
	{ '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '>' , '#' , '#' , '#' , '>' , '>' }, // )
	{ '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '>' , '#' , '>' , '#' , '#' , '#' , '>' , '>' }, // ID
	{ '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '=' , '#' , '#' , '#' , '#' , '#' , '#' }, // func
	{ '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '=' , '#' , '#' , '#' , '#' , '#' , '#' }, // array
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , '<' , '<' , '<' , '=' , '#' }, // ,
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '<' , '#' , '<' , '<' , '<' , '#' , '#' }, // $
};


int sem_check(TToken * token, seman check)
{
	htab_listitem * item;
	
	if(uStack_count(global.local_symbols) > 0)
	{
		item = htab_lookup(uStack_top(htab_t*,global.local_symbols), token->data->data);
		if (item == NULL)
		{
			item = htab_lookup(global.global_symbol, token->data->data);
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
			
			
		}	
		else if ((item->type == type_function) && (check == check_var))
		{
			throw_error(error_type);
		}
		else if ((item->type == type_variable) && (check == check_func))
		{
			throw_error(error_type);
		}
		
		
	}
	else
	{
		item = htab_lookup(global.global_symbol, token->data->data);
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
		
	}
	return 0;
}


/**
*Fucntion to recognize sign from token and revert it to operator_number made in expr.h
* @param token token
* @return operator_number
**/
operator_number recon_sign(TToken * token, parse_context context)
{
	TToken * pom;
	pom = token_init();

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
		//printf("asaaaa\n");
		pom = precedence_table[uStack_top(int, stack)][recon_sign(token,context)];
	}
	else
	{
		uStack_remove(stack);
		pom = precedence_table[uStack_top(int, stack)][recon_sign(token,context)];
		uStack_push(int, stack,operator_non_term);
	}
	
	if ((token->type == token_identifier) && ((recon_sign(token,context)) == operator_ID ))
	{
		
		sem_check(token, check_var);
	}
	else if ((token->type == token_identifier) && ((recon_sign(token,context)) == operator_func ))
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
	//printf("uStack_top--: %d\n", uStack_top(int, stack));
	if (uStack_top(int, stack) == operator_non_term)
    {
		uStack_remove(stack);
		//printf("uStack_top---: %d\n", uStack_top(int, stack));
		if (rule == rule_19)
		{
			//uStack_remove(stack);
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
	       	gen_ins(rule, global.ins_list, stack_top(var_stack), tmp, new_var);
	       	stack_pop(var_stack);
	       	stack_push(var_stack, new_var);
	       	return 0;
	    }
	    else
	    {
	    fprintf(stderr,"ERROR: Excpects: < but it gets: %d \n",uStack_top(int, stack));
	    return 1;
	    }

	}
	else
	{
	fprintf(stderr,"ERROR: Excpects: E but it gets: %d \n",uStack_top(int, stack));
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

	//printf("stack_top: %d\n", (int)stack_top(stack));
	//printf("dolar: %d\n",operator_dolar );
	if (token->type == token_semicolon)
	{
		fprintf(stderr,"ERROR: Wrong syntax\n");
		return 1;
	}

	do
	{
		
		//if (token->type == token_colon)
		//	fprintf(stderr, "there\n" );
		//fprintf(stderr, "asdasda\n");
		//fprintf(stderr,"stack_top: %d\n", uStack_top(int,stack));
		//fprintf(stderr,"token_type: %d\n", token->type);
		//fprintf(stderr,"uStack_count: %ld\n", uStack_count(stack));

		//this condition handle minus or plus operator in the begining of expression (-2 mod 3)
		if (((uStack_top(int,stack)) != operator_non_term) && ((token->type == token_sub) || (token->type == token_add)) && (!(uStack_top(int,stack) == operator_ID)))
			uStack_push(int,stack,operator_non_term);

		
		switch(get_sign(token,stack,Func_call)){
			case sign_equal:
				uStack_push(int, stack , recon_sign(token,Func_call));
				
				if(token->type == token_int || token->type == token_double)
					stack_push(var_stack,create_const(token));

				token_free(token);
				token = token_get();
				break;

			case sign_less:
				if (uStack_top(int , stack) != operator_non_term)
				{
					uStack_push(int, stack, sign_less);
					uStack_push(int,stack,recon_sign(token,Func_call));
				}
				else{
					uStack_remove(stack);
					uStack_push(int,stack,sign_less);
					uStack_push(int,stack,operator_non_term);
					uStack_push(int, stack,recon_sign(token,Func_call));
				}

				if(token->type == token_int || token->type == token_double)
					stack_push(var_stack,create_const(token));

				token_free(token);
				token = token_get();
				break;	

			case sign_greater:
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
						fprintf(stderr,"ERROR: Excpects: < but it gets: %d \n",uStack_top(int,stack));
						return 1;
					}
				}
				//This condition handle rule E -> (E)
				else if(uStack_top(int,stack) == operator_right_parenthesis)
				{
					uStack_remove(stack);
			
					//fprintf(stderr, " stack %d\n",uStack_top(int, stack) );
					switch (uStack_top(int, stack))
					{

						case operator_non_term:
						{
							uStack_remove(stack);
							//This handle rule E -> (E);
							if (uStack_top(int, stack) == operator_left_parenthesis)
							{
								uStack_remove(stack);
								//uStack_push(int, stack,operator_non_term);
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
										fprintf(stderr,"ERROR: Excpects: < but it gets: %d \n",uStack_top(int, stack));
										return 1;
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
										fprintf(stderr,"ERROR: Excpects: < but it gets: %d \n",uStack_top(int, stack));
										return 1;
									}
								}
								else
								{

									fprintf(stderr,"ERROR: Excpects: < but it gets: %d \n",uStack_top(int, stack));
									return 1;
								}
							}
							//This handle rule E -> func(E,E...)
							else if (uStack_top(int, stack) == operator_comma)
							{
								//fprintf(stderr,"ahoooooooooooooo\n");
								int number_param = 1;
								int check_E = 1;
								uStack_remove(stack);
								while (!(uStack_top(int, stack) == operator_left_parenthesis))
								{
									//fprintf(stderr, "uStack_top: %d\n",uStack_top(int, stack) );
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
								//printf("asdasd\n");
								if (uStack_top(int, stack) == operator_func)
								{
									uStack_remove(stack);
									//printf("asdaaaaaaaaaaaaaa\n");
									if (uStack_top(int, stack) == sign_less)
									{
										uStack_remove(stack);
										uStack_push(int, stack,operator_non_term);
										printf("Precedence syntax used rule 21: E -> func(E,E..) with %d parametrs\n",number_param);
										/*while(uStack_count(stack)){
											printf("stack %d\n",uStack_top(int, stack) );
											uStack_remove(stack);
										}*/
										//printf("uStack_count %d\n",uStack_count(stack) );
									}
									else
									{
										fprintf(stderr,"ERROR: Excpects: < but it gets: %d \n",uStack_top(int, stack));
										return 1;
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
									fprintf(stderr,"ERROR: Excpects: < but it gets: %d \n",uStack_top(int, stack));
									return 1;
								}
							}
							else
							{
								fprintf(stderr,"ERROR Wrong syntax\n");
								return 1;
							}
							break;


					}
				}
				//This condition should handle with others rules, ended with E
				else if (uStack_top(int, stack) == operator_non_term)
				{
					//printf("uStack_top-: %d\n", uStack_top(int, stack));
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

	          			default:
	          				fprintf(stderr,"ERROR:: Excpects: operator but it gets: %d \n",uStack_top(int,stack));
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
		/*if ( (uStack_count(stack) == 2) && (uStack_top(int, stack) == operator_non_term) && ((token->type == token_colon ) || (token->type == token_do) ||\
	 //(token->type == token_then) || (token->type == token_of)))
			//uStack_remove(stack);*/

		//printf("------uStack_top: %d\n", uStack_top(int, stack));
		//printf("------token_type: %d\n", token->type);
		//printf("------uStack_count: %d\n", stack_count(stack));

	}while( !((uStack_count(stack) == 2) && (uStack_top(int,stack) == operator_non_term ) && ((recon_sign(token,Func_call)) == operator_dolar )) );

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
		//((TIns *)global.ins_list->act->data)->adr3 = result;
		// _free(((TIns *)global.ins_list->act->data)->adr3);
	}
	//token_free(token);
	
	return 0;

}

