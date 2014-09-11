#ifndef GEN_H
#define GEN_H

#include "list.h"
#include "expr.h"

/* 3 addres instructions */
typedef enum {
	ins_assign,		//0
	ins_not,		//1
	ins_mul,		//2
	ins_div,		//3
	ins_mod,		//4
	ins_and,		//5
	ins_add,		//6
	ins_subb,		//7
	ins_or,			//8
	ins_equal,		//9
	ins_nequal,		//10
	ins_less,		//11
	ins_lesseq,		//12
	ins_greater,	//13
	ins_greateq,	//14
	ins_in,			//15
	ins_nop,		//16
	ins_jmp,		//17
	ins_call		//18
} TInsType;

typedef struct TIns
{
	TInsType type;
	void *adr1;
	void *adr2;
	void *adr3;
}TIns;


TIns *gen_ins(TRule rule, TList *list, void *adr1, void *adr2, void *adr3);

#endif