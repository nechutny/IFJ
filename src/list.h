/********************************************************************/
/*Projekt:Implementace interpretu imperativního jazyka IFJ14        */
/*Jména řešitelů: Stanislav Nechutný, Jiří Kulda, Petr Lukeš,       */
/*                Michchal Jásenský,                                */
/*Loginy řešitelů: xnechu01, xkulda00, xlukes06, xjasen00           */
/********************************************************************/

#ifndef LIST_H
#define LIST_H

//structure for list element
typedef struct _TNode {
	void * data;
	struct _TNode * n;	//next node
} TNode;

//structure for list
typedef struct _TList {
	TNode * first;
	TNode * last;
	TNode * act;
} TList;

//create new list
TList* list_init();

//insert element into list
void list_insert_node(TList *list, TNode *node);

//inser new element into list
void list_insert(TList *list, void *data);

#endif