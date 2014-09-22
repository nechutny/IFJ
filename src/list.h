#ifndef LIST_H
#define LIST_H


typedef struct _TNode {
	void * data;
	struct _TNode * n;	//next node
} TNode;

typedef struct _TList {
	TNode * first;
	TNode * last;
	TNode * act;
} TList;

TList* list_init();

void list_insert_node(TList *list, TNode *node);

void list_insert(TList *list, void *data);

#endif