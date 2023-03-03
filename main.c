#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>



#include "common.h"



#define INI_IMPLEMENTATION 
#include "ini.h"

#define IRC_IMPLEMENTATION 
#include "irc.h"

#define STRUTIL_IMPLEMENTATION 
#include "strutil.h"



const char *mst = "siesta";
const char *hst = "irc.undernet.org";
const char *prt = "6667";
const char *nck = "siestu";
const char *chn = "#pantasya";
const char *pss = NULL;

int sck;

typedef struct IrcMsg IrcMsg;

struct IrcMsg {
	char *usr;
	char *cmd;
	char *par; 
	char *txt;
};

typedef enum {
	GAMESTATE_INIT=0,
	GAMESTATE_START,
	GAMESTATE_MAX
} GameState;


typedef struct Player Player;

struct Player {
	char *nick;
	size_t score;
};



Player **players=NULL;
size_t nplayers=0;

GameState gameState=GAMESTATE_INIT;



void sig_handler(int signum) {
	if(signum==SIGALRM) {
	}
}



Player *Player_New(char *nick,size_t score) {
	Player *player=malloc(sizeof(*player));
	if(player) {
		player->nick=strdup(nick);
		player->score=score;
	}
	return player;
}

void Players_Free(Player ***players,size_t *nplayers) {
	for(size_t i=0;i<*nplayers;i++) {
		free((*players)[i]->nick);
		(*players)[i]->nick=NULL;
		(*players)[i]->score=0;
	}
	free(*players);
	*players=NULL;
	*nplayers=0;	
}


void Player_Add(Player ***players,size_t *nplayers,Player *player) {
	*players=realloc(*players,sizeof(**players)*(*nplayers+1));
	(*players)[(*nplayers)++]=player;
}

void Players_Load(Player ***players,size_t *nplayers,char *fn) {
	char nick[STRING_MAX];
	size_t score=0;
	FILE *fin=fopen(fn,"r");
	if(fin) {
		while(fscanf(fin,"%s %zu",nick,&score)==2) {
			Player_Add(players,nplayers,Player_New(nick,score));
		}
	fclose(fin);
	}
}

void Players_Save(Player **players,size_t nplayers,char *fn) {
	FILE *fout=fopen(fn,"w");
	if(fout) {
		for(size_t i=0;i<nplayers;i++) {
			fprintf(fout,"%s %zu",players[i]->nick,players[i]->score);
		}
		fclose(fout);
	}
}

ssize_t Player_Find(Player **players,size_t nplayers,char *nick) {
	for(size_t i=0;i<nplayers;i++) {
		if(!strcasecmp(nick,players[i]->nick)) {
			return i;
		}
	}
	return -1;
}

int cmpByScoreDesc(const void *l,const void *r) {
	Player a=**(Player**)l;
	Player b=**(Player**)r;
	if(a.score<b.score) return 1;
	if(a.score>b.score) return -1;
	return 0;
}



