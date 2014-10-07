#include <string.h>
#include "symbol.h"
#include "string.h"
#include "types.h"
#include "list.h"
#include "htable.h"
#include "error.h"

symbolVariable * find(symbolVariable *text,symbolVariable *word);
void merge(char VstupPole[], char PomPole[], int Leva, int Stred, int Prava);
void merge_sort(char VstupPole[], char PomPole[], int Leva, int Prava);
symbolVariable * sort(symbolVariable *text);
symbolVariable * copy(symbolVariable *text,symbolVariable *start,symbolVariable *end);