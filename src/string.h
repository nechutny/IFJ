/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14        */
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš,       */
/*                Michchal Jásenský,                                */
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00           */
/********************************************************************/

#ifndef STRING_H
#define STRING_H

typedef struct _string {
	int allocated;
	int length;
	char data[];
} TString;
/* naalokuje novej string */
TString * string_new();

/* přidá řetězec do stringu, vrací 1 pokud se povedl, nula pokud ne */
TString* string_add( TString * string, char * text );

int string_add_chr( struct _string * string, char c );
/* vymaže text */
void string_clear( struct _string * string );

/* přepíše string (asi zavolat string_clear a string_add */
TString* string_rewrite( struct _string * string, char * text ); 

/* zruší string */
void string_free( struct _string * string );

#endif
