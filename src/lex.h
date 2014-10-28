/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14        */
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš,       */
/*                Michchal Jásenský,                                */
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00           */
/********************************************************************/

#ifndef LEX_H
#define LEX_H

#include <stdio.h>

#include "string.h"
#include "types.h"
#include "main.h"


/* stavy konečnýho automatu */
typedef enum {
	state_init,
	state_colon,
	state_identifier,
	//state_slash,
	state_apostrophe,
	//state_line_comment,
	state_multiline_comment,
	state_dot,
	//state_star,
	//state_parenthesis_left,
	state_string,
	state_double,
	state__double_e,
	state_double_e,
	state__double_sign_e,
	state__double_dot,
	state_int,
	state_greater,
	state_less,
	state_invalid,
	state_grille
} TState;

TToken * token_init();
void set_identifier( TToken * token);
void token_free( TToken * token );
void token_return_token(TToken * token);
TToken *token_get();
TToken * token_data_add( TToken * token, char * text );
TToken *token_last;

#endif
