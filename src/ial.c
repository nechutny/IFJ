#include <string.h>
#include "symbol.h"
#include "garbage.h"
#include "string.h"
#include "types.h"
#include "list.h"
#include "htable.h"
#include "error.h"


//vraci pozici nebo 0 pri nenalezeni a -1 pri chybe
int find(symbolVariable *text,symbolVariable *word)
{
	if (text->type != variable_string || word->type != variable_string)
		return -1;

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
			return tind - word_length;
		else
			return 0;
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

//vraci 0 nebo -1 pri chybe
int sort(symbolVariable *text)
{	
	if (text->type != variable_string)
		return -1;

	int text_length=strlen	(text->value.value_string);
	int Leva = 0;
	int Prava = text_length-1;
	char PomPole[text_length];
	
	merge_sort(text->value.value_string, PomPole, Leva, Prava);
	return 0;
}

//vrati prekopirovany symbolVariable nebo pri chybe null
symbolVariable * copy(symbolVariable *text,int start, int end)
{
	if (text->type!=variable_string || start>end || start>255 || end>255)
		return NULL;
	symbolVariable * copied=_malloc(sizeof(symbolVariable));
	copied->type=text->type;
	for (int i=start; i<=end+1; i++)
	{
		copied->value.value_string[i-1]=text->value.value_string[i-1];
	}
	if (text->value.value_string[end+1]!='\0')
		copied->value.value_string[end+1]='\0';
	return copied;
}