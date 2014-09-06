#ifndef STRING_H
#define STRING_H

#include "string.h"

typedef struct _string {
	char * data;
	int allocated;
	int length;
} TString;
/* naalokuje novej string */
TString * string_new();

/* přidá řetězec do stringu, vrací 1 pokud se povedl, nula pokud ne */
int string_add( struct _string * string, char * text );

int string_add_chr( struct _string * string, char c );
/* vymaže text */
void string_clear( struct _string * string );

/* přepíše string (asi zavolat string_clear a string_add */
int string_rewrite( struct _string * string, char * text ); 

/* zruší string */
void string_free( struct _string * string );

#endif