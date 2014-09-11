#ifndef LIST_H
#define LIST_H


typedef struct _TNode {
	void * data;
	struct _TNode * n;	//next node
	struct _TNode * p;	//previous node
} TNode;

typedef struct _TList {
	TNode * first;
	TNode * last;
	TNode * act;
} TList;

TList* list_init();

void list_insert(TList *list, void *data);

void list_insert_next(TList *list, void *data);

void list_insert_prev(TList *list, void *data);

void list_next(TList *list);

void list_prev(TList *list);

#endif