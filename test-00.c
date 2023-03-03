#include <stdio.h>
#include <stdlib.h>
#include <time.h>



#define CARD_IMPLEMENTATION 
#include "card.h"

#define DECK_IMPLEMENTATION 
#include "deck.h"



int *deck=NULL;
int ndeck=0;

int *hands[3];
int nhands[3];

int nplayers=3;



int main() {

	srand(time(NULL));



	Deck_New(&deck,&ndeck);

	Cards_Shuffle(&deck,ndeck);

	for(int j=0;j<nplayers;j++) {
		hands[j]=NULL;
		nhands[j]=0;
		for(int i=0;i<(j==0?13:12);i++) {
			int card=Cards_PopFront(&deck,&ndeck);
			if(card==-1) break;
			Cards_PushBack(&hands[j],&nhands[j],card);
		}
		Cards_Print(hands[j],nhands[j]);
	}

	

	for(int i=0;i<nplayers;i++) {
 		Cards_Free(&hands[i],&nhands[i]);
	}
	
	Cards_Free(&deck,&ndeck);



	return 0;
}