void parsein(IrcMsg *im) {

	char *msg=strdup(im->txt);

	trim(msg);

	if(!strcmp(msg,".start")) {

		if(gameState==GAMESTATE_INIT) {
			
			gameState=GAMESTATE_START;

			alarm(1);
			
		} else if(gameState==GAMESTATE_START) {
			notice(sck,im->usr,GAME_TITLE" %s: game is already started",im->usr);
		}
	} else if(!strncmp(msg,".score",6)) {

		char nn[STRING_MAX];

		if(!strcmp(msg,".score")) {
			ssize_t k=Player_Find(players,nplayers,im->usr);

			if(k==-1) {
				notice(sck,im->usr,GAME_TITLE" %s: your score is 0",im->usr);
			} else {			
				notice(sck,im->usr,GAME_TITLE" %s: your score is %zu",players[k]->nick,players[k]->score);
			}
			
		} else if(sscanf(msg,".score %s",nn)==1) {
			ssize_t k=Player_Find(players,nplayers,nn);
			if(k==-1) {
				notice(sck,im->usr,GAME_TITLE" %s: player %s not found",im->usr,nn);
			} else {
				if(!strcasecmp(nn,players[k]->nick)) 
					notice(sck,im->usr,GAME_TITLE" %s: your score is %zu",players[k]->nick,players[k]->score);
				else			
					notice(sck,im->usr,GAME_TITLE" %s: score of %s is %zu",im->usr,players[k]->nick,players[k]->score);
			}
		} 
	
	} else if(!strcmp(msg,".top") && nplayers) {
		qsort(players,nplayers,sizeof(*players),cmpByScoreDesc);
		char msg[STRING_MAX];
		char str[STRING_MAX];
		msg[0]='\0';
		for(size_t i=0;i<(nplayers>10?10:nplayers);i++) {
			if(i!=0) strcat(msg,", ");
			sprintf(str,"(%zu) ",i+1);
			strcat(msg,str);
			strcat(msg,players[i]->nick);
			sprintf(str," %zu",players[i]->score);
			strcat(msg,str);
		}
		privmsg(sck,chn,GAME_TITLE" top10: %s",msg);
	}
	
	free(msg);
	msg=NULL;

}



void parsesrv(IrcMsg *im,char *line) {
	im->cmd=line;
	im->usr = (char*)hst;
	if(!im->cmd || !*im->cmd) return; 
	if(im->cmd[0] == ':') {
		im->usr = im->cmd + 1;
		im->cmd = skip(im->usr, ' ');
		if(!*im->cmd) return;
		skip(im->usr, '!');
	}
	skip(im->cmd, '\r');
	im->par = skip(im->cmd, ' ');
	im->txt = skip(im->par, ':');
	trim(im->par);
}



int main(void) {

	char line[STRING_MAX];
	size_t llen=STRING_MAX;
	ssize_t rlen=0;

	IrcMsg im;

	srand(time(NULL));
	
	signal(SIGALRM, sig_handler);

  ini_t *config = ini_load("config.ini");

  const char *val; 

  val = ini_get(config,"default","host");

  hst=val?val:hst;

  val = ini_get(config,"default","port");

  prt=val?val:prt;

  val = ini_get(config,"default","nick");

  nck=val?val:nck;

  val = ini_get(config,"default","password");

  pss=val?val:pss;

  val = ini_get(config,"default","channel");

  chn=val?val:chn;

  val = ini_get(config,"default","master");

  mst=val?val:mst;



	sck=Irc_Connect(hst,prt);

  if (sck<0) {
  	DieWithUserMessage("Irc_Connect() failed", "unable to connect");
  }

	if(pss) raw(sck,"PASS %s\r\n",pss);
	raw(sck,"NICK %s\r\n",nck);
	raw(sck,"USER %s %s %s :%s\r\n",nck,nck,nck,nck);

	while((rlen=Irc_Recv(sck,line,llen))!=-1) { 

		if(rlen>0) {

			im.cmd=NULL;
			im.usr=NULL;
			im.par=NULL;
			im.txt=NULL;

			parsesrv(&im,line);		

	/*
			printf("cmd: %s\n",im.cmd);
			printf("usr: %s\n",im.usr);
			printf("par: %s\n",im.par);
			printf("txt: %s\n",im.txt);
	//*/
			
			if(!strcmp(im.cmd,"PING")) {
				line[1]='O';
				raw(sck,"PONG :%s\r\n",im.txt);
			} else if(!strcmp(im.cmd,"001")) {
				raw(sck,"JOIN %s\r\n",chn);
			} else if(!strcmp(im.cmd,"PRIVMSG")) {
				printf("%s <%s> %s\n",im.par,im.usr,im.txt);
				parsein(&im);				
			}

		}

		line[0]='\0';
		llen=STRING_MAX;
		rlen=0;

  }

	return 0;
	
}


