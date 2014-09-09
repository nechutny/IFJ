#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "types.h"
#include "string.h"
#include "main.h"
#include "garbage.h"

/* naalokuje novej string */
TString * string_new() {
	TString * string;
	
	string = ( TString * ) _malloc( sizeof( TString ) );
	if( string == NULL ) {
		fprintf( stderr, "%s", strerror( errno ) );
		return NULL;
	}
	
	string->data = (char *) _malloc( sizeof( char ) * 50 );
	if( string->data == NULL ) {
		fprintf( stderr, "%s", strerror( errno ) );
		_free( string );
		return NULL;
	}
	
//	memcpy( string, 0, sizeof( char ) * 10 );
	string->data[0] = '\0';
	string->allocated = 50;
	string->length = 0;
	
	return string;
}

int string_add_chr( TString * string, char c ) {
	if( string->allocated < ( string->length + 1 ) ) {
		string = ( TString * ) _realloc( string, ( sizeof( char ) * ( string->allocated + 10 ) ) );
		if( string == NULL ) {
			fprintf( stderr, "%s", strerror( errno ) );
			return 0;
		}		
		string->allocated += 10;
	}

	string->data[ string->length ] = c;
	string->data[ string->length +1 ] = '\0'; 
	
	string->length++;
	return 1;
}

/* přidá řetězec do stringu, vrací 1 pokud se povedl, nula pokud ne */
int string_add( TString * string, char * text ) {
	int len = strlen( text );
	int total_len = len + string->length;
	
	if( ( total_len + 1 ) >= string->allocated ) {
		string->data = ( char * ) _realloc( string->data, ( sizeof( char ) * ( string->allocated + ( ( total_len+1 ) / string->allocated ) * 10 ) ) );
		if( string == NULL ) {
			fprintf( stderr, "%s", strerror( errno ) );
			return 0;
		}
		string->allocated = string->allocated + ( ( total_len+1) / string->allocated ) * 10;	
	}
	string->length = string->length + len;
	string->allocated = string->allocated + (total_len / string->allocated ) * 10;
		
	strcat( string->data, text );
	
	return 1;
}	
	

/* vymaže text */
void string_clear( TString * string ) {
	string->data[0] = '\0';
	string->length = 0;
}

/* přepíše string (asi zavolat string_clear a string_add */
int string_rewrite( TString * string, char * text ) {
	string_clear( string );
	return string_add( string, text );
}
	
/* zruší string */
void string_free( TString * string ) {
	if( string == NULL )
		return;

	_free( string->data );
	_free( string );
	
	string = NULL;
}
