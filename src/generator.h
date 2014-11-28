/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14		*/
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš, 		*/
/*                Michchal Jásenský,								*/
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00 			*/
/********************************************************************/

#ifndef GEN_H
#define GEN_H

#include "list.h"
#include "expr.h"
#include "types.h"

/* 3 addres instructions */
typedef enum {
	ins_lab,		//0
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
	ins_greater,		//13
	ins_greateq,		//14
	ins_in,			//15
	ins_jmp,		//16
	ins_call,		//17
	ins_uminus,		//18
	ins_assign,		//19
	ins_incall,		//20
	ins_push_htab,		//21
	ins_xor			//22
} TInsType;

//structure for instruction
typedef struct TIns
{
	void *adr1;
	void *adr2;
	void *adr3;
	TInsType type;
}TIns;

//create instruction fox expresion
void gen_expr(TRule rule, void *adr1, void *adr2, void *adr3);
//create instuction for control
void gen_code(TInsType type, void *adr1, void *adr2, void *adr3);

#endif
