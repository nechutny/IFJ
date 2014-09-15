#include "types.h"
#include "garbage.h"

TVar *var_from_token(TToken *token)
{
//	printf("token type %d\n",token->type);
	TVar *var = _malloc(sizeof(TVar));
	switch(token->type)
	{
		case token_int:
			var->type = var_int;
			var->data.i = atoi(token->data->data);
			break;		
		case token_double:
			var->type = var_double;
			var->data.d = atof(token->data->data);
			break;
		default:
//			fprintf(stderr, "unknown token type: %d\n", token->type);
			_free(var);
			return NULL;
	}

	return var;
}

TVar *create_var(){
	TVar *var = _malloc(sizeof(TVar));
	var->type = var_ndef;
	return var;
}