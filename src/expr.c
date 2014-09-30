#include "expr.h"
#include "garbage.h"
#include "generator.h"
#include "types.h"
#include "uStack.h"
#include "symbol.h"
#include "error.h"
#include "debug.h"
#include "builtin.h"
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

symbolFunction *func = NULL;

int sem_check(TToken * token, seman check)
{
	htab_listitem * item;
	if(check == check_func)
	{
		item = htab_lookup(global.global_symbol,token->data);
	}
	else
	{
	item = VariableExists(token->data);
	}

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
		printf("\n123\n");
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
			//print_debug(debug_prec, "stack_top in ser %d \n", uStack_top(int,stack));
			//this case solved unary minus ...
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
				func = htab_lookup(global.global_symbol, token->data)->ptr.function;
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
		case token_end:
		case token_invalid:
			return operator_dolar;
			
		case token_write:
		case token_readln:
			throw_error(error_syntax_in_precedence);

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
			//print_debug(debug_prec, "blblb");
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
	//print_debug(debug_prec, "recon %d ", recon_sign(token,context,stack));
	if ((token->type == token_identifier) && ((recon_sign(token,context,stack)) == operator_ID ))
	{
		
		sem_check(token, check_var);
	}
	else if ((token->type == token_identifier) && ((recon_sign(token,context,stack)) == operator_func ))
	{
		sem_check(token, check_func);
	}
	
	//print_debug(debug_prec, "znak %c", pom );
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
			print_debug(debug_prec, "Precedence syntax used rule %d",rule);
		//  gen_ins(rule, global.ins_list, NULL, NULL, NULL);
			return 0;
		}
		else if (uStack_top(int, stack) == sign_less)
		{
			uStack_remove(stack);
			uStack_push(int, stack,operator_non_term);
			print_debug(debug_prec, "Precedence syntax used rule %d",rule);
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
	{   // 2 + - 4
	
		if (uStack_top(int, stack) == sign_less)
		{
			uStack_remove(stack);
			uStack_push(int, stack,operator_non_term);
			print_debug(debug_prec, "Precedence syntax used rule %d",rule);
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
		throw_error(error_syntax_in_precedence);
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

	int i = 0;
	TStack *var_stack;
	var_stack = stack_init();
	symbolVariable *new_var = NULL;

	uStack_t *tmp = NULL;

	uStack_init(func_args_stack);
	uStack_init(func_args);
	uStack_push(uStack_t*, func_args_stack, func_args);

	uStack_init(stack);
	uStack_push(int, stack, operator_dolar);
	
	if ((Func_call == context_args) || (Func_call == context_write) || (Func_call == context_readln))
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
		
		//fprintf(stderr,"stack_top: %d\n", uStack_top(int,stack));
		//fprintf(stderr,"token_type: %d\n", token->type);
		//fprintf(stderr,"uStack_count: %ld\n", uStack_count(stack));
		
		switch(get_sign(token,stack,Func_call)){
			case sign_equal:
				uStack_push(int, stack , recon_sign(token,Func_call,stack));
				
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
					{stack_push(var_stack,create_const(token));
				//    printf("++++++++++%s   %d\n", token->data, Func_call);
					}
				else if(token->type == token_identifier)
				{
					//printf("++++++++++%s\n", token->data);
					htab_listitem* hitem = VariableExists(token->data);
					if(hitem->type == type_variable)
						stack_push(var_stack,hitem->ptr.variable);
				}

				token_free(token);
				token = token_get();
				break;  

			case sign_greater:
				//print_debug(debug_prec, "aasdasd");
				//This condition handle rule E -> ID
				if (uStack_top(int,stack) == operator_ID){
					uStack_remove(stack);
					if(uStack_top(int,stack) == sign_less)
					{
						uStack_remove(stack);
						uStack_push(int,stack,operator_non_term);
						print_debug(debug_prec, "Precedence syntax used rule 1: E -> ID");
						if(Func_call == context_write || Func_call == context_readln)
						{
							uStack_push(symbolVariable *, uStack_top(uStack_t*,func_args_stack), stack_top(var_stack));
						}
						else if(func != NULL)
						{                
							print_debug(debug_generator, "type: %d", func->args);
							htab_listitem *hitem = htab_lookup(func->local_symbol, func->args[i].name->data);
							print_debug(debug_generator, "type: %d", hitem->type);
							gen_code(ins_assign, hitem->ptr.variable, NULL, stack_top(var_stack));
							i++;
						}
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
									print_debug(debug_prec, "Precedence syntax used rule 2: E -> (E)");
								}
								else if (uStack_top(int, stack) == operator_func)
								{
									uStack_remove(stack);
									if (uStack_top(int, stack) == sign_less)
									{
										uStack_remove(stack);
										uStack_push(int, stack,operator_non_term);
										print_debug(debug_prec, "Precedence syntax used rule 20: E -> func(E)");

										if(Func_call != context_write && Func_call != context_readln)
										{    
											new_var = symbol_variable_init2(func->returnType);
											stack_push(var_stack, new_var);
											gen_code(ins_call,func,NULL,new_var);
											func = NULL;
											i = 0;
										}
										else
										{
											switch(Func_call)
											{
												case context_write:
													gen_code(ins_incall, 0, uStack_pop(uStack_t *, func_args_stack),NULL);
													uStack_init2(func_args);
													uStack_push(uStack_t*, func_args_stack, func_args);
													break;
												case context_readln:
													tmp = uStack_pop(uStack_t *, func_args_stack);
													gen_code(ins_incall, (void*)1ULL, uStack_top(symbolVariable*, tmp), NULL);
													break;
												default:
													printf("not yet incall\n");
											}
										}
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
										print_debug(debug_prec, "Precedence syntax used rule 22: E -> array[E]");
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
										throw_error(error_function_parametr_precedence);
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
										
										if(Func_call != context_write && Func_call != context_readln)
										{    
											new_var = symbol_variable_init2(func->returnType);
											stack_push(var_stack, new_var);
											gen_code(ins_call,func,NULL,new_var);
											func = NULL;
											i = 0;
										}
										else
										{
											switch(Func_call)
											{
												case context_write:
													gen_code(ins_incall, 0, uStack_pop(uStack_t *, func_args_stack),NULL);
													uStack_init2(func_args);
													uStack_push(uStack_t*, func_args_stack, func_args);
													break;
												default:
													printf("not rule 21: %d %d\n",Func_call, context_write);
											}
										}
										print_debug(debug_prec, "Precedence syntax used rule 21: E -> func(E,E..) with %d parametrs",number_param);
									}
									else
									{
										throw_error(error_sign_less_precedence);
									}
								}   
								else
								{
									throw_error(error_missing_func_precedence);
								}
							}
							else
							{
								throw_error(error_left_parenthesis_precedence);
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

									gen_code(ins_call,func,NULL,NULL);
									func = NULL;
									i = 0;
									print_debug(debug_prec, "Precedence syntax used rule 19: E -> func() ");

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
							throw_error(error_operator_precedence);
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

	print_debug(debug_prec, "Precedence syntax analysis OK! ");
	//fprintf(stderr, " token_type END: %d\n",token->type );
	if ((token->type == token_colon ) || (token->type == token_do) ||\
	 (token->type == token_then) || (token->type == token_of) || (token->type == token_to)\
	 || (Func_call == context_index) || (token->type == token_end))
	{
		token_return_token(token);
	}
	else
	{
		token_free(token);
	}

	if(result != NULL && Func_call == context_assign)
	{
	//    symbolVariable *qqq = stack_top(var_stack);
		//printf("++++--------++++++++------%d\n",qqq );
		gen_code(ins_assign, result, NULL, stack_top(var_stack));
	}
	else if(result != NULL){
		if(uStack_top(TList *,global.ins_list_stack)->last != NULL)
		{
			TIns *ins = uStack_top(TList *,global.ins_list_stack)->last->data;
			ins->adr3 = result;
		}
		else
		{
			symbolVariable *var = stack_top(var_stack);
			if(var != NULL) copy_variable(result, var);
		}
	}
	//token_free(token);
	
	return 0;
}
