/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14        */
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš,       */
/*                Michchal Jásenský,                                */
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00           */
/********************************************************************/

#include <string.h>
#include "symbol.h"
#include "garbage.h"
#include "string.h"
#include "types.h"
#include "list.h"
#include "error.h"
#include "ial.h"


/**
 * Remove all items from hash table
 *
 * @param	t	reference on hash table
 */
void htab_clear(htab_t *t)
{
	htab_listitem* item = NULL;

	for(unsigned i = 0; i < t->htab_size; i++)
	{ // Loop on list with indexes
		while((item = t->list[i]) != NULL)
		{ // loop on items
			htab_remove(t,item->key);
		}
	}
}


// Used for access to hash table in foreach
htab_t* target;


/**
 * Function for usage in htab_copy creating copy of all components of hash table item
 *
 * @param	key	Key of item for copy
 * @param	type	Type of item
 * @param	item	Pointer to hash table item for copy
 */
void htab_copy_foreach(char key[], item_type type, htab_listitem* item)
{
	htab_listitem* new = htab_create(target, key);
	unsigned int size = sizeof(symbolVariable)+sizeof(char)*(strlen(key)+1);

	// Copy symbolVariable
	symbolVariable* sym_v = memcpy(_malloc(size), item->ptr.variable, size);

	// Copy htable item
	memcpy(new, item, sizeof(htab_listitem)+sizeof(char)*(strlen(key)+1));
	new->next = NULL;

	// Set new pointer
	new->ptr.variable = sym_v;
}


/**
 * Copy hash table of local symbols
 *
 * @param	source	pointer to structure for copy
 * @return	pointer to new copy
 *
 */
htab_t *htab_copy(htab_t * source)
{
	target = htab_init(source->htab_size);
	htab_foreach(source, htab_copy_foreach);

	return target;
}


/**
 * Call function on each memeber of given hash table
 *
 * @param	t		reference on hash table
 * @param	function	pointer on function
 */
void htab_foreach(htab_t *t, void (*function)(char key[], item_type type, htab_listitem* item))
{
	htab_listitem* item = NULL;

	for(unsigned i = 0; i < t->htab_size; i++)
	{ // Loop on list with indexes
		item = t->list[i];
		while(item != NULL)
		{ // loop on items
			function(item->key, item->type, item);
			item = item->next;
		}
	}
}


/**
 * Destrohy hash table and free memory
 *
 * @param	t	reference on hash table
 */
void htab_free(htab_t *t)
{
	if(t != NULL)
	{
		htab_clear(t);
		_free(t);
	}
}


/**
 * Hash function used for table
 *
 * @param	str		string for calculating hash
 * @param	htab_size	size fo hash table
 * @return	calculated hash
 */
unsigned int hash_function(const char *str, unsigned htab_size)
{
	unsigned int h=0;
	const unsigned char *p;
	for(p=(const unsigned char*)str; *p!='\0'; p++)
	{
		h = 65599*h + *p;
	}
	return h % htab_size;
}


/**
 * Inicialize hash table and return pointer
 *
 * @param	size	size of hash table
 * @return	pointer on structure or NULL if error
 */
htab_t* htab_init(unsigned size)
{
	htab_t* result = _malloc(sizeof(htab_t)+size*sizeof(htab_listitem));
	if(result == NULL)
	{ // check malloc
		return NULL;
	}

	result->htab_size = size;

	for(unsigned i = 0; i < size; i++)
	{ // inicialize NULL values
		result->list[i] = NULL;
	}

	return result;
}


/**
 * Helpfull function for creating item in hash table
 *
 * @param	key	string used for key
 * @return	pointer on structure or NULL if error
 */
