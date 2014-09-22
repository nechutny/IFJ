#include <stdio.h>
#include <math.h>

#include "interpret.h"
#include "garbage.h"
#include "generator.h"
#include "list.h"
#include "symbol.h"

void do_math(char c, symbolVariable *adr1, symbolVariable *adr2, symbolVariable *adr3){
	double a = 0, b = 0;

	if(adr1->type == variable_string && adr2->type == variable_string && c == '+')
	{
		adr3->type = variable_string;
		strcpy(adr3->value.value_string, adr2->value.value_string);
		strcat(adr3->value.value_string, adr2->value.value_string);
		return;
	}
	else if((adr1->type == variable_string || adr2->type == variable_string) && c == '+'){
		fprintf(stderr, "one variable is not string\n" );
		exit(4);
	}
	else if(adr1->type == variable_string || adr2->type == variable_string)
	{
		fprintf(stderr, "string\n");
		exit(4);
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
		fprintf(stderr," adr1 Semantic error incopatibile types in expresion\n");
		exit(12);
	}
	if(adr2->type == variable_double)
		b = adr2->value.value_double;	
	else if( adr2->type == variable_integer)
		b = adr2->value.value_number;
	else 
	{
		fprintf(stderr,"adr2 Semantic error incopatibile types in expresion\n");
		exit(12);
	}
	switch(c){
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
				fprintf(stderr, "Semantic error dividing by 0\n");
				exit(10);
			}
			adr3->type = variable_double;
			adr3->value.value_double = a/b;
			break;
	/*	case 'm':
			if(adr3->type == variable_double)
				adr3->value.value_double = fmod(a, b);
			else
				adr3->value.value_number = fmod(a, b);
			break;*/
	}
	adr3->inicialized = 1;
}

void compare(TInsType type, symbolVariable *adr1, symbolVariable *adr2, symbolVariable *adr3){
	double a = 0, b = 0;
	int x = 0;

	adr3->type = variable_boolean;

	if(adr1->type != adr2->type){
		fprintf(stderr, "incopatibile type\n" );
		exit(4);
	}

	switch(adr1->type){
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
			fprintf(stderr," adr1 Semantic error incopatibile types in expresion\n");
			exit(12);
	}
	
	switch(adr2->type){
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
			fprintf(stderr," adr2 Semantic error incopatibile types in expresion\n");
			exit(12);
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
			fprintf(stderr, "internal error\n");
	}
	adr3->inicialized = 1;
}

void logic(char c, symbolVariable *adr1, symbolVariable *adr2, symbolVariable *adr3){
	adr3->type = variable_boolean;
	if(adr1->type != variable_boolean || adr2->type != variable_boolean){
		fprintf(stderr, "one variable is not bool\n");
		exit(12);
	}
		
	if(c == 'a')
		adr3->value.value_boolean = adr1->value.value_boolean && adr2->value.value_boolean;
	else if(c == 'o')
		adr3->value.value_boolean = adr1->value.value_boolean || adr2->value.value_boolean;

	adr3->inicialized = 1;

}

void interpret(){
	TNode *node = uStack_top(TList *,global.ins_list_stack)->first;
	TIns *ins;
	while(node != NULL)
	{
		ins = node->data;
		switch(ins->type)
		{
			case ins_add:
				do_math('+', ins->adr1, ins->adr2, ins->adr3);
				break;
			case ins_subb:
				do_math('-', ins->adr1, ins->adr2, ins->adr3);
				break;
			case ins_mul:
				do_math('*', ins->adr1, ins->adr2, ins->adr3);
				break;
			case ins_div:
				do_math('/', ins->adr1, ins->adr2, ins->adr3);
				break;
			case ins_equal:
				compare(ins->type, ins->adr1, ins->adr2, ins->adr3);
				break;
			case ins_nequal:
				compare(ins->type, ins->adr1, ins->adr2, ins->adr3);
				break;
			case ins_less:
				compare(ins->type, ins->adr1, ins->adr2, ins->adr3);
				break;
			case ins_lesseq:
				compare(ins->type, ins->adr1, ins->adr2, ins->adr3);
				break;
			case ins_greater:
				compare(ins->type, ins->adr1, ins->adr2, ins->adr3);
				break;
			case ins_greateq:
				compare(ins->type, ins->adr1, ins->adr2, ins->adr3);
				break;
			case ins_and:
				logic('a', ins->adr1, ins->adr2, ins->adr3);
				break;
			case ins_or:
				logic('o', ins->adr1, ins->adr2, ins->adr3);
				break;
			case ins_lab:
				break;
			case ins_jmp:
				if(ins->adr1 == NULL || ((symbolVariable *)ins->adr1)->value.value_boolean == 0)
				{
					node = ins->adr3;
					continue;
				}
				break;
			case ins_uminus:
				switch(((symbolVariable *)ins->adr1)->type)
				{
					case variable_integer:
						((symbolVariable *)ins->adr1)->value.value_number = 0 - ((symbolVariable *)ins->adr1)->value.value_number;
						break;
					case variable_double:
						((symbolVariable *)ins->adr1)->value.value_double = 0 - ((symbolVariable *)ins->adr1)->value.value_double;
						break;
					case variable_boolean:
						if(((symbolVariable *)ins->adr1)->value.value_boolean)
							((symbolVariable *)ins->adr1)->value.value_boolean = 0;
						else
							((symbolVariable *)ins->adr1)->value.value_boolean = 1;
						break;
					default:
						fprintf(stderr, "unar minus\n");
						exit(4);
				}
			case ins_call:
				uStack_top(TList *,global.ins_list_stack)->act = node->n;
				uStack_push(TList *, global.ins_list_stack, ((symbolFunction*)ins->adr1)->ins);
				node = uStack_top(TList *,global.ins_list_stack)->first;
				break;
			default:
				printf("NOT YET\n");	
		}
		node = node->n;
		if(node == NULL && global.ins_list_stack->count > 1)
		{
			uStack_remove(global.ins_list_stack);
			node = uStack_top(TList *,global.ins_list_stack)->act;
		}
	}

}
