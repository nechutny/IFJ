/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14        */
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš,       */
/*                Michchal Jásenský,                                */
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00           */
/********************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "types.h"
#include "string.h"
#include "main.h"
#include "garbage.h"

/* Size of alloc blocks */
#define ALLOC_SIZE	32


/**
 * Prepare new string structure
 *
 * @return	pointer to new string structure
 */
TString * string_new()
{
	TString * string;
	
	string = _malloc(sizeof(TString) + sizeof(char)*ALLOC_SIZE);
	if( string == NULL )
	{
		fprintf(stderr, "%s", strerror(errno));
		return NULL;
	}
	
	string->data[0] = '\0';
	string->allocated = ALLOC_SIZE;
	string->length = 0;
	
	return string;
}


/**
 * Add one character to string structire
 *
 * @param	string	Pointer to string structure as target
 * @param	c	character for add
 * @return	0 ... error, 1 ... success
 */
int string_add_chr( TString * string, char c )
{
	if( string->allocated < (string->length+1))
	{
		string->allocated += ALLOC_SIZE;
		string = _realloc(string, sizeof(TString) + (sizeof(char)*(string->allocated)));
		
		if(string == NULL)
		{
			fprintf(stderr, "%s", strerror(errno));
			return 0;
		}
	}

	string->data[ string->length ] = c;
	string->length++;
	string->data[ string->length ] = '\0'; 
	
	return 1;
}

/**
 * Add char to string
 *
 * @param	string	Pointer to string structure as target
 * @param	text	Character for add
 * @return	New pointer to structure
 */
TString* string_add( TString * string, char * text )
{

	unsigned int len = strlen(text);
	
	unsigned int total_len = len+(string->length)+1;
	
	if( total_len > string->allocated )
	{
		string->allocated = total_len;
		string = _realloc(string, sizeof(TString) + (sizeof(char)*(string->allocated)));
		if(string == NULL)
		{
			fprintf(stderr, "%s", strerror(errno));
			return 0;
		}
	}
	
		
	strncat(string->data, text, len);

	string->length += len;
	
	return string;
}	


/**
 * Clear content of string structure
 *
 * @param	string	pointer to string structure
 */
void string_clear( TString * string ) {
	string->data[0] = '\0';
	string->length = 0;
}


/**
 * Rewrite whole content of string to new value
 *
 * @param	string	pointer to string structure
 * @param	text	new content
 * @return	new pointer to structure
 */
TString* string_rewrite( TString * string, char * text ) {
	string_clear( string );
	return string_add( string, text );
}
	

/**
 * Unallocate string
 *
 * @param	string	pointer to string structure
 */
void string_free( TString * string ) {
	if( string == NULL )
		return;

	//_free( string->data );
	_free( string );
	
	//string = NULL;
}
