/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14        */
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš,       */
/*                Michchal Jásenský,                                */
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00           */
/********************************************************************/

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <stdarg.h>


#define DEBUG_MESSAGES             		0
#define DEBUG_MESSAGE_PARSER            0
#define DEBUG_MESSAGE_PREC          	0
#define DEBUG_MESSAGE_SYMBOL            0
#define DEBUG_MESSAGE_INTERPRET         0
#define DEBUG_MESSAGE_GENERATOR         0

typedef enum {
    debug_parser,
    debug_prec,
    debug_symbol,
    debug_interpret,
    debug_generator,
} debug_level;


#if DEBUG_MESSAGES == 1
    void print_debug(debug_level lvl, const char *fmt, ...);
#endif

#if DEBUG_MESSAGES == 0
    #define print_debug(...) /**/
#endif


#endif
