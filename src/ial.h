/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14        */
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš,       */
/*                Michchal Jásenský,                                */
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00           */
/********************************************************************/

#include <string.h>
#include "symbol.h"
#include "string.h"
#include "types.h"
#include "list.h"
#include "error.h"

#ifndef _IAL_H_
#define _IAL_H_



typedef enum {
	type_function,
	type_variable
} item_type;

// List item
typedef struct  htab_listitem_s {
  struct htab_listitem_s *next;
  union {
	struct TsymbolVariable *variable;
	struct TsymbolFunction *function;
  } ptr;
  item_type type;
  char key[];
} htab_listitem;

// Hash table
typedef struct  htab {
  unsigned htab_size;
  struct htab_listitem_s *list[];
} htab_t;


// Hashing function
unsigned hash_function(const char *str, unsigned htab_size);

// Create and inicialize hash table
htab_t *htab_init(unsigned size);

htab_t *htab_copy(htab_t * source);

// Searching in hash table
htab_listitem *htab_lookup(htab_t *t, const char *key);

// Create new item in hash table and return pointer
htab_listitem* htab_create(htab_t *t, const char *key);

// Call function on each item
void htab_foreach(htab_t *t, void (*function)(char key[], item_type type, htab_listitem* item));

// Find and destroy (item)!
void htab_remove(htab_t *t, const char *key);

// Remove all items from hash table
void htab_clear(htab_t *t);

// Destroy whole hash table
void htab_free(htab_t *t);

//  Print statistics
void htab_statistics(htab_t *t);


symbolVariable * find(symbolVariable *text,symbolVariable *word);
void merge(char VstupPole[], char PomPole[], int Leva, int Stred, int Prava);
void merge_sort(char VstupPole[], char PomPole[], int Leva, int Prava);
symbolVariable * sort(symbolVariable *text);
symbolVariable * copy(symbolVariable *text,symbolVariable *start,symbolVariable *end);



#endif
