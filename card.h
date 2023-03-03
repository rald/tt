#ifndef CARD_H
#define CARD_H

#include <stdlib.h>

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

char Card_Value(char *str) {
	char *rank="A23456789TJQK";
	char *suit="CSHD";

	int r=-1,s=-1;

	for(int i=0;rank[i];i++) if(rank[i]==str[0]) { r=i; break; }
	
	for(int j=0;suit[j];j++) if(suit[j]==str[1]) { s=j; break; }

	if(r==-1 || s==-1) return -1;

	return s*13+r;
}

#endif /* CARD_IMPLEMENTATION */

#endif /* CARD_H */
