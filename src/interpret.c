#include <stdio.h>

#include "interpret.h"
#include "garbage.h"
#include "generator.h"
#include "list.h"
#include "symbol.h"


void interpret(){
	double a,b;

	while(global.ins_list->act != NULL)
	{
		if(	((symbolVariable *)((TIns *)global.ins_list->act->data)->adr1)->type == variable_double || 
			((symbolVariable *)((TIns *)global.ins_list->act->data)->adr2)->type == variable_double)
			
			((symbolVariable *)((TIns *)global.ins_list->act->data)->adr3)->type = variable_double;
	
		else
			((symbolVariable *)((TIns *)global.ins_list->act->data)->adr3)->type = variable_integer;

		if(	((symbolVariable *)((TIns *)global.ins_list->act->data)->adr1)->type == variable_double)
			a = ((symbolVariable *)((TIns *)global.ins_list->act->data)->adr1)->value.value_double;

		else if(((symbolVariable *)((TIns *)global.ins_list->act->data)->adr1)->type == variable_integer)
			a = ((symbolVariable *)((TIns *)global.ins_list->act->data)->adr1)->value.value_number;
		else
			fprintf(stderr,"Semantic error incopatibile types in expresion\n");
		
		if(((symbolVariable *)((TIns *)global.ins_list->act->data)->adr2)->type == variable_double)
			b = ((symbolVariable *)((TIns *)global.ins_list->act->data)->adr2)->value.value_double;	
		else if( ((symbolVariable *)((TIns *)global.ins_list->act->data)->adr2)->type == variable_integer)
			b = ((symbolVariable *)((TIns *)global.ins_list->act->data)->adr2)->value.value_number;
		else 
			fprintf(stderr,"Semantic error incopatibile types in expresion\n");

		printf("a: %f, b: %f\n", a, b);

		switch(((TIns *)global.ins_list->act->data)->type)
		{
			case ins_add:
				if(((symbolVariable *)((TIns *)global.ins_list->act->data)->adr3)->type == variable_double)
					((symbolVariable *)((TIns *)global.ins_list->act->data)->adr3)->value.value_double = a+b;
				else
					((symbolVariable *)((TIns *)global.ins_list->act->data)->adr3)->value.value_number = a+b;
				break;
			case ins_subb:
				if(((symbolVariable *)((TIns *)global.ins_list->act->data)->adr3)->type == variable_double)
					((symbolVariable *)((TIns *)global.ins_list->act->data)->adr3)->value.value_double = a-b;
				else
					((symbolVariable *)((TIns *)global.ins_list->act->data)->adr3)->value.value_number = a-b;
				break;

			case ins_mul:
				if(((symbolVariable *)((TIns *)global.ins_list->act->data)->adr3)->type == variable_double)
					((symbolVariable *)((TIns *)global.ins_list->act->data)->adr3)->value.value_double = a*b;
				else
				((symbolVariable *)((TIns *)global.ins_list->act->data)->adr3)->value.value_number = a*b;
				break;

			case ins_div:
				if(b == 0){
				fprintf(stderr, "Semantic error dividing by 0\n");
					exit(10);
				}
				((symbolVariable *)((TIns *)global.ins_list->act->data)->adr3)->type = variable_double;
				((symbolVariable *)((TIns *)global.ins_list->act->data)->adr3)->value.value_double = a/b;
				break;
			default:
				printf("not yet\n");
		}
		printf("mezivysledek %ld\n", ((symbolVariable *)((TIns *)global.ins_list->act->data)->adr3)->value.value_number);
		list_next(global.ins_list);
	}
}
