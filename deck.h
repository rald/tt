#ifndef DECK_H
#define DECK_H



#include <stdio.h>
#include <stdlib.h>



void Deck_New(int **deck,int *ndeck);
void Deck_Shuffle(int **deck,int ndeck);
void Deck_PushFront(int **deck,int *ndeck,int card);
void Deck_PushBack(int **deck,int *ndeck,int card);
int Deck_PopFront(int **deck,int *ndeck);
int Deck_PopBack(int **deck,int *ndeck);
void Deck_Free(int **deck,int *ndeck);



#ifdef DECK_IMPLEMENTATION



void Deck_New(int **deck,int *ndeck) {
	*ndeck=52;
	*deck=malloc(sizeof(**deck)*(*ndeck));
	for(int i=0;i<*ndeck;i++) {
		(*deck)[i]=i;
	}
}






#endif /* DECK_IMPLEMENTATION */



#endif /* DECK_H */


