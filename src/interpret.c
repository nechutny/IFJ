/**
 *	@project	IFJ 2014/2015
 *	@file		interpret.c
 *
 *	@author		Petr Lukeš - xlukes06
 */

#include <stdio.h>
#include <math.h>

#include "interpret.h"
#include "garbage.h"
#include "generator.h"
#include "list.h"
#include "symbol.h"
#include "builtin.h"
#include "debug.h"
#include "error.h"
#include "uStack.h"

int pushed = 0;

symbolVariable* get_var(TString *name)
{
	if(!strcmp(name->data, "cond1")) return global.cond1;
	if(!strcmp(name->data, "cond2")) return global.cond2;
	if(!strcmp(name->data, "partresult")) return global.partresult;


	if(pushed == 1)
	{
		global.local_symbols->count--;
		pushed = 2;
	}
	else if(pushed == 2)
	{
		global.local_symbols->count++;
	}

	print_debug(debug_interpret, "name: %s",name->data);
	htab_listitem *hitem = htab_lookup(global.constant_symbol, name->data);
	int i = uStack_count(global.local_symbols) - 1;
	while(hitem == NULL && i >= 0)
	{
		hitem =  htab_lookup(uStack_offset(htab_t *, global.local_symbols, i), name->data);
		i--;
	}
	if(hitem == NULL)
	{
		hitem = htab_lookup(global.global_symbol, name->data);
	}
	if(hitem == NULL)
	{
		print_debug(debug_interpret,"hitem null name: %s",name->data);
	}
	return hitem->ptr.variable;
}

uStack_t *get_var_in_stack(uStack_t *name_stack)
{
	symbolVariable *var;
	uStack_init(var_stack);

	for(int i = 0; i < uStack_count(name_stack); i++)
	{
		var = get_var(uStack_offset(TString *, name_stack, i));
		uStack_push(symbolVariable *,var_stack,var);
	}

	return var_stack;
}

void do_math(char c, symbolVariable *adr1, symbolVariable *adr2, symbolVariable *adr3)
{
	int a = 0, b = 0;

	if(!adr1->inicialized || !adr2->inicialized)
	{
		throw_error(error_uninicialized);
	}

	if(adr1->type == variable_string && adr2->type == variable_string && c == '+')
	{
		adr3->type = variable_string;
		strncpy(adr3->value.value_string, adr1->value.value_string,255);
		strncat(adr3->value.value_string, adr2->value.value_string,255);
		return;
	}
	else if((adr1->type == variable_string || adr2->type == variable_string) && c == '+')
	{
		throw_error(error_not_string);
	}
	else if(adr1->type == variable_string || adr2->type == variable_string)
	{
		throw_error(error_string);
	}

	if(adr1->type == variable_double || adr2->type == variable_double)
		adr3->type = variable_double;
	else
		adr3->type = variable_integer;

	if(adr1->type == variable_double)
		a = adr1->value.value_double;
	else if(adr1->type == variable_integer)
		a = adr1->value.value_number;
	else
	{
		throw_error(error_incopatible_types);
	}

	if(adr2->type == variable_double)
		b = adr2->value.value_double;	
	else if( adr2->type == variable_integer)
		b = adr2->value.value_number;
	else 
	{
		throw_error(error_incopatible_types);
	}

	print_debug(debug_interpret, "a: %d b: %d c:%c",a,b,c);
	adr3->inicialized = 1;
	
	switch(c)
	{
		case '+':
			if(adr3->type == variable_double)
				adr3->value.value_double = a+b;
			else
				adr3->value.value_number = a+b;
			break;

		case '-':
			if(adr3->type == variable_double)
				adr3->value.value_double = a-b;
			else
				adr3->value.value_number = a-b;
			break;

		case '*':
			if(adr3->type == variable_double)
				adr3->value.value_double = a*b;
			else
				adr3->value.value_number = a*b;
			break;

		case '/':
			if(b == 0){
				throw_error(error_dividing_by_zero);
			}
			adr3->type = variable_double;
			adr3->value.value_double = a/b;
			break;
	}
}

