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

int cmpSortRankSuit(const void *a,const void *b) {
	int l=*(int*)a;
	int r=*(int*)b;
	if(Card_GetRank(l)<Card_GetRank(r)) return -1;
	if(Card_GetRank(l)>Card_GetRank(r)) return 1;
	if(Card_GetSuit(l)<Card_GetSuit(r)) return -1;
	if(Card_GetSuit(l)>Card_GetSuit(r)) return 1;
	return 0;
}

int cmpSortValue(const void *a,const void *b) {
	int l=*(int*)a;
	int r=*(int*)b;
	if(l<r) return -1;
	if(l>r) return 1;
	return 0;
}

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

		qsort(hands[j],nhands[j],sizeof(*hands[j]),cmpSortRankSuit);

		int k=0,l=0;
		int r=Card_GetRank(hands[j][0]);
		while(k<nhands[j]) {
			if(Card_GetRank(hands[j][k])!=r && k>=3) {
				l=k;
				r=Card_GetRank(hands[j][k]);
			} else break;
			k++;
		}

		qsort(hands[j]+l,nhands[j]-l,sizeof(*hands[j]),cmpSortValue);
		
		Cards_Print(hands[j],nhands[j]);
	}

	for(int i=0;i<nplayers;i++) {
 		Cards_Free(&hands[i],&nhands[i]);
	}
	
	Cards_Free(&deck,&ndeck);

	return 0;
}


