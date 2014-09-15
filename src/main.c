#include <stdio.h>
#include "lex.h"
#include "types.h"
#include "expr.h"
#include "garbage.h"
#include "parser.h"
#include "generator.h"
#include "interpret.h"

int main(int argc, char* argv[])
{
	global_init();
	
	global.file=fopen(argv[1],"r");
	if (global.file==NULL)
		return 0;
		
	parser();


	interpret();

	printf("vysledek %d\n", ((TVar *)((TIns *)global.ins_list->last->data)->adr3)->data.i);

	/*precedence(global.file);
	while(global.ins_list->act != NULL){
		printf("ins: %d list first: %d list act: %d list last: %d\n", ((TIns*)global.ins_list->act->data)->type, 
		global.ins_list->first, global.ins_list->act, global.ins_list->last);
		list_next(global.ins_list);
	}
	printf("test end\n");
	token = token_get(global.file);
	while (token->type!=token_eof)
	{
		//printf("%s ",token->data->data);
		//printf("%d\n",token->type);
		token_free(token);
		token = token_get(global.file);
	}*/
	return 0;
}
