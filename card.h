#ifndef CARD_H
#define CARD_H

#include <stdlib.h>
#include <ctype.h>



int Card_GetRank(int card);
int Card_GetSuit(int card);
char *Card_ToString(int card);
int Card_Value(char *str);
void Card_Print(int card);
void Cards_Shuffle(int **cards,int ncards);
void Cards_PushFront(int **cards,int *ncards,int card);
void Cards_PushBack(int **cards,int *ncards,int card);
int Cards_PopFront(int **cards,int *ncards);
int Cards_PopBack(int **cards,int *ncards);
int cmpSortRankSuit(const void *a,const void *b);
int cmpSortValue(const void *a,const void *b);
void Cards_AutoMeld(int **cards,size_t ncards);
void Cards_Free(int **cards,int *ncards);



#ifdef CARD_IMPLEMENTATION



int Card_GetRank(int card) {
	return card%13;
}



int Card_GetSuit(int card) {
	return card/13;
}



char *Card_ToString(int card) {
	char *rank="A23456789TJQK";
	char *suit="CSHD";
	char *res=calloc(3,sizeof(*res));

	char r=rank[Card_GetRank(card)];
	char s=suit[Card_GetSuit(card)];

	res[0]=r;
	res[1]=s;
	res[2]='\0';

	return res;
}



int Card_Value(char *str) {
	char *rank="A23456789TJQK";
	char *suit="CSHD";

	int r=-1,s=-1;

	for(int i=0;rank[i];i++) if(rank[i]==toupper(str[0])) { r=i; break; }
	
	for(int j=0;suit[j];j++) if(suit[j]==toupper(str[1])) { s=j; break; }

	if(r==-1 || s==-1) return -1;

	return s*13+r;
}



void Card_Print(int card) {
	char *str=Card_ToString(card);
	printf("%s",str);
	free(str);
	str=NULL;
}



void Cards_Print(int *cards,int ncards) {
	for(int i=0;i<ncards;i++) {
		if(i!=0) printf(" ");
		Card_Print(cards[i]);
	}
	printf("\n");
}



void Cards_Shuffle(int **cards,int ncards) {
	for(int i=ncards-1;i>0;i--)	{
		int j=rand()%(i+1);
		int tmp=(*cards)[i];
		(*cards)[i]=(*cards)[j];
		(*cards)[j]=tmp;
	}
}



void Cards_PushFront(int **cards,int *ncards,int card) {
	*cards=realloc(*cards,sizeof(**cards)*(*ncards+1));
	(*ncards)++;
	for(int i=*ncards-1;i>0;i--) {
		(*cards)[i]=(*cards)[i-1];		
	}
	(*cards)[0]=card;
}



void Cards_PushBack(int **cards,int *ncards,int card) {
	*cards=realloc(*cards,sizeof(**cards)*(*ncards+1));
	(*cards)[(*ncards)++]=card;
}



int Cards_PopFront(int **cards,int *ncards) {
	int res=-1;
	if(*ncards>0) {
		res=(*cards)[0];
		if(*ncards==1) {
			Cards_Free(cards,ncards);
		} else {
			for(int i=0;i<*ncards-1;i++) {
				(*cards)[i]=(*cards)[i+1];		
			}
			*cards=realloc(*cards,sizeof(**cards)*(*ncards-1));
			(*ncards)--;
		}
	}
	return res;
}



int Cards_PopBack(int **cards,int *ncards) {
	int res=-1;
	if(*ncards>0) {
		res=(*cards)[(*ncards)-1];
		if(*ncards==1) {
			Cards_Free(cards,ncards);
		} else {
			*cards=realloc(*cards,sizeof(**cards)*(*ncards-1));
			(*ncards)--;
		}
	}
	return res;
}



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



void Cards_AutoMeld(int **cards,size_t ncards) {

	qsort(*cards,ncards,sizeof(**cards),cmpSortRankSuit);

	size_t k=0,l=0,m=0;
	int r=Card_GetRank((*cards)[0]);
	while(k<ncards) {
		if(Card_GetRank((*cards)[k])==r) {
			k++; m++;
		} else if(m>=3) {
			l=k;
			m=0;
			r=Card_GetRank((*cards)[k]);
		} else break;
	}

	qsort((*cards)+l,ncards-l,sizeof(**cards),cmpSortValue);
}



void Cards_Free(int **cards,int *ncards) {
	free(*cards);
	cards=NULL;
	*ncards=0;
}



#endif /* CARD_IMPLEMENTATION */



#endif /* CARD_H */


