/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14        */
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš,       */
/*                Michchal Jásenský,                                */
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00           */
/********************************************************************/

#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include "symbol.h"

void parser();
void parser_file();
void parser_vars();
void parser_var();
void parser_function();
void parser_args(symbolFunction* func);
void parser_body();
void parser_main();
void parser_code();
void parser_if();
void parser_while();
void parser_repeat();
void parser_for();
void parser_switch();

void check_semicolon();

#define HASH_TABLE_SIZE	16

#endif
