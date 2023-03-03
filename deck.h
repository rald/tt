#ifndef DECK_H
#define DECK_H



#include <stdio.h>
#include <stdlib.h>



void Deck_New(int **deck,int *ndeck);

void Deck_Shuffle(int *deck,int ndeck);



#ifdef DECK_IMPLEMENTATION



void Deck_New(int **deck,int *ndeck) {
	*ndeck=52;
	*deck=malloc(sizeof(**deck)**ndeck);
	for(int i=0;i<*ndeck;i++) {
		(*deck)[i]=i;
	}
}

void Deck_Print(int *deck,int ndeck) {
	for(int i=0;i<ndeck;i++) {
		if(i!=0) printf(", ");
		char *str=Card_ToString(deck[i]);
		printf("%s",str);
		free(str);
		str=NULL;
	}
	printf("\n");
}

void Deck_Shuffle(int *deck,int ndeck) {
	for(int i=ndeck-1;i>0;i++)	{
		int j=rand()%(i+1);
		int tmp=deck[i];
		deck[i]=deck[j];
		deck[j]=tmp;
	}
}



#endif /* DECK_IMPLEMENTATION */



#endif /* DECK_H */



