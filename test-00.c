#include <stdio.h>


#define CARD_IMPLEMENTATION 
#include "card.h"

#define DECK_IMPLEMENTATION 
#include "deck.h"

int *deck=NULL;
int ndeck=0;

int main() {



	Deck_New(&deck,&ndeck);

	for(int i=0;i<ndeck;i++) {
		char *str=Card_ToString(deck[i]);
		printf("%s %2d %d %2d\n",str,Card_GetRank(deck[i]),Card_GetSuit(deck[i]),Card_Value(str));
		free(str);
		str=NULL;
	} 

	return 0;
}