void compare(TInsType type, symbolVariable *adr1, symbolVariable *adr2, symbolVariable *adr3){
	double a = 0, b = 0;
	int x = 0;

	if(!adr1->inicialized || !adr2->inicialized)
	{
		throw_error(error_uninicialized);
	}

	adr3->type = variable_boolean;

	if(adr1->type != adr2->type)
	{
		throw_error(error_incopatible_types);
	}

	switch(adr1->type)
	{
		case variable_double:
			a = adr1->value.value_double;
			break;
		case variable_integer:
			a = adr1->value.value_number;
			break;
		case variable_boolean:
			a = adr1->value.value_boolean;
			break;
		default:
			throw_error(error_incopatible_types);
	}
	
	switch(adr2->type)
	{
		case variable_double:
			b = adr2->value.value_double;
			break;
		case variable_integer:
			b = adr2->value.value_number;
			break;
		case variable_boolean:
			b = adr2->value.value_boolean;
			break;
		default:
			throw_error(error_incopatible_types);
	}

	if( adr1->type == variable_string)
		x = strcmp(adr1->value.value_string,adr2->value.value_string);

	switch(type){
		case ins_equal:
			if( adr1->type == variable_string)
			{
				if(x)
					adr3->value.value_boolean = 0;
				else
					adr3->value.value_boolean = 1;
			}
			else
				adr3->value.value_boolean = (a == b);
			break;

		case ins_nequal:
			if( adr1->type == variable_string)
			{
				if(x)
					adr3->value.value_boolean = 1;
				else
					adr3->value.value_boolean = 0;
			}
			else
				adr3->value.value_boolean = (a != b);
			break;

		case ins_less:
			if( adr1->type == variable_string)
			{
				if(x < 0)
					adr3->value.value_boolean = 1;
				else
					adr3->value.value_boolean = 0;
			}
			else
				adr3->value.value_boolean = (a < b);
			break;

		case ins_lesseq:
			if( adr1->type == variable_string)
			{
				if(x < 0 || x == 0)
					adr3->value.value_boolean = 1;
				else
					adr3->value.value_boolean = 0;
			}
			else
				adr3->value.value_boolean = (a <= b);
			break;

		case ins_greater:
			if( adr1->type == variable_string)
			{
				if(x > 0)
					adr3->value.value_boolean = 1;
				else
					adr3->value.value_boolean = 0;
			}
			else
				adr3->value.value_boolean = (a > b);
			break;

		case ins_greateq:
			if( adr1->type == variable_string)
			{
				if(x > 0 || x == 0)
					adr3->value.value_boolean = 1;
				else
					adr3->value.value_boolean = 0;
			}
			else
				adr3->value.value_boolean = (a >= b);
			break;

		default:
			throw_error(error_unkown);
	}
	adr3->inicialized = 1;
}

void logic(char c, symbolVariable *adr1, symbolVariable *adr2, symbolVariable *adr3){
	adr3->type = variable_boolean;
	
	if(adr2 == NULL && c == 'n')
	{
		print_debug(debug_interpret,"negace adr_type: %d %d",adr1->type, variable_boolean);
		if(adr1->type != variable_boolean)
		{
			throw_error(error_incopatible_types);
		}
	}
	else
		if(adr1->type != variable_boolean || adr2->type != variable_boolean)
		{
			throw_error(error_incopatible_types);
		}
		
	switch(c)
	{
		case 'a':
			adr3->value.value_boolean = adr1->value.value_boolean && adr2->value.value_boolean;
			break;
		case 'o':
			adr3->value.value_boolean = adr1->value.value_boolean || adr2->value.value_boolean;
			break;
		case 'x':
			adr3->value.value_boolean = adr1->value.value_boolean ^ adr2->value.value_boolean;
			break;
		case 'n':
			if(adr1->value.value_boolean)
				adr3->value.value_boolean = 0;
			else
				adr3->value.value_boolean = 1;
			break;
	}
	adr3->inicialized = 1;

}

