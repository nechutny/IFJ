#include <stdio.h>

#include "generator.h"
#include "garbage.h"
#include "list.h"

TIns *gen_ins(char *rule, TList *list, void *adr1, void *adr2, void *adr3)
{
	TIns *ins = _malloc(sizeof(TIns));
	ins->adr1 = adr1;
	ins->adr2 = adr2;
	ins->adr3 = adr3;

	switch(rule[0])
	{
		
		case '1':
			ins->type = ins_assign;
			break;		
		case '2':
			ins->type = ins_assign;
			break;
		case '3':
			ins->type = ins_not;
			break;
		case '4':
			ins->type = ins_mul;
			break;
//		case '5':
//			ins->type = ins_div;
//			break;
		case '6':
			ins->type = ins_div;
			break;
		case '7':
			ins->type = ins_mod;
			break;
		case '8':
			ins->type = ins_and;
			break;
		case '9':
			ins->type = ins_add;
			break;
		default:
			printf("not yet\n");
	}

	list_insert(list, ins);
	return ins;
}

