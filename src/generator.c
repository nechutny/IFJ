#include <stdio.h>

#include "generator.h"
#include "garbage.h"
#include "list.h"
#include "expr.h"

void gen_expr(TRule rule, void *adr1, void *adr2, void *adr3)
{
	TIns *ins = _malloc(sizeof(TIns));
		
	ins->adr1 = adr1;
	ins->adr2 = adr2;
	ins->adr3 = adr3;

	switch(rule)
	{	
		case rule_3:
			ins->type = ins_not;
			break;
		case rule_4:
			ins->type = ins_mul;
			break;
		case rule_5:
			ins->type = ins_div;
			break;
//		case rule_6:
//			ins->type = ins_div;
//			break;
		case rule_7:
			ins->type = ins_mod;
			break;
		case rule_8:
			ins->type = ins_and;
			break;
		case rule_9:
			ins->type = ins_add;
			break;
		case rule_10:
			ins->type = ins_subb;
			break;		
		case rule_11:
			ins->type = ins_or;
			break;
		case rule_12:
			ins->type = ins_equal;
			break;
		case rule_13:
			ins->type = ins_nequal;
			break;
		case rule_14:
			ins->type = ins_less;
			break;
		case rule_15:
			ins->type = ins_lesseq;
			break;
		case rule_16:
			ins->type = ins_greater;
			break;
		case rule_17:
			ins->type = ins_greateq;
			break;
		case rule_18:
			ins->type = ins_in;
			break;
		default:
			printf("not yet\n");
	}
	uStack_push(TIns *, global.ins_list, ins);
	return;
}

void gen_code(TInsType type, void *adr1, void *adr2, void *adr3){
	TIns *ins = _malloc(sizeof(TIns));
	ins->type = type;
	ins->adr1 = adr1;
	ins->adr2 = adr2;	
	ins->adr3 = adr3;
	
	uStack_push(TIns *, global.ins_list, ins);
}