#include <stdio.h>

#include "interpret.h"
#include "garbage.h"
#include "generator.h"
#include "list.h"


void interpret(){
	double a,b;

	while(global.ins_list->act != NULL)
	{
		if(	((TVar *)((TIns *)global.ins_list->act->data)->adr1)->type == var_double || 
			((TVar *)((TIns *)global.ins_list->act->data)->adr2)->type == var_double)
			
			((TVar *)((TIns *)global.ins_list->act->data)->adr3)->type = var_double;
	
		else
			((TVar *)((TIns *)global.ins_list->act->data)->adr3)->type = var_int;

		if(	((TVar *)((TIns *)global.ins_list->act->data)->adr1)->type == var_double)
			a = ((TVar *)((TIns *)global.ins_list->act->data)->adr1)->data.d;

		else if(((TVar *)((TIns *)global.ins_list->act->data)->adr1)->type == var_int)
			a = ((TVar *)((TIns *)global.ins_list->act->data)->adr1)->data.i;
		else
			fprintf(stderr,"Semantic error incopatibile types in expresion\n");
		
		if(((TVar *)((TIns *)global.ins_list->act->data)->adr2)->type == var_double)
			b = ((TVar *)((TIns *)global.ins_list->act->data)->adr2)->data.d;	
		else if( ((TVar *)((TIns *)global.ins_list->act->data)->adr2)->type == var_int)
			b = ((TVar *)((TIns *)global.ins_list->act->data)->adr2)->data.i;
		else 
			fprintf(stderr,"Semantic error incopatibile types in expresion\n");

		printf("a: %f, b: %f\n", a, b);

		switch(((TIns *)global.ins_list->act->data)->type)
		{
			case ins_add:
				if(((TVar *)((TIns *)global.ins_list->act->data)->adr3)->type == var_double)
					((TVar *)((TIns *)global.ins_list->act->data)->adr3)->data.d = a+b;
				else
					((TVar *)((TIns *)global.ins_list->act->data)->adr3)->data.i = a+b;
				break;
			case ins_subb:
				if(((TVar *)((TIns *)global.ins_list->act->data)->adr3)->type == var_double)
					((TVar *)((TIns *)global.ins_list->act->data)->adr3)->data.d = a-b;
				else
					((TVar *)((TIns *)global.ins_list->act->data)->adr3)->data.i = a-b;
				break;

			case ins_mul:
				if(((TVar *)((TIns *)global.ins_list->act->data)->adr3)->type == var_double)
					((TVar *)((TIns *)global.ins_list->act->data)->adr3)->data.d = a*b;
				else
				((TVar *)((TIns *)global.ins_list->act->data)->adr3)->data.i = a*b;
				break;

			case ins_div:
				if(b == 0){
				fprintf(stderr, "Semantic error dividing by 0\n");
					exit(10);
				}
				((TVar *)((TIns *)global.ins_list->act->data)->adr3)->type = var_double;
				((TVar *)((TIns *)global.ins_list->act->data)->adr3)->data.d = a/b;
				break;
			default:
				printf("not yet\n");
		}
		printf("mezivysledek %d\n", ((TVar *)((TIns *)global.ins_list->act->data)->adr3)->data.i);
		list_next(global.ins_list);
	}
}