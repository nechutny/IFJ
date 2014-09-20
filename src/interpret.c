#include <stdio.h>

#include "interpret.h"
#include "garbage.h"
#include "generator.h"
#include "list.h"
#include "symbol.h"

void do_math(char c, symbolVariable *adr1, symbolVariable *adr2, symbolVariable *adr3){
	double a = 0, b = 0;

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
	}
	adr3->inicialized = 1;
}

void compare(TInsType type, symbolVariable *adr1, symbolVariable *adr2, symbolVariable *adr3){
	double a = 0, b = 0;

	adr3->type = variable_boolean;

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

	switch(type){
		case ins_equal:
			adr3->value.value_boolean = (a == b);
			break;
		case ins_nequal:
			adr3->value.value_boolean = (a != b);
			break;
		case ins_less:
			adr3->value.value_boolean = (a < b);
			break;
		case ins_lesseq:
			adr3->value.value_boolean = (a <= b);
			break;
		case ins_greater:
			adr3->value.value_boolean = (a > b);
			break;
		case ins_greateq:
			adr3->value.value_boolean = (a >= b);
			break;
		default:
			fprintf(stderr, "internal error\n");
	}
	adr3->inicialized = 1;
}

void interpret(){
	int i = 0;
	TIns *ins;
	while(i != global.ins_list->count)
	{
		ins = uStack_offset(TIns *, global.ins_list, i);
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
			default:
				printf("NOT YET\n");	
		}
		i++;
	}

}