void interpret(){
	TNode *node = uStack_top(TList *,global.ins_list_stack)->first;
	TIns *ins, *func_call = NULL;
	
	while(node != NULL)
	{
		ins = node->data;
		print_debug(debug_interpret, "ins type: %d",ins->type);
		switch(ins->type)
		{
			case ins_assign:
				print_debug(debug_interpret, "assign where: %s what: %s", ((TString *)ins->adr1)->data, ((TString *)ins->adr3)->data);
				copy_variable(get_var(ins->adr1), get_var(ins->adr3));
				break;
			case ins_add:
				do_math('+', get_var(ins->adr1), get_var(ins->adr2), get_var(ins->adr3));
				break;
			case ins_subb:
				do_math('-', get_var(ins->adr1), get_var(ins->adr2), get_var(ins->adr3));
				break;
			case ins_mul:
				do_math('*', get_var(ins->adr1), get_var(ins->adr2), get_var(ins->adr3));
				break;
			case ins_div:
				do_math('/', get_var(ins->adr1), get_var(ins->adr2), get_var(ins->adr3));
				break;
			case ins_equal:
				compare(ins->type, get_var(ins->adr1), get_var(ins->adr2), get_var(ins->adr3));
				break;
			case ins_nequal:
				compare(ins->type, get_var(ins->adr1), get_var(ins->adr2), get_var(ins->adr3));
				break;
			case ins_less:
				compare(ins->type, get_var(ins->adr1), get_var(ins->adr2), get_var(ins->adr3));
				break;
			case ins_lesseq:
				compare(ins->type, get_var(ins->adr1), get_var(ins->adr2), get_var(ins->adr3));
				break;
			case ins_greater:
				compare(ins->type, get_var(ins->adr1), get_var(ins->adr2), get_var(ins->adr3));
				break;
			case ins_greateq:
				compare(ins->type, get_var(ins->adr1), get_var(ins->adr2), get_var(ins->adr3));
				break;
			case ins_and:
				logic('a', get_var(ins->adr1), get_var(ins->adr2), get_var(ins->adr3));
				break;
			case ins_or:
				logic('o', get_var(ins->adr1), get_var(ins->adr2), get_var(ins->adr3));
				break;
			case ins_xor:
				logic('x', get_var(ins->adr1), get_var(ins->adr2), get_var(ins->adr3));
				break;			
			case ins_not:
				logic('n', get_var(ins->adr1), NULL, get_var(ins->adr3));
				break;
			case ins_lab:
				break;
			case ins_jmp:
				if(ins->adr1 == NULL || get_var(ins->adr1)->value.value_boolean == 0)
				{
					node = ins->adr3;
					continue;
				}
				break;
			case ins_uminus:
				switch(get_var(ins->adr1)->type)
				{
					case variable_integer:
						get_var(ins->adr1)->value.value_number = 0 - get_var(ins->adr1)->value.value_number;
						break;
					case variable_double:
						get_var(ins->adr1)->value.value_double = 0 - get_var(ins->adr1)->value.value_double;
						break;
					case variable_boolean:
						if(get_var(ins->adr1)->value.value_boolean)
							get_var(ins->adr1)->value.value_boolean = 0;
						else
							get_var(ins->adr1)->value.value_boolean = 1;
						break;
					default:
						throw_error(error_incopatible_types);
						exit(4);
				}
				break;
			case ins_call:
				pushed = 0;
				func_call = ins;
				uStack_top(TList *,global.ins_list_stack)->act = node->n;
				uStack_push(TList *, global.ins_list_stack, ((symbolFunction*)ins->adr1)->ins);
				node = uStack_top(TList *,global.ins_list_stack)->first;
				continue;
			case ins_incall:
				switch(((long long)ins->adr1))
                {
                    case 0:
                	    pascal_write(get_var_in_stack(ins->adr2));
                	    break;
                	case 1:
                		pascal_readln(get_var(ins->adr2));
                		break;
                	case 2:
                		pascal_length(get_var(ins->adr2));
                		break;
                	case 3:
						//pascal_copy
                	case 4:
						//pascal_find
                	case 5:
						//pascal_sort
                		break;
                    default:
                    	print_debug(debug_interpret, "NOT yet\n");
                }
				break;
			case ins_push_htab:
				//print_debug(debug_interpret,"local_symbols count before: %d", uStack_count(global.local_symbols));
				pushed = 1;
				uStack_push(htab_t *, global.local_symbols, htab_copy(ins->adr1));
				print_debug(debug_interpret,"local_symbols count after: %d", uStack_count(global.local_symbols));
				break;
			
			default:
				print_debug(debug_interpret, "NOT YET\n");	
		}
		node = node->n;
		if(node == NULL && global.ins_list_stack->count > 1)
		{
			uStack_remove(global.ins_list_stack);
			node = uStack_top(TList *,global.ins_list_stack)->act;

			if(func_call != NULL)
			{
				htab_listitem *hitem = htab_lookup(uStack_top(htab_t*, global.local_symbols), ((symbolFunction*)func_call->adr1)->name->data);
				uStack_remove(global.local_symbols);
				if(hitem->type == type_variable)
				{
					copy_variable(get_var(func_call->adr3), hitem->ptr.variable);
				}
				else
					print_debug(debug_interpret, "********without return*******\n");
			}
		}
	}

}