htab_listitem* htab_create(htab_t *t, const char *key)
{
	// Get index and pointer
	unsigned index = hash_function(key, t->htab_size);
	htab_listitem* item_ptr = NULL;
	htab_listitem* item = t->list[index];
	htab_listitem* nitem;

	if(item == NULL)
	{ // Pointer is null (item not found), so create new item

		// allocate memory for structure size + size of key (length + 1 byte for null-terminating)
		nitem = _malloc(sizeof(htab_listitem) + sizeof(char)*(strlen(key)+1));
		if(nitem == NULL)
		{ // Check malloc
			return NULL;
		}

		// Set values
		memcpy(nitem->key,key,strlen(key)+1);
		nitem->next = NULL;

		item = t->list[index] = nitem;
		if(item == NULL)
		{
			return NULL;
		}
	}
	else
	{ // Possible result
		while(item != NULL)
		{ // Loop on table and find if item exists
			if(strcmp(item->key,key) == 0)
			{ // Found
				item_ptr = item;
				break;
			}
			else
			{ // Next
				item_ptr = item;
				item = item->next;
			}

		}

		if(item_ptr != NULL && item != item_ptr)
		{ // Not found, so create
			// allocate memory for structure size + size of key (length + 1 byte for null-terminating)
			nitem = _malloc(sizeof(htab_listitem) + sizeof(char)*(strlen(key)+1));
			if(nitem == NULL)
			{ // Check malloc
				return NULL;
			}

			// Set values
			memcpy(nitem->key,key,strlen(key)+1);
			nitem->next = NULL;
			item = nitem;
			if(item == NULL)
			{
				return NULL;
			}
			item_ptr->next = item; // link in list
		}
	}

	return item;
}


/**
 * Find or create new item in hash table
 *
 * @param	t	reference on hash table
 * @param	key	string used as index for item
 * @return	pointer on structure or NULL if don't exist
 */
htab_listitem* htab_lookup(htab_t *t, const char *key)
{
	// Get index and pointer
	unsigned index = hash_function(key, t->htab_size);
	htab_listitem* item_ptr = NULL;
	htab_listitem* item = t->list[index];

	if(item == NULL)
	{ // Pointer is null (item not found)
		return NULL;
	}
	else
	{ // Possible result
		while(item != NULL)
		{ // Loop on table and find if item exists
			if(strcmp(item->key,key) == 0)
			{ // Found
				item_ptr = item;
				break;
			}
			else
			{ // Next
				item_ptr = item;
				item = item->next;
			}
		}

		if(item_ptr != NULL && item != item_ptr)
		{ // Not found
			return NULL;
		}
	}

	return item;
}


/**
 * Remove item from hash table
 *
 * @param	t	reference on hash table
 * @param	key	string used as index for item
 */
void htab_remove(htab_t *t, const char *key)
{
	// index and pointers
	unsigned index = hash_function(key, t->htab_size);
	htab_listitem* item_ptr = t->list[index];
	htab_listitem* item = t->list[index];

	while(item != NULL)
	{ // lets search
		if(strcmp(item->key,key) == 0)
		{ // found
			if(item_ptr == item)
			{ // Found as first in list, so reset list
				t->list[index] = item->next;
			}
			item_ptr->next = item->next;

			_free(item);

			break;
		}
		else
		{ // Next..
			item_ptr = item;
			item = item->next;
		}
	}

}


/**
 * Print hash table statistics on stdout
 *
 * @param	t	reference on hash table
 */
void htab_statistics(htab_t *t)
{
	htab_listitem* item = NULL;
	unsigned min, max, sum, count;
	max = sum = 0;
	min = -1;

	for(unsigned i = 0; i < t->htab_size; i++)
	{ // Loop on items
		count = 0;
		item = t->list[i];
		while(item != NULL)
		{
			item = item->next;
			sum++;
			count++;
		}

		if(count < min || min == -1)
		{ // found new minimum
			min = count;
		}
		if(count > max)
		{ // found new maximum
			max = count;
		}

	}

	printf("\nHash table size:\t%d",t->htab_size);
	printf("\nTotal items:\t\t%d",sum);
	printf("\nMaximum items:\t%d",max);
	printf("\nMinimum items:\t%d",min);
	printf("\nAverage items:\t%f\n",((double)sum/(double)t->htab_size));
}


//vraci pozici nebo 0 pri nenalezeni a NULL pri chybe
symbolVariable * find(symbolVariable *text,symbolVariable *word)
{
	if (text->type != variable_string || word->type != variable_string)
		return NULL;

	symbolVariable * var;
	int text_length=strlen(text->value.value_string);
	int word_length=strlen(word->value.value_string);
	int fail[text_length - 1];
	int k, r, wind, tind;

	fail[0] = 0;
	for (k = 1; k <= word_length; k++)
	{
		r = fail[k - 1];
		while (r > 0 && word->value.value_string[r - 1] != word->value.value_string[k - 1])
			r = fail[r - 1];
		fail[k] = r + 1;
	}

	tind = 1;
	wind = 1;
	while (tind - 1 < text_length && wind - 1 < word_length)
	{
		if (wind == 0 || text->value.value_string[tind - 1] == word->value.value_string[wind - 1])
		{

			tind++;
			wind++;
		}
		else
		{
			wind = fail[wind - 1];
		}
	}
		if (wind > word_length)
		{
			var=symbol_variable_init2(variable_integer);
			var->value.value_number = tind - word_length;
			var->inicialized = 1;
			return var;
		}
		else
		{
			var=symbol_variable_init2(variable_integer);
			var->value.value_number = 0;
			var->inicialized = 1;
			return var;
		}
}

void merge(char VstupPole[], char PomPole[], int Leva, int Stred, int Prava){

   int i,j;

    for (i = Stred + 1; i > Leva; i--) {
    	PomPole[i-1] = VstupPole[i-1];
    }
    for (j = Stred; j < Prava; j++) {
        PomPole[Prava+Stred-j] = VstupPole[j+1];
    }
    for (int k = Leva; k <= Prava; k++) {
        if( PomPole[j] < PomPole[i] ){
        	VstupPole[k] = PomPole[j--];
        }
        else{
        	VstupPole[k] = PomPole[i++];
        }
    }
}

void merge_sort(char VstupPole[], char PomPole[], int Leva, int Prava){
    int Stred;
    if( Prava > Leva )
    {
        Stred = ( Leva + Prava ) / 2;
        merge_sort(VstupPole, PomPole, Leva, Stred);
        merge_sort(VstupPole, PomPole, Stred+1, Prava);

        merge(VstupPole, PomPole, Leva, Stred, Prava);
    }
}

//vraci 0 nebo NULL pri chybe
symbolVariable * sort(symbolVariable *text)
{
	if (text->type != variable_string)
		return NULL;

	int text_length=strlen	(text->value.value_string);
	int Leva = 0;
	int Prava = text_length-1;
	char PomPole[text_length];
	symbolVariable * text2=symbol_variable_init2(variable_string);
	copy_variable(text2, text);

	merge_sort(text2->value.value_string, PomPole, Leva, Prava);
	text2->inicialized = 1;
	return text2;
}

/**
 * Copy one variable to another
 *
 * @param	*text	Pointer to source varibale
 * @param	*start	Integer, offset of start
 * @param	*end	Length of copied
 *
 * @return	Pointer to new symbolVaribale containing copied value
 */

symbolVariable * copy(symbolVariable *text,symbolVariable *start,symbolVariable *end)
{
	if (	text->type != variable_string		||
		start->value.value_number > 255		||
		start->value.value_number < 0		||
		end->value.value_number > 255		||
		end->value.value_number < 0		||
		text == NULL				||
		start == NULL				||
		end == NULL				)
	{
		return NULL;
	}

	symbolVariable * copied = symbol_variable_init2(variable_string);
	copied->inicialized = 1;

	strncpy(&(copied->value.value_string), &(text->value.value_string[ start->value.value_number-1 ]), end->value.value_number);

	copied->value.value_string[end->value.value_number] = '\0';
	return copied;
}
